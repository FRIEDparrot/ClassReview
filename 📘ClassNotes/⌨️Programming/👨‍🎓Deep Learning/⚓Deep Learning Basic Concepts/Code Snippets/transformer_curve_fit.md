
```python fold title:curve-fitting-by-transformer
from torch.utils.data import TensorDataset, DataLoader
from my_transformer import (
    PositionalEncoding,
    ResidualAttentionBlock,
    ResidualFFNBlock
)
import torch
from torch import nn
import matplotlib.pyplot as plt


# simple function fit by attention  transformer
def func(x):
    return 2 * torch.sin(x) + x ** 0.8

class SimpleSelfAttentionTransformer(nn.Module):
    def __init__(self,
                 d_model,
                 hidden_dim=64,
                 num_heads=8,
                 lr=1e-3):
        """
        :param num_heads:
        :param d_model: hidden size of x expected (the 3rd dimension of x)
        :param hidden_dim: embedding size
        """
        assert hidden_dim % num_heads == 0
        super(SimpleSelfAttentionTransformer, self).__init__()
        self.lr = lr

        # since the embedding only takes integer,  we use a linear layer as embedding
        self.embedding = nn.Linear(d_model, hidden_dim)
        # max-seq-len = 1000 here for simplicity
        # add dropout for stability
        self.position_encoding = PositionalEncoding(1000, hidden_dim)

        self.k_proj = nn.Linear(hidden_dim, hidden_dim)
        self.q_proj = nn.Linear(hidden_dim, hidden_dim)
        self.v_proj = nn.Linear(hidden_dim, hidden_dim)

        # create a positional encoding layer
        self.attention = ResidualAttentionBlock(input_size=hidden_dim, num_heads=num_heads, dropout=0.1)
        self.ffn = ResidualFFNBlock(hidden_dim, n_layers=3, dropout=0.05)
        self.output_header = LinearRegressionHeader(hidden_dim)
        
        self.device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        self.to(self.device)

        # Keep track of best model state
        self.best_val_loss = float('inf')
        self.best_state_dict = None

    @staticmethod
    def criterion(y_pred: torch.Tensor, y_true: torch.Tensor):
        loss = nn.functional.mse_loss(y_pred, y_true.to(y_pred.device))
        return loss

    def optimizer(self):
        return torch.optim.Adam(self.parameters(), lr=self.lr)

    def forward(self, x):
        # x : [batch_size, seq_len, hidden_size]
        x = self.embedding(x.to(self.device)) * torch.sqrt(torch.tensor(x.shape[-1]))
        x = self.position_encoding(x)

        # project the x into q, k and v
        q = self.q_proj(x)  # now we not add positional embeddings
        k = self.k_proj(x)
        v = self.v_proj(x)

        output, attn_weights = self.attention(q, k, v)
        output = self.ffn(output)  # [batch_size, seq_len, hidden_size]
        output = self.output_header(output)
        return output

    def train_model(self, train_dataloader, val_dataloader, epoch_num=1500,
                    scheduler_patience=20, early_stop_patience=200, log_interval=50):
        """
        Integrated training process

        Args:
            train_dataloader: DataLoader for training data
            val_dataloader: DataLoader for validation data
            epoch_num: Maximum number of epochs
            scheduler_patience: Patience for learning rate scheduler
            early_stop_patience: Patience for early stopping
            log_interval: How often to log training progress

        Returns:
            History of training and validation losses
        """
        optim = self.optimizer()

        scheduler = torch.optim.lr_scheduler.ReduceLROnPlateau(
            optim, mode='min', factor=0.75, patience=scheduler_patience, min_lr=1e-6
        )

        patience_counter = 0
        history = {'train_loss': [], 'val_loss': []}

        for epoch in range(epoch_num):
            # =======  training  =======
            self.train()  # set model to training mode
            avg_train_loss = 0
            for x_train, y_train in train_dataloader:
                y_p = self(x_train)
                l = self.criterion(y_p, y_train)
                optim.zero_grad()
                l.backward()
                # add gradient clip for stability
                torch.nn.utils.clip_grad_norm_(self.parameters(), max_norm=1.0)
                optim.step()
                avg_train_loss += l.item()

            avg_train_loss = avg_train_loss / len(train_dataloader)
            scheduler.step(avg_train_loss)
            history['train_loss'].append(avg_train_loss)

            # =======  validation and early stop mechanism  =======
            avg_val_loss = self.evaluate(val_dataloader)
            history['val_loss'].append(avg_val_loss)

            # early stop mechanism
            if avg_val_loss < self.best_val_loss:
                self.best_val_loss = avg_val_loss
                patience_counter = 0
                # record the best model
                self.best_state_dict = self.state_dict()
            else:
                patience_counter += 1
                if patience_counter >= early_stop_patience:
                    print(f"Early stop at epoch {epoch}")
                    break

            if epoch % log_interval == 0:
                print(f"Epoch {epoch} avg_train_loss: {avg_train_loss}, val_loss: {avg_val_loss}")

        # Load the best model
        if self.best_state_dict is not None:
            self.load_state_dict(self.best_state_dict)

        return history

    def evaluate(self, dataloader):
        """Evaluate the model on the given dataloader"""
        self.eval()
        with torch.no_grad():
            val_loss = 0
            for x_batch, y_batch in dataloader:
                y_pred = self(x_batch)
                l = self.criterion(y_pred, y_batch)
                val_loss += l.item()

        return val_loss / len(dataloader)

    def predict(self, x):
        """Make predictions using the model"""
        self.eval()
        with torch.no_grad():
            return self(x)

    def plot_results(self, x_val, y_val, show_plot=True):
        """
        Plot the model predictions against validation data

        Args:
            x_val: Validation input data
            y_val: Validation target data
            show_plot: Whether to display the plot

        Returns:
            The figure object
        """
        y_pred = self.predict(x_val)

        x_val_flat = x_val.cpu().ravel()
        y_val_flat = y_val.cpu().ravel()
        y_pred_flat = y_pred.cpu().ravel()

        plt.figure(figsize=(10, 6))
        plt.plot(x_val_flat, y_val_flat, '-', label='validation')
        plt.plot(x_val_flat, y_pred_flat, label='predict')
        plt.legend()
        plt.title('Transformer Model Predictions')
        plt.xlabel('x')
        plt.ylabel('y')

        if show_plot:
            plt.show()

        return plt.gcf()


class LinearRegressionHeader(nn.Module):
    def __init__(self, input_size):
        super(LinearRegressionHeader, self).__init__()
        self.linear = nn.Linear(input_size, 1)

    def forward(self, x):
        return self.linear(x)


if __name__ == "__main__":
    x_train = torch.arange(0, 10, 0.05)  # 200 data for train

    # input 10 samples once
    seq_len = 10
    x_train = x_train.reshape((-1, seq_len, 1))
    y_train = func(x_train) + 0.1 * torch.randn(x_train.shape)

    # generate validation data
    x_val = torch.linspace(0, 10, 200).reshape((-1, seq_len, 1))
    y_val = func(x_val) + 0.1 * torch.randn(x_val.shape)

    model = SimpleSelfAttentionTransformer(d_model=1, hidden_dim=64, num_heads=8)

    train_dataset = TensorDataset(x_train, y_train)
    val_dataset = TensorDataset(x_val, y_val)
    train_dataloader = DataLoader(train_dataset, batch_size=50, shuffle=True)
    val_dataloader = DataLoader(val_dataset, batch_size=20)

    # Train the model using the integrated method
    history = model.train_model(train_dataloader, val_dataloader)

    # Generate test data for visualization
    x_test = torch.arange(0, 10, 0.1).reshape((-1, seq_len, 1))
    y_test = func(x_test)

    # Plot the results
    model.plot_results(x_test, y_test)
```

