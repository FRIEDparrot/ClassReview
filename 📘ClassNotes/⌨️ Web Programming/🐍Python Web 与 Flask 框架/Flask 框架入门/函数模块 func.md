

```python 
from sqlalchemy import create_engine, func  
from sqlalchemy.orm import sessionmaker  
from your_model import Admin  # 确保正确导入你的模型  

# 创建数据库引擎  
engine = create_engine('your_database_url')  
Session = sessionmaker(bind=engine)  
session = Session()  

# 查询 Admin.id 的最大值  
max_id = session.query(func.max(Admin.id)).scalar() or 0  

print(max_id)
```