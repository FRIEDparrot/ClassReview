应用情形 :  针对  Markdown 代码在动态编辑的过程中， 我们需要动态给其中的折叠按钮绑定折叠事件。

MutationObserver 是一个用于监视 DOM 变化的 API，可以方便地监听子节点的添加、删除或更改。这对于动态渲染内容时非常有用，例如在博客中动态加载 Markdown 内容后，自动绑定事件。 

以下是一个 MutationObserver 的示例：

```typescript
const observer = new MutationObserver((mutations) => {
    mutations.forEach((mutation) => {
        if (mutation.type === 'childList') {
            // 当有新的子节点添加时
            console.log('子节点被添加或移除');
            // 这里可以调用其他方法，例如重新绑定事件
        }
    });
});

// 监听特定元素的变化
const targetNode = document.querySelector('.markdown-body');
if (targetNode) {
    observer.observe(targetNode, { childList: true, subtree: true });
}
```

在这个示例中，我们创建了一个 MutationObserver 实例，当 `.markdown-body` 元素的子节点发生变化时，会触发回调函数，允许开发者执行相应的逻辑，比如重新绑定折叠按钮的事件。这种方法有效应对动态内容的变化，。

Sources:
https://chat.deepseek.com/a/chat/s/31f25fae-1ee0-4628-942c-b302c3a75cd7

Powered by MaxAI