If we implement the above code in built-in functions, we can use the following code : 

```python fold title:
from transformer_curve_fitting import func
import torch
from torch import nn
from my_transformer import PositionalEncoding
from torch.utils.data import DataLoader, TensorDataset
import matplotlib.pyplot as plt


# implementation using the built-in transformer package
class BuiltInTransformer(nn.Module):
    def __init__(self, d_model,
                 hidden_dim=64,
                 nhead=8,
                 num_layers=3,
                 lr=0.001,
                 dropout=0.1,
                 activation="relu"):
        super().__init__()

        self.device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

        self.embed = nn.Linear(d_model, hidden_dim)

        self.pos_encoder = PositionalEncoding(max_seq_len=1000, d_model=hidden_dim)
        self.encoder_layer = nn.TransformerEncoderLayer(
            d_model=hidden_dim,
            nhead=nhead,
            dim_feedforward=hidden_dim * 4,  # increase the FFN hidden size
            dropout=dropout,
            activation=activation,
            norm_first=True,  # use layer norm before attention
            batch_first=True,  # **important for  performance**
        )
        self.transformer_encoder = nn.TransformerEncoder(
            self.encoder_layer,
            num_layers=num_layers
        )
        # output all hidden layer as a value to fit
        self.output_layer = nn.Linear(hidden_dim, 1)
        self.lr = lr
        self.to(self.device)

    def forward(self, x):
        x = self.embed(x) * (x.shape[-1] ** 0.5)  # scale embedding
        x = self.pos_encoder(x)  # (batch_size, seq_len, hidden_dim)
        x = self.transformer_encoder(x)  # (batch_size, seq_len, hidden_dim)
        
        # output all hidden layer as a value to fit
        x = self.output_layer(x)  # (batch_size, seq_len, 1)
        return x.squeeze(-1)  # (batch_size, seq_len)

    def fit(self, x, y, epochs=100):
        x = x.to(self.device)
        y = y.to(self.device)
        x_val = x
        y_val = y

        # set up optimizer and loss function
        optimizer = torch.optim.Adam(self.parameters(), lr=self.lr)
        criterion = nn.MSELoss()  # mean squared error loss
        scheduler = torch.optim.lr_scheduler.ReduceLROnPlateau(
            optimizer, mode='min', factor=0.5, patience=10, min_lr=1e-6
        )  # reduce learning rate on plateau

        dataloader = DataLoader(TensorDataset(x, y), batch_size=32, shuffle=True)

        # early stopping
        patience = 50
        patience_counter = 0
        best_state = None
        best_val_loss = float('inf')
        best_state_dict = None

        for epoch in range(epochs):
            self.train()  # set model to training mode
            avg_loss = 0.0
            for x_batch, y_batch in dataloader:
                y_p = self(x_batch)
                loss = criterion(y_p, y_batch.to(self.device))
                optimizer.zero_grad()
                loss.backward()
                torch.nn.utils.clip_grad_norm_(self.parameters(), max_norm=1.0)  # Increased from 1.0
                optimizer.step()
                avg_loss += loss.item()
            avg_loss /= len(dataloader)

            # evaluate on validation set
            self.eval()  # set model to evaluation mode (turn off dropout)
            with torch.no_grad():
                val_loss = criterion(self(x_val), y_val.to(self.device)).item()
                if val_loss < best_val_loss:
                    best_val_loss = val_loss
                    best_state_dict = self.state_dict()
                    patience_counter = 0
                else:
                    patience_counter += 1
                    if patience_counter >= patience:
                        print(f"Early stopping at epoch {epoch + 1}")
                        # Load best model
                        self.load_state_dict(best_state_dict)
                        break

            scheduler.step(val_loss)  # Use validation loss for scheduler

            if epoch % 50 == 0:
                print(f"Epoch {epoch}: train_loss={avg_loss:.4f}, val_loss={val_loss:.4f}")

        # Always load the best model before returning
        if best_state_dict is not None:
            self.load_state_dict(best_state_dict)

        return self  # return the trained model

    def predict(self, x):
        x = x.to(self.device)
        self.eval()  # set model to evaluation mode (turn off dropout)
        return self(x)  # (batch_size, seq_len)


if __name__ == "__main__":
    seq_len = 10  # sequence length

    x_train = torch.arange(0, 10, 0.05).reshape(-1, seq_len, 1)
    y_train = func(x_train.flatten()).reshape(-1, seq_len) + 0.1 * torch.randn(x_train.shape[0], seq_len)

    x_test = torch.arange(0, 10, 0.1).reshape(-1, seq_len, 1)
    y_test = func(x_test.flatten()).reshape(-1, seq_len)

    model = BuiltInTransformer(d_model=1,
                               hidden_dim=64,
                               nhead=8,
                               num_layers=3,
                               lr=0.001)
    model.fit(x_train, y_train, epochs=1500)

    y_p = model.predict(x_test)
    plt.scatter(x_train.flatten().numpy(), y_train.flatten().numpy(), label="train", alpha=0.4, s=0.5)
    plt.plot(x_test.flatten().numpy(), y_test.flatten().numpy(), label="test")
    plt.plot(x_test.flatten().numpy(), y_p.cpu().flatten().detach().numpy(), label="pred")
    plt.legend()
    plt.show()
```
