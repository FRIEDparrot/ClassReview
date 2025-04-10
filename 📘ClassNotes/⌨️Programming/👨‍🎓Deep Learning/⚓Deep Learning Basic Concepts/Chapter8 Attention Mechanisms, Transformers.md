## 1. Attention Mechanisms
### (1) Introduction 
The attention  mechanism is derived from the cognitive neuroscience. and ==the transformer architecture is solely based on attention mechanisms==. 

The vision transformers **outperform ResNets significantly in image classification when it cames out the  very large datasets**. 

> [!NOTE] Application of Transformers 
> transformers have been extensively used in large-scale pretraining, , which can be adapted to perform different tasks with model update. 
> The pretrain transformers can be  applied as <mark style="background: transparent; color: red">encoder-only (e.g., BERT), encoder-decoder (e.g., T5), and decoder-only (e.g., GPT series)</mark>

### (2) Basic Concepts 
We consider  the  cues  where  the  only nonvolitional cues are available. We can simply use a parameterized fully connected layer or even non-parameterized max or average pooling in this case. 

The attention mechanisms use  <mark style="background: transparent; color: red">volational cues</mark> compare to the traditional fully connected layer.  we can call the volitional cues as <b><mark style="background: transparent; color: orange">queries</mark></b>,  and when the query is given, the attention  mechanisms ==**bias selection over the sensory inputs**==  via <b><mark style="background: transparent; color: orange">attention pooling</mark></b>. where the  **sensory inputs** are called <b><mark style="background: transparent; color: orange">values</mark></b>,  every  value is paired with a <b><mark style="background: transparent; color: orange">key</mark></b> (which is **nonvolitional cues**).  

![[attachments/Pasted image 20250401200358.png|450]]

the  attention pool  guides the bias selection over values (sensory inputs) 

For the average pooling, it can be treated as a **weighted average of inputs,** (<mark style="background: transparent; color: red">weights of the average pooling are uniform</mark>) while attention pooling <b><mark style="background: transparent; color: orange">aggregates values using weighted average</mark></b>, and weights  are computed between  the given  query and different keys. 

we can  use a heatmap plot for display the weights of attention pooling : 
```python
import matplotlib.pyplot as plt  
import numpy as np  
import torch  
  
mat = torch.eye(10).reshape((10, 10))  
  
plt.pcolormesh(mat, cmap='hot')  
plt.colorbar()  
plt.show()
```
![[attachments/Pasted image 20250401212347.png|300]]
### (3) Non-Parametric Attention Pooling  
To recapitulate,  the **interactions between queries (volitional cues) and keys (nonvolitional cues) result in attention pooling**.  

the average pooling is  just crudely average the data :  
$$y = \frac{1}{n} \sum_{i = 1}^{n} y_{i}$$
but for the nonparametric  attention pooling, we use the following  attention mechanism  : 
$$y   = \sum_{i = 1}^{n}  \frac{ K (x - x_{i})}{  \sum_{j = 1}^{n}  K (x - x_{j})} y_{i} =  \sum_{i = 1}^{n}   \alpha(x, x_{i}) y_{i} \tag{8.1.3.1}$$
the above  estimator is called <b><mark style="background: transparent; color: orange">Nadaraya-Waston kernel regression</mark></b>. where  **the attention weights over all the key-value pairs are a valid probability distribution: they are non-negative and sum up to one**. in the above equation, apparantly that the smaller the diff $(x - x_{i})$ is,  the `val_data` contributes more on predicting the corresponding data.   

For simple Gaussian Kernel, $K$ is 
$$K(u) = \frac{1}{\sqrt{2 \pi}} \exp  \left(- \frac{u^{2}}{2}\right)$$
and the prediction calculation becomes : 
$$y =  \frac{ \sum_{i = 1}^{n}  \exp \left(- \frac{1}{2} (x -  x_{i})^{2}\right) y_{i}}{\sum_{j = 1}^{n}  \exp (- \frac{1}{2} (x -  x_{j})^{2})} =    \sum_{i = 1}^{n}  \text{softmax} \left(- \frac{1}{2} (x - x_{i})^{2}\right) y_{i} =  \sum_{i = 1}^{n} w_{i}y_{i}$$
where $x$ is $x_{train}$, $y_i$ is corresponding value $y_{train}$ on $x_{train}$. which actually make a weighted sum of $y$ on each predicted point.  

we calculate the `query_key_diffs` matrix to record the difference of train and validation data. which <b><mark style="background: transparent; color: orange">calculate the weight of attention as</mark></b> : 
$$w_{i} = \text{softmax} \left(- \frac{1}{2} (x - x_{i})^{2}  \right)$$
the value of weight vector on each $x$ is the weight of $x_{train}$ at location $x_{i}$. 

