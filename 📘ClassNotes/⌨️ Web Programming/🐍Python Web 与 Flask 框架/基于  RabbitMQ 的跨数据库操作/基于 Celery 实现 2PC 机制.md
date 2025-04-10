**Celery 已经完善了**：任务队列的管理、消费者的启动和停止、任务的分发和执行等底层操作,  同时支持 json 的数据解析和传入。是更加稳定的框架

对于虚拟环境下， 采用 : 
```python
.venv\Scripts\celery -A rabbitmq_service worker --loglevel=info
``` 

### **检查 Celery 的并发模式**
Celery 在 Windows 上默认使用 `prefork` 并发模式，这可能会导致权限问题。可以尝试将并发模式改为 `solo`，避免使用多进程。
```python
celery -A rabbitmq_service worker --loglevel=info -P solo

celery -A rabbitmq_service worker --loglevel=warning -P solo 
```



```sh
 ACCESS_REFUSED - Login was refused using authentication mechanism PLAIN. For details see the broker logfile
 # 登录错误， 需要对应用户名
```
