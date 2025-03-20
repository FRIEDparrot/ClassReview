在 SQLAlchemy 中，`Mapped` 是一种类型提示，它通常用来标记 ORM 映射类中的字段。`Mapped` 类型是 SQLAlchemy 2.0 中引入的，用于替代较旧的 `Column` 类型，并结合 PEP 563 提供了更好的类型推断支持。它可以帮助开发者通过静态类型检查更好地推断出数据库模型字段的类型。

### Mapped 函数的使用

在 SQLAlchemy 2.0 版本中，`Mapped` 被用来描述模型类中的字段。你会在 SQLAlchemy 2.0 中的模型类中看到类似下面的定义：

```python
from sqlalchemy.orm import Mapped, mapped_column
from sqlalchemy import Integer, String
from sqlalchemy.ext.declarative import declarative_base

Base = declarative_base()

class User(Base):
    __tablename__ = 'users'

    id: Mapped[int] = mapped_column(Integer, primary_key=True)  # 使用 Mapped 映射列
    name: Mapped[str] = mapped_column(String)  # 使用 Mapped 映射列
```

在这个例子中：
- `Mapped[int]` 是指 `id` 列是一个整数类型。
- `Mapped[str]` 是指 `name` 列是一个字符串类型。
- `mapped_column` 函数用于创建 SQLAlchemy ORM 映射列。

### 一对一 ORM (One-to-One Relationship)

在 SQLAlchemy 中，一对一关系可以通过使用 `relationship()` 和 `ForeignKey()` 来实现。

#### 一对一示例：

```python
from sqlalchemy import ForeignKey
from sqlalchemy.orm import relationship

class User(Base):
    __tablename__ = 'users'
    
    id: Mapped[int] = mapped_column(Integer, primary_key=True)
    profile: Mapped["Profile"] = relationship("Profile", uselist=False, back_populates="user")

class Profile(Base):
    __tablename__ = 'profiles'
    
    id: Mapped[int] = mapped_column(Integer, primary_key=True)
    user_id: Mapped[int] = mapped_column(Integer, ForeignKey('users.id'))
    user: Mapped[User] = relationship("User", back_populates="profile")
```

在这个例子中：

- `User` 和 `Profile` 之间是一个一对一关系。
- `uselist=False` 表示在 `User` 到 `Profile` 的关系中，每个 `User` 对应一个唯一的 `Profile`。
- `ForeignKey` 确保 `Profile` 中的 `user_id` 列与 `User` 表的主键关联。

### 一对多 ORM (One-to-Many Relationship)

一对多关系是常见的关系类型，通常用来表示一个实体可以关联多个其他实体。

#### 一对多示例：

```python
class User(Base):
    __tablename__ = 'users'
    
    id: Mapped[int] = mapped_column(Integer, primary_key=True)
    posts: Mapped[list["Post"]] = relationship("Post", back_populates="user")

class Post(Base):
    __tablename__ = 'posts'
    
    id: Mapped[int] = mapped_column(Integer, primary_key=True)
    user_id: Mapped[int] = mapped_column(Integer, ForeignKey('users.id'))
    user: Mapped[User] = relationship("User", back_populates="posts")
```

在这个例子中：

- `User` 和 `Post` 之间是一对多关系，一个 `User` 可以有多个 `Post`。
- `back_populates` 在这两个模型中互相配对，表示相互间的关联。

### 多对多 ORM (Many-to-Many Relationship)

多对多关系表示一个实体可以关联多个其他实体，反之亦然。为了实现多对多关系，SQLAlchemy 使用了**联接表**，这个表没有映射到模型类上，它只用来存储两个实体之间的关系。

#### 多对多示例：
```python
from sqlalchemy import Table, ForeignKey

association_table = Table(
    'association', Base.metadata,
    mapped_column(Integer, ForeignKey('users.id'), primary_key=True),
    mapped_column(Integer, ForeignKey('groups.id'), primary_key=True)
)

class User(Base):
    __tablename__ = 'users'
    
    id: Mapped[int] = mapped_column(Integer, primary_key=True)
    groups: Mapped[list["Group"]] = relationship("Group", secondary=association_table, back_populates="users")

class Group(Base):
    __tablename__ = 'groups'
    
    id: Mapped[int] = mapped_column(Integer, primary_key=True)
    users: Mapped[list["User"]] = relationship("User", secondary=association_table, back_populates="groups")
```

在这个例子中：
- `User` 和 `Group` 之间是多对多关系。
- `association_table` 是一个联接表，它通过 `ForeignKey` 引用 `users` 和 `groups` 表的主键。
- `secondary` 参数告知 SQLAlchemy 使用 `association_table` 来进行关系的映射。

### 允许列名与数据库中列名不一致吗？
是的，SQLAlchemy 允许列名和数据库中的列名不一致。你可以通过 `column` 参数在 `mapped_column` 中指定数据库列名，而不必遵循 Python 类中的命名规则。例如：

```python
class User(Base):
    __tablename__ = 'users'
    
    id: Mapped[int] = mapped_column(Integer, primary_key=True)
    first_name: Mapped[str] = mapped_column("first_name_column", String)  # 映射到数据库中列名为 'first_name_column'
    last_name: Mapped[str] = mapped_column("last_name_column", String)  # 映射到数据库中列名为 'last_name_column'
```

在这个例子中，`first_name` 和 `last_name` 字段映射到数据库中实际的列名 `first_name_column` 和 `last_name_column`。

通过这种方式，你可以在 ORM 类中使用 Python 风格的命名，而在数据库中使用不同的列名。