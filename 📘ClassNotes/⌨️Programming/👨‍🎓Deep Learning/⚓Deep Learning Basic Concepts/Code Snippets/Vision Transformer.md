
```python fold title:Original-Transformer-for-vision 
import torch
from torch import nn
import kagglehub
from torchvision.datasets import ImageFolder
from torchvision import transforms
import os
from torch.utils.data import DataLoader, Dataset
from PIL import Image
from machine_translation.my_transformer import PositionalEncoding
from matplotlib import pyplot as plt

"""
The Transformer model would overfit easily, so we must implement some regularization. 
"""

# load the data from the dataset
class ImageDataset(Dataset):
    def __init__(self, folder, transform=None):
        self.folder: ImageFolder = folder
        if transform is None:  # if no transform is provided, use the default transform
            self.transform = transforms.Compose([
                transforms.ToTensor(),
            ])
        else:
            self.transform = transform

    def __len__(self):
        return len(self.folder)

    def __getitem__(self, index):
        img = self.folder.imgs[index][0]
        img_data = self.transform(Image.open(img).convert('RGB'))  # 3 * 224 * 224
        img_label = self.folder.imgs[index][1]
        return img_data, img_label
    # DataLoader(batch_size=32, pin_memory=True)


class PatchEmbedding(nn.Module):
    def __init__(self, img_size, input_channels=3, patch_size=16, embed_dim=512):
        super().__init__()
        assert img_size % patch_size == 0, 'Image size must be divisible by patch size'
        self.embedding = nn.Conv2d(in_channels=input_channels,
                                   out_channels=embed_dim,
                                   kernel_size=patch_size,
                                   stride=patch_size)
        # flatten w and h dimensions
        self.flatten = nn.Flatten(start_dim=2, end_dim=-1)

    def forward(self, x):
        x = self.embedding(x)  # (batch_size, embed_dim, w, h)
        x = self.flatten(x).transpose(1, 2)  # flatten the output of the conv layer
        return x


class ImageTransformer(nn.Module):
    def __init__(self,
                 img_size,
                 n_patches=16,
                 input_channels=3,
                 embed_dim=1024,
                 nhead=8,
                 dropout=0.1):
        super(ImageTransformer, self).__init__()
        self.patch_size = n_patches  # 16 * 16 patches for the transformer
        self.n_patches = (img_size // self.patch_size) ** 2  # number of patches

        # for patch embedding layer, we use3 input channels to
        self.patch_embedding = PatchEmbedding(img_size=img_size,
                                              input_channels=input_channels,
                                              patch_size=self.patch_size,
                                              embed_dim=embed_dim)
        # use bos_token to initialize the transformer, which is learned during training
        self.bos_token = nn.Parameter(torch.zeros(1, 1, embed_dim)) 
        
        self.positional_encoding = PositionalEncoding(max_seq_len=self.n_patches + 1,
                                                      d_model=embed_dim,
                                                      dropout=dropout)
        # embedding layer (not change the input size)
        self.transformer_layer = nn.TransformerEncoderLayer(d_model=embed_dim,
                                                            nhead=nhead,
                                                            batch_first=True)

        self.transformer = nn.TransformerEncoder(self.transformer_layer, num_layers=2)

    def forward(self, x: torch.Tensor):
        x = self.patch_embedding(x)  # (batch_size, n_patches, embed_dim)
        seq_bos = self.bos_token.expand(x.shape[0], 1, -1)  # (batch_size, 1, embed_dim)  bos token
        seq = torch.cat([seq_bos, x], dim=1)  # add bos token to the sequence
        seq = self.positional_encoding(seq)  # add positional encoding to the sequence
        output = self.transformer(seq)  # (batch_size, n_patches+1, embed_dim)
        return output


class ImageClassificationHead(nn.Module):
    def __init__(self, d_model, num_classes=10, dropout=0.25):
        """
        The image classification head for the transformer model.

        :param d_model: output dimension of the transformer model
        :param num_classes:
        """
        super(ImageClassificationHead, self).__init__()
        self.linear1 = nn.Linear(d_model, num_classes)
        self.dropout = nn.Dropout(dropout)
        self.linear2 = nn.Linear(num_classes, num_classes)
    
    def forward(self, x):
        """
        Add 2 linear layers and dropout to improve the generalization of the model.
        :param x:
        :return:
        """
        # x: (batch_size, n_patches+1, embed_dim)
        out = x[:, 0, :]  # take the first element as the output of <bos> token
        # size of out: (batch_size, embed_dim)
        out = self.dropout(out)
        out = self.linear1(out)  # (batch_size, num_classes)
        out = self.dropout(out)
        out = self.linear2(out)
        return out


class ImageClassifier(nn.Module):
    def __init__(self, num_classes,
                 lr=1e-4):
        super(ImageClassifier, self).__init__()
        self.transformer = ImageTransformer(img_size=224, embed_dim=1024, n_patches=16, nhead=8, dropout=0.1)
        self.head = ImageClassificationHead(d_model=1024, num_classes=num_classes)
        self.loss = nn.CrossEntropyLoss()
        self.optimizer = torch.optim.Adam(self.parameters(), lr=lr)
        self.scheduler = torch.optim.lr_scheduler.ReduceLROnPlateau(self.optimizer, mode='min', factor=0.75)
        self.device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
        self.to(self.device)

    def forward(self, x):
        x = self.transformer(x)  # attention mechanism
        x = self.head(x)
        return x

    def criterion(self, y_pred, y_true):
        return self.loss(y_pred, y_true)

    def train_process(self, train_loader, val_loader, epochs=10):
        for epoch in range(epochs):
            train_avg_loss = 0
            self.train()  # set the model to training mode
            for x_train, y_train in train_loader:
                x_train = x_train.to(self.device)
                y_train = y_train.to(self.device)
                y_pred = self(x_train)
                train_loss = self.criterion(y_pred, y_train)

                self.optimizer.zero_grad()
                train_loss.backward()
                torch.nn.utils.clip_grad_norm_(self.parameters(),
                                               max_norm=1)  # clip the gradient norm to prevent exploding gradients
                self.optimizer.step()  # update the parameters

                train_avg_loss = train_avg_loss + train_loss.item()
            train_avg_loss = train_avg_loss / len(train_loader)
            self.scheduler.step(train_avg_loss)  # update the learning rate

            # validation process
            self.eval()  # set the model to evaluation mode
            with torch.no_grad():
                total_loss = 0
                total_correct = 0
                for x_train, y_train in val_loader:
                    x_train = x_train.to(self.device)
                    y_train = y_train.to(self.device)
                    y_pred = self(x_train)
                    loss = self.criterion(y_pred, y_train)
                    total_loss += loss.item() * x_train.size(0)  # accumulate the loss
                    equals: torch.Tensor = predicted == y_train
                    total_correct += equals.sum().item()
                    _, predicted = torch.max(y_pred.data, 1)

                val_loss = total_loss / len(val_loader.dataset)
                val_acc = total_correct / len(val_loader.dataset) * 100
                print(
                    f"Epoch {epoch + 1}: Training Loss: {train_avg_loss:.4f}, "
                    f"Validation Loss: {val_loss:.4f}, Validation Accuracy: {val_acc:.2f}%"
                )

            # save the model per 5 epochs
            if (epoch + 1) % 5 == 0:
                torch.save(self.state_dict(), f"model_epoch_{epoch + 1}.pth")

    def predict(self, x):
        self.eval()  # set the model to evaluation mode
        with torch.no_grad():
            x = x.to(self.device)
            y_pred = self(x)
            _, predicted = torch.max(y_pred.data, 1)
            # return the predicted class
            return predicted

    def test_process(self, test_loader):
        # test the model on the test set
        self.eval()
        with torch.no_grad():
            total_correct = 0
            for x_test, y_test in test_loader:
                x_test = x_test.to(self.device)
                y_test = y_test.to(self.device)
                y_pred = self(x_test)
                _, predicted = torch.max(y_pred.data, 1)
                total_correct += torch.tensor(predicted == y_test).sum().item()
            test_acc = total_correct / len(test_loader.dataset) * 100
            print(f"Test Accuracy: {test_acc:.2f}%")

    def load(self, path):
        self.load_state_dict(torch.load(path))


def main():
    # The image classification  task using transformer model.
    path = kagglehub.dataset_download("gpiosenka/sports-classification")

    train_folder = ImageFolder(os.path.join(path, "train"), allow_empty=False)
    test_folder = ImageFolder(os.path.join(path, "test"), allow_empty=False)
    valid_folder = ImageFolder(os.path.join(path, "valid"), allow_empty=False)

    # create the dataset and dataloader
    train_dataset = ImageDataset(train_folder)
    test_dataset = ImageDataset(test_folder)
    valid_dataset = ImageDataset(valid_folder)

    # shuffle the train loader
    train_loader = DataLoader(train_dataset, batch_size=32, pin_memory=True, shuffle=True)
    valid_loader = DataLoader(valid_dataset, batch_size=32, pin_memory=True, shuffle=False)
    test_loader = DataLoader(test_dataset, batch_size=32, pin_memory=True, shuffle=False)

    model = ImageClassifier(num_classes=len(train_folder.classes), lr=1e-4)  # create the model
    model.train_process(train_loader, valid_loader, epochs=100)
    model.test_process(test_loader)


if __name__ == '__main__':
    main()

```