```python 
def diff(queries, keys) -> torch.Tensor:  
    return queries.reshape((-1, 1)) - keys.reshape((1, -1))
diffs = diff(x_val, x_train) 
# result : 
tensor([[-0.0353, -0.1191, -0.2778,  ..., -4.6282, -4.7051, -4.7224],
        [-0.0303, -0.1141, -0.2728,  ..., -4.6232, -4.7001, -4.7174],
        [-0.0253, -0.1091, -0.2678,  ..., -4.6182, -4.6951, -4.7124],
        ...,
        [ 4.9497,  4.8659,  4.7072,  ...,  0.3568,  0.2799,  0.2626],
        [ 4.9547,  4.8709,  4.7122,  ...,  0.3618,  0.2849,  0.2676],
        [ 4.9597,  4.8759,  4.7172,  ...,  0.3668,  0.2899,  0.2726]])
```

we give the following full implementation : 

```python
import matplotlib.pyplot as plt  
import torch  
from torch.nn import functional as F  
  
n = 50  
n_val = 50  

def func(x):  
    return 2 * torch.sin(x) + x ** 0.8  

x_train, _ = torch.sort(torch.rand(n) * 5)  # generate random data in range  0-5  
y_train = func(x_train) + 0.5 * torch.randn(x_train.shape)  
  
x_val = torch.arange(0, 5, 5 / n_val)  
y_val = func(x_val)  
  
plt.subplot(1, 2, 1)  # plot the data  
plt.plot(x_train, y_train, 'o', label='train')  
plt.plot(x_val, y_val, '-', label='validation')  # plot validation data  
  
  
# make the  attention pool  
def diff(queries, keys) -> torch.Tensor:  
    return queries.reshape((-1, 1)) - keys.reshape((1, -1)) 

def attention_pool(query_key_diffs, values):  
    w = F.softmax(-query_key_diffs ** 2 / 2, dim=1)  
  
    # use sum softmax (-1/2 * (x -x_i)**2 ) y_i  as prediction  
    return torch.matmul(w, values), w  
  
  
diffs = diff(x_val, x_train)  
y_p, attention_weights = attention_pool(  
    diffs, y_train.reshape((-1, 1))  
)  
  
plt.subplot(1, 2, 2)  # plot the attention weights  
plt.pcolormesh(attention_weights.reshape((n_val, n)), cmap='hot')  
plt.colorbar()  # add a colorbar to show the attention weights  
plt.xlabel('validation points')  
plt.ylabel('train points')  
  
plt.subplot(1, 2, 1)  # plot the predicted values  
plt.plot(x_train, y_p, label='predicted')  
plt.legend()  
# set the size of the plot  
plt.gcf().set_size_inches(10, 5)  
  
  
plt.show()
```
in the above case,  the **keys are the training points `x_train`, and the queries are the validation points `x_val`**. The attention weights are computed based on the difference (`diff`) between **queries and keys**. 

the result is  as follows : 
![[attachments/Pasted image 20250401222341.png|450]]
which shows the weight of each  point at every validation point. 
### (4)  Parametric  Attention Pooling 
#### 1. Introduction 
In the training process, we train the attention pool in minibatch. in which every ==**minibatch contains $n$ matrices**==, we represent it by  $X_1, X_2 ,  \dots X_n$,  and we train the  transformer module by add a learnable parameter $w$ to the original equation for the train process, which is called <b><mark style="background: transparent; color: orange">parametric attention pooling</mark></b> : 
$$f(x) = \sum_{i = 1}^{n} \alpha(x, x_{i}) y_{i} =  \sum_{i = 1}^{n} \text{softmax} \left(-\frac{1}{2} ((x - x_{i} ) w)^{2}\right) y _{i}$$

for the learning process, we define a simple loss by 
$$ l   = \sum_{i = 1}^{n} \frac{1}{2} (y_{i} -\hat{y}_{i} )^{2}$$
and define the parameter $w$  as gradient parameter.  

#### 2. Batch Matrix Multiplication 
we firstly introduce the following function : 
```python
torch.bmm(X, Y)  # batch  matrix muliplication 
```
in the above function, we may use 
$$X :  n \times  a \times  b   \text{ matrix}\qquad  Y : n  \times  b  \times  c\space  \text{matrix}$$
then the result would  be  $n \times a \times c$, which is the result of batch multiplication of matrices. 

#### 3. Model Definition 
Note that the  transformer should <mark style="background: transparent; color: red">have a set of keys</mark>, which in the above case is the linspace 50 `x_val` from 0 to 5, so in the real transformer module, 

In a standard Transformer model, **key-value (K-V) pairs <mark style="background: transparent; color: red">(the x_train, y_train above)</mark> are dynamically generated from the input sequence** using learnable linear projections, and each time like we give `x_val` and `y_val` to train the model. 

We need to <b><mark style="background: transparent; color: orange">clearly sepecify the dimension of  input </mark></b> as :    **`d_model`** (in original Transformer paper) or **`embed_dim`** (in PyTorch). 

And in prediction,  src: $(S,E)$ $for unbatched input, $(S,N,E)$ if batch_first=False or $(N, S, E)$ if batch_first=True. where E is embed_dim. 

