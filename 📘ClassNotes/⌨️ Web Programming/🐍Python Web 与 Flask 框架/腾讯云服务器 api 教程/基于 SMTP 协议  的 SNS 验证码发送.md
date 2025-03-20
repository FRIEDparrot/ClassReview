SMTP服务（Simple Mail Transfer Protocol）是一种用于在网络上发送电子邮件的协议，它定义了电子邮件如何被发送和接收的标准。SMTP服务通常用于发送电子邮件，而不是接收邮件 

首先， 采用如下方法安装腾讯云 api  : 
```python 
pip install tencentcloud-sdk-python
```

首先, python 有自带的 smtplib 部分, 参考 [腾讯云教程](https://cloud.tencent.com/document/product/1288/97581) 进行 python 调用发送邮件 

```python 
from email.utils import formataddr  

# 单个地址  
name = "John Doe"  
email = "john.doe@example.com"  
formatted_address = formataddr((name, email))  
print(formatted_address)  # Output: "John Doe <john.doe@example.com>"  

# 多个地址  
addresses = [  
    ("John Doe", "john.doe@example.com"),  
    ("Jane Smith", "jane.smith@example.com"),  
]  
formatted_addresses = [formataddr(pair) for pair in addresses]  
print(", ".join(formatted_addresses))  
# Output: "John Doe <john.doe@example.com>, Jane Smith <jane.smith@example.com>"
```
