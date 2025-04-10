## 1. Introduction 
Consider that for a sequence of Image, like Video, or <mark style="background: transparent; color: red">tasked with  producing a sequentially  structured prediction.</mark> 

RNN (Recurrent Neural Networks) is developed for  **capturing the dynamics of  sequences** via the <mark style="background: transparent; color: red">recurrent connections</mark>. 
considering that **it's the feedforward nature that makes the computation of neural network unambigious**. But RNN defined  in a precise way that ensures that no such  ambiguity can arise. 

> [!NOTE] Nature of RNNs
> RNNs are unrolled across the time steps, 

the unfolded structure of RNN in timestamp is as follows : 
![[Excalidraw/Chapter7 RNN 2025-02-24 09.51.42]]
for video and sequencing model, they can be  represented as varying length sequence of fixed-length vectors 
## 2.  Models In RNN  
Firstly, we consider the input as 
$$x_{1}, x_{2}, \dots  x_{T} $$
where **each feature $x_t$ indexed by a time step $t \in  Z^+$ lies in $R^d$** 

for example, if we want to predict the future weather, and use the historical weather in a sequence. and we wish our predict a sequentially structured  target $y_1, y_2, \dots  y_t$ by given a fixed input, 

the structure include : 
**(i) aligned**: where the input at **each time step aligns with a corresponding target** (e.g., part of speech tagging); 
**(ii) unaligned**: where the input and target **do not necessarily exhibit a step-for-step correspondence** (e.g., machine translation) 

### (1) Auto-regressive model 
We consider the trade prediction, One simple strategy for estimating the conditional expectation **would be a linear regression model**. <mark style="background: transparent; color: red">The trader is  interest to know the probablity distribution as follows</mark>: 
$$P[(x_{t}  |  x_{t-1} , \dots   x_{1})]$$
note if weonly need the prediction, we can focus more on estimating the conditional expectation :
$$E[(x_{t}  |  x_{t-1} , \dots   x_{1})]$$
such models that <b><mark style="background: transparent; color: orange">regress the value of a signal on the  previous values of the same signal</mark></b>, is called **auto regressive models**. Which is target at solving the  problem : 
$$P[(x_{t}  |  x_{t-1} , \dots   x_{1})]$$
in  that case, we look at the condition on some window length $\tau$, and use observation  $x_{t -1}, \dots  x _{t - \tau}$ for prediction. Since the input is fixed number, we only need to train **any linear model or deep network that requires fixed-length vectors as inputs**.
![[attachments/Pasted image 20250224105556.png|300]]
it  leads to models that estimate $x_{t}$ with  $\hat{x}_t = P(x_t | h_t)$,  we note that in this network, **we should consider how to correct the prediction by observing the difference of  $\hat{x}_{t}$ and $x_{t}$** in this network, leading to the network that estimate $x_t$ and <b><mark style="background: transparent; color: blue">update the hidden layer with prediction time as follows</mark></b>: 
$$\Large\boxed{h_{t} = g (h_{t-1}, x_{t-1})}$$

> [!NOTE] Latent Autogressive Models 
> Since $h_t$ is never observed, these models are also called latent autoregressive models. 

To construct training data from historical data, one typically **creates examples by sampling windows randomly**. In general, we do not expect time to stand still. we often assume that **while the specific values of $x_t$ might change, the dynamics according to which each subsequent observation is generated given the previous observations do not**.  Statisticians call dynamics that do not change **stationary**.

### (2) Sequence Models  
 like an autoregressive problem, we can reduce language modeling to autoregressive prediction **by decomposing the joint density of a sequence $p(x_{t} | x_{1}, \dots  x_{T})$ into the product of  conditional densities**. 
$$P(x_1, \ldots, x_T) = P(x_1) \prod_{t=2}^{T} P(x_t | x_{t-1}, \ldots, x_1).  $$
Note that if we are working with **discrete signals like words**, then the autoregressive model must be a probabilistic classifier, outputting a full probability distribution over the vocabulary for what word will come next, given the leftwards context. 

#### Examples
in the following case, we may predict a vibration with $\sin(x)$ with 0.01 time step and 0.2 variance like follows: 
![[attachments/Pasted image 20250224121740.png|400]]

we can just  build the **linear regression model (1-layer Linear model)** for the prediction of the response of time $t$, we can  use the simple code as follows :  
```python
import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader, Dataset

# Define a simple linear regression model
class LinearRegression(nn.Module):
    def __init__(self, input_dim, output_dim, lr=0.01):
        super(LinearRegression, self).__init__()
        self.linear = nn.Linear(input_dim, output_dim)
        self.optimizer = optim.SGD(self.parameters(), lr=lr)
        self.criterion = nn.MSELoss()

    def forward(self, x):
        return self.linear(x)

    def fit(self, dataloader, epochs=5):
        for epoch in range(epochs):
            for features, labels in dataloader:
                self.optimizer.zero_grad()
                outputs = self.forward(features)
                loss = self.criterion(outputs, labels)
                loss.backward()
                self.optimizer.step()
            
            print(f'Epoch [{epoch+1}/{epochs}], Loss: {loss.item():.4f}')

# Define a custom dataset class
class TimeSeriesDataset(Dataset):
    def __init__(self, x, tau):
        """
        x: Time series data (1D tensor)
        tau: Number of time steps to use for prediction
        """
        self.x = x
        self.tau = tau
        # Create features and labels
        self.features = torch.stack([self.x[i : i+self.tau] for i in range(len(self.x) - self.tau)])
        self.labels = self.x[self.tau:].reshape((-1, 1))

    def __len__(self):
        return len(self.features)

    def __getitem__(self, idx):
        return self.features[idx], self.labels[idx]

# Generate some synthetic data
T = 100  # Total time steps
tau = 10  # Number of time steps to use for prediction
x = torch.linspace(0, 10, T) + torch.randn(T) * 0.5  # Synthetic time series data

# Create dataset and dataloader
dataset = TimeSeriesDataset(x, tau)
dataloader = DataLoader(dataset, batch_size=10, shuffle=True)

# Initialize the model
input_dim = tau  # Input dimension is tau (number of time steps used for prediction)
output_dim = 1   # Output dimension is 1 (predicting the next time step)
model = LinearRegression(input_dim, output_dim, lr=0.01)

# Train the model
model.fit(dataloader, epochs=5)
```

we train the dataset with $\tau$  features  and  the features is a sequence of  $[\boldsymbol{x}_{T-\tau}, \dots  \boldsymbol{x}_{T-1}]->$   input dimension is $T-\tau$ with each length $\tau$ ,   and `labels` are the corresponding next time steps, wich has output $T-\tau$ with 1 length

Also for extracting the text from article, we can use `TfidfVectorizer` (see [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/👨‍🎓机器学习算法(sklearn)/1.机器学习算法和文本分类挖掘(Naive Bayes)|Text extraction methods]]),we can extract the frequency of  uni-gram,  bi-gram and tri-gram  

> [!NOTE] 
> Estimating the forwad direction may a lot easier than   the reverse direction  

### (3) Markov Models
Markov model  decomposite the joint probability to a product of probablities like follows:
$$ P(x_1, \ldots, x_{\tau}) = P(x_1) \prod_{t=2}^{\tau} P(x_t | x_{t-1}) $$
We often find it useful to work with models that proceed as though a Markov condition were satisfied.  

Even when we know that this is only approximately true, we often find it useful to work with models that proceed as though a **Markov condition** were satisfied (**the state never relies on the previeous  state but just rely on current state**, see [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/👨‍🎓机器学习算法(sklearn)/补充知识/6. 马尔科夫MCS抽样(MCMC)|Markov MonteCarlo Sampling]]).  

> [!NOTE] 
> Even today’s massive RNN- and transformer-based language models seldom incorporate more than thousands of words of context
> With discrete data, a true Markov model **simply counts the number of times that each word has occurred in each context, producing the relative frequency** estimate of $P (x_{t} | x_{t-1})$. Whenever the data assumes only discrete values (as in language), the most likely sequence of words can be computed efficiently using dynamic programming.

### (4) RNN decoding order  
We have to represent the factorization of a text sequence, as a l**eft-to-right chain** of conditional probabilities. but **we may use right-to-left-chain as follows**: 
$$P(x_{1},  \dots  x_{T} ) =  \prod^{1}_{t=T} P(x_{t}| x_{t+1} ,  \dots x_{T})$$
However, there are many reasons why factorizing text in the same directions as we read it (left-to-right for most languages, but **right-to-left for Arabic and Hebrew) is preferred for the task of language modeling**. 