also note we have **Single-Head Attention (Head Dim = Embed Dim)**  case and   **Multi-Head Attention** where  we can initailize the keys, queries and values pairs  from nhead configuration. in pytorch, default is `nhead=8`, we calculate  `head_num` by 

```python
head_dim = embed_dim // num_heads
``` 

and use `head_dim` to initialize the dimension of  keys and values. 

> [!Note] In Book training method 
> In the book we use : 
> ```python
> class NWKernelRegression(d2l.Module): 
> 	def __init__(self, keys, values, lr):
> ```
> where  keys and values are pre-specified for stability. 

also, if we  not use loss through the entire epoch, the overfitting would be serve and  the producing unstable  (noisy) predictions. 
![[attachments/Pasted image 20250403100551.png|400]]
also, in the training Process, we can use `optim.lr_scheduler.ReduceLROnPlateau` for reduce learning rate when the loss is not reduce while training. 
```python
scheduler = torch.optim.lr_scheduler.ReduceLROnPlateau(optimizer, patience=5, factor=0.5, verbose=True) 
```

`````ad-note
title: **Popular Schedulers in PyTorch**
collapse: open
The following schedulers can be found in `torch.optim.lr_scheduler` :  
1. **StepLR**: Decreases the learning rate by a factor at predefined epoch intervals.
2. **MultiStepLR**: Similar to StepLR but allows specifying multiple milestones for learning rate decay.
3. **ExponentialLR**: Applies exponential decay to the learning rate at each epoch.
4. **CosineAnnealingLR**: Gradually decreases the learning rate following a cosine curve, which can help with cyclical refinement.
5. **OneCycleLR**: Implements the 1cycle policy, where the learning rate first increases then decreases, often leading to faster convergence.  
`````

attention-based models **can be sensitive to learning rate.** By automatically reducing the learning rate when training plateaued, the model was able to fine-tune its parameters more precisely,  

we give this implementation as follows :
```python
import numpy as np  
from torch import nn  
import torch  
from torch.nn import functional as F  
from torch.optim import Adam, SGD  
from torch.utils.data import DataLoader, TensorDataset  
from torch.nn.init import xavier_uniform_  
from matplotlib import pyplot as plt  
  
  
def diff(queries, keys):  
    # calculate the difference between x1 and x2  
    return queries.unsqueeze(2) - keys.unsqueeze(1)  # (batch_size, input_size, input_size)  
  
  
def func(x):  
    # define a  simple nonlinear function  
    return 2 * torch.sin(x) + x ** 0.8  
  
  
class SimpleTransformer(nn.Module):  
    def __init__(self, input_size, hidden_dim=64, lr=0.01):  
        super(SimpleTransformer, self).__init__()  
        self.lr = lr  
  
        self.input_size = input_size  
        self.hidden_dim = hidden_dim  
        # treat each input as token, and use linear mapping to project it to hidden_dim space  
        self.w_keys = nn.Linear(1, hidden_dim)  
        self.w_queries = nn.Linear(1, hidden_dim)  # used for initialization of queries projection matrices  
        self.w_values = nn.Linear(1, hidden_dim)  # used for initialization of value projection matrices  
        self.w_out = nn.Linear(hidden_dim, 1)  # used for initialization of output projection matrix  
  
        nn.init.xavier_uniform_(self.w_queries.weight)  
        nn.init.xavier_uniform_(self.w_keys.weight)  
        nn.init.xavier_uniform_(self.w_values.weight)  
        nn.init.xavier_uniform_(self.w_out.weight)  
  
    def attention_pool(self, queries, keys, values):  
        """  
        We use keys-value pair inside model        :param queries:        :param keys:        :param values:        :return:  
        """        # use scaled dot-product attention  
        attention_scores = torch.bmm(queries, keys.transpose(-1, -2)) / (self.hidden_dim ** 0.5)  
        attention_weights = F.softmax(attention_scores, dim=-1)  # calculate the attention weights  
        # use batch matrix muplication to calculate the weighted sum        return torch.bmm(attention_weights, values), attention_weights  
  
    def forward(self, x):  
        x = x.unsqueeze(-1)  # Now shape: (batch_size, input_size, 1)  
        keys = self.w_keys(x)  # [batch_size, input_size, hidden_size]  
        values = self.w_values(x)  # [batch_size, input_size, hidden_size]  
        queries = self.w_queries(x)  # [batch_size, input_size, hidden_size]  
        hidden, w = self.attention_pool(queries, keys, values)  
        output = self.w_out(hidden).squeeze(-1)  # project the hidden state to output space  
        return output  
  
    def loss(self, y_pred, y_true):  
        return F.mse_loss(y_pred, y_true)  # mean squared error loss  
  
    def optimizer(self):  
        return Adam(self.parameters(), lr=self.lr, weight_decay=0.001)  
  
  
if __name__ == "__main__":  
    total_samples = 10000  
    input_size = 50  # input size  
    batch_size = 200  # batch size  
    epochs_num = 50  # number of epochs  
  
    x_train = torch.rand(total_samples, input_size) * 5  # generate random x values between 0 and 5  
    y_train = func(x_train) + 0.02 * torch.randn(total_samples, input_size)  # add noise to train y  
    dataloader = DataLoader(TensorDataset(x_train, y_train),  
                            batch_size=batch_size, shuffle=True)  # make data for batch training  
  
    model = SimpleTransformer(input_size=input_size, lr=0.001)  # define the model  
    optimizer = model.optimizer()  
    # automatically reduce the learning rate when the loss hit a plateau  
    scheduler = torch.optim.lr_scheduler.ReduceLROnPlateau(optimizer, patience=5, factor=0.5)  
  
    for epoch in range(epochs_num):  
        epoch_loss = 0  
        for x_train, y_train in dataloader:  
            y_pred = model(x_train)  
            loss = model.loss(y_pred, y_train)  
            optimizer.zero_grad()  
            loss.backward()  
            optimizer.step()  # update the parameters  
            epoch_loss += loss.item()  
  
        # no need to backward  
        avg_epoch_loss = epoch_loss / len(dataloader)  # calculate the average loss for each epoch  
        scheduler.step(avg_epoch_loss)  # reduce the learning rate when the loss is not improving  
  
        if epoch % 10 == 0:  
            with torch.no_grad():  
                model.eval()  
                x_true = torch.tensor(np.array([np.linspace(0, 5, input_size)]), dtype=torch.float32)  
                test_loss = model.loss(model(x_true), func(x_true)).item()  
  
            print(  
                f"Epoch {epoch}, Train Loss: {avg_epoch_loss}, Test Loss: {test_loss}")  # print the loss for each epoch  
  
    x_true = np.linspace(0, 5, input_size, dtype=np.double)  
    y_true = 2 * np.sin(x_true) + x_true ** 0.8  
  
    with torch.no_grad():  
        model.eval()  
        x_tensor = torch.tensor(x_true.reshape(1, -1), dtype=torch.float32)  
        y_pred = model(x_tensor).numpy().reshape(-1)  
  
  
    plt.plot(x_true, y_true, label='True')  
    plt.plot(x_true, y_pred, label='Pred')  
    plt.legend()  
    plt.show()  # plot the true and predicted y  
  
    print(y_pred)
```
note we use the <mark style="background: transparent; color: red">common dot-product attention mechanism to predict</mark>. 

