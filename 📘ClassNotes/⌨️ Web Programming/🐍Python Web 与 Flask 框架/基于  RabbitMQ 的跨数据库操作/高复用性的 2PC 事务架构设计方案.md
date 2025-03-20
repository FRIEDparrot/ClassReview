针对需要快速构建多个 2PC 服务的场景，我们可以设计一个更模块化、可复用的架构。以下是改进方案 : 

#### 架构设计思路
1. **分层解耦**
```python
[业务服务] -> [2PC 协调层] -> [消息中间件] <- [2PC 参与者集群]
```

2. **核心组件**
```python
class TransactionPhase(Enum):
    PREPARE = "prepare"
    COMMIT = "commit"
    ROLLBACK = "rollback"

class TwoPCProtocol:
    TX_ID_HEADER = "x-transaction-id"
    SERVICE_HEADER = "x-service-name"
    PHASE_HEADER = "x-transaction-phase"
```

#### 改进后的实现方案
```python
from dataclasses import dataclass
from typing import Callable, Dict

@dataclass
class TransactionParticipant:
    prepare_handler: Callable[[str, dict], bool]
    commit_handler: Callable[[str, dict], None]
    rollback_handler: Callable[[str, dict], None]

class TwoPCCoordinator:
    def __init__(self, rabbit_manager):
        self.rmq = rabbit_manager
        self.pending_transactions = {}  # 持久化存储建议改用数据库
        self.participants: Dict[str, TransactionParticipant] = {}
        
    def register_service(self, service_name: str, participant: TransactionParticipant):
        self.participants[service_name] = participant

    def start_transaction(self, services: list, payload: dict) -> str:
        tx_id = f"tx_{uuid.uuid4()}"
        self.pending_transactions[tx_id] = {
            'services': services.copy(),
            'state': 'initialized',
            'payload': payload
        }
        self._send_prepare_requests(tx_id)
        return tx_id

    def _send_prepare_requests(self, tx_id: str):
        tx = self.pending_transactions[tx_id]
        for service in tx['services']:
            self.rmq.publish(
                exchange='2pc',
                routing_key=f"{service}.prepare",
                body=json.dumps(tx['payload']),
                headers={
                    TwoPCProtocol.TX_ID_HEADER: tx_id,
                    TwoPCProtocol.PHASE_HEADER: TransactionPhase.PREPARE.value
                }
            )
        tx['state'] = 'preparing'

class TwoPCConsumer:
    def __init__(self, rabbit_manager, coordinator: TwoPCCoordinator):
        self.rmq = rabbit_manager
        self.coordinator = coordinator
        
        # 自动声明动态队列
        self.rmq.channel.exchange_declare(exchange='2pc', exchange_type='topic')
        result = self.rmq.channel.queue_declare(queue='', exclusive=True)
        self.queue_name = result.method.queue
        self.rmq.channel.queue_bind(exchange='2pc', queue=self.queue_name, routing_key="#.prepare_response")

    def start_consuming(self):
        def callback(ch, method, properties, body):
            headers = properties.headers
            tx_id = headers[TwoPCProtocol.TX_ID_HEADER]
            service_name = headers[TwoPCProtocol.SERVICE_HEADER]
            phase = headers[TwoPCProtocol.PHASE_HEADER]

            if phase == TransactionPhase.PREPARE.value:
                success = self._handle_prepare_response(tx_id, service_name, body)
                if success:
                    self.coordinator._handle_prepare_complete(tx_id)

        self.rmq.channel.basic_consume(
            queue=self.queue_name,
            on_message_callback=callback,
            auto_ack=True
        )

class RabbitMQManager:
    def __init__(self):
        # ...保持原有连接逻辑...
        self.coordinator = TwoPCCoordinator(self)
        self.consumer = TwoPCConsumer(self, self.coordinator)

# 业务服务使用示例
class OrderService:
    def prepare_handler(self, tx_id, payload):
        # 尝试预创建订单
        return random.random() > 0.5  # 模拟50%成功率

    def commit_handler(self, tx_id, payload):
        # 正式创建订单
        pass

    def rollback_handler(self, tx_id, payload):
        # 清理预创建数据
        pass

# 初始化
rabbit_mgr = RabbitMQManager()
order_participant = TransactionParticipant(
    prepare_handler=OrderService().prepare_handler,
    commit_handler=OrderService().commit_handler,
    rollback_handler=OrderService().rollback_handler
)
rabbit_mgr.coordinator.register_service("order_service", order_participant)
```


#### 关键改进点
1. **动态路由机制**
- 使用 Topic Exchange 实现消息路由
- 服务标识格式：`{service_name}.{action}`
- 示例路由键：
  - `order_service.prepare`
  - `payment_service.commit`

2. **事务生命周期管理**
```python
state_machine = {
    'initialized': ['preparing'],
    'preparing': ['committing', 'rolling_back'],
    'committing': ['completed'],
    'rolling_back': ['failed']
}
```

3. **参与者注册机制**
- 支持动态注册业务服务
- 统一处理入口与个性化处理逻辑分离

4. **消息协议标准化**
```python
{
  "headers": {
    "x-transaction-id": "tx_123",
    "x-service-name": "order_service",
    "x-transaction-phase": "prepare"
  },
  "body": {...}
}
```

#### 部署架构建议

```
[Client] 
  ↓ HTTP
[API Gateway] → 启动事务 → [2PC Coordinator]
                          ↓ 广播Prepare
                          → [Service A] (参与者)
                          → [Service B] (参与者)
                          ↑ 收集响应
                    成功 → 广播Commit
                    失败 → 广播Rollback
```

#### 扩展性优化
1. **持久化层插件**
```python
class TransactionStorage:
    def save_transaction(self, tx_id, state): ...
    def get_transaction(self, tx_id): ...

class MySQLStorage(TransactionStorage): ...
class RedisStorage(TransactionStorage): ...
```

2. **监控与补偿机制**
```python
class TransactionMonitor:
    def __init__(self):
        self.retry_policy = {
            'max_retries': 3,
            'backoff_factor': 2
        }

    def check_timeout_transactions(self):
        # 定期扫描超时事务
        # 触发自动补偿
```

3. **Saga模式支持**
```python
class SagaCoordinator(TwoPCCoordinator):
    def compensate_transaction(self, tx_id):
        # 逆向执行已完成的步骤
```

该方案通过标准化的消息协议、动态服务注册机制和灵活的路由策略，能够快速接入新的分布式事务需求。每个业务服务只需实现对应的 prepare/commit/rollback 处理逻辑，即可自动接入 2PC 流程，实现关注点分离和代码复用。