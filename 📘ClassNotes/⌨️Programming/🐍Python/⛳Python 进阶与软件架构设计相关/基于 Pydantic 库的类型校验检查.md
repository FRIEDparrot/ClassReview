---
~
---
Pydantic 是一个用于**数据验证和设置管理的 Python 库**。它的主要优势在于结合了 Python 的类型注解，使得在构建数据模型时能自动进行有效性验证与数据解析。Pydantic 最常用于处理 API 输入、配置管理、数据校验等场景。

例如 : 
```python
from pydantic import BaseModel, EmailStr, field_validator
from typing import Optional

class RequestLoginForm(BaseModel):
    email: Optional[EmailStr] = None  # 允许邮箱为空
    username: Optional[str] = None    # 允许用户名为空
    password: Optional[str] = None
    code: Optional[str] = None        # 仅用于验证码登录

    @field_validator("password", mode="before")
    def validate_password_or_code(cls, value, values):
        """ 确保至少有一种登录方式被正确提供 """
        if not value and not values.get("code"):
            raise ValueError("密码和验证码必须至少提供一个")
        return value

    @field_validator("email", mode="before")
    def validate_email_or_username(cls, value, values):
        """ 确保邮箱和用户名至少提供一个 """
        if not value and not values.get("username"):
            raise ValueError("邮箱和用户名必须至少提供一个")
        return value
```

