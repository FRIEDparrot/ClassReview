如果需要实时通信或处理大量数据，可以考虑使用 WebSocket 或 Server-Sent Events (SSE)。
```ts
const deleteItems = (keys: string[]) => {
    const socket = new WebSocket('ws://yourserver.com/delete');

    socket.onopen = () => {
        socket.send(JSON.stringify({ keys })); // 发送键数组
    };

    socket.onmessage = (event) => {
        console.log('Delete result:', event.data);
    };

    socket.onerror = (error) => {
        console.error('WebSocket error:', error);
    };
};
```
#### 优点：

- 实时通信。
    
- 适合大量数据或长连接场景。