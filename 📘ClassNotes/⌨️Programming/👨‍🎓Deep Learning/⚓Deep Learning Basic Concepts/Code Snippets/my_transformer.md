
```python fold title:full-transformer-implementation 
import torch  
from torch import nn  
from matplotlib import pyplot as plt  
import math  
  
  
def masked_softmax(x, valid_lens: torch.Tensor, masked_value=-1e9):  
    """  
    Masked softmax implementation, Used for softmax layer after the attention calculation  
    .. example::        .. code-block python ::            x = torch.rand(2, 2, 3)            valid_lens = torch.tensor([1, 2])            masked_softmax(x, valid_lens)  # 2 * 2 * 3 masked by [1, 2]            masked_softmax(x, torch.tensor([[1, 2], [1, 3]]))  # masked by [[1, 2], [1, 3]]  
    :param x: 3D tensor, attention scores shaped [batch_size, queries_len, keys_len]    :param valid_lens: **array of valid lens** of each key, to mask the future tokens and perform other functions    :param masked_value: value to fill mask    :return:  
    """    
    b, q, k = x.shape  
    if valid_lens.dim() == 1:  # [batch_size]  
        valid_lens = valid_lens.repeat_interleave(q)  # [b * q]  
    else:  
        assert valid_lens.shape == (b, q)  
        valid_lens = valid_lens.reshape(-1)  # [b * q]  
  
    # Flatted by keys    x_flat = x.reshape(-1, k)  # [b * q, k]  
  
    # [k, 1] <= [b * q] -> [k, b * q]    mask = (torch.arange(k).unsqueeze(-1) < valid_lens).transpose(0, 1)  # [b * q, k]  
    x_flat_masked = x_flat.masked_fill(~mask, masked_value)  
    return nn.functional.softmax(x_flat_masked, dim=-1).reshape(x.shape)  
  
  
def apply_window_mask(scores, window_mask):  
    """  
    Apply window mask to attention scores    :param scores: Attention scores tensor of shape [batch_size, query_len, key_len]    :param window_mask: Window mask tensor of shape [num_windows, query_len, key_len]    :return: Masked scores with the same shape as input scores  
    """    if window_mask is None:  
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
        :param queries: [batch_size, query_len, num_hiddens]   -> which is query sentence batch        
        :param keys: [batch_size, key_len, num_hiddens]        
        :param values: [batch_size, value_len, num_hiddens]        
        :param valid_lens: masked by valid_lens        
        :param window_mask: masked by window [num_windows, query_len, key_len]  
        note : window mask is for constraint the attention range        
        :return:  
        """  
        #  in attention mechanism, key_len == value_len, since we need to bmm the key and value  
        assert keys.shape[1] == values.shape[1]  
        d = torch.tensor(queries.shape[-1])  # d = hiddens_dim  
        scores = torch.bmm(queries, keys.transpose(1, 2)) / torch.sqrt(d)  
  
        # apply window mask in the attention  # [batch_size, query_len, key_len]  
        scores = apply_window_mask(scores, window_mask)  
        if valid_lens is not None:  
            attn = masked_softmax(scores, valid_lens)  # [batch_size, query_len, key_len]  
        else:  
            attn = nn.functional.softmax(scores, dim=-1)  
        attn = self.dropout(attn)  
        self.attn = attn  
        return torch.bmm(attn, values), attn  # [batch_size, query_len, num_hiddens]  
  
    def show_attention_scores(self, batch_idx=0):  
        if self.attn is None:  
            raise RuntimeError('call forward first')  # [batch_size, query_len, key_len]  
        # show the heat map of attention scores        attn_mat = self.attn[batch_idx].detach().cpu()  
        fig = plt.figure(figsize=(10, 10))  
        ax = fig.add_subplot(111)  
        ax.imshow(attn_mat.numpy(), cmap='gray_r')  
        ax.set_xticks(range(len(attn_mat)))  
        ax.set_yticks(range(len(attn_mat)))  
        ax.set_xticklabels(range(len(attn_mat)))  
        ax.set_yticklabels(range(len(attn_mat)))  
        plt.show()  
  
  
class AdditiveAttention(nn.Module):  
    """  
    Additive attention scores    """  
    def __init__(self, num_hiddens=512, dropout=0.1):  
        super(AdditiveAttention, self).__init__()  
        self.W_q = nn.LazyLinear(num_hiddens, bias=False)  
        self.W_k = nn.LazyLinear(num_hiddens, bias=False)  
        self.W_v = nn.Linear(num_hiddens, 1, bias=False)  
  
        self.dropout = nn.Dropout(dropout)  
  
    def forward(self, queries, keys, values, valid_len=None, window_mask=None):  
        proj_q = self.W_q(queries)  # [batch_size, query_len, num_hiddens]  
        proj_k = self.W_k(keys)  # [batch_size, key_len, num_hiddens]  
        # note that it's apparent that can't be added together  
        proj_q = proj_q.unsqueeze(2)  # [query_len, 1]  
        proj_k = proj_k.unsqueeze(1)  # [1 , key_len]  
        # use spreading to calculate the attention weights       
        features = torch.tanh(proj_q + proj_k)  # [batch_size, query_len, key_len, num_hiddens]  
        scores = self.W_v(features).squeeze(-1)  # [batch_size, query_len, key_len]  
        scores = apply_window_mask(scores, window_mask)  
  
        if valid_len is not None:  
            attn = masked_softmax(scores, valid_len)  
        else:  
            attn = nn.functional.softmax(scores, dim=-1)  
        attn = self.dropout(attn)  
        return torch.bmm(attn, values), attn  
  
  
class PositionalEncoding(nn.Module):  
    """Apply sine/cosine positional encoding to input sequence."""
    def __init__(self, max_seq_len, d_model, dropout=0):  
        """  
        :param max_seq_len: size of input sequence, must >= seq_len        
        :param d_model:  length of the  embedding encode sequence (hidden_size)  
        .. warning :: d_model should not to be too big, about 64 or 128 is recommended.  
        pe dimension : [max_seq_len, hidden_dim]        """        super().__init__()  
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
        """        assert x.ndim == 3 and x.shape[2] == self.pe.shape[-1], 'Input shape should be [batch_size, seq_len, d_model]'  
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
  
  
class FeedForwardNetwork(nn.Module):  
    def __init__(self, input_num, hidden_num, output_num, n_layers=2):  
        super(FeedForwardNetwork, self).__init__()  
  
        assert n_layers >= 2  
  
        self.net = nn.Sequential()  
        self.net.add_module('linear_start', nn.Linear(input_num, hidden_num))  
        self.net.add_module('relu_start', nn.ReLU())  
        for i in range(n_layers - 2):  
            self.net.add_module('linear{}'.format(i), nn.Linear(hidden_num, hidden_num))  
            self.net.add_module('relu{}'.format(i), nn.ReLU())  
  
        self.net.add_module('linear_end'.format(n_layers - 1), nn.Linear(hidden_num, output_num))  
  
    def forward(self, x):  
        """  
        :param x:        :return:  
        """        x = self.net(x)  
        return x  
  
  
class ResidualFFNBlock(nn.Module):  
    def __init__(self, d_model, hidden_num=None, n_layers=2, dropout=0.2):  
        super(ResidualFFNBlock, self).__init__()  
        if hidden_num is None:  
            hidden_num = 4 * d_model  # use 4 times of d_model as hidden size  
        self.ffn = FeedForwardNetwork(input_num=d_model,  
                                      hidden_num=hidden_num,  
                                      output_num=d_model,  
                                      n_layers=n_layers)  
        self.norm = nn.LayerNorm(d_model)  
        self.dropout = nn.Dropout(dropout)  
  
    def forward(self, x):  
        output = self.ffn(x)  
        output = self.norm(x + output)  # add & normalize  
        output = self.dropout(output)  
        return output  
  
  
class MyAttentionLayer(nn.Module):  
    def __init__(self, d_model, num_heads=8, lr=0.0005, dropout=0.2):  
        """  
        Attention layer implementation  
        note that d_model == num_hiddens  
        :param d_model:        :param num_heads:        :param lr: learning rate        :param dropout:        """        super(MyAttentionLayer, self).__init__()  
        # note that d_model is input size  
        self.d_model = d_model  
        self.num_heads = num_heads  
        assert d_model % num_heads == 0  
        self.head_dim = d_model // num_heads  
  
        # use d_model as num_hiddens  
        self.num_hiddens = self.d_model  
  
        self.w_q = nn.Linear(d_model, self.num_hiddens)  
        self.w_k = nn.Linear(d_model, self.num_hiddens)  
        self.w_v = nn.Linear(d_model, self.num_hiddens)  
  
        # the output dim is hidden dim, while  
        self.w_o = nn.Linear(self.num_hiddens, self.num_hiddens)  
  
        # use dot-product attention as attention in attention layer  
        self.attention = DotProductAttention(dropout=dropout)  
        self.lr = lr  
  
    def forward(self, queries, keys, values, valid_lens=None, window_mask=None, average_attn_weights=True):  
        """  
  
        :param queries: The query sequence [batch_size, num_queries, d_model]        :param keys:        :param values:        :param valid_lens:        :param window_mask:        :param average_attn_weights:  
        :return:  
            output : [batch_size, num_queries, d_model], which is same as queries        """        batch_size, num_queries, _ = queries.shape  
  
        # reshape the input into  [batch_size, num_heads, num_kv, d_model/num_heads]  
        q = self.reshape_by_num_heads(self.w_q(queries))  
        k = self.reshape_by_num_heads(self.w_k(keys))  
        v = self.reshape_by_num_heads(self.w_v(values))  
  
        if valid_lens is not None:  
            # copy valid_lens by num_heads times in batch  
            valid_lens = valid_lens.repeat_interleave(self.num_heads, dim=0)  
  
        # applying attention  
        output, attention_weights = self.attention(  
            q, k, v, valid_lens, window_mask  
        )  # [batch_size * heads, query_len, num_hiddens/heads]  
  
        attention_weights = attention_weights.reshape(batch_size, self.num_heads, num_queries, num_queries)  
        if average_attn_weights:  
            attention_weights = attention_weights.sum(dim=1) / self.num_heads  
  
        # transpose output  
        output = output.reshape(  
            batch_size, self.num_heads, num_queries, self.num_hiddens // self.num_heads  
        ).permute(0, 2, 1, 3).reshape(batch_size, num_queries, self.num_hiddens)  
  
        output = self.w_o(output)  # [batch_size, query_len, embed_size]  
        return output, attention_weights  
  
    def reshape_by_num_heads(self, x):  
        """  
        reshape the x by num-heads        """        b, s, h = x.shape  # [batch_size, seq_len, hidden_size]  
        num_heads = self.num_heads  
        assert h % num_heads == 0, "hidden size must be the "  
        x = x.reshape(b, s, num_heads, h // num_heads)  
        # reshape the num-heads into the second dim  
        x = x.permute(0, 2, 1, 3)  
        x = x.reshape(b * num_heads, s, -1)  
        return x  
  
  
class ResidualAttentionBlock(nn.Module):  
    """  
    Attention Block With Residual Connection    """  
    def __init__(self, input_size, num_heads=8, dropout=0.2):  
        super().__init__()  
        self.attention = MyAttentionLayer(input_size, num_heads, dropout=dropout)  
        self.norm = nn.LayerNorm(input_size)  
        self.dropout = nn.Dropout(dropout)  
  
    def forward(self, queries, keys, values, valid_lens=None, window_mask=None, average_attn_weights=True):  
        # note that  
        output, attn_weights = self.attention(queries,  
                                              keys,  
                                              values,  
                                              valid_lens,  
                                              window_mask,  
                                              average_attn_weights)  
  
        output = self.norm(queries + output)  # create residual connection  
        output = self.dropout(output)  
        return output, attn_weights  
```