and this is the result predicted : 
![[attachments/Pasted image 20250403132241.png|300]]
Appearantly, this model still struggles with capturing non-linearity, we give another more  complex  but even more precise  example as follows : 

```python
import numpy as np  
import torch  
from torch import nn  
import torch.nn.functional as F  
from torch.optim import Adam  
from torch.utils.data import DataLoader, TensorDataset  
import matplotlib.pyplot as plt  
  
  
def func(x):  
    # Define a simple nonlinear function  
    return 2 * torch.sin(x) + x ** 0.8  
  
  
class ImprovedTransformer(nn.Module):  
    def __init__(self, input_size, hidden_dim=64, num_heads=4, dropout=0.1):  
        super(ImprovedTransformer, self).__init__()  
        self.input_size = input_size  
        self.hidden_dim = hidden_dim  
  
        # Position encoding for better spatial awareness  
        self.register_buffer("position_ids", torch.arange(input_size).expand((1, -1)))  
        self.position_embeddings = nn.Embedding(input_size, hidden_dim)  
  
        # Input embedding  
        self.input_embedding = nn.Linear(1, hidden_dim)  
  
        # Multi-head attention with proper scaling  
        self.num_heads = num_heads  
        self.head_dim = hidden_dim // num_heads  
        self.q_proj = nn.Linear(hidden_dim, hidden_dim)  
        self.k_proj = nn.Linear(hidden_dim, hidden_dim)  
        self.v_proj = nn.Linear(hidden_dim, hidden_dim)  
        self.out_proj = nn.Linear(hidden_dim, hidden_dim)  
  
        # Feed-forward network  
        self.ff_layer1 = nn.Linear(hidden_dim, hidden_dim * 4)  
        self.ff_layer2 = nn.Linear(hidden_dim * 4, hidden_dim)  
  
        # Layer norms  
        self.norm1 = nn.LayerNorm(hidden_dim)  
        self.norm2 = nn.LayerNorm(hidden_dim)  
  
        # Output projection  
        self.output_projection = nn.Linear(hidden_dim, 1)  
  
        # Dropout  
        self.dropout = nn.Dropout(dropout)  
  
        # Initialize weights  
        self._init_weights()  
  
    def _init_weights(self):  
        for module in self.modules():  
            if isinstance(module, nn.Linear):  
                nn.init.xavier_uniform_(module.weight)  
                if module.bias is not None:  
                    nn.init.zeros_(module.bias)  
  
    def forward(self, x):  
        batch_size = x.shape[0]  
  
        # Reshape input for processing individual positions  
        x_reshaped = x.view(batch_size, self.input_size, 1)  
  
        # Create embeddings  
        inputs = self.input_embedding(x_reshaped)  # [batch_size, input_size, hidden_dim]  
  
        # Add position embeddings        position_embeddings = self.position_embeddings(self.position_ids)  
        hidden_states = inputs + position_embeddings  
  
        # Multi-head attention  
        residual = hidden_states  
        hidden_states = self.norm1(hidden_states)  
  
        # Project to queries, keys, values  
        q = self.q_proj(hidden_states).view(batch_size, -1, self.num_heads, self.head_dim).transpose(1, 2)  
        k = self.k_proj(hidden_states).view(batch_size, -1, self.num_heads, self.head_dim).transpose(1, 2)  
        v = self.v_proj(hidden_states).view(batch_size, -1, self.num_heads, self.head_dim).transpose(1, 2)  
  
        # Calculate attention scores with scaling  
        scores = torch.matmul(q, k.transpose(-1, -2)) / (self.head_dim ** 0.5)  
        attention_probs = F.softmax(scores, dim=-1)  
        attention_probs = self.dropout(attention_probs)  
  
        # Apply attention  
        context = torch.matmul(attention_probs, v)  
        context = context.transpose(1, 2).contiguous().view(batch_size, -1, self.hidden_dim)  
        hidden_states = self.out_proj(context)  
  
        # Add residual connection  
        hidden_states = residual + hidden_states  
  
        # Feed-forward network  
        residual = hidden_states  
        hidden_states = self.norm2(hidden_states)  
        hidden_states = self.ff_layer1(hidden_states)  
        hidden_states = F.gelu(hidden_states)  
        hidden_states = self.dropout(hidden_states)  
        hidden_states = self.ff_layer2(hidden_states)  
        hidden_states = self.dropout(hidden_states)  
        hidden_states = residual + hidden_states  
  
        # Project to output  
        output = self.output_projection(hidden_states).squeeze(-1)  
  
        return output  
  
    def loss_fn(self, y_pred, y_true):  
        return F.mse_loss(y_pred, y_true)  
  
  
if __name__ == "__main__":  
    # Set random seed for reproducibility  
    torch.manual_seed(42)  
    np.random.seed(42)  
  
    # Data parameters  
    total_samples = 500  # Increased samples for better coverage  
    input_size = 50  
    batch_size = 32  
  
    # Create training data with better distribution  
    # Strategy 1: Generate more samples in challenging regions    x_regions = [  
        torch.rand(total_samples // 2, input_size) * 5,  # Full range  
        torch.rand(total_samples // 4, input_size) * 2 + 1.5,  # Around the peak  
        torch.rand(total_samples // 4, input_size) * 2 + 3  # Descending region  
    ]  
    x_train = torch.cat(x_regions, dim=0)  
    y_train = func(x_train) + 0.01 * torch.randn(x_train.size())  # Reduced noise  
  
    # Create validation data for early stopping    x_val = torch.linspace(0, 5, input_size).repeat(100, 1)  
    y_val = func(x_val) + 0.01 * torch.randn(x_val.size())  
  
    # Create dataloaders  
    train_dataset = TensorDataset(x_train, y_train)  
    train_dataloader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True)  
    val_dataset = TensorDataset(x_val, y_val)  
    val_dataloader = DataLoader(val_dataset, batch_size=batch_size)  
  
    # Initialize model and optimizer  
    model = ImprovedTransformer(input_size=input_size)  
    optimizer = Adam(model.parameters(), lr=0.001, weight_decay=1e-6)  
    scheduler = torch.optim.lr_scheduler.ReduceLROnPlateau(  
        optimizer, mode='min', factor=0.5, patience=10, verbose=True, min_lr=1e-6  
    )  
  
    # Training with early stopping  
    num_epochs = 500  
    best_val_loss = float('inf')  
    patience = 20  
    patience_counter = 0  
  
    for epoch in range(num_epochs):  
        # Training  
        model.train()  
        train_loss = 0  
        for x_batch, y_batch in train_dataloader:  
            # Forward pass  
            y_pred = model(x_batch)  
            loss = model.loss_fn(y_pred, y_batch)  
  
            # Backward pass and optimization  
            optimizer.zero_grad()  
            loss.backward()  
            # Gradient clipping for stability  
            torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1.0)  
            optimizer.step()  
  
            train_loss += loss.item()  
  
        avg_train_loss = train_loss / len(train_dataloader)  
  
        # Validation  
        model.eval()  
        val_loss = 0  
        with torch.no_grad():  
            for x_batch, y_batch in val_dataloader:  
                y_pred = model(x_batch)  
                loss = model.loss_fn(y_pred, y_batch)  
                val_loss += loss.item()  
  
        avg_val_loss = val_loss / len(val_dataloader)  
  
        # Learning rate scheduling  
        scheduler.step(avg_val_loss)  
  
        # Print progress  
        if epoch % 20 == 0:  
            print(f"Epoch {epoch}, Train Loss: {avg_train_loss:.6f}, Val Loss: {avg_val_loss:.6f}")  
  
        # Early stopping  
        if avg_val_loss < best_val_loss:  
            best_val_loss = avg_val_loss  
            patience_counter = 0  
            # Save best model  
            best_model_state = model.state_dict().copy()  
        else:  
            patience_counter += 1  
            if patience_counter >= patience:  
                print(f"Early stopping at epoch {epoch}")  
                break  
  
    # Load best model  
    model.load_state_dict(best_model_state)  
  
    # Evaluate the model  
    x_true_np = np.linspace(0, 5, input_size, dtype=np.float32)  
    y_true_np = 2 * np.sin(x_true_np) + x_true_np ** 0.8  
  
    with torch.no_grad():  
        model.eval()  
        x_tensor = torch.tensor(x_true_np.reshape(1, -1), dtype=torch.float32)  
        y_pred = model(x_tensor).numpy().reshape(-1)  
  
    # Plot results  
    plt.figure(figsize=(10, 6))  
    plt.plot(x_true_np, y_true_np, label='True', linewidth=2)  
    plt.plot(x_true_np, y_pred, label='Pred', linewidth=2)  
    plt.legend(fontsize=12)  
    plt.title("Function Approximation with Transformer", fontsize=14)  
    plt.xlabel("x", fontsize=12)  
    plt.ylabel("y", fontsize=12)  
    plt.grid(True, alpha=0.3)  
    plt.show()
```