For converting a probablity over steps 1 to $t$ into 
$$P(x_{t+1},  \dots  x_{1}) =  P(x_{t}, \dots  x_{1}) P(x_{t + 1} | x_{t}, \dots  x_{1})$$
Also use stronger predictive models for predicting adjacent words versus words at arbitrary other locations. also if we change $x_t$, we may be able to influence what happens for $x_{t+1}$ going forward but not the converse.  In some contexts, this makes it easier to predict  $P(x_t+1 | x_t)$ 

### (5) Learning Language Models 
#### 1. Model Brief 
We assume that the tokens (say it as words num) in a text sequence of length $T$  are $x_{1}, x_{2},   \dots  x_{T}$,  
$$ P (x_{1}, x_{2} , \dots  x_{r})$$
<b><mark style="background: transparent; color: orange">The Ideal Language Model</mark></b> would be able to generate natural text just on its own, just  by **drawing one token at a time** 
$$x_{t} \sim  P(x_{t} | x_{t-1} , \dots   x_{1})$$
also, it would be sufficient for generating a meaningful dialog, simply by conditioning the text on previous dialog fragments. Also note it would need to understand the text rather than just generate grammatically sensible content. 

Nonetheless, language models are of great service even in their limited form. For instance, the phrases “to recognize speech” and “to wreck a nice beach” sound very similar. This can cause ambiguity in speech recognition, which is easily resolved through a language model that <b><mark style="background: transparent; color: orange">rejects the second translation as outlandish</mark></b>. Likewise, in a document summarization algorithm it is worthwhile knowing that “dog bites man” is much more frequent than “man bites dog”, or that “I want to eat grandma” is a rather disturbing statement, whereas “I want to eat, grandma” is much more benign. 

firstly, we model a document  by a sequence of tokens,  
$$P (x_{1},  x_{2}, \dots  x_{n} ) = \prod^{T}_{t =1}  P(x_{t}  | x_{1} , \dots   x_{t-1})$$
with the above Markov Model applied, we got for n-grams, the conditional probablity is like that :  
![[attachments/Pasted image 20250224170648.png|550]]
we need to calculate the probability of words and the conditional probability of a word given the previous few words. **Note that such probabilities are language model parameters**

for <b><mark style="background: transparent; color: orange">Word Frequency</mark></b>, we use the following expression : 
![[attachments/Pasted image 20250224171026.png|300]]

