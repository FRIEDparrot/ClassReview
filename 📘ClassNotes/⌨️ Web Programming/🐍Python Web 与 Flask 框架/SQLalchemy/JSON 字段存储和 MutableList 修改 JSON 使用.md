
1. **JSON字段未使用Mutable扩展**  
    `msg_received`字段为JSON类型，若未使用SQLAlchemy的`MutableList`扩展，对列表的修改不会被ORM检测，导致更新不生效。 

```python
from sqlalchemy.ext.mutable import MutableList
from sqlalchemy import JSON

class PersonalData(Base):
    __tablename__ = 'personal_data'
    # 其他字段...
    msg_received = Column(MutableList.as_mutable(JSON), default=[], server_default='[]')
```

因此以下是一个正确的采用 MultableList 的字段定义方式， 以支持使用 append, + 等符号来进行修改 : 注意 `MutableList.as_mutable` 必须要加上

```python
from sqlalchemy import Integer, String, Column, JSON  
from typing import Optional, List, TypedDict  
from sqlalchemy.ext.mutable import MutableList

class PersonalData(BaseModel):  
    # noinspection PyTypeChecker  
    __tablename__ = 'table_personal_data'  
  
    session_factory = dbManagerAccounts.session_factory  
    Search_Keys = ['id', 'user_id']  
    Order_Keys = ['id', 'user_id']  
  
    id: Mapped[int] = mapped_column(primary_key=True, unique=True)  
    user_id: Mapped[int] = mapped_column(Integer, unique=True,  
                                         nullable=False)  
    bio: Mapped[str] = mapped_column(String(255), nullable=True)  
    interest_tags: Mapped[List[str]] = mapped_column(MutableList.as_mutable(JSON), nullable=False,  
                                                     default=[])  
    # we store the last 100 history tags  and filter that not in the current interest tags  
    recommend_tags: Mapped[List[str]] = mapped_column(MutableList.as_mutable(JSON), nullable=False,  
                                                      default=[])  
    following_num: Mapped[int] = mapped_column(Integer, default=0)  
    followers_num: Mapped[int] = mapped_column(Integer, default=0)  
    blogs_num: Mapped[int] = mapped_column(Integer, default=0)  
    history: Mapped[List[PersonalDataSchema.History]] = mapped_column(  
        MutableList.as_mutable(JSON), nullable=False,  
        default=[], server_default='[]'  
    )  
    favorite_folder: Mapped[List[PersonalDataSchema.FavoriteFolder]] = mapped_column(  
        MutableList.as_mutable(JSON), nullable=False,  
        default=[], server_default='[]'  
    )  
    preferred_msg_lang: Mapped[str] = mapped_column(String(10), nullable=True)  
    msg_count: Mapped[int] = mapped_column(Integer, default=0)  # unread message count  
    msg_received: Mapped[List[int]] = mapped_column(MutableList.as_mutable(JSON),  
                                                    nullable=False,  
                                                    default=[],  
                                                    server_default='[]')  
    msg_sent: Mapped[List[int]] = mapped_column(MutableList.as_mutable(JSON),  
                                                nullable=False,  
                                                default=[],  
                                                server_default='[]')
```