the above code  make a much accurate  prediction : 
![[attachments/Pasted image 20250403162524.png|450]] 
We note that in the above code both multi-head mechanism, temperature and  attention  scoring functions are used, which we would  explain further in the later section. 

##  2. Attention  Scoring Functions 
#### 1. Concept 
We used a gaussian kernal in the above case to <mark style="background: transparent; color: red">model the  interaction between queries and  keys</mark>. 

In general attention pooling process, keys and values are automatically generated in the model. and the attention weights are computed by keys and queries, and then feed into softmax function to calculate attention weights,  finally <mark style="background: transparent; color: red">compute the weighted values according to the  attention weights as output</mark>. 
![[attachments/Pasted image 20250403171015.png|500]] 

Then the  attention pooling  $f$ is defined  as : 
$$f(q, (k_{1}, v_{1} ) , \dots  (k_{m}, v_{m})) =  \sum_{i = 1}^{m} \alpha (q, k_{i}) v_{i} \in  R^{v}$$
where  $\alpha$ is the <mark style="background: transparent; color: red">attention scoring function</mark> to calculate the attention weights. <b><mark style="background: transparent; color: orange">Different choices of the attention scoring function a lead to different behaviors of attention pooling.</mark></b> We introduce two scoring function. 

#### 2.  Masked Softmax Operation  
Subce the softmax function is for map the calcuated weights to the sum of 1. so here we implement a `masked_softmax` operation. which will  **mask any  value beyond the valid length as zero**. 

