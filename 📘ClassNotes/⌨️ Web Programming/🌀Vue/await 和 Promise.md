当一个 `Promise` 被拒绝（`reject`），它并不会返回 `null`，而是进入 `Promise` 的拒绝状态，传递给 `reject` 的值会作为拒绝原因（rejection reason）。在代码中：

```typescript
const p = new Promise((resolve, reject) => {
    img.onload = () => {
        resolve(img);
    };
    img.onerror = (err) => {
        console.error(err);
        reject(err);
    };
});
return p;
```

如果 `img.onerror` 被触发，`reject(err)` 会将 `err` 作为拒绝原因传递给 `Promise` 链。为了处理这种情况，你可以通过 `.catch` 或 `try...catch` 来捕获它。

---

### 如何判断 `Promise` 是否被拒绝

1. **使用 `.catch`** 通过 `.catch` 方法捕获被拒绝时的值：
    
    ```typescript
    p.catch((err) => {
        console.log("Promise was rejected with reason:", err);
    });
    ```
    
2. **使用 `async/await` 和 `try...catch`** 使用 `await` 时捕获 `Promise` 的拒绝：
    
    ```typescript
    async function handlePromise() {
        try {
            const result = await p;
            console.log("Promise resolved with:", result);
        } catch (err) {
            console.log("Promise was rejected with reason:", err);
        }
    }
    handlePromise();
    ```
    
3. **通过拒绝值进行判断** 你可以传递任何值给 `reject`，如 `null`、`undefined`、一个错误对象，或自定义信息。在 `Promise` 被拒绝后，可以检查该值：
    
    ```typescript
    p.catch((reason) => {
        if (reason === null) {
            console.log("Rejected with null");
        } else if (reason instanceof Error) {
            console.error("Rejected with an error:", reason.message);
        } else {
            console.log("Rejected with:", reason);
        }
    });
    ```
    

---

### 被拒绝时返回的内容

- 默认情况下，`reject` 会将其传递的值作为拒绝的理由。
- 如果没有传递值，`Promise` 的拒绝值为 `undefined`。

### 示例

```typescript
const p1 = new Promise((_, reject) => reject(null));
const p2 = new Promise((_, reject) => reject(new Error("Something went wrong")));
const p3 = new Promise((_, reject) => reject());

p1.catch((reason) => console.log("p1 rejected with:", reason)); // null
p2.catch((reason) => console.log("p2 rejected with:", reason)); // Error: Something went wrong
p3.catch((reason) => console.log("p3 rejected with:", reason)); // undefined
```

---

### 总结

1. **返回值**：当 `Promise` 被拒绝时，返回的不是 `null`，而是 `reject` 的参数值。
2. **判断方法**：可以使用 `.catch` 或 `try...catch` 来捕获拒绝，并根据值类型判断原因。
3. **默认值**：如果 `reject` 未传值，则返回 `undefined`。

确保你总是通过 `.catch` 或 `try...catch` 处理可能的拒绝，以避免未捕获的拒绝导致应用错误。



`Promise` 本身是异步操作，但这并不意味着你不需要 `await`。`await` 和 `Promise` 是配合使用的，`await` 只是让你以同步的方式编写异步代码。让我们来详细解释一下。

### `Promise` 和 `await` 的关系

1. **`Promise` 本身的异步性质**： `Promise` 表示一个异步操作的结果，它会在未来的某个时间点返回结果（resolved）或者抛出错误（rejected）。你可以使用 `.then()` 和 `.catch()` 来处理它们，但这只是回调式的方式，代码逻辑有时会显得不太直观，尤其是在处理多个异步操作时。
    
    ```typescript
    const p = new Promise((resolve, reject) => {
        setTimeout(() => resolve("done"), 1000);
    });
    
    p.then(result => {
        console.log(result);  // 输出 "done" 
    });
    ```
    
    这段代码中的 `p` 是异步的，它会在 1 秒后打印 "done"。但你通常使用 `.then()` 来注册回调。
    