Then here we give the  implementation of  encoder and decoder of transformer :  

```python 
class TransformerEncoder(nn.Module):  
    def __init__(self,  
                 vocab_size,  
                 embed_dim,  
                 num_heads,  
                 n_layers=3,  
                 ffn_layers=2,  
                 dropout=0.2):  
        """  
  
        :param vocab_size: vocabulary size of encoder        :param embed_dim: embedding size        :param num_heads:        :param n_layers:        :param ffn_layers:        :param dropout:        """        super(TransformerEncoder, self).__init__()  
        self.vocab_size = vocab_size  
        self.embed_dim = embed_dim  
        self.num_heads = num_heads  
  
        # specify a much larger encoding  
        self.positional_encoding = PositionalEncoding(max_seq_len=vocab_size, d_model=embed_dim)  
        self.embedding = nn.Embedding(num_embeddings=vocab_size, embedding_dim=embed_dim)  
          
        # stack n layers in transformer encoder  
        self.attention_forward_layer = nn.ModuleList([  
            nn.Sequential(  
                ResidualAttentionBlock(input_size=embed_dim, num_heads=num_heads, dropout=dropout),  
                ResidualFFNBlock(d_model=embed_dim, n_layers=ffn_layers, dropout=dropout)  
            ) for _ in range(n_layers)  
        ])  
  
    def forward(self, queries, keys, values, valid_lens=None, window_mask=None, average_attn_weights=True):  
        q = self.embedding(queries)  
        k = self.embedding(keys)  
        v = self.embedding(values)  
        r"""  
        Since the variance of dot product use `attention_weights = softmax(Q·K^T / \sqrt{d})`          
        Without scaling, as the embedding dimension grows larger,   
            the dot products would have larger variance.   
        """  
        q = self.positional_encoding(q * math.sqrt(self.embed_dim))  
        k = self.positional_encoding(k * math.sqrt(self.embed_dim))  
        v = self.positional_encoding(v * math.sqrt(self.embed_dim))  
  
        return self.attention_forward_layer(q, k, v, valid_lens, window_mask, average_attn_weights)
```