**We note  we may got serious  overfitting when we use no  dropout in  ImageTransformer**, and to reach better accuracy, we may use the following  mechanism for reach better  accuracy in prediction : 

1.  Try to <mark style="background: transparent; color: red">not use dropout in positional Encoding</mark>, and we use it in the <mark style="background: transparent; color: red">Transformer Encoder layer</mark>.  
2. <b><mark style="background: transparent; color: orange">Use 2 layers in the  classification</mark></b>,  and use GELU activation function  (This is an optimized version of ReLU), and prevent the gradient dissappering problem around zero.  
3. Use label smoothing  for better 
4. Use **weight decay (L2 norm)** in  optimizer (for small  model and relative low data quantity, set to `1e-5`,  and for Larger model, we can set to `1e-3` or larger) 
5. Use `CosAnnelingLR` instead of the `ReduceLROnPlateau` (see [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/🔥Pytorch Basics/学习率调度器 Scheduler 的应用范围|Scheduler]])  
6. `AdamW` is better for transforers,  try to use it instead  of Adam.  

**RAdam (Rectified  Adam)** : 
At the  start, it use the smaller learning rate to avoid variance explosion,  and later it  gradually close to Adam. 
<b><mark style="background: transparent; color: orange">This optimizer is more stable than Adam</mark></b> 