#### 2) Laplace Smooth
The Laplace smooth equation is to add a small constant to all counts, 
$$\hat{P}(x) = \frac{n(x) + \epsilon_1 / m}{n + \epsilon_1}$$
$$\hat{P}(x') = \frac{n(x, x') + \epsilon_2 \hat{P}(x')}{n(x) + \epsilon_2}$$
$$\hat{P}(x'') | x, x' = \frac{n(x, x', x'') + \epsilon_3 \hat{P}(x'')}{n(x, x') + \epsilon_3}$$
where we use  $\epsilon_{1}, \epsilon_{2}, \epsilon_{3}$ as the hyperparameter. when $\epsilon_{1} \rightarrow 0$, no smoothing applied, and for $\epsilon_{1}\rightarrow \infty$,  $\hat{P}(x)$ **approaches  uniform 1/m**

#### 3)  Measure the quality  of model by sequence (Loss of Language Model)
Considering that if we **compute the likelihood of the sequence, to evaluate the quality of sentence**, since long sentence may relatively less than short sequence in our train set, it may produce much smaller likelihood than the short one, which miss the equivalent of average. 

We can define the <b><mark style="background: transparent; color: orange">entropy, surprisal and cross-entropy</mark></b> when we introduced the softmax regression. 

the  language model allow us to spend bits in compressing   the sequence. So we can use cross-entropy  for average loss of tokens in a sequence. (**measure it by the cross-entropy loss averaged over all the n tokens of a sequence**)
$$\frac{1}{n} \sum_{i = 1}^{n}  - \log_{}   P (x_{t} | x_{t-1} , \dots  x_{1}) $$
where $x_t$ is actual **token observed at  time step $t$ from the sequence**. 

The scientists often prefer to use <b><mark style="background: transparent; color: orange">perplexity</mark></b> to evaluate the likelihood that a word would happen next `->` it use $\exp(-\log_{}\overline{P})$ to project it  into range $(1, +\infty)$,**when $\overline{P} = 1$, it out as 1, and $\overline{P} = 0$ that out as inf**:    
$$\Large \boxed{\text{perplexity} = \exp\left(- \frac{1}{n} \sum_{i = 1}^{n}   \log_{}   P (x_{t} | x_{t-1} , \dots  x_{1}) \right)}$$

> [!hint] Similarity Evaluation 
> The smaller perplexity is, the better (more similiar) this option is. 

**Perplexity can be best understood as the geometric mean of the number of real choices that we have when deciding which token to pick next**.  
• In the best case scenario, the **model always perfectly estimates the probability of the target token as 1**. In this case the perplexity of the model is 1.
• In the<mark style="background: transparent; color: red"> worst case scenario</mark>, the model always predicts the probability of the target token as 0. In this situation, the <mark style="background: transparent; color: red">perplexity is positive infinity</mark>. 
• At the baseline, the model predicts a uniform distribution over all the available tokens of the vocabulary. In this case, the perplexity **equals the number of unique tokens of the vocabulary**. In fact, if we were to store the sequence without any compression, this would be the best we could do to encode it. Hence, this provides a nontrivial upper bound that any useful model must beat. 

#### 4. Sequence Partition 
For a language model using neural network  and perplexity to evaluate the predicting result.  We first assume that it process a minibatch of sequences with  predefined length at a time.  

we consider that  a sequece has n tokens. to iterator  all the tokens of dataset,  we discard first $d$ tokens (where d is uniformly sampled at random), 

the goal is to predict the next token based on what tokens we have seen so far, i.e.   Obtaining 5 pairs of input sequences and target sequences from partitioned length-5 subsequences. 
![[attachments/Pasted image 20250225212402.png|450]]
we note that the argument `batch_size` specifies the number of subsequence examples (self.b) in each minibatch and `num_steps` is the subsequence length in tokens (self. n).  **Also, a minibatch of target sequences can be obtained by shifting the input sequences by one token.** 

## 3. Basic Recurrent Neural Networks
### (1) Consideration with previous state cooperation
For the condition probablity of token  $x_t$  at time step $t$,  we depend on $n-1$ previous tokens to make predition on next token,  If we want to <mark style="background: transparent; color: red">incorporate the possible effect of tokens earlier than time step</mark> $t -  (n-1)$ on time $x_t$, we need to increase $n$, but <b><mark style="background: transparent; color: orange">the number of model parameters would also increase exponentially with parameter n</mark></b>.

so rather than modeling it with  $P(x_t| x_{t- 1, \dots  x_{t -n + 1 }})$ we use : 
$$\Large P(x_t| x_{t- 1, \dots  x_{t -n + 1 }})\approx  P (x_{t} |  h_{t-1} )$$
where
$$h_{t} = f(x_{t},  h_{t-1})$$
since complex function $f$, the latent variable model is not an approximation.  but $h_t$ may simply store all the data it has observed so far, which leads to heavy computational burden. 

We note that <b><mark style="background: transparent; color: orange">hidden layers and hidden states refer to two very different concepts.</mark></b> Hidden states can only be computed by looking at data at previous time steps.

Recurrent neural networks (RNNs) are neural networks with <b><mark style="background: transparent; color: blue">hidden states</mark></b>. 

### (2) Hidden States 
For nn that without  hidden state (with hidden layer), just  
$$H = \phi (X W_{xh}  + b_{n}) \qquad  O= H W_{hp} + b_{q}$$
We assume that  for a minibatch  of inputs $X_{t} \in  R^{n \times  d}$ at time step $t$,  each row of $X_t$  corresponds to 1 example at time step $t$ from the sequence. next, **the hidden layer output of time step $t$ is denoted by $H_{t}\in  R^{n\times  h}$,**  but <b><mark style="background: transparent; color: orange">RNN save the</mark></b> $H_{t-1}$ as a <b><mark style="background: transparent; color: orange">time parameter</mark></b> **from the previous time step**. 

With $H_{t-1}$ saved, not only $W_{xh}$ we have, but also $W_{hh}$, and  we can add the two part together : 
$$\Large \boxed{H_{t} = \phi(X_{t}  W_{xh} + H_{t-1} W_{hh}  + b_{h})}$$
where $W_{xh} \in  R^{d \times  h}, W_{hh} \in  R^{h \times  h}$ 
thus we captured and retained the sequence’s historical information up to their current time step. 

> [!HINT] Hidden State & Recurrent Layer
> Since we retain the sequence's  historical  infomation up to  their current  time step, such a hidden layer output is called a **hidden state**. Layers that perform the computation (above equation) in RNNs are called **recurrent layers**.

and the output is : 
$$O_{t} = H_{t} W_{hq} + b_{q}$$
and make the following structure :  
![[attachments/Pasted image 20250225221840.png|450]]


> [!Hint] Construct RNN in torch 
> in torch library, we can just use `nn.RNN` library for construct **Recurrent Neural Network**, But note that what must be specified are: `input_sie`, `hidden_size`, `num_layers`

There's an  example : 
```python
import torch  
import torch.nn as nn  

# Define hyperparameters  
input_size = 10  # Dimension of input features  
hidden_size = 20  # Dimension of the hidden layer  
num_layers = 1  # Number of layers in the RNN  
batch_size = 3  # Batch size  
sequence_length = 5  # Length of the sequence  

# Create a simple RNN model  
rnn = nn.RNN(input_size, hidden_size, num_layers, batch_first=True) 
# Create input data (batch_size, sequence_length, input_size)  
input_data = torch.randn(batch_size, sequence_length, input_size)  
# Initialize the hidden state (num_layers, batch_size, hidden_size)  
h0 = torch.zeros(num_layers, batch_size, hidden_size)  
# Forward propagation 
output, hn = rnn(input_data, h0) 
print("Output shape:", output.shape)  # (batch_size, sequence_length, hidden_size)  
print("Hidden state shape:", hn.shape)  # (num_layers, batch_size, hidden_size)
```

![[attachments/Pasted image 20250226095726.png|400]]
For `RNN`,  the **input is a sequence of time**,  for tensor with shape  `(seq_len,  batch_size,  input_size)`, the output sequence  size is `(seq_len, batch_size, hidden_size)` (note that **output size is same as hidden size**) 

> [!HINT] Hidden 
> The output layer in `nn.RNN` is same  as hidden layer, which neglect the output layer, which isn't exist in RNN. 
> 
> Also, To Implement this, we often add a Linear Layer after `nn.RNN` to convert the hidden layer output to actual output. 

![[Excalidraw/Chapter7 RNN 2025-02-26 10.08.32|650]]

```python
import torch
import torch.nn as nn

class RNNScratch(nn.Module):
    def __init__(self, num_inputs, num_hiddens, sigma=0.01):
        super().__init__()
        self.num_hiddens = num_hiddens
        self.W_xh = nn.Parameter(torch.randn(num_inputs, num_hiddens) * sigma)
        self.W_hh = nn.Parameter(torch.randn(num_hiddens, num_hiddens) * sigma)
        self.b_h = nn.Parameter(torch.zeros(num_hiddens))

    def forward(self, inputs, state=None):
        if state is None:
            state = torch.zeros((inputs.size(1), self.num_hiddens), device=inputs.device)
        outputs = []
        for X in inputs:  # Shape of inputs: (num_steps, batch_size, num_inputs)
            state = torch.tanh(torch.matmul(X, self.W_xh) + torch.matmul(state, self.W_hh) + self.b_h)
            outputs.append(state)
        return torch.stack(outputs), state

# Parameters
batch_size, num_inputs, num_hiddens, num_steps = 2, 16, 32, 100

# Initialize RNN
rnn = RNNScratch(num_inputs, num_hiddens)

# Create input tensor
X = torch.ones((num_steps, batch_size, num_inputs))

# Forward pass
outputs, state = rnn(X)

# Output shapes
print(f"Outputs shape: {outputs.shape}")  # Should be (num_steps, batch_size, num_hiddens)
print(f"Final state shape: {state.shape}")  # Should be (batch_size, num_hiddens)
```

### (3) Gradient Clipping  
Dealing with vanishing and exploding gradients is a **fundamental problem when designing RNNs and has inspired some of the biggest advances in modern neural network architectures**. 

Even modern RNNs still often suffer from exploding gradients.  One inelegant but ubiquitous solution is to simply clip the gradients forcing the resulting “clipped” gradients to take smaller values. we assume that  the  objective function $f$ is sufficiently smooth (objective is Lipschitz continuous). say a vector x, but **pushing it in the direction of the negative gradient $\boldsymbol{g}$**, 
$$f (x) -f (y) \leq L ||x - y||$$
we can update the parameter vector  by subtracting $\eta g$  on $x$,  where $\eta$ is learning rate, and each update  takes the form  $x \leftarrow   x - \eta g$ ,  by substitute $y$ upper, we obtain : 
$$f(x) - f (x - \eta  \boldsymbol{g }) \leq  L \eta ||g||$$
if we adopt a gradieent clipping heuristic <mark style="background: transparent; color: red">projecting the gradients g onto a ball of some given radius θ as follows</mark> : 
$$g \leftarrow  \min  \left(1, \frac{\theta}{||g||}\right)\boldsymbol{g}$$
this norm method would **limit the norm of $\vec{g}$  to $\theta$.** 
```python
import torch
import torch.nn as nn

def clip_gradients(model, grad_clip_val):
    """
    Clips gradients of the model parameters to prevent exploding gradients.
    Args:
        model: The neural network model.
        grad_clip_val: The maximum allowed L2 norm for gradients.
    """
    # Collect all trainable parameters
    params = [p for p in model.parameters() if p.requires_grad]   # get gradient parameters 
    # Compute the L2 norm of all gradients
    total_norm = torch.norm(torch.stack([torch.norm(p.grad.detach(), 2) for p in params]), 2)  
    # Scale gradients if the norm exceeds grad_clip_val
    if total_norm > grad_clip_val:
        for param in params:
            param.grad.data.mul_(grad_clip_val / total_norm)
```

> [!HINT] Lib for gradient clipping 
> for pytorch, it has provided the library for gradient clipping, which is `torch.nn.utils.clip_grad_norm`

```python
import torch.nn as nn
import torch.nn.utils.clip_grad_norm_ as clip_gradients
#  just add the layer of gradient clipping:  
clip_gradients(model.parameters(), grad_clip_val)
```

and in real training case, we can use the following method in a training loop 
```python
# Forward pass
outputs = model(inputs)
loss = criterion(outputs, targets) 
# Backward pass
loss.backward()  
# Clip gradients
clip_gradients(model, grad_clip_val=1.0)
# Update weights
optimizer.step() 
```

Note that gradient clipping can just mitigate the problem of exploding gradients but<mark style="background: transparent; color: red"> does not address the problem of vanishing gradients</mark>. 

### (4) Back Propagation Computation in RNN
For  hidden states and outputs at each time step, they can be represented as : 
$$\boxed{h_{t}  = f (x_{t}, h_{t-1} , w_{h})}$$
$$o_{t} = g(h_{t}, w_{o})$$
for that we have a chain of values : 
$$\left\{\dots  (x_{t-1} , h_{t-1}, o_{t-1}) , (x_{t}, h_{t}, o_{t}) , \dots \right\}$$For the **discrepancy between  otuput $o_t$ and the desired target $y_t$ is evaluated by  the function of**  : 
$$L (x_{1} , \dots  x_{t},  \dots y_{1}, \dots  y_{t} , w_{h}, w_{o}) = \frac{1}{T}\sum_{t = 1}^{T} l (y_{t} , o_{t})$$
we can derive that 
$$\frac{\partial L}{\partial w_{h}} =  \frac{1}{T}  \sum_{t = 1}^{T} \frac{\partial l (y_{t}, o_{t} )}{\partial o_{t}} \frac{\partial g(h_{t}, w_{o})}{\partial h_{t}} \frac{\partial h_{t}}{\partial w_{h}}$$

> [!NOTE]  Weight Share across time steps 
> For RNN structure, the weight $w_h$  is shared across the time steps, which is : 
> $$h_{t-1} = f(x_{t-1}, h_{t-2}, w_{h})$$
> Different Time step use same hidden layer weight. Note $w_h$ include $w_{hf}, w_{xf},  w_{hi}, w_{xi}, \dots$ (8 weights), and <b><mark style="background: transparent; color: orange">these weights are all shared across time.</mark></b>  

where the chain rule yields that the <mark style="background: transparent; color: red">back propagation process equation</mark> is : 
$$\Large \boxed{\frac{\partial h_t}{\partial w_h} = \frac{\partial f(x_t, h_{t-1}, w_h)}{\partial w_h} + \frac{\partial f(x_t, h_{t-1}, w_h)}{\partial h_{t-1}} \frac{\partial h_{t-1}}{\partial w_{h}}}$$
This is a recurrent expansion equation, where  $\frac{\partial h_{t-1} }{\partial w_{h} }$ can  be represented into the above equation. so after expand all the equation, we got : 
$$a_t = b_t + \sum_{i=1}^{t-1} \left( \prod_{j=i+1}^{t} c_j \right) b_{i}$$
where : 
$$\begin{align*} a_t &= \frac{\partial h_t}{\partial w_h}, \\ b_t &= \frac{\partial f(x_t, h_{t-1}, w_h)}{\partial w_h}, \\ c_t &= \frac{\partial f(x_t, h_{t-1}, w_h)}{\partial h_{t-1}}. \end{align*}$$
and got the final back propgation equation as : 
$$\frac{\partial h_t}{\partial w_h} = \frac{\partial f(x_t, h_{t-1}, w_h)}{\partial w_h} + \sum_{i=1}^{t-1} \left( \prod_{j=i+1}^{t} \frac{\partial f(x_j, h_{j-1}, w_h)}{\partial h_{j-1}} \right) \frac{\partial f(x_i, h_{i-1}, w_h)}{\partial w_h}$$
note in actual computation, if we compute full equation above, it would be very slow and gradients can blow up, so we truncate the time step to $\tau$  steps, 

### (5) Randomized Truncation 
We can firstly replace $\frac{\partial h_t}{\partial w_h}$ by a random variable  which is correct in expectation. Then we can <mark style="background: transparent; color: red">replace  the partial part</mark> $\frac{\partial h_t}{\partial w_h}$ by $\xi_t$  which subordinate to the Bernoulli distribution : 
$$0 \leq \pi_{t} \leq  1 \qquad   P(\xi_{t} = 0) = 1-\pi_{t}\qquad  P(\xi_{t} = \pi_{t}^{-1}) = \pi_{t}$$
![[Excalidraw/Chapter7 RNN 2025-02-27 10.44.08|250]]
we can easily got that : 
$$E(\xi_{t}) = 1 $$
then the above back propagation process equation becomes :
$$ z_{t} = \frac{\partial f(x_t, h_{t-1}, w_h)}{\partial w_h} + \xi_t \frac{\partial f(x_t, h_{t-1}, w_h)}{\partial h_{t-1}} \frac{\partial h_{t-1}}{\partial w_h}$$
note the approximate result **follows the expectation rule** : 
$$E(z_{t})  =  \frac{\partial h_{t}}{\partial w_{h}}$$
> [!NOTE] 
> Actually randomized truncation does not work much better than regular truncation 


![[attachments/Pasted image 20250227110805.png|350]]
- The first row is the randomized truncation that partitions the text into segments of varying lengths.
- The second row is the regular truncation that breaks the text into subsequences of the same length. This is what we have been doing in RNN experiments
- The third row is the full backpropagation through time that leads to a computationally infeasible expression.

the RNN backpropagation computation graph is as follows : 
![[attachments/Pasted image 20250227111342.png|500]]

## 4. Modern RNN  
There has been a tremendous amount of innovation in RNN architectures, culminating in several complex designs that have proven successful in practice. 
### (1) LSTM architecture(nn.LSTM)
LSTM (long short term memory  architecture),  also lighter weight is called GRU (gated recurrent unit). 

LSTMs resemble standard recurrent neural networks but here each ordinary <b><mark style="background: transparent; color: orange">recurrent node is replaced by a memory cell</mark></b>.

Aach memory cell contains an internal state.  for  simple RNN,  it have  long-term-memory in the form off weights, which change slowly during training, A <mark style="background: transparent; color: red">memory cell is a composite unit</mark>, built from simpler nodes in a specific connectivity pattern, with the novel inclusion of multiplicative nodes. 

For <b><mark style="background: transparent; color: orange">Gated memory Cell</mark></b>,  it is equippped with an internal state and a number of multiplicative gates that determine whether 
1.  **a given input should impact the internal state** **(the input gate)** 
2. the internal state should be flushed to 0 **(the forget gate)**
3. the internal state of a given neuron should be allowed to impact the cell’s output **(the output gate)**.


key distinction between vanilla RNNs and LSTMs is that <b><mark style="background: transparent; color: orange">the latter support gating of the hidden state </mark></b>. so that we have dedicaed mechanisms for <mark style="background: transparent; color: red">when a hidden state should be updated or reset</mark> 

#### 1. Fundamental Gates 
For LSTM gates,  we represent that Input, Forget and Output gates as $I_t, F_t$ and  $O_t$, in the hidden layer, intput is $X_{t}$ and $H_{t-1}$  then we can calculate each gate output as : 
$$\boxed{\begin{align*}
I_t &= \sigma(X_t W_{xt} + H_{t-1}W_{h t} + b_t), \\
F_t &= \sigma(X_t W_{xf} + H_{t-1}W_{hf} + b_f), \\
O_t &= \sigma(X_t W_{x_0} + H_{t-1}W_{h_0} + b_0),
\end{align*}} \tag{7.4.1}$$
output are like this shape : 
![[attachments/Pasted image 20250228145701.png|400]]

> [!NOTE] Activation Function   
> The activation function  $\sigma$  here is different from $tanh$ in $C_t$ activation function. 
> $\sigma$ is  sigmoid function but not  ReLU `->` **since it's $a$ gate signal function in range (0, 1)**

#### 2. Input Node 
We firstly introduce input node (cell state) $\tilde{C}_{t}\in R^{n \times  h}$, it use  a [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Pytorch & sklearn的使用基础和基本代码#3. tanh 激活函数|tanh function]] with value range for $(-1,1)$ 
$$C_{t} = \tanh(X_{t}W_{xc} + H_{t-1}W_{hc} + b_{c}) \tag{7.4.2}$$
![[attachments/Pasted image 20250228150003.png|400]]

**Internal State**
1. The input gate $I_t$ governs how much we take new  data into account via $\tilde{C}_t$ 
2. The forget gate  $F_t$ governs how much of internal state $C_{t-1} \in R^{n \times  h}$ retain.
we can use Hadamard product to give the relation : 
$$\boxed{C_{t} = F_{t} \odot C_{t}- 1   + I_{t} \odot  \tilde{C}_{t}} \tag{7.4.3}$$
for $F_t < 1$, the network forget last status, and for $I_{t} > 1$, network receive  the input layer parameters. 

then we calculate the next hidden layer as : 
$$\boxed{H_{t}  =  O_{t} \odot \tanh (C_{t})} \tag{7.4.4}$$
so the final structure becomes : 
![[attachments/Pasted image 20250302173744.png|600]]

we give a LSTM  cell building Process as follows **(firstly initialize each W, b randomly)** note there are 4 parameters with 2 inputs, so we initialize as follows : 
```python
import torch
import torch.nn as nn

# Define the LSTM cell
class LSTMCell(nn.Module):
    def __init__(self, num_inputs, num_hiddens, sigma=0.01):
        super(LSTMCell, self).__init__()
        self.num_inputs = num_inputs
        self.num_hiddens = num_hiddens
        
        # Function to initialize weights
        init_weight = lambda *shape: nn.Parameter(torch.randn(*shape) * sigma
        
        # Function to create a tuple of weights and bias for a gate
        triple = lambda: (init_weight(num_inputs, num_hiddens),
                          init_weight(num_hiddens, num_hiddens),
                          nn.Parameter(torch.zeros(num_hiddens)))
        
        # Initialize weights and biases for input, forget, output gates, and cell state 
        self.W_xi, self.W_hi, self.b_i = triple()  # Input gate
        self.W_xf, self.W_hf, self.b_f = triple()  # Forget gate
        self.W_xo, self.W_ho, self.b_o = triple()  # Output gate
        self.W_xc, self.W_hc, self.b_c = triple()  # Cell state

    def forward(self, inputs, state):
        # Unpack the previous hidden state and cell state
        h_prev, c_prev = state
        # Input gate
        i = torch.sigmoid(torch.matmul(inputs, self.W_xi) + torch.matmul(h_prev, self.W_hi) + self.b_i)
        # Forget gate
        f = torch.sigmoid(torch.matmul(inputs, self.W_xf) + torch.matmul(h_prev, self.W_hf) + self.b_f)
        # Output gate
        o = torch.sigmoid(torch.matmul(inputs, self.W_xo) + torch.matmul(h_prev, self.W_ho) + self.b_o)
        # Cell state
        c_tilde = torch.tanh(torch.matmul(inputs, self.W_xc) + torch.matmul(h_prev, self.W_hc) + self.b_c)
        c_next = f * c_prev + i * c_tilde
        
        # Hidden state
        h_next = o * torch.tanh(c_next)
        
        return h_next, c_next

# Example usage
num_inputs = 10
num_hiddens = 20
lstm_cell = LSTMCell(num_inputs, num_hiddens)

# Dummy input and initial state
inputs = torch.randn(1, num_inputs)
state = (torch.zeros(1, num_hiddens), torch.zeros(1, num_hiddens))

# Forward pass
h_next, c_next = lstm_cell(inputs, state)
print("Next hidden state:", h_next)
print("Next cell state:", c_next)
```
for python actual building process, we can just use `nn.LSTM` Module for build it : 
```python
lstm = nn.LSTM(input_size, hidden_size, num_layers, batch_first, bidirectional, dropout)  # batch size is for control the input and output dimension order 
rnn = nn.RNN(input_size, hidden_size, num_layers, batch_first=True)
```
also note  LSTM is really similar as rnn, it also use hidden_size as the output layer. 

`batch_first`: If True, the shape of the input and output tensors will be (batch_size, seq_len, input_size); if False, the shape will be `(seq_len,batch_size, input_size)`

for input tensor : 
If `batch_first=True` the shape will be (batch_size, seq_len, input_size).
If `batch_first=False` the shape will be (seq_len, batch_size, input_size). 
where `seq_len` is the length of total sentence.  the dimension of `input_size` must be self-related.  

### (2)  GRU (Gated Recurrent Units)
#### 1. Basic Structure 
GRU includes Reset Gate and Update Gate,  The  structure is as follows: 
![[attachments/Pasted image 20250306105654.png|300]]
where the <b><mark style="background: transparent; color: orange">Reset gate and  update Gate are computed as follows : </mark></b> 
$$\begin{align*}
R_t &= \sigma(X_tW_{xr}+H_{t-1}W_{hr}+b_r), \\
Z_t &= \sigma(X_tW_{xz}+H_{t-1}W_{hz}+b_z),
\end{align*}$$
where $\sigma$ is also sigmoid Function that  limit output to $(0,1)$ 

#### 2. Candidate Hidden State 
For the  $R_t$ with the regular updating mechanics, leading to tHe
$$\boxed{\tilde{H}_{t} = \tanh(X_{t}  W_{xh} + (R_{t} \,  \odot \; H_{t-1}) W_{hh} + b_{h} )}$$
This result is a candidate hidden state,  We note that the **influence of the previous states can be reduced with the elementwise multiplication** of $R_{t}$ and $H_{t-1}$, **For $R_t$ close to 1 ,  we recover a vinilla RNN, which is**  
$$\tilde{H}_{t} = \tanh(X_{t}  W_{xh}   + H_{t-1} W_{hh}  + b_{h}) \qquad  \text{(vinilla RNN) }$$
This is the candidate state. 
#### 3. Hidden State 
Incorporating the effect of update gate $Z_t$,  we combine $H_t$ and $\tilde{H}_t$ to the next state : 
$$H_t = Z_t \odot H_{t-1} + (1 - Z_t) \odot \tilde{H}_t. \tag{10.2.3}$$
where from the upper 
$$\tilde{H}_{t} = \tanh(X_{t}  W_{xh} + (R_{t} \,  \odot \; H_{t-1}) W_{hh} + b_{h} )$$
![[attachments/Pasted image 20250306113034.png|470]]

Following gives a example for creating the  GRU unit Module as follows :
```python
import torch  
from torch import nn  

class GRUScratch(nn.Module):  
    def __init__(self, num_inputs, num_hiddens, sigma=0.01):  
        super().__init__()  
        self.num_inputs = num_inputs  
        self.num_hiddens = num_hiddens  
        self.sigma = sigma  
        init_weight = lambda *shape: nn.Parameter(torch.randn(*shape) * sigma)  
        triple = lambda: (init_weight(num_inputs, num_hiddens),  
                           init_weight(num_hiddens, num_hiddens),  
                           nn.Parameter(torch.zeros(num_hiddens)))  
        self.W_xz, self.W_hz, self.b_z = triple() # Update gate  
        self.W_xr, self.W_hr, self.b_r = triple() # Reset gate  
        self.W_xh, self.W_hh, self.b_h = triple() # Candidate hidden state  
        
	def forward(self, inputs, H=None):  
        outputs = []  
        for X in inputs:  
            if H is None:  
                H = torch.zeros(X.shape[0], self.num_hiddens, device=X.device)  # Initialize H with correct device  
            Z = torch.sigmoid(torch.matmul(X, self.W_xz) + torch.matmul(H, self.W_hz) + self.b_z)  
            R = torch.sigmoid(torch.matmul(X, self.W_xr) + torch.matmul(H, self.W_hr) + self.b_r)  
            H_tilde = torch.tanh(torch.matmul(X, self.W_xh) + torch.matmul(R * H, self.W_hh) + self.b_h)  
            H = Z * H + (1 - Z) * H_tilde  
            outputs.append(H)  
        return torch.stack(outputs), (H,)
    
    def __repr__(self):  
        return f"GRUScratch(num_inputs={self.num_inputs}, num_hiddens={self.num_hiddens}, sigma={self.sigma})"  

data = d2l.TimeMachine(batch_size=1024, num_steps=32) 
gru = GRUScratch(num_inputs=len(data.vocab), num_hiddens=32)
model = d2l.RNNLMScratch(gru, vocab_size=len(data.vocab), lr=4) 
trainer = d2l.Trainer(max_epochs=50, gradient_clip_val=1, num_gpus=1) 
trainer.fit(model, data)
```

> [!HINT] Advantage  
> Compared with LSTMs, GRUs achieve similar performance but tend to be lighter computationally. Generally, compared with simple RNNs, **gated RNNs like LSTMs and GRUs can better capture dependencies for sequences with large time step distances**. GRUs contain basic RNNs as their extreme case whenever the reset gate is switched on. They can also skip subsequences by turning on the update gate.

#### 4. Torch Usage  
For GRU unit, we use `nn.GRU` for contruct a GRU network. This is a simple example :
```python
import torch  
import torch.nn as nn  

# Define input dimension, hidden layer dimension, and number of layers  
input_size = 10  # Input feature dimension  
hidden_size = 20  # Hidden layer dimension  
num_layers = 1  # Number of GRU layers  

# Create GRU model  
gru = nn.GRU(input_size, hidden_size, num_layers)  

# Define input data (sequence length, batch size, input dimension)  
seq_len = 5  # Sequence length  
batch_size = 3  # Batch size  
input_data = torch.randn(seq_len, batch_size, input_size)  

# Initialize hidden state (number of layers, batch size, hidden layer dimension)  
h0 = torch.zeros(num_layers, batch_size, hidden_size)  

# Forward propagation  
output, hn = gru(input_data, h0)  

print("Output shape:", output.shape)  # (Sequence length, batch size, hidden layer dimension)  
print("Final hidden state shape:", hn.shape)  # (Number of layers, batch size, hidden layer dimension)  
```

### (3) Deep RNNs 
The standard method for building this sort of deep RNN is strikingly simple: we <b><mark style="background: transparent; color: orange">stack the RNNs on top of each other</mark></b>. 

Hence for DeepRNN with   L hidden Layers, the structure is as follows, any RNN cell at each time step <mark style="background: transparent; color: red">depends on both the same layer’s value at the previous time step and the previous layer’s value at the same time step</mark>.
![[attachments/Pasted image 20250306114624.png|250]]
we can easily get a deep gated RNN by replacing the hidden state computation by an LSTM or a GRU. which gives :
![[attachments/Pasted image 20250306115345.png]]
and 
![[attachments/Pasted image 20250306115353.png]]

### (4) BiDirectional Recurrent Neural Networks 
For the  case when mask random tokens and  take the context in both directions into account and predict what in the middle,  for the RNN  layers, we can <b><mark style="background: transparent; color: orange">implement two undirectional RNN layers chained together in opposite direction. </mark></b>

firstly we can create two RNNs, all receive the input from $X$ and output as $O$, but in the second layer the connection is that $W_{hh}$ is from the next Hidden Unit. 
![[attachments/Pasted image 20250317120719.png]]
which  create the following calculation equation : 
$$\overrightarrow{H_t} = \phi(X_tW_{xh}^{(f)} + \overrightarrow{H_{t-1}}W_{hh}^{(f)} + b_h^{(f)})$$
$$\overleftarrow{H_t} = \phi(X_tW_{xh}^{(b)} + \overleftarrow{H_{t+1}}W_{hh}^{(b)} + b_h^{(b)})$$
and also the output layer calculation : 
$$O_{t} =  \overrightarrow{H_t} W_{hq}  +  \overleftarrow{H_{t}} W_{hq}' + b_{q}$$
where it shares the same  bias $b_q$ 

note that we may use a mask or placeholder for  the unknown element. 

> [!NOTE]
> The weights for the masked element are not removed or randomized—they are part of the model's parameters and are updated during training.
> mask x3​ by replacing it with a placeholder (e.g., `0` or a special token).

1. **Mask or Replace $x_3$ with a Placeholder**
- If $x_3$ is unknown and you want the model to predict it, you can replace $x_3$ with a special placeholder token (e.g., `[MASK]`, `null`, or a zero vector) in the input sequence.
- For example: 
$$ X = (x_1, x_2, \text{[MASK]}, x_4, x_5)$$
- The model will process the sequence and use the bidirectional context (from $x_1, x_2$ and $x_4, x_5$) to predict $x_3$.
- <b><mark style="background: transparent; color: orange">In frameworks like TensorFlow or PyTorch, you can use a mask tensor to achieve this</mark></b>.

```python
import torch
import torch.nn as nn
import torch.optim as optim

# Define a simple Bi-RNN model
class BiRNN(nn.Module):
    def __init__(self, input_size, hidden_size, output_size):
        super(BiRNN, self).__init__()
        self.hidden_size = hidden_size
        self.rnn = nn.RNN(input_size, hidden_size, bidirectional=True, batch_first=True)
        self.fc = nn.Linear(hidden_size * 2, output_size)  # Bidirectional, so hidden_size * 2

    def forward(self, x):
        # Forward pass through the Bi-RNN
        out, _ = self.rnn(x)  # out shape: (batch_size, seq_len, hidden_size * 2)
        
        # Predict the output for each time step
        out = self.fc(out)  # out shape: (batch_size, seq_len, output_size)
        return out

# Hyperparameters
input_size = 10  # Size of each input element
hidden_size = 16  # Size of the hidden state
output_size = 10  # Size of the output (e.g., vocabulary size for classification)
seq_len = 5  # Length of the input sequence
batch_size = 1  # Batch size

# Create the model
model = BiRNN(input_size, hidden_size, output_size)

# Define a loss function and optimizer
criterion = nn.CrossEntropyLoss()  # For classification tasks
optimizer = optim.Adam(model.parameters(), lr=0.001)

# Example input sequence (batch_size, seq_len, input_size)
# Let's assume x3 is masked (replaced with zeros)
x = torch.randn(batch_size, seq_len, input_size)  # Random input
x[0, 2, :] = 0  # Mask x3 (index 2) by setting it to zeros

# Target (true value for x3)
target = torch.tensor([3])  # Assume x3 should be class 3 (for classification)

# Forward pass
output = model(x)  # Output shape: (batch_size, seq_len, output_size)

# Compute the loss only for the masked position (x3)
# We only care about the prediction at t=2 (index 2)
predicted_x3 = output[0, 2, :]  # Prediction for x3
loss = criterion(predicted_x3.unsqueeze(0), target)  # Compute loss

# Backward pass and optimization
optimizer.zero_grad()
loss.backward()
optimizer.step()

print("Prediction for x3:", torch.argmax(predicted_x3).item())
print("Loss:", loss.item())
```

## 5. Machine Translation
### (1) Encoder-Decoder Seq2Seq Architecture for machine translation
For example,we use the encoder-decoder architecture, in which the  encoder takes a variable -length sequence  as input. ad the decoder acts like language model

> [!HINT] 
> A Seq2Seq model consists of:
> - **Encoder**: Processes the input sequence (e.g., a sentence in the source language) and encodes it into a fixed-size context vector (hidden state).
> - **Decoder**: Generates the output sequence (e.g., translated sentence in the target language) step-by-step, conditioned on the encoder's output.

The overall architechure is like that: 
![[attachments/Pasted image 20250331161605.png|500]]

**for machine translation, both  encoder and  deoder  are  implemented by RNNs**,  

the encoder RNN will take a variable-length sequence as input and transform it into a fixed-shape hidden state. Note here that the <mark style="background: transparent; color: red">Attention Machanism allow  us to access encoder inputs  without convert the  input into  fixed -length representation</mark>. 

The output state, which  **consists a separate RNN**,  ==predict the target  token by the  hidden-state given by RNN encoder== 

### (2) Teacher forcing mechanism
Teacher forcing is a training technique commonly used in sequence-to-sequence (Seq2Seq) models. this training method  is **guild the model with correct tokens to output  during the decoding phase** rather than relying solely on the model's own predictions. 

firstly, the  decoder would start as a sequence `<bos>` meaning beginging of the sequence. 

then <b><mark style="background: transparent; color: orange">At each step,  use the true target word as input instead of the model's prediction</mark></b>, which makes sure the prediction more accurate. 
![[Excalidraw/Chapter7 RNN 2025-03-31 17.04.39|600]]

Teacher forcing **feeds the true previous target token** (from the training dataset) as input to the decoder at each step. 

> [!HINT] MT datasets 
> for machine translation datasets, we can use  https://opus.nlpl.eu/ for specific language search and sample. for example, we can use  
> [Open Subtitles](https://opus.nlpl.eu/OpenSubtitles/en&zh-CN/v2024/OpenSubtitles)or ted dataset for a practice for machine translation. 

### (3) Encoder 
For encoder,  we can use an embedding layer to obtain the feature vector  for each token in the input sequence. The weight of an embedding layer is a matrix. 

Note that we use the encoded indices rather than  original characters  as input :
- **Encoder Input**: we compile the Original sequence `["this", "is", "a", "a", "dog"]` to <b><mark style="background: transparent; color: orange">word indices</mark></b> `[7, 4, 11, 11, 14]`. where <mark style="background: transparent; color: red">1 word mapped to 1 indices</mark>. 
In sequence models like Seq2Seq, we need to handle variable-length sequences by **padding them to a fixed length**. like : 

| Sentence (raw)            | Tokenized (word indices) | Padded/Truncated (max_len=5) |
|---------------------------|--------------------------|------------------------------|
| "I love NLP"              | [4, 12, 7]               | [4, 12, 7, 0, 0]             |
| "Hello"                   | [9]                      | [9, 0, 0, 0, 0]              |
| "This is a long sentence" | [2, 5, 1, 8, 10, 3]      | [2, 5, 1, 8, 10] (truncated) |
we can use pytorch's  build-in tools like torchtext :
```python
from torchtext.data.utils import get_tokenizer
from torchtext.vocab import build_vocab_from_iterator

# Tokenizer (splits sentences into words)
tokenizer = get_tokenizer("basic_english")

# Example tokenization
tokens = tokenizer("I love NLP")  # ['i', 'love', 'nlp']

# Build vocabulary from dataset
sentences = ["I love NLP", "Hello world", "This is a test"]
vocab = build_vocab_from_iterator(
    map(tokenizer, sentences),
    specials=["<pad>", "<unk>"],
)
vocab.set_default_index(vocab["<unk>"])

# Convert sentence to indices
vocab(tokenizer("I love NLP"))  # Output: [2, 3, 4]
```
or using the state-of-the-art hugging face transformers : 

```python
from transformers import AutoTokenizer

# Load a pretrained tokenizer (e.g., BERT)
tokenizer = AutoTokenizer.from_pretrained("bert-base-uncased")

# Tokenize a sentence
encoded = tokenizer("I love NLP", return_tensors="pt")
print(encoded.input_ids)  # Tensor of indices
print(tokenizer.convert_ids_to_tokens(encoded.input_ids[0]))
```

and then, we use [nn.Embedding](https://pytorch.org/docs/stable/generated/torch.nn.Embedding.html#torch.nn.Embedding) (takes word indices and maps them to dense vectors), which is sparse matrix layer to <b><mark style="background: transparent; color: orange">obtain the  feature vector of each token  in the input sequence </mark></b>, note Embedding module ==just convert the word into a unique vector==; then we can *just input the indices array and generate a corresponding matrix* : 
![[Excalidraw/Chapter7 RNN 2025-03-31 18.36.54]]

> [!warning] 
>  note the vocabulary size dependents on how many words dataset contains in total. and finally it would use an Linear layer to map the time-step output as argmax to remapping to the sequence **consists of all original words in train target set**. 
>  
>  note the vocabulary size of encoder and decoder **do not need to be identical**, but they **must be consistent with their respective input/output data** 

that is the mapping **example of nn.Embedding** :  
```python
embedding = nn.Embedding(vocab_size=30, embed_size=3)  #
input_indices = torch.tensor([7, 4, 11, 11, 14])  # shape: (5,)
embedded_vectors = embedding(input_indices)  # shape: (5, 3)
```

> [!important] 
> note the embedding layer <mark style="background: transparent; color: red">convert a variant-length sentence to a fixed length vector</mark>. which make different words/sentence have the input of same length. 

we can use the parameter initialization : 
```python
def init_seq2seq(module): #@save
    """Initialize weights for Seq2Seq."""
    if type(module) == nn.Linear:
        nn.init.xavier_uniform_(module.weight)
    if type(module) == nn.GRU:
        for param in module._flat_weights_names:
            if "weight" in param:
                nn.init.xavier_uniform_(module._parameters[param])
```

firstly, we can use a function $f$ for the transformation of the RNN's  recurrent layer. which use $x_t$ and $h_{t-1}$ as input : 
$$h_{t} =  f(x_{t}, h_{t-1})$$
then we generate the hidden state $h_1, h_2, \dots h_T$, the encoder <mark style="background: transparent; color: red">transforms the  hidden states at all time  into a vatiable c through custimized  function</mark> $q$ : 
$$c =  q(h_{1}, \dots,  h_{T})$$
We use the embedding layer as the input of  next rnn layer,  which can just be <mark style="background: transparent; color: red">implemented with a multilayer GRU</mark>. 

```python
l1 =  nn.Embedding(vocab_size, embed_size) 
l2 = nn.GRU(embed_size,  num_hiddens,  num_layers,  dropout=0.0)

input_sentence = [1,2,3,4,5, ... ] 
encoded_vecs = l1(input_sentence) 
output, h_n = l2(encoded_vecs)  
```

where : 
- output is tensor of shape $(L,D∗H_{out})$ for unbatched input,  
- **h_n** is tensor of shape (D∗num_layers,Hout) or (D∗num_layers,N,Hout) containing the final <mark style="background: transparent; color: red">hidden state for the input sequence</mark>. 


> [!NOTE] Output of encoded layer : Context Variable 
> After we reach the hidden layer result $h_{1}, \dots h_{T}$, we can use $q$ to calculte another context variable $c$, 

### (4) Decoder 
When training the decoder, we will give a target sequence  $y_1, y_2, \dots y_t$, for each time step $t$. we use  the  decoder **assigns a predicted probablity to the possible token occuring at time step $t+1$.** which <mark style="background: transparent; color: red">can be predicted by the context variable</mark> $\boldsymbol{c}$ and previous given $y_1, \dots y_{t'}$. 

i.e., we can use an output layer to compute the predictive distribution <mark style="background: transparent; color: red">(use a softmax function)</mark> 
$$p (y_{t +1} |y_{1}, y_{2} , \dots  y_{t} , c )$$
over the subsequent layer. 
> [!HINT] Output of decoder 
>  The **final output at each time step** is typically a probability distribution over the **target vocabulary**, represented as **word indices** after applying `argmax`.

Then, we can use a function $g$ to express the transformation of the decoder's hidde layer:
$$s_{t} = g(y_{t-1} , c , s_{t-1})$$

in the decoder process, it use target output sequence $X$ and **encoder state** as input. note we directly <mark style="background: transparent; color: red">use the hidden state at the final time step of the encoder to initialize the hidden state of the decoder</mark>, so the RNN encoder and the RNN decoder have the same number of layers and hidden units (but time step may have discrepancy).  

![[attachments/Pasted image 20250401105929.png]]
where X shape: (batch_size, num_steps)

The following code gives an implementation :  
```python
import torch
import torch.nn as nn
import torch.nn.functional as F

class Seq2SeqEncoder(nn.Module):
    """Simplified RNN encoder for sequence to sequence learning"""
    def __init__(self, vocab_size, embed_size, num_hiddens, num_layers, dropout=0):
        super().__init__()
        self.embedding = nn.Embedding(vocab_size, embed_size)
        self.rnn = nn.GRU(embed_size, num_hiddens, num_layers, dropout=dropout)
        
        # Initialize weights
        for name, param in self.rnn.named_parameters():
            if 'weight' in name:
                nn.init.xavier_uniform_(param)
        
    def forward(self, X):
        # X shape: (batch_size, seq_len)
        X = X.t()  # Transpose to (seq_len, batch_size)
        embeddings = self.embedding(X)  # (seq_len, batch_size, embed_size)
        outputs, hidden = self.rnn(embeddings)
        # outputs: (seq_len, batch_size, num_hiddens)
        # hidden: (num_layers, batch_size, num_hiddens)
        return outputs, hidden
```

The decoder implementation is as follows : 
```python 
class Seq2SeqDecoder(nn.Module):
    """Simplified RNN decoder for sequence to sequence learning"""
    def __init__(self, vocab_size, embed_size, num_hiddens, num_layers, dropout=0):
        super().__init__()
        self.embedding = nn.Embedding(vocab_size, embed_size)
        # Input is embed_size + num_hiddens (for context)
        self.rnn = nn.GRU(embed_size + num_hiddens, num_hiddens, num_layers, dropout=dropout)
        self.fc = nn.Linear(num_hiddens, vocab_size)
        # Initialize weights
        for name, param in self.rnn.named_parameters():
            if 'weight' in name:
                nn.init.xavier_uniform_(param)
        nn.init.xavier_uniform_(self.fc.weight)
        
    def forward(self, X, hidden, context):
        # X shape: (batch_size, seq_len)
        X = X.t()  # Transpose to (seq_len, batch_size) 
        embeddings = self.embedding(X)  # (seq_len, batch_size, embed_size)
	
		# **There we handle the  
        context = context.unsqueeze(0).expand(embeddings.size(0), -1, -1)
        
        # Concatenate embeddings and context
        rnn_input = torch.cat([embeddings, context], dim=-1)
        
        outputs, hidden = self.rnn(rnn_input, hidden)
        predictions = self.fc(outputs)
        
        # Transpose back to (batch_size, seq_len, vocab_size)
        predictions = predictions.transpose(0, 1)
        return predictions, hidden
```

For seq  
```python
class Seq2Seq(nn.Module):
    """Simplified Seq2Seq model"""
    def __init__(self, encoder, decoder):
        super().__init__()
        self.encoder = encoder
        self.decoder = decoder
        
    def forward(self, src, trg):
        # src: (batch_size, src_seq_len)
        # trg: (batch_size, trg_seq_len)
        enc_outputs, hidden = self.encoder(src)
        outputs, _ = self.decoder(trg, hidden, hidden[-1])    
        return outputs
```

and also, in prediction process, we can  directly use the output of the timestep $t'-1$ to predict the result of timestep  $t'$. 
![[attachments/Pasted image 20250401114238.png|450]]

note that we use  `outputs, _ = self.decoder(trg, hidden, hidden[-1]) ` to <mark style="background: transparent; color: red">make the hidden state generated by the  encoder</mark> as the initial hidden state of decoder. 


> [!CAUTION] Size match between encoder and  decoder 
> In Seq2Seq Structure, The `hidden_num` of decoder must match the `hidden_num` of encoder. 
> 1. We use the last layer of `hidden state` of encoder to initalize that in decoder  
> 2. The context size is reshaped as :  `[decode_seq_len(extended), batch_size, hidden_num(encoder output)]` , <mark style="background: transparent; color: red">which is also last layer of hiddden state</mark> .  note that method duplicate the context transfer. but this method is necessary, which is analyzed in [[📘ClassNotes/⌨️Programming/👨‍🎓Deep Learning/⚓Deep Learning Basic Concepts/Implements/Seq2Seq 上下文向量的传递方式|Seq2Seq Context Transfer Method]]

### (5) Precision Evaluation (n-grams criterion)
BLEU (Bilingual Evaluation Understudy) is often used for measuring the quality of output sequences for different applications. 

In principle, for any n-grams in the predicted sequence, BLEU evaluates whether this n-grams appears in the target sequence.

Precision $p$ is the ratio of the number of matched n-grams in the predicted and target sequences to the number of n-grams in the predicted sequence. For example, given a target sequence $A, B, C, D, E, F$, and a predicted sequence $A, B, B, C,  D$, we have p1 = 4/5, p2 = 3/4, p3 = 1/3, and p4 = 0. 

<mark style="background: transparent; color: red">where p is calculated by longest  n-grams match, maximum (3) is B,C,D, and for 2 gram match is (AB, BC, CD) in total bigrams in prediction (4)</mark>
$$p_{n} =  \frac{\text{number of  match unigrams}}{\text{total  n-grams in prediction}}$$
BLEU is defined as: 
$$\text{BLEU} =  \exp\left(\min\left( 0,1  - \frac{\text{len}_{target}}{\text{len}_{pred}}\right)\right) \prod_{n=1}^{k} p_{n} ^{\frac{1}{2^{n}  }}$$
where k is the longest n-grams for matching (in above case is 3, so the `p4=0` is neglected). also the $p_n$ is greater when hit a longer match (with the part $p_{n}^{\frac{1}{2^{n}}}$, whcih is greater than 1 when make a match). 


In the **real application case**, we can use `bleu` library from hugging face : 
```python
import evaluate 
predictions = ["hello there general kenobi", "foo bar foobar"]
references = [
	 ["hello there general kenobi", "hello there !"],
	 ["foo bar foobar"]
]
bleu = evaluate.load("bleu")
results = bleu.compute(predictions=predictions, references=references)
print(results)
```

### (6) Greedy Search and Tree search 
For greedy search, it <mark style="background: transparent; color: red">selects the token with the highest conditional probability in softmax result</mark>. 
![[attachments/Pasted image 20250401124142.png]]
the strategy of which is 
$$\begin{align*}
y_{t'}=\underset{y\in\mathcal{Y}}{\operatorname{argmax}}\,P(y\mid y_1,\ldots,y_{t'-1},c).
\end{align*}$$

In our machine translation example, if the decoder truly recovered the probabilities of the underlying generative process, then this would give us the most likely translation. ==But unfortunately, there is no guarantee that greedy search will give us this sequence==. 

we note that <mark style="background: transparent; color: red">in the conditional probablity output, different time step may have different conditional probablity</mark>, hence if we choose C at second position, we may got higher probablity at next place, then it may be a better solution than before. 
![[attachments/Pasted image 20250401132919.png]]

> [!warning]
> The best solution we want is the one that match expression 
> $$\max \prod_{t=1}^{T} P(y_{t}| y_{1}, y_{2}, \dots  y_{t-1} , c)$$ 
> so greedy search not give us the best result. 

one way is use exhausting search, which is not  applicable.

Applicably, we can use <mark style="background: transparent; color: red">beam search</mark> for better solution. we can select $k$  token with the highest predicted probabilities. and then try the possible search result.  

![[attachments/Pasted image 20250401165117.png|500]]

Note this method is select k top At time step 1, and it not consider the step later. 

note that beam search may not gain a good result, but we can hybrid the search techniques : 

**Stochastic Beam Search / Diverse Beam Search**. 
- **Pros**: Introduces randomness or diversity to explore more paths.
- **Cons**: Still not guaranteed to find the best sequence, but better coverage than vanilla Beam Search. 

Also note we can use a trade-off way : to keep the branch within a probablity top limitation, that is : 
$$\text{keep branch that} :  |max(p) - p_{b} | < \text{limit} $$
and it may reach a reasonable search result. 


we choose the sequence with the highest of the following score as the output sequence: 
$$\begin{align*}
\frac{1}{L^\alpha} \log P(y_1, \ldots, y_L \mid c) = \frac{1}{L^\alpha} \sum_{t'=1}^L \log P(y_{t'} \mid y_1, \ldots, y_{t'-1}, c)
\end{align*}$$

## 6. Application : English-Chinese Machine Translation 
### (1) Introduction and data loading 
In this application practicing, we will use `20k-en-zh-translation-pinyin-hs` dataset from hugging face to train our machine translation model to do a en-zh translation task.  

firstly, we load the dataset by  `datasets` library, the description can be found at https://huggingface.co/datasets/swaption2009/20k-en-zh-translation-pinyin-hsk.  

since every line starts of different prefix represent different data, we  can use following to save them to a csv file for better training : 
```python
import datasets 

pinyin_hsk_dataset = load_dataset("swaption2009/20k-en-zh-translation-pinyin-hsk", split="train")  

# region dataset preparation  
def save_dataset_to_csv(dataset, filename):  
    """  
    Save the dataset to a csv file.    :param dataset:    :param filename:    :return:  
    """    text_lines = dataset['text']  
    total_samples = len(text_lines) // 5  
    print("Total samples:", total_samples)  
    english = [line.split(": ")[1] for line in text_lines if line.startswith("english: ")]  
    hsk = [line.split(": ")[1] for line in text_lines if line.startswith("hsk: ")]  
    mandarin = [line.split(": ")[1] for line in text_lines if line.startswith("mandarin: ")]  
    pinyin = [line.split(": ")[1] for line in text_lines if line.startswith("pinyin: ")]  
  
    dataset_fields = ["english", "hsk", "mandarin", "pinyin"]  
    dataframe_tmp = pd.DataFrame(list(zip(english, hsk, mandarin, pinyin)), columns=dataset_fields)  
    dataframe_tmp.to_csv(filename, encoding='utf-8', index=False) 
    
# and then save data to csv file 
save_dataset_to_csv(pinyin_hsk_dataset, "pinyin_hsk_dataset.csv") 

# then we can load data by: 
data = pd.read_csv("./pinyin_hsk_dataset.csv") 
```

### (2) Self-defined tokenizer 
We give the implementation of the self-defined tokenizer :  we can define a tokenizer by  ingesting the  word just from the original dataset.  

```python
from tokenizers import Tokenizer, models, trainers 

data = pd.read_csv("./pinyin_hsk_dataset.csv")  
en_sentences = data["english"].tolist()  # list of english sentences  
zh_sentences = data["mandarin"].tolist() 

def train_tokenizer(corpus, max_size=1000):  
    tokenizer = Tokenizer(models.WordPiece(unk_token="[UNK]"))  
    trainer = trainers.WordPieceTrainer(  
        vocab_size=max_size,  
        special_tokens=["[UNK]", "[CLS]", "[SEP]", "[PAD]", "[MASK]"],  
    )  
    tokenizer.train_from_iterator(corpus, trainer=trainer)  
    return tokenizer 


```





in training process, it would output the  
![[attachments/Pasted image 20250410015144.png]]

when the loss downs to about 1.28-1.3 in train process, the machine translation model can predict in a really good performance as follows : 
```python
English sentences:  ['[CLS]hey,randy,haveyouhadyourlunchyet?[SEP]']  
Chinese sentences:  ['[CLS]嘿，蓝迪，你吃午饭了吗？[SEP]' ]
Chinese predictions:  ['[CLS]嘿，蓝迪，你吃午饭了吗？[SEP]2828282828282828282828282828282828282828282828282828282828282828282828282828282828282828282828282828',  
```

   