for $X$, the tensor is a vector with size `batch_size x input_size x embed_size`, the following code 

```python
def masked_softmax(X, valid_lens):
    """Perform softmax operation by masking elements on the last axis."""
    # X: 3D tensor, valid_lens: 1D or 2D tensor
    def _sequence_mask(X, valid_len, value=0):
        maxlen = X.size(1)
        mask = torch.arange((maxlen), dtype=torch.float32,
                          device=X.device)[None, :] < valid_len[:, None]
        X[~mask] = value
        return X
    
    if valid_lens is None:
        return nn.functional.softmax(X, dim=-1)
    else:
        shape = X.shape
        if valid_lens.dim() == 1:
            valid_lens = torch.repeat_interleave(valid_lens, shape[1])
        else:
            valid_lens = valid_lens.reshape(-1)
        # On the last axis, replace masked elements with a very large negative
        # value, whose exponentiation outputs 0
        X = _sequence_mask(X.reshape(-1, shape[-1]), valid_lens, value=-1e6)
        return nn.functional.softmax(X.reshape(shape), dim=-1
```

in the above function, ==firstly it reshape $X$ into an 2 dim array==, then it use  an arange function to travel around the batch size, use  `[None, :]` is add a dimension in first (same as `.unsqueeze(0)`) and just mask the first dimension. where  `input_size  > valid_len(i)` (for batch_size it also compatible by `resize` array). 

for example,  
```python
masked_softmax(torch.rand(2, 2, 4), torch.tensor([2, 3])) 
# it mask the final specified dimensions into 0 
tensor([[[0.4818, 0.5182, 0.0000, 0.0000],
	[0.7267, 0.2733, 0.0000, 0.0000]],
	[[0.4077, 0.3150, 0.2773, 0.0000],
	[0.3726, 0.3198, 0.3076, 0.0000]]])

# also compatible for multi-mask operation 
masked_softmax(torch.rand(2, 2, 4), torch.tensor([[1, 3], [2, 4]]))   
tensor([[[1.0000, 0.0000, 0.0000, 0.0000],
	[0.4266, 0.2706, 0.3028, 0.0000]],
	[[0.6468, 0.3532, 0.0000, 0.0000],
	[0.3808, 0.1502, 0.2060, 0.2630]]]) 
```

