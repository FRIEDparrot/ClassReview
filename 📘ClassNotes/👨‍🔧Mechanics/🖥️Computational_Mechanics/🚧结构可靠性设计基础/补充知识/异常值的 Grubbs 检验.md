参考 [Grubbs检验法](https://zhuanlan.zhihu.com/p/482006257) 和 [KS检验、t检验、f检验、Grubbs检验、狄克逊(Dixon)检验、卡方检验小结](https://zhuanlan.zhihu.com/p/370145340)

首先取 
$$Z = \frac{X - \overline{X}}{\sigma}$$
由于异常值的存在使得 $\sigma$ 增加, 如果包含 N 个观察值, 则 Z 极限为:
$$\frac{N - 1}{\sqrt{N}}$$
如果 Z 值大于该临界值, 则 P 值小于 0.05. 

```python
# grubbs method max test with alpha = 0.01  
import re  
import numpy as np  
import pandas as pd  
from outliers import smirnov_grubbs as grubbs  
  
def replace_space(df:pd.DataFrame)->pd.DataFrame:  
    df_cleaned = df.map(lambda cell: pd.NA if isinstance(cell, str) and re.fullmatch(r'\s*', cell) else cell)  
    return df_cleaned  
  
dataframe = replace_space(pd.read_csv("FJJZ_data.csv"))  
l = len(dataframe)  
print(l)  
  
data_new = pd.DataFrame(columns=dataframe.columns, dtype=np.float32)  
  
for c in dataframe.columns:  
    dt = np.array(dataframe[c].dropna(), dtype=np.float32)  
    dt_new = grubbs.max_test(dt,alpha=0.01)  
    print(len(dt), len(dt_new))  
    col_data = np.append(dt_new, [0 for i in range(l-len(dt_new))])  
    data_new[c] = col_data  
  
data_new.replace(0, pd.NA, inplace=True)  
data_new.to_csv("grubbs_check.csv", index=False)
```

一般而言, 样本量 < 50 采用 S-H 检验, 大于 50 采用 K-S 检验