```python fold title:
self.scheduler = torch.optim.lr_scheduler.ReduceLROnPlateau(self.optimizer, mode='min', factor=0.75) 
# we can use a new  scheduler for better performance : 
self.scheduler = torch.optim.lr_scheduler.CosineAnnealingLR(self.optimizer, T_max=10, eta_min=0) 
# in the  code, we needn't specify the actual  
self.scheduler.step()  # update the learning rate 
```

2. The error in the  classification head : 
	1. There is no need to  dropout twice
	2. since  `num_classes` is very  small, <b><mark style="background: transparent; color: orange">the unit in the hidden layer can't  be set to num_classses</mark></b>, it must be specified, a small number of hidden has small classification ability and can't have a good performance on the generalization. so it's easy to make transformer too complex and decrease the complexity of classification function.  

```python fold title:
class ImageClassificationHead(nn.Module):
    def __init__(self, d_model, num_classes=10, dropout=0.25):
        """
        The image classification head for the transformer model.

        :param d_model: output dimension of the transformer model
        :param num_classes:
        """
        super(ImageClassificationHead, self).__init__()
        self.linear1 = nn.Linear(d_model, num_classes)
        self.dropout = nn.Dropout(dropout)
        self.linear2 = nn.Linear(num_classes, num_classes)
    
    def forward(self, x):
        """
        Add 2 linear layers and dropout to improve the generalization of the model.
        :param x:
        :return:
        """
        # x: (batch_size, n_patches+1, embed_dim)
        out = x[:, 0, :]  # take the first element as the output of <bos> token
        # size of out: (batch_size, embed_dim)
        out = self.dropout(out)
        out = self.linear1(out)  # (batch_size, num_classes)
        out = self.dropout(out)
        out = self.linear2(out)
        return out
```

2.  Data Augmentation sample : 
```python fold title:
self.transform = transforms.Compose([ transforms.Resize((256, 256)), transforms.RandomResizedCrop(224, scale=(0.6, 1.0), ratio=(0.75, 1.33)), transforms.RandomHorizontalFlip(p=0.5), transforms.RandomVerticalFlip(p=0.2), transforms.RandomRotation(degrees=15), transforms.ColorJitter(brightness=0.3, contrast=0.3, saturation=0.3, hue=0.1), transforms.RandomAffine(degrees=0, translate=(0.1, 0.1)), transforms.RandomPerspective(distortion_scale=0.2, p=0.3), transforms.ToTensor(), transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]), transforms.RandomErasing(p=0.25, scale=(0.02, 0.2)) ])
```