2. **`await` 的作用**： `await` 用来等待一个 `Promise` 被解析（resolved）或拒绝（rejected）。它会暂停当前异步函数的执行，直到 `Promise` 返回结果。使用 `await` 让你可以像写同步代码一样编写异步代码，避免回调地狱。
    
    ```typescript
    async function testAsync() {
        const p = new Promise((resolve, reject) => {
            setTimeout(() => resolve("done"), 1000);
        });
    
        const result = await p;  // 等待 Promise 完成
        console.log(result);  // 输出 "done"
    }
    
    testAsync();
    ```
    
    在这个例子中，`await p` 会让 `testAsync` 函数暂停，直到 `p` 完成，之后再继续执行，打印结果。这种方式的好处是代码的结构看起来更像同步代码，更易于理解和维护。
    

### 为什么 `await` 仍然有用

- **同步化异步代码**：尽管 `Promise` 本身是异步的，`await` 可以让你在 `async` 函数中等待 `Promise` 结果，像同步代码一样处理异步操作，从而简化代码结构。
- **避免回调地狱**：当你有多个依赖顺序执行的异步操作时，`await` 可以避免多层嵌套的 `.then()`，使代码更易读。

### 示例：多个异步操作

#### 没有 `await`（回调地狱）：

```typescript
const p1 = new Promise((resolve) => setTimeout(() => resolve('Result 1'), 1000));
const p2 = new Promise((resolve) => setTimeout(() => resolve('Result 2'), 1000));

p1.then(result1 => {
    console.log(result1);
    return p2;
}).then(result2 => {
    console.log(result2);
});
```

#### 使用 `await`：

```typescript
async function runAsync() {
    const result1 = await p1;  // 等待 p1 完成
    console.log(result1);

    const result2 = await p2;  // 等待 p2 完成
    console.log(result2);
}

runAsync();
```

使用 `await` 使得异步操作的顺序更加清晰，代码结构更接近同步代码。

### 总结

- **`Promise` 是异步的**，但它本身不会自动阻塞代码的执行。你仍然需要 `await` 来等待 `Promise` 的解析，特别是当你希望按顺序处理多个异步操作时。
- **`await` 是用来等待 `Promise` 的**，它让你写异步代码时，能够像同步代码那样逐行执行，从而使代码更简洁易读。
- `Promise` 和 `await` 可以结合使用，但它们并不是互相替代的。`await` 让你能够以同步的方式编写异步代码。




`.then()` 不返回 `true` 的原因和 JavaScript 中的 Promise 行为相关。具体来说，在您的原始代码中，以下几点可能导致混淆：

1. **Promise 内的返回值**：在 `.then()` 中返回的值不会影响外部函数的返回值。即使在 `.then()` 中返回 `true`，这个值也不会被传递给 `logoutUser` 函数的调用者。

2. **异步处理**：`DeleteUserToken()` 是一个异步操作。当您调用 `DeleteUserToken().then(...)` 时，`logoutUser` 函数已经在 `return false;` 行结束了。所以，不论在 `.then()` 中发生什么，`logoutUser` 函数的返回值仍然是 `false`。

### 示例分析

对于以下代码：

```javascript
export function logoutUser(): boolean {
    DeleteUserToken()
        .then((response) => {
            return true; // 这个值不会影响 logoutUser 的返回值
        })
        .catch((error) => {
            return false; // 这个值也不会影响 logoutUser 的返回值
        });
    return false; // 这里始终返回 false
}
```

### 如何获取正确的返回值

如果您希望根据 `DeleteUserToken()` 的调用结果返回 `true` 或 `false`，使用 `async/await` 是一种更好的方法，如下所示：

```javascript
export async function logoutUser(): Promise<boolean> {
    try {
        await DeleteUserToken(); // 等待删除用户令牌的结果
        return true; // 如果成功，则返回 true
    } catch (error) {
        return false; // 如果失败，则返回 false
    }
}
```

### 总结

在原始代码中，`.then()` 和 `.catch()` 返回的值只对它们的上下文有效，而不会改变 `logoutUser` 函数的返回值。通过使用 async/await，可以明确函数的期望返回值。