### 3. Scoring Functions 
#### 1) Additive attention  
We can use  additive attention as  scoring function. which is :  
$$\boxed{\Large\alpha (q , k) = w_{v}^{T} \tanh (W_{q} q + W_{k}  k)}$$
where $W_q$ and $W_k$ are learnable parameters. $W_q \in R^{h \times q}$ and  $W_{k} \in R^{h \times  k}$,  also, $w_{v} \in R^{h}$, where $h$ is the hidden size. 

in the textbook, the $W_{k}, W_{q}$ and $W_{v}$ are initialized by `LazyLinear` function. and the shape of samples is  `(batch size, number of steps or sequence length in tokens, feature size)` 

```python
def additive_score(queries, keys):
	features  = (self.W_q(querys).unsqueeze(2) + self.W_k(keys).unsqueeze(1)).squeeze  (-1)   # since w_v is input x 1 linear
	scores = self.W_v ( torch.tanh (features)) ;
	return scores

self.attention_weights = masked_softmax(scores, valid_length);
```

#### 2. Scaled Dot-Product  Attention 
the dot-product attention is using `nn.matmul(queries, keys)` as the attention function. We assume that the  product operation requires <mark style="background: transparent; color: red">both query and key have the same length</mark> $d$ (we will map the input into hidden keys and queries), then 
$$a (q, k) = \frac{q^{T} k  }{ \sqrt{d}} \qquad  y =  \text{softmax} \left(\frac{QK^{T}}{\sqrt{d}}\right) V \in R^{n \times  v}$$
also we use dropout for model regularization. 

```python
class DotProductAttention(nn.Module):  
    def __init__(self, dropout=0.1, num_heads=8):  
        super(DotProductAttention, self).__init__()  
        self.dropout = nn.Dropout(dropout)  
        self.num_heads = num_heads  
  
    def forward(self, queries, keys, values, mask=None):  
		 # queries: [batch_size, num_queries, query_dim]  
        assert queries.size()[-1] == keys.size()[-1] and queries.size()[-1] == values.size()[-1]  
        d = queries.size()[-1]  
        scores = torch.bmm(queries, keys.transpose(1, 2)) / (d ** 0.5)
        self.attn = F.softmax(scores, dim=-1)  # use masked_softmax to handle the mask 
        y_pred = torch.bmm(self.dropout(self.attn), values)  
        return y_pred   # return the predicted values
```

we note that num_heads are to be implemented. 

> [!HINT] The application of additive attention scoring function and  scaled-dot product scoring function 
> When queries and keys are ==vectors of different lengths==, we can use the additive attention scoring function. **When they are the same, the scaled dot-product attention scoring function is more computationally efficient**. 
> since we know that Additive attention function use $W_q, W_{k}$ for the uniform of the queries and keys, we can also use it in  scaled-dot production to fit the different-length input. but it must be specified. 

