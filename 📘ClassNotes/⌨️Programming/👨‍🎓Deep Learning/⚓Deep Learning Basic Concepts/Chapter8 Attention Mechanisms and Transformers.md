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
but for the nonparametric  attention pooling, we **use the following  attention mechanism**  : 
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
```python fold
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
in the above case,  the **keys are the training points `x_train`, and the queries are the validation points `x_val`**. The <b><mark style="background: transparent; color: orange">attention weights are computed based on the difference</mark></b> (`diff`) between **queries and keys**. 


the result is  as follows : 
![[attachments/Pasted image 20250401222341.png|450]]
which shows the weight of each  point at every validation point. 

We note in non-parametric pooling,   **keys and values should have the  same length**.  which ==use 1key  to mapped to 1 value==, constructing the following attention structure : 
![[Excalidraw/Chapter8 Attention Mechanisms, Transformers 2025-04-22 09.59.21|400]]
we note for the later dot-product attention, we **use the hidden layer weight for the parameter in the attention**. 

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

```python fold  title:single_head_implementation 
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
        """        
        # use scaled dot-product attention  
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

> [!NOTE] Embedding Dimesion for reshape the input size 
>  for the  Transformer input, we firstly add a Embedding layer  before it to reshape the input to the correct size (and oftren neglect this layer when comparing complexity with others) and the real input dim for Transformer can be seen  as : 
>  $$\text{seq  len} \space \times \text{hidden dim}$$  
>  where real input adds `batch size` before seq_len dim 
>  

we firstly give a full implementation by [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Code Snippets/my_transformer|my_transformer]] and the following result is given (since transformer need a lot of data for training)
![[attachments/Pasted image 20250424164638.png]] 

We note that in the above code both **multi-head mechanism, temperature and  attention  scoring functions** are used, which we would  explain further in the later section. 

##  2. Attention  Scoring Functions 
#### 1. Concept 
We used a gaussian kernal in the above case to <mark style="background: transparent; color: red">model the  interaction between queries and  keys</mark>. 

In general attention pooling process, keys and values are automatically generated in the model. and the attention weights are computed by keys and queries, and then feed into softmax function to calculate attention weights,  finally <mark style="background: transparent; color: red">compute the weighted values according to the  attention weights as output</mark>. 
![[attachments/Pasted image 20250403171015.png|500]] 

Then the  attention pooling  $f$ is defined  as : 
$$f(q, (k_{1}, v_{1} ) , \dots  (k_{m}, v_{m})) =  \sum_{i = 1}^{m} \alpha (q, k_{i}) v_{i} \in  R^{v} \tag{11.3.1}$$
where  $\alpha$ is the <mark style="background: transparent; color: red">attention scoring function</mark> to calculate the attention weights. <b><mark style="background: transparent; color: orange">Different choices of the attention scoring function a lead to different behaviors of attention pooling.</mark></b> We introduce two scoring function. 

#### 2.  Masked Softmax Operation  
Subce the softmax function is for map the calcuated weights to the sum of 1. so here we implement a `masked_softmax` operation. which will  **mask any  value beyond the valid length as zero**. 

for $X$, the tensor is a vector with size `batch_size x input_size x embed_size`, the following code mask the valid data length  to a fixed `valid_len` and  mask the  data out of  valid len to a value (which is oftenly  about $-\infty$)

We firstly figure out the following questions : 

> [!question] what does softmax mask in real case? keys or queries?
> The input `x` of  the softmax is the score of shape `[batch_size, num_queries, num_keys]`
> 
> - It encodes, ==**for each query**, how many of the K key/value positions are valid== (i.e. not padding).
> - In practice those numbers all come from the **key sequence** (or encoder output) lengths.  

#####  What shape can `valid_lens` take?
Two common options :
1. **Per‐batch** (*one length per example*)
    ```python
    valid_lens.shape == (B,)
    # e.g. [5, 7, 6, 8, …]  # each between 0 and K
    ```
The implementation then **broadcasts** that same length across all Q queries in each example. 
2. **Per‐query** (*one length per example per query*)
    ```python
    valid_lens.shape == (B, Q)
    # e.g. [[5,5,5,5,…], [7,7,7,7,…], …]
    ```

You might use this if, for some reason, different query positions need different key‐masks (uncommon in standard cross‐attention).

we note when generating the `valid_lens` in practice,  we may use : 
```python fold title:valid-len-generation
# keys: LongTensor of shape (B, K), with pad_idx marking padding
pad_idx = 0
non_pad_mask = (keys != pad_idx).long()      # (B, K) of 1s and 0s
key_lengths  = non_pad_mask.sum(dim=1)       # (B,) counts per example

# Option A: use per‐batch lengths
valid_lens = key_lengths                    # shape (B,)

# Option B: expand to per‐query if you want
valid_lens = key_lengths.unsqueeze(1).repeat(1, Q)  # shape (B, Q)
```

- Finally,  we give the implementation as follows : 
```python fold title:implementation 
def masked_softmax(x, valid_lens: torch.Tensor, masked_value=-1e9):
    """
    Used for softmax layer after the attention calculation

    :param x: 3D tensor, attention scores shaped [batch_size, queries_len, keys_len]
    :param valid_lens: **array of valid lens** of each key, to mask the future tokens and perform other functions
    :param masked_value: value to fill mask
    :return:
    """
    b, q, k = x.shape
    if valid_lens.dim() == 1:  # [batch_size]
        valid_lens = valid_lens.repeat_interleave(q)  # [b * q] 
    else:
        assert valid_lens.shape == (b, q)
        valid_lens = valid_lens.reshape(-1)  # [b * q]

    # Flatted by keys
    x_flat = x.reshape(-1, k)  # [b * q, k]
    
    # [k, 1] <= [b * q] -> [k, b * q]
    mask = (torch.arange(k).unsqueeze(-1) < valid_lens).transpose(0, 1)  # [b * q, k] 
    x_flat_masked = x_flat.masked_fill(~mask, masked_value)
    return nn.functional.softmax(x_flat_masked, dim=-1).reshape(x.shape)
```

### 3. Scoring Functions
#### 1) Additive attention
We can use  additive attention as  scoring function. which is :  
$$\boxed{\Large\alpha (q , k) = w_{v}^{T} \tanh (W_{q} q + W_{k}  k)}$$
where $W_q$ and $W_k$ are learnable parameters. $W_q \in R^{h \times q}$ and  $W_{k} \in R^{h \times  k}$,  also, $w_{v} \in R^{h}$, where $h$ is the hidden size. 

In the textbook, the $W_{k}, W_{q}$ and $W_{v}$ are initialized by `LazyLinear` function. and the shape of samples is  `(batch size, number of steps or sequence length in tokens, feature size)` 

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
also we use dropout for model regularization. `

```python
class DotProductAttention(nn.Module):  
    def __init__(self, dropout=0.1):  
        super(DotProductAttention, self).__init__()  
        self.dropout = nn.Dropout(dropout)  
        self.attn = None  
  
    def forward(self,  
                queries: torch.Tensor,  
                keys: torch.Tensor,  
                values: torch.Tensor,  
                valid_lens=None,  
                window_mask=None):  
        """  
        :param queries: [batch_size, query_len, num_hiddens]   -> which is query sentence batch        :param keys: [batch_size, key_len, num_hiddens]        :param values: [batch_size, value_len, num_hiddens]        :param valid_lens: masked by valid_lens        :param window_mask: masked by window [num_windows, query_len, key_len]  
        note : window mask is for constraint the attention range        :return:  
        """  
        #  in attention mechanism, key_len == value_len, since we need to bmm the key and value  
        assert keys.shape[1] == values.shape[1]  
        d = torch.tensor(queries.shape[-1])  # d = hiddens_dim  
        scores = torch.bmm(queries, keys.transpose(1, 2)) / torch.sqrt(d)   
        attn = masked_softmax(scores, valid_lens)  # [batch_size, query_len, key_len]  
        self.attn = self.dropout(attn)  
        return torch.bmm(attn, values)  # [batch_size, query_len, num_hiddens]
```

we note that num_heads are to be implemented. 

> [!HINT] The application of additive attention scoring function and  scaled-dot product scoring function 
> When queries and keys are ==vectors of different lengths==, we can use the additive attention scoring function. **When they are the same, the scaled dot-product attention scoring function is more computationally efficient**. 
> since we know that Additive attention function use $W_q, W_{k}$ for the uniform of the queries and keys, we can also use it in  scaled-dot production to fit the different-length input. but it must be specified. 

#### 3. Window mask  
window mask is applied on the scores, we can give the implementation  of the window mask as follows :  
```python fold title:apply-window-mask 
def apply_window_mask(scores, window_mask):  
    """  
    Apply window mask to attention scores    :param scores: Attention scores tensor of shape [batch_size, query_len, key_len]    :param window_mask: Window mask tensor of shape [num_windows, query_len, key_len]    :return: Masked scores with the same shape as input scores  
    """   
     if window_mask is None:  
        return scores  
  
    b, q, k = scores.shape  
    assert window_mask.shape[1] == q and window_mask.shape[2] == k  
    num_windows = window_mask.shape[0]  
    assert b % num_windows == 0  
  
    # for multi-head attention,  
    scores = scores.reshape(  
        (b // num_windows, num_windows, q, k)  
        # [b * h (lurk), w, q, k]  
    ) + window_mask[None, :, :, :]  # broadcast the window mask by (b * h)  
  
    return scores.reshape(b, q, k)  # combine first 2 dims
```

then just use the following:  
```python
scores = apply_window_mask(scores, window_mask) 
```

## 3. Bahadanau Attention 
We recall that we use  [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter7 RNN#(3) Encoder|RNN encoder]] to transform the vary-length input sequence context to fixed-shape variable. 

Graves designed a differentiable attention model to align text characters with the much longer pen trace, where the alignment moves only in one direction. Bahdanau et al. proposed a differentiable attention model without the severe unidirectional alignment limitation. **==if not all the input tokens are relevant, the model aligns (or attends) only to parts of the input sequence that are relevant to the current prediction.==** This is achieved by treating the context variable as an output of attention pooling. 
### (1) Attention Modeling 
we represent the  **context variable $c$ here by $c_{t'}$ as decoding time step** $t'$,  and <mark style="background: transparent; color: red">the context variable at decoding time step t' is the output of attention pooling</mark>  as : 
$$c_{t'} = \sum_{t = 1}^{T} \alpha (s_{t'-1}, h_{t}) h_{t}$$

different from the encoder-decoder structure in chapter 7 : 
![[attachments/Pasted image 20250401114238.png|450]] 
we add embedding layer  before each recurrent layer. and  the model constructed as follows : 
![[attachments/Pasted image 20250404092356.png|400]]
recall [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter7 RNN#(3) Encoder|nn.Embedding]] here, we may use Embedding layer  to format the source sequence and  target sequence. note that **besides the  context parameter $c$,  this network also use Attention parameter**, which $\alpha$ can be calculated in section 2.3. 

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

note that we use  `Seq2SeqEncoder` implemented in [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter7 RNN#(3) Encoder|Chapter7 RNN]], with  `return outputs, hidden` return **both output and  hidden state**. 

> [!NOTE]
> Bahdanau attention treats the decoder hidden state at the previous time step as the query, and the encoder hidden states at all the time steps as both the keys and values. 

## 4. Multi-head Attention
### (1) Multi-head model 
The Attention layer has its own library (see [nn.MultiheadAttention](https://pytorch.org/docs/stable/generated/torch.nn.MultiheadAttention.html) ) :   
```python fold title:
from torch import nn 
multihead_attn = nn.MultiheadAttention(embed_dim, num_heads)
attn_output, attn_output_weights = multihead_attn(query, key, value)
```

#### 1. Concepts 
In the case with same set of queries, keys and values, <mark style="background: transparent; color: red">we may want the model to combine knowledges from</mark> <b><mark style="background: transparent; color: orange">different behaviors of the samee attention mechanism. for example, we may need shorter-range and longer-range message  in a sequence</mark></b>. 

So it may be benificial to allow the attention  to **jointly use different representation subspaces  of queries, keys and values.**  

<b><mark style="background: transparent; color: orange">Instead of performing a single  attention pooling</mark></b>, we ==transform queries, keys and values with $h$ independently learned linear projections==. which leads to the following network structure : 
![[attachments/Pasted image 20250407104555.png|350]]
firstly, we compute <b><mark style="background: transparent; color: orange">each attention head as</mark></b> : 
$$h_{i}  =   f(W_{i}^{(q)}q , W_{i}^{(k)}k , W_{i}^{(v)} v) \in  R^{p_{v}}$$
where $f$ is attention pooling. Also <b><mark style="background: transparent; color: orange">the  multi-head attention output</mark></b> is another linear transformation via learnable parameters $W_{o}$ ,  which <mark style="background: transparent; color: red">combine the output  value of different heads together</mark> : 
$$W_{o} \in R^{p_{o}  \times  h p_{v}}$$
$h$ <mark style="background: transparent; color: red">attention pooling outputs are concatenated and transformed</mark>. 
$$\text{output} = W_{o} (\boldsymbol{h}) \in  R^{p_{o}}$$

> [!HINT] Pytorch `num_hiddens` initialization 
> We note that **in pytorch,  the input dimension d_model determines the size of the hidden layer**. (For FFN, the hidden size is  `4 * d_model`)  
> If the hidden layer need to be custimized, we can inherit `nn.TransformerEncoderLayer` and re-write the feed-forward  network. 

also, in multi-head attention process, the <mark style="background: transparent; color: red">hidden_layer are split into  different  groups, </mark> 

> [!HINT] complexity of the transformers
> **since the input keys and queries is of shape $n \times d$**,  and we calculate W as [[#2. Scaled Dot-Product Attention]] ($Q K^T/\sqrt{d}$) , which made a  $n \times d$ mat mutplicate with $d \times n$ matrix, 
> we have :
> $$\text{step 1 mat mul complexity} =  n \times n  \times  d$$
> then the output value ($n \times  n$) need another multipliucation with  $n \times d$  value matrix :
> $$\text{step 2 mat mul complexity} =  n \times  d \times  n $$
> so the overall complexity is $O(n^2 d)$ with  maximum path length $O(1)$, which is more effecient than recurrent layer ($nd^2$ with  length $O(n)$) and CNN $(k n d^2)$, where $k$ is kernel size. 
> - Also note quadratic computational complexity with respect to the sequence length makes self-attention prohibitively slow for very long sequences. 

#### 2. How to calculate Multi-head attention model 

> We note that  multi-head attention is just split the hidden-layer into num-heads  and combine it into batch_size 

While using **the multi-head attention**. since queries, keys and values are shaped of  
$$(\text{batch size} \times \text{seq len} \times \text{hidden size})$$
we will reshape `keys, queries` and `values` by n-heads as follows : 
- split the data by n-heads 
- place the head dimension on the second dimension
- combine the heads dimension to batch_size dimension

and finally resulted shape is : 
$$(\text{batch size} \times \text{head num}) \times \text{seq len} \times    \frac{\text{hidden size}}{\text{head num}}$$
so we give the following multi-head implementaion : 
```python fold title:muti-head-implementation 
def transpose_by_num_heads(X, num_heads):
    """
    reshape the x by num-heads 
    """ 
    b, s, h = X.shape
    assert h % num_heads == 0, "hidden size must be the "
    X = X.reshape(b, s, num_heads, h//num_heads)
    # reshape the num-heads into the second dim 
    X = X.permute(0, 2, 1, 3)  
    X = X.reshape(b * num_heads, s, -1)
    return X
```

```python fold title:test-code 
# test the above code : 
queries = torch.rand(5, 10, 256)
n_heads = 4
q_by_heads = transpose_by_num_heads(queries, n_heads) 
q_by_heads.shape # torch.Size([20, 10, 64]) 
```
then we just use it as a larger batch_size q,k,v.

Also see [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/🔥Pytorch Basics/torch.bmm 批量矩阵相乘的使用方法|torch.bmm usage]] 
### (2) Self-Attention  and  cross attention   
**usually** in a transformer,  we <b><mark style="background: transparent; color: orange">project the input into k,q, v</mark></b>, and just use an $X$ for the input of transformer 

<b><mark style="background: transparent; color: orange">There are two main cases</mark></b>: 
1. **Self-attention (standard in transformers):** 
    - Q, K, and V come from the **same source**.
    - But you still use different linear projections for them.
2. **Cross-attention (like in encoder-decoder transformers, e.g., T5, BERT2GPT):**  
    - Q comes from one input (e.g., a question),
    - K and V come from another (e.g., a context passage).
    - Example: 

For self-attention,  we have the following: 
$$\Large  \boxed{y_{i} = f (x_{i} , (x_{1}, x_{1}),  \dots  (x_{n}, x_{n}))  \in  R^{d}}$$
where $x_1,  x_2, \dots$  are tokens.
which replaces the  attention pooling f explained in [[#2. Attention Scoring Functions]] in  equation (11.3.1), which <mark style="background: transparent; color: red">just use queries as keys </mark>. 

### (3) Positional Encoding  
#### 1. Basic Conceptions 
For self-attention machinisms, it ditches the input for  parallel computaion (multi-heads attention). 

However, <b><mark style="background: transparent; color: orange">the attention machinisms can't retain the positional information, which leads to poor performance for positional sequence.</mark></b> To <mark style="background: transparent; color: red">retain the positional information, </mark>we can <b><mark style="background: transparent; color: orange">add positional Encoding layer before it</mark></b>. Here we describe a fixed positional encoding based on sine and cosine functions. 

For the input reprensentation $X\in R^{n \times  d}$, where hidden size is $d$ for $n$ token sequence, We **use a positional  embedding matrix** $P \in R^{n \times d}$ **of the same shape as $X$**. and **output $X+P$ instead of just $X$**.

The element of matrix $P$ the $i^{th}$ row and  $(2j)^{th}$ column are as : 
$$p_{i, 2j} =  \sin \left(\frac{i}{10000^{\frac{2j}{d}}}\right)$$
$$p_{i, 2j + 1} = \cos  \left(\frac{i}{10000^{\frac{2j}{d}}}\right)$$
in the denominator part, for different rows $i$, the sin function gives different proportion (such as $\sin x, \sin 2x, \sin nx$), and the denominator gives maximum $10000^{2}$ quotient to **limit the minimum of $i$ in $\frac{\pi}{2}$ range**.  

> [!warning] `max_len` and `d_model` size
> 1. The  sin and cos function remains the same  in the seq_len  direction. 
> 2. `d_model (num_hiddens)` needn't to be very big.  
> 3. Since the positional encoding <mark style="background: transparent; color: red">takes the though of binary encoding (just  make it continuous)</mark> (see [designing-positional-encoding-blog](https://huggingface.co/blog/designing-positional-encoding)), it is better that $2^{d\_{model}} >= max\_seq\_len$, then the result of each encodinged position wouldn't repeat.  

![[Excalidraw/Chapter8 Attention Mechanisms, Transformers 2025-04-28 20.48.20|500]]
In the positional embedding matrix P ,  <u>rows correspond to positions within a sequence and columns represent different positional encoding dimensions.</u>  
> [!NOTE] Position and Encoded Dimension 
> we refer **the $x$ range from 0 to `max_len` as the `position`** and the y(j) range from 0 to `num_hiddens` as `encoded_dimension` 

in the above figure, we see that the lower encoding dimension have higher frequency. (Frequency decreased monotonically along the encoding dimension relates to absolute positional information).  

> [!question] Dropout Settings 
> <b><mark style="background: transparent; color: orange">We often set dropout here as 0 when training the network</mark></b>, why the dropout should be set as 0 ?  
> 
> In Positional Encoding, the encoding  result contains the Position information,  which is important for training. If we set some of it as 0, it may repeat since it is actually a binary-encoding method. **leading to unstable training process** (if there is early stop, the process would stop incorrectly).  
> 
> <mark style="background: transparent; color: red">The Common Practice is to set the dropout in positional encoding as 0, and add dropout on embedding layer</mark>, which retains the most of position information. 

> [!HINT] difference when use `model.train()` and `model.eval()` 
> In `model.train()`, the dropout is taken into consideration while in `model.eval()` it not.  
> So we may delete `model.train()` in train process when debugging  to check the dropout problem (whether or not use dropout)

#### <b><mark style="background: transparent; color: orange">Real case implementation</mark></b> 
We note we have the relationship : 
$$\exp(a \ln b )=  \exp  (\ln b^{a} ) = b^{a}$$
so we can replace the 
$$\frac{i}{10000^{\frac{2j}{d}}}  = i \times   \exp\left(-  \frac{2j}{d } * \ln  10000  \right)$$
we can firsly calculate  $- \frac{2j}{d} * \ln 10000$. 

since `10^0.001` is really small number (when $n$ is large ), which leads to the result become `inf`, so <mark style="background: transparent; color: red">the  original formula is unstable </mark>and can be substitute by the following : 
```python fold title:optimized-equation 
div_term = torch.exp(np.arange(0, d_model, 2) * -(math.log(10000.0) / d_model))  
  
pe = torch.zeros(d_model, max_len)  
pe[0::2, :] = torch.sin(position * div_term)  
pe[1::2, :] = torch.cos(position * div_term)
```

**We give an implementation as follows** : 
```python fold title:implementation_of_position_encoder
import numpy as np  
import math  
  
  
class PositionalEncoding(nn.Module):  
    """Apply sine/cosine positional encoding to input sequence."""  
  
    def __init__(self, max_seq_len, d_model, dropout=0):  
        """  
        :param max_seq_len: size of input sequence, must >= seq_len        :param d_model:  length of the  embedding encode sequence (hidden_size)  
        : warning : d_model should not to be too big, about 64 or 128 is recommended.  
        pe dimension : [d_ , hidden_dim]       
         """        
        super().__init__()  
        self.position = torch.arange(max_seq_len).unsqueeze(1).float()  # [max_seq_len, 1]  
        self.div_term = torch.exp(torch.arange(0, d_model, 2) * (-math.log(10000.0) / d_model))
        # [d_model/2]  
        pe = torch.zeros(max_seq_len, d_model)  # [max_seq_len, d_model], which would be broadcast by batch_size  
        pe[:, 0::2] = torch.sin(self.position * self.div_term)  
        pe[:, 1::2] = torch.cos(self.position * self.div_term)  
  
        # Shape: (d_model , max_len) to broadcast by batch_size  
        self.register_buffer('pe', pe.unsqueeze(0))  # [1, max_seq_len, d_model]  
        self.dropout = nn.Dropout(dropout)  
  
    def forward(self, x):  
        """  
        add positional encoding to input sequence x.  
        note: seq_len <= max_seq_len, so we can use positional encoding  
        :param x: [batch_size, seq_len, d_model]        :return: [batch_size, seq_len,  d_model]  
        """       
         assert x.ndim == 3 and x.shape[2] == self.pe.shape[-1], 'Input shape should be [batch_size, seq_len, d_model]'  
        seq_len = x.shape[1]  
        x = x + self.pe[:, :seq_len]  
        return self.dropout(x)  # [batch_size, seq_len, d_model]  
  
    def draw_encoding_figure(self):  
        plt.figure(figsize=(15, 15))  
        plt.subplot(2, 1, 1)  # plot the positional encoding  
        plt.pcolormesh(self.pe[0].transpose(0, 1).detach().numpy(), cmap='RdBu')  
        plt.ylabel('Encoded dimension(Columns)')  
        plt.xlabel('Position(Rows)')  
        plt.colorbar()  
        plt.title('Positional Encoding')  
  
        # plot first several rows sine and cosine value  
        plt.subplot(2, 1, 2)  
        for i in range(6, 10):  
            # plot the values  according to seq_len as x-axis  
            plt.plot(self.pe[0, 0:50, i].detach().numpy(), label=f'Embedding {i}')  
        plt.legend()  
        plt.show()  
```
and then construct a positional encoding as follows :  
```python 
p = PositionalEncoding(1000, 50, 0.2)  
p.draw_encoding_figure()
``` 
It output the following figure : 
![[attachments/Pasted image 20250428210535.png|500]]

#### 2. Absolute Positional Information 
In the positional information above, the encoding dimension relates to the absolute positional information. 

This encoding rule match the binary encode method, where <b><mark style="background: transparent; color: orange">the higher bit shifting frequency is lower than the lower bit</mark></b>  (see [designing-positional-encoding-blog](https://huggingface.co/blog/designing-positional-encoding)):  
```python
1: 001 
2: 010 
3: 011
4: 100 
```

#### 3. Relative positional encoding information 
The above positional encoding is not only a model gives the absolute  positional information but also a model easy for learn attend by relative position. **For any fixed positon offset $\delta$,  the position encoding at position $i + \delta$** <mark style="background: transparent; color: red">can be represented  by a  linear projection</mark>. 

For any fixed offset $\delta$,  we  denote  
$$\omega_{j}  = \frac{1}{10000^{\frac{2j}{d}}}, \quad  
p_{i, 2j}= \sin (i \omega_{2j}) \quad p_{i,2j + 1} = \cos (i\omega_{2j + 1})\quad  j = 0, 1, \dots $$
then for a position shift $\delta$,  <mark style="background: transparent; color: red">we have the following sin-cosine projection relation</mark> :  
$$\Large\boxed{\left[\begin{matrix}
\cos (\delta w_{j}) & \sin (\delta w_{j})  \\  
-\sin( \delta w_{j}) &  \cos(\delta w_{j})  
\end{matrix}\right] \left[\begin{matrix}
p_{i, 2j }   \\  p_{i, 2j + 1} 
\end{matrix}\right] =  \left[\begin{matrix}
p_{i+ \delta, 2 j} \\
p_{i+ \delta, 2 j + 1} 
\end{matrix}\right]}$$
where the 2 × 2 projection matrix does not depend on any position index i.

> [!warning] Problems for stacking self-attention layers 
> Very deep stacks of self-attention layers can **suffer from gradient instability during training**, similar to deep feedforward or RNN architectures. This can slow down or prevent convergence. 
> One Mitigation is  to use <mark style="background: transparent; color: red">residual  connections (in standard Transformers we introduce later)</mark>. layer normalization, and gradient clipping. Architectures like Pre-LN (LayerNorm before attention) can also help stabilize training.

## 5. Real-Transformer Architecture
### (1) Architecture Overview 
We  have discussed in the previous section that **self-attention enjoys both parallel computation and the shortest maximum path length**. The transformer model is ==solely based on attention mechanisms without any convolutional or recurrent layer==. 

**As an instance of encoder-decoder architecture**, the transformer is composed of an encoder and a decoder. Different from [[#3. Bahadanau Attention]] that is for sequence-to-sequence learning. 

In the  transformer architecture, <b><mark style="background: transparent; color: orange">the input  sequence and output sequence are added with positional encoding</mark></b> before feeding into encoder and decoder. 

The following  picture illustrates the transformer architecture : 
![[attachments/Pasted image 20250414152637.png|400]]
In the embedding layer, the source sentence and targets are transformed into word embeddings and added with positional encoding. 

Each layer of the encoder <b><mark style="background: transparent; color: orange">has main layer and Add & norm layer</mark></b>. (either is denoted as sublayer).  Inspired by the [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter6 CNN#4. ResNet (Residual Networks) and ResNeXt|ResNet Part]] we have alreday learned, we <mark style="background: transparent; color: red">add an residual  connection for better performance to cope with deep network</mark>. 

After the encoder layer, there is a <mark style="background: transparent; color: red">encoder-decoder attention in the third layer of the decoder.</mark> However, **each position in the decoder is allowed to only attend to all positions in the decoder up to that position**. 

### (2) Positiowise  Feed-Forward Networks 
In the above structure, the  FFN is Feed-Forward Networks  that  <b><mark style="background: transparent; color: orange">transforms the representation at all the sequence positions using the same MLP </mark></b> 

```python fold title:PositionalWiseFFN
class PositionalWiseFFN(nn.Module):  
    """  
    Position-wise Feed Forward Network    """  
    def __init__(self, d_model, num_hiddens, num_outputs):  
        """  
        :param d_model: The number of expected features in the input (required).        
        :param num_hiddens: The number of neurons in the hidden layer (required).        
        :param num_outputs: The number of neurons in the output layer (required).        
        """        
        super(PositionalWiseFFN, self).__init__()  
        self.fc1 = nn.Linear(d_model, num_hiddens)  
        self.relu = nn.ReLU()  
        self.fc2 = nn.Linear(num_hiddens, num_outputs)  
        self.network = nn.Sequential(  
            self.fc1,  
            self.relu,  
            self.fc2  
        )  
  
    def forward(self, x):  
        """  
        Position-wise Feed Forward Network Forward  
        :param x: sequence of embeddings, shape: `(batch_size, seq_len, embed_dim)`        
        :return: output of the position-wise feed forward network, shape: `(batch_size, seq_len, output_dim)`  
        """        
        return self.network(x)
```

### (3) LayerNorm and  BatchNorm
we give an implementation of layer norm as follows :
In the layer norm  layer, <b><mark style="background: transparent; color: orange">the last dimension must be specified, which normalized the last dimensions</mark></b> : 
```python fold title:
>>> input = torch.randn(N, C, H, W)
>>> # Normalize over the last three dimensions (i.e. the channel and spatial dimensions)
>>> # as shown in the image below
>>> layer_norm = nn.LayerNorm([C, H, W])
>>> output = layer_norm(input)
```

for  layer norm,  if the output of layer norm is 2, we calculate : 
$$\text{mean} = \frac{a + b }{2} , \quad  \text{std} =  \left(\frac{1}{n} \sum_{i = 1}^{n}  (x_{i} - \mu)^{2} \right) = \frac{a - b}{2}$$
hence for 2 number, the layer norm result  must be $[-1, 1]$   

For batch norm, it <b><mark style="background: transparent; color: orange">calculate normalization through channels</mark></b>, (**second dimension**). which **normalize the second dimension**. and the  number `num_features` should be the samee as dimension. 

```python fold title:
# test the batch norm and layer norm 
layer_norm = nn.LayerNorm(3) #  2 is normalized_shape 
bn = nn.BatchNorm1d(3) 
X =  torch.tensor([[1,2, 3], [4,5,6], [7,9, 12]], dtype=torch.float32) 
#  where layer norm is : 
tensor([[-1.2247,  0.0000,  1.2247],
        [-1.2247,  0.0000,  1.2247],
        [-1.1355, -0.1622,  1.2978]], grad_fn=<NativeLayerNormBackward0>)  
# batch norm is normalized through in diemension of columns 
tensor([[-1.2247, -1.1625, -1.0690],
        [ 0.0000, -0.1162, -0.2673],
        [ 1.2247,  1.2787,  1.3363]], grad_fn=<NativeBatchNormBackward0>) 
```

### (4) Transformer Curve-Fitting Implementation    
We implement the code in the [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Code Snippets/my_transformer|my_transformer]]  and   [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Code Snippets/transformer_curve_fit|transformer_curve_fit]]  

![[attachments/Pasted image 20250428213219.png|350]]

We note that in [nn Transformer Module](https://pytorch.org/docs/stable/nn.html#transformer-layers), `TransformerEncoder` layer and  `TransformerDecoder` Layer  are all implemented.  
![[attachments/Pasted image 20250428220406.png|600]]

> [!NOTE] `nn.TransformerEncoder` attention 
> The `nn.TransformerEncoder` is **self-attention only**. Which map the input into $k,q$ and $v$ seperately. 

here we implement the  curve-fitting by the built-in packages :  
```python fold title:curve-fitting-by-transformer
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
  
        # output all hidden layer as a value to fit        x = self.output_layer(x)  # (batch_size, seq_len, 1)  
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

### (5) Transformer Encoder-Decoder Implementation 
For a full implementation of transformer architecture, we firstly recall what we did in [[#(3) Non-Parametric Attention Pooling]],  where we <b><mark style="background: transparent; color: orange">use the actual train data as keys, train values as values</mark></b>. 

Firstly, We give the  [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Code Snippets/my_transformer|transformer full implementation]] and [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Code Snippets/transformer_curve_fit|transformer_curve_fit]] for a curve fitting task.  

## 6. Transformer for Vision
### (1) Model Overview 
Since the transformer firstly used for sequence learning, which <mark style="background: transparent; color: red">multiply different sequence for dot-product attention</mark>. (to find similarity of each sequence data). However, in the traditional **image classification model, it use CNNs for processing pixels, and output the result by a dense layer**. Then the [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter6 CNN#4. ResNet (Residual Networks) and ResNeXt|ResNet]] , [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter6 CNN#5. DenseNet|DenseNet]] are all structures based on CNN. 

One thought of <mark style="background: transparent; color: red">replacing convolution with self-attention</mark> 

The <b><mark style="background: transparent; color: orange">Vision Transformers</mark></b> (ViTs) is a structure that **extract patches from images and feed them  into a transformer  encoder to obtain a global reprentation**. 

For a input image with height $h$ and  width $w$,  we  specify <mark style="background: transparent; color: red">the height and width of each path are both</mark> $p$, 

<b><mark style="background: transparent; color: orange">The vision transformers divide the image into a sequence of</mark></b> length $m = \frac{hw}{p^{2}}$, since the patch is $p^{2} \times  c$, where  $c$  is color bit it takes. We can flat each patch into a $p \times p \times  c$ sequence with   $seq\_len = m$ 

also, since the `<cls>` token **attends to all the image patches  via self-attention**,  the transformer encoder would <mark style="background: transparent; color: red">encode the position of each patch</mark> and also encode sequence as input (exactly as [[#(3) Positional Encoding|transformer encoder]]). in  the following figure, the  <mark style="background: transparent; color: red">flattened image patches  are  transformed  via patch embeddings and transformer encoder blocks</mark>   
![[attachments/Pasted image 20250418075624.png|480]]
we **note that  `<cls>` representation  is further transformed into the output label** in image classification. 

### (2) Patch  Embedding 
For example, for a $8 \times  12$ image which size is 96,  we can split it into 16 patches,  <mark style="background: transparent; color: red">Splitting an image into patches and linearly projecting these flattened patches can be simplified as a single convolution operation,</mark> which is just like : 

we note we can review the CNN input in [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Chapter6 CNN|Chapter6 CNN]] part, also see [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/🔥Pytorch Basics/nn.Flatten 使用|nn.Flatten Usage]] 
```python fold title:patch_size
class PatchEmbedding(nn.Module):
	def __init__(self, img_size=96, patch_size=16, num_hiddens=512):
		img_size = (96, 96)   # also can be not square like (96, 64) 
		patch_size = (16, 16)
		num_patches  = (img_size[0]//patch_size[0], img_size[1]//patch_size[1]) 

		# the convolution implementation 
		self.conv = nn.Conv2d(1, num_hiddens, kernel_size = patch_size, stride = patch_size)   # use num_hiddens as channel number 

	def forward(self, X):
		X = self.conv(X)
		X = nn.Flatten(start_dim=2, end_dim=-1)
		X = X.transpose(1,2)
		return X

X = torch.randn(batch_size, 3, img_size, img_size)   
# then use X as input tensor  
```

> [!Note]
>  The **patch embedding** output the tensor of shape `[batch_size, num_hiddens, num_patches[0], num_patches[1]]`,  which can be flatten (see [Flatten Usage](https://pytorch.org/docs/stable/generated/torch.nn.Flatten.html)) into `(batch_size, num_hiddens,  num_patches[0] x num_patches[1] )` sequence with seq_len of `num_patches[0] x num_patches[1]` and hidden size `num_hiddens`.


> [!Warning] Size after patch embedding 
> Size after the 2d Convolution is `(batch_size, embed_dim,img_size// patch_size , img_size//patch_size )`,  Hence we flatten the last 2 dims and make it as : 
> $$\text{batch size} \times \text{seq len} \times \text{embed dim}$$
> so each patch is of size <b><mark style="background: transparent; color: orange">(also the dimension we initialize the `bos` token)</mark></b> is:  
> > $$1 \times \text{embed dim}$$

We note that in the hugging face, For most custom ViTs, self-implementation are suggested.
```python fold title:
from transformers import ViTModel  
  
model = ViTModel.from_pretrained("google/vit-base-patch16-224")  
print(model.embeddings.patch_embeddings)  # Direct access now
# ViTPatchEmbeddings(
#  (projection): Conv2d(3, 768, kernel_size=(16, 16), stride=(16, 16))
# )
```

### (3) Vision Transformer Encoder 
in the transformer encoder,  it use linear  layer for mapping `num_hiddens` units  to  `num_outputs`. 

```python fold title:Visual_Transformer_perception 
import torch.nn as nn  

class ViTMLP(nn.Module):  
    def __init__(self, mlp_num_hiddens, mlp_num_outputs, dropout=0.5):  
        super().__init__()  
        self.dense1 = nn.LazyLinear(mlp_num_hiddens)  
        self.gelu = nn.GELU()  
        self.dropout1 = nn.Dropout(dropout)  
        self.dense2 = nn.Linear(mlp_num_hiddens, mlp_num_outputs) 
        self.dropout2 = nn.Dropout(dropout)  
		
    def forward(self, x):  
        return self.dropout2(self.dense2(self.dropout1(self.gelu(  
            self.dense1(x)  
        ))))  
```

> [!NOTE] Norm Layer Position  
> we note that in [[#(1) Model Overview]],  the norm layer is often placed before the MLP block and **multi-head attention** block. <mark style="background: transparent; color: red">Which in contrast with the post-normalization layer shown in</mark> [[#5. Real-Transformer Architecture]],  This is  because <b><mark style="background: transparent; color: orange">pre-normalization leads to more effective or efficient training for transformers</mark></b> (introducted in https://arxiv.org/pdf/1906.01787)

We note that `GELU` is a similiar  function like ReLU, which is **Gaussian Error Linear Units function**; 

### (4) Full Architecture for vision transformer  

We use 2 layers clarified in the [[#(1) Model Overview]], the ViT block include the layer normalization  and 
```python fold title:Visual_Transformer_block
class ViTBlock(nn.Module):  
    def __init__(self, num_hiddens, norm_shape, mlp_num_hiddens, num_heads, dropout, use_bias=False):  
        super().__init__()  
        self.ln1 = nn.LayerNorm(norm_shape)  
        self.attention = MultiHeadAttention(num_hiddens, num_heads, dropout, use_bias)    # the multihead attention has implemented previously.  
        self.ln2 = nn.LayerNorm(norm_shape)  
        self.mlp = ViTMLP(mlp_num_hiddens, num_hiddens, dropout)  

    def forward(self, X, valid_lens=None):  
        X = self.ln1(X)  
        # use self-attention ()
        attention_out = self.attention(X, X, X, valid_lens)   # queries, keys and values  are all X
        X = X + attention_out  
        X = self.ln2(X)  
        return X + self.mlp(X)  
```

the fully network architecture is as follows : 
![[Excalidraw/Chapter8 Attention Mechanisms, Transformers 2025-04-18 09.58.51|400]]
1. **Extracting the [CLS] token** (or `<bos>` token) from the final layer. 
2. **Passing it through a LayerNorm + linear head** to map from `num_hiddens` to `num_classes` 


> [!NOTE] Training Process  
> In the Vision Transformer training process , <b><mark style="background: transparent; color: orange"> it may converge slower than  CNN at start</mark></b>, even like  increase from 2% when it start learning.  

## 7. Large-Scale Pretraining  
For a better  generalized  models or even generalists  that can perform multiple tasks with adaption. We use pretraining-data more commonly,  the transformers can be used in three different modes: encoder-only, encoder-decoder, and decoder-only.  

### (1) Encoder-Only Structure 
The input-tokens all input tokens attend to each other. A transformer encoder consists of self-attention layers, where all input tokens attend to each other. <b><mark style="background: transparent; color: orange">For example, vision transformers are encoder-only, </mark></b> whch just convert a sequence of input image patches into the representation of a special “`<cls>`” token.  

For **BERT(Bidirectional Encoder Representations from Transformers)**, which trained on large random-masked sequence (since random-masked, it is a bidirectional encoder )

The  pretrained BERT <b><mark style="background: transparent; color: orange">can be fine-tuned to downstream encoding  task, just by adding a classification head after the transformer encoder structure</mark></b>. In that case the BERT **can be fine-tuned for sentiment analysis.**  Note during fine-tuning, the cross-entropy loss between the prediction and the label on sentiment analysis data is minimized via gradient-based algorithms. 
![[attachments/Pasted image 20250419172848.png|300]]
where the attention input matrix is using self attention and add the target into output attention :  
![[attachments/Pasted image 20250421102825.png|250]]
We note that in the BERT network, it has the language processing ability, but <b><mark style="background: transparent; color: orange">when adding a layer for the downstream task, the training data is still needed to train the model (which is called fine-tune)</mark></b> since the weight of the task head is randomly initialized. So the task head still need to be trained. for example, see [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Implements/BERT 模型对下游任务的适配和详解|BERT model tasks examples]].

### (2) Encoder-Dcoder Structure 
For the encoder, it <mark style="background: transparent; color: red">outputs the same  number of the output representations as the input tokens</mark>. 

Meanwhile, the transformer architecture  also <mark style="background: transparent; color: red">have a decoder that autoregressively predicts the target  sequence of </mark> arbitary length (which can be reached by call model of decoder again and again). 

we note multi-head attention allows the target token  to <b><mark style="background: transparent; color: orange">attend to all input tokens</mark></b>. 

> [!examples] Examples
> BART (Lewis et al., 2019) and T5 (Raffel et al., 2020) are two concurrently proposed encoder-decoder transformers.  while the former emphasizes noising input (e.g., masking, deletion, permutation, and rotation) and the latter highlights multitask unification with **comprehensive ablation studies.** 

Taking  T5 as an example, it is used to predict ==**"span corruption"**== 
```python fold title:
# for the sentence : 
I love this red car
# input is : 
I <X> this <Y> 
# which use this target for prediction : |
<X> love <Y> red car <Z> 
```
- At timestep 1: `<X>` → decoder learns to predict `love`
- At timestep 2: `love` → it learns to predict `<Y>`
- At timestep 3: `<Y>` → predict `red`  

finally the `<X>, <Y>` and all special tokens are removed for the final prediction. 
![[attachments/Pasted image 20250421103021.png|400]]

> [!NOTE]  In this architecture 
> each target token attends to all input tokens;
> each target token attends to present and past target tokens only (causal).

### (3) Decoder-Only Architecture
The decoder-only transformers remove the encoder and  remove the entire encoder and the decoder sublayer with the encoderdecoder cross-attention from the original encoder-decoder architecture. 

For example, <mark style="background: transparent; color: red">GPT (generative pre-training) model</mark> use the Decoder-Architecture as the backbone. For example, it just use the decoder without encoder-output. 
![[attachments/Pasted image 20250421112209.png|400]]
Note that the causal attention pattern in the transformer decoder enforces that **each token can only attend to its past tokens** <b><mark style="background: transparent; color: orange">by a masked self-attention</mark></b> (token-by-token prediction cannot attend to future tokens).

> [!warning] Core Machanism of decoder-only architecture
> The <mark style="background: transparent; color: red">Input-Output Concanatenate</mark> is the Core Architecture in decoder-only transformer. 
> 
> For Decoder-Only-Architecture, **The input and output would be concatenate into 1 sequence when training.** which allows different seqence length of iput and output.  
> 
> Also, when calculating the loss, the input 

## 8. Full Transformer Implementation 
### (1) Usage of the Built-in Pytorch Transformer function  
Note that the pytorch doesn't implement positional encoding in transformer structure.  

If we want to use a transformer encoder layer, use : 
```python fold title:TransformerEncoderLayer
>>> encoder_layer = nn.TransformerEncoderLayer(d_model=512, nhead=8, batch_first=True)
>>> src = torch.rand(32, 10, 512)
>>> out = encoder_layer(src) 
```

### (2) Calculation  for parameters  of Vision Transformer  
For a **6 layer with 768 hidden num, 1024 FF layer Transformer**, we can estimate the parameter number as follows : 

For every layer, the output have `num_hidden x num_hidden` params,  and `3 * d_model * num_hiddens`  params, where  `d_model` 
$$\text{Parameter Number} = \text{Layers} \times (\text{Width}^{2}  + Width \times MLP(FF)  + 
Width  \times 3)$$
result is about :
$$6 \times   (768^{2}  + 3 * 768 +  768  * 1024) \approx  8 Millions$$

## 9. Vision Transformer  Implementation 
### (1) How to make a  Good Vision Transformer  
In  the following code, we will test the Image Classification Task on dataset `sports-classification` (kagglehub) and  `CIFAR-10`  

> [!danger] Main Problems in transformer traning 
> In a lot of case,  we often see the problem :  
> 1. hard to traning  after above 50% accuracy  
> 2. overfitting (according to [How to train Vision transformers](https://arxiv.org/abs/2106.10270),  the vision transformer  are generally found to perform best in settings with large amounts of training data or <b><mark style="background: transparent; color: orange">need strong model regularization  or data augmentation</mark></b> to avoid overfitting) 
> 3. Increasing AugReg(Data Augmentation)  may need a decrease in weight decay(model nomalization, or L2 norm). 
> 4. The evidence suggests that for a 32x32 image, using a larger patch size (e.g., 8 or even 16) is generally better than using a smaller patch size (e.g., 4). 
> 5. **Warm-up learning rate and cosine anneling scheduler**  
> 6. pre-CNN layer for more characteristics <b><mark style="background: transparent; color: orange">(combining CNN and Vision Transformer architectures)</mark></b>  `->` if no pre-cnn layer,  the positional encoding map the 3 channels to embed dim, so if we use the cnn layer at first, it would have more features to input into the transformer. 

For smaller dataset, we can <mark style="background: transparent; color: red">use the following techniques</mark> to handle the problems above : 
1. **Use the Image Augmentation `->` Transformers need more data to tran the dataset**,  or <b><mark style="background: transparent; color: orange">serious overfitting would occur</mark></b>.  Also use when use the Image enhancement. <b><mark style="background: transparent; color: orange">CutMix ,  Mixup and RandAug are very important for improving the  training accuracy</mark></b>.  
Overfitting Image as follows (`nhead=4, num_layer=4, lr = 1e-4, batch_size=2048`) , learn very fast at before and becoming overfitting quickly,  Also if we add random erase in the end of the data, it may be hard for model to converge, but we can add more layers in the model.  
![[attachments/Pasted image 20250610225533.png|600]]
2. Use <b><mark style="background: transparent; color: orange"> wider (but not so wide) hidden dim in classification head (for example : 1024 rather than 512)</mark></b>  
3. For vision transformer, If we use `num_heads = 4`,  It would be very hard to increase precision (the improve of the traning accuracy would be very slow). 
4. <b><mark style="background: transparent; color: orange">Reduce batch size</mark></b>  (super high `batch_size`, like  2048-8192 may lead the model to converge into unwanted result, so if we use 1024 or others it would be more stable). 
5. Use  `n_layers = 8` for better result when use strong data augmentation. 
6. higher dropout in  transformer  (0.2-0.25) and lower dropout in head (0.05 - 0.1)
7. <b><mark style="background: transparent; color: orange">Knowledge distillation can be helpful to the model training</mark></b> 
8. **Use Pre-training Model when available** (we will not use it in this example, but `ResNet50  + Vit` is available as  **Hybrid CNN-ViT**) 
9. Use `AdamW` instead of `Adam`  (stable) in VIT  
10. Pre-Generated Mean and Std (Not always  use ImageNet for Normalization !)

| Model Type                    | Data Augmentation | Training Techniques                 | Top-1 Test Accuracy |
| ----------------------------- | ----------------- | ----------------------------------- | ------------------- |
| Basic ViT (No Pretraining)    | No                | Basic Training Setup                | 60% ~ 80%           |
| ViT + Data Augmentation       | Yes               | CutMix, Mixup, RandAug, etc.        | 85% ~ 90%           |
| DeiT / CaiT (Lightweight ViT) | Yes               | Distillation, Strong Regularization | 90% ~ 95%           |
| ConvMixer / Hybrid ViT        | Yes               | Convolutional Embedding + ViT       | Up to 96%           |

for `batch_size = 2048, lr = 2e-4` : 
![[attachments/Pasted image 20250611120515.png|600]]
we note that **if we use small batch**, we need to <mark style="background: transparent; color: red">reduce the learning rate to give the model better training stability</mark>. 

For the `batch_size` about 256, we can use  learning rate about 2e-6 to 3e-6 for better training (good at start but still underfit at the) : 
![[attachments/Pasted image 20250611125611.png|500]]
Train after 120 epochs : 
![[attachments/Pasted image 20250611142308.png|500]]
For smaller batch size and remove pre-normal layer : 
![[attachments/Pasted image 20250611154415.png|500]]
decrease the patch size and it becomes very hard to converge :  
![[attachments/Pasted image 20250611181928.png|500]]
if we increase the learning rate, and also increase the embed_dim to 1024, the result would be differnet : 
![[attachments/Pasted image 20250611203423.png|500]]
also, if we add CNNlayer before and use 512 as classification layer,   
![[attachments/Pasted image 20250613075258.png|500]]
Fix wrong : remove normalization layer in forward layer, and 
```python fold title:
def forward(self, x: torch.Tensor):  
    x = self.patch_embedding(x)  # (batch_size, n_patches, embed_dim)  
    seq_bos = self.bos_token.expand(x.shape[0], 1, -1)  # (batch_size, 1, embed_dim)  bos token  
    seq = torch.cat([seq_bos, x], dim=1)  # add bos token to the sequence  
    seq = self.positional_encoding(seq)  # add positional encoding to the sequence  
    #  seq = self.norm1(seq)  pre-normalization  is wrong, we should set normal 
    seq = self.transformer(seq)  # (batch_size, n_patches+1, embed_dim)   
    return output
```
use  `norm_first=True` and `activation = 'gelu'` in `self.transformer_layer`, like following  
```python fold title:
self.transformer_layer = nn.TransformerEncoderLayer(d_model=embed_dim,  
                                                    nhead=nhead,  
                                                    activation='gelu',  
                                                    batch_first=True,  
                                                    norm_first=True,  
                                                    dropout=dropout)
```

### (2) Implementation  
1. Generate the mean and standard derivation of the data (see [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Implements/Welford 法求解数据整体方差|Mean and std calculation by Welford Method]]). 