## 3. Bahadanau Attention 
We recall that we use  [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter7 RNN#(3) Encoder|RNN encoder]] to transform the vary-length input sequence context to fixed-shape variable. 

Graves designed a differentiable attention model to align text characters with the much longer pen trace, where the alignment moves only in one direction. Bahdanau et al. proposed a differentiable attention model without the severe unidirectional alignment limitation. **==if not all the input tokens are relevant, the model aligns (or attends) only to parts of the input sequence that are relevant to the current prediction.==** This is achieved by treating the context variable as an output of attention pooling.  

### (1) Attention Modeling 
we represent the  context variable $c$ here by $c_{t'}$ as decoding time step $t'$,  and <mark style="background: transparent; color: red">the context variable at decoding time step t' is the output of attention pooling</mark>  as : 
$$c_{t'} = \sum_{t = 1}^{T} \alpha (s_{t'-1}, h_{t}) h_{t}$$

different from the encoder-decoder structure in chapter 7 : 
![[attachments/Pasted image 20250401114238.png|450]] 

we add embedding layer  before each recurrent layer. and  the model constructed as follows : 
![[attachments/Pasted image 20250404092356.png]]
recall [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter7 RNN#(3) Encoder|nn.Embedding]] here, we may use Embedding layer  to format the source sequence and  target sequence. note that besides the  context parameter $c$,  this network also use Attention parameter, which $\alpha$ can be calculated in section 2.3.  

### (2) Decoder with Attention 
We call what we define as AttentionDecoder to implement the RNN encoder-decoder with Bahdanau attention. we can initialize the  

> [!HINT] Application Input 
> In small cases the $k,v$ input is self-attention, which $q$ and $k,v$ are from the same  sequence. (BERT text processing)
> But in real cases,  cross-attention is applied  more extensively. For example, in machine translation application, the queries $Q$ is the target language to translated to,  and $k,v$ is from the  original language sequence.  

```python
class AttentionDecoder(nn.Module, ABC):
    """The base attention-based decoder interface in PyTorch."""
    def __init__(self):
        super().__init__()
    
    @property
    @abstractmethod
    def attention_weights(self):
        """Returns the attention weights from the most recent forward pass.
        Returns:
            torch.Tensor: The attention weights with shape (batch_size, num_steps, num_queries)
        """
        raise NotImplementedError
```

For a Seq2Seq Implementation, we can define a simple network with GRU and a dense layer. 

-  At each decoding time step, the **decoder final-layer hidden state at the previous time step is used as the query of the attention. As a result, ==both the attention output and the input embedding are concatenated as input of the RNN decoder**==. 

-  note that we use `init_state` to make a `state` variable at first time stamp. 

```python
class Seq2SeqAttentionDecoder(AttentionDecoder):
    def __init__(self, vocab_size, embed_size, num_hiddens, num_layers,
                 dropout=0.1):
        super().__init__()
        self.embedding = nn.Embedding(vocab_size, embed_size)
        self.rnn = nn.GRU(embed_size + num_hiddens, num_hiddens, num_layers,
                          dropout=dropout)
        self.dense = nn.Linear(num_hiddens, vocab_size)
        # For storing attention weights
        self._attention_weights = None 
        
    def init_state(self, enc_outputs, enc_valid_lens):
        # outputs shape: (batch_size, num_steps, num_hiddens)
        # hidden_state shape: (num_layers, batch_size, num_hiddens)
        outputs, hidden_state = enc_outputs
        return (outputs.permute(1, 0, 2), hidden_state, enc_valid_lens)
    
    def forward(self, X, state):
        # X shape: (batch_size, num_steps)
        # state: (outputs, hidden_state, enc_valid_lens)
        enc_outputs, hidden_state, enc_valid_lens = state
        X = self.embedding(X).permute(1, 0, 2)  # (num_steps, batch_size, embed_size)
        
        outputs = []
        self._attention_weights = []
        for x in X:
            # x shape: (batch_size, embed_size)
            # hidden_state shape: (num_layers, batch_size, num_hiddens)
            query = hidden_state[-1].unsqueeze(1)  # (batch_size, 1, num_hiddens)
            
            # Simple dot-product attention
            # enc_outputs shape: (batch_size, num_steps, num_hiddens)
            scores = torch.bmm(query, enc_outputs.permute(0, 2, 1))
            scores = scores.squeeze(1)  # (batch_size, num_steps)
            
            # Masking for valid lengths
            mask = torch.arange(scores.shape[1], device=X.device)[None, :] < enc_valid_lens[:, None]
            scores[~mask] = -1e6
            
            attention_weights = torch.softmax(scores, dim=1)
            self._attention_weights.append(attention_weights)
            
            # Context vector
            context = torch.bmm(attention_weights.unsqueeze(1), enc_outputs)
            context = context.squeeze(1)  # (batch_size, num_hiddens)
            
            # Concatenate input and context
            x_and_context = torch.cat((x, context), dim=1)
            
            # RNN step
            out, hidden_state = self.rnn(x_and_context.unsqueeze(0), hidden_state)
            outputs.append(out)
        
        outputs = torch.cat(outputs, dim=0)  # (num_steps, batch_size, num_hiddens)
        outputs = self.dense(outputs)  # (num_steps, batch_size, vocab_size)
        return outputs.permute(1, 0, 2), [enc_outputs, hidden_state, enc_valid_lens]
    
    @property
    def attention_weights(self):
        return torch.stack(self._attention_weights, dim=1)  # (batch_size, num_steps, num_queries)
```

note that we use  `Seq2SeqEncoder` implemented in [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter7 RNN#(3) Encoder|Chapter7 RNN]], with  `return outputs, hidden` return both output and  hidden state. 

> [!NOTE]
> Bahdanau attention treats the decoder hidden state at the previous time step as the query, and the encoder hidden states at all the time steps as both the keys and values. 

## 4. Multi-head Attention
In the case with same set of queries, keys and values, <mark style="background: transparent; color: red">we may want the model to combine knowledges from</mark> <b><mark style="background: transparent; color: orange">different behaviors of the samee attention mechanism. for example, we may need shorter-range and longer-range message  in a sequence</mark></b>. 

So it may be benificial to allow the attention  to **jointly use different representation subspaces  of queries, keys and values.**  

<b><mark style="background: transparent; color: orange">Instead of performing a single  attention pooling</mark></b>, we ==transform queries, keys and values with $h$ independently learned linear projections==. which leads to the following network structure :
![[attachments/Pasted image 20250407104555.png|350]]

firstly, we compute <b><mark style="background: transparent; color: orange">each attention head as</mark></b> :
$$h_{i}  =   f(W_{i}^{(q)}q , W_{i}^{(k)}k , W_{i}^{(v)} v) \in  R^{p_{v}}$$
where $f$ is attention pooling. Also <b><mark style="background: transparent; color: orange">the  multi-head attention output</mark></b> is another linear transformation via learnable parameters $W_{o}$ ,  which <mark style="background: transparent; color: red">combine the output  value of different heads together</mark> : 
$$W_{o} \in R^{p_{o}  \times  h p_{v}}$$
and  finally 
$$\text{output} = W_{o} (\boldsymbol{h}) \in  R^{p_{o}}$$
