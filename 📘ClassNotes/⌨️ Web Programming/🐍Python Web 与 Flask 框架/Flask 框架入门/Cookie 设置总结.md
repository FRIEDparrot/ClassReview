在设置 Cookie 时，后端响应头的 `Set-Cookie` 字段控制了 Cookie 的行为。可以通过以下几个属性来设置不同的 Cookie 策略：

1. **Secure**：
    - 如果设置了 `Secure`，那么 Cookie 只有在 HTTPS 连接中才能被发送。这意味着如果使用的是 HTTP，而不是 HTTPS，浏览器将不会发送该 Cookie。
2. **Domain**：
- `Domain` 属性用于指定哪些域名可以访问该 Cookie。如果不设置，默认为当前设置 Cookie 的域。通过设置 `Domain`，你可以允许某个域名及其子域访问这个 Cookie。例如，如果你在 `example.com` 下设置了 Cookie，可以通过 `Domain=.example.com` 让 `sub.example.com` 也能访问到这个 Cookie。
3. **HttpOnly**：
- 如果设置了 `HttpOnly`，该 Cookie 将不能通过 JavaScript（`document.cookie`）访问。它只能通过 HTTP 请求发送到服务器。这样可以防止一些跨站脚本（XSS）攻击。

### 有效的组合
#### 1. **Secure + HttpOnly**:
- 这种组合意味着 Cookie 只能在 HTTPS 协议下发送，并且客户端的 JavaScript 无法访问它。该 Cookie 主要用于存储重要的身份验证信息，如 session token。这是安全的设置。
#### 2. **Secure + Domain** : 
- 这种组合允许跨子域传递 Cookie，但是只有在 HTTPS 连接时才会发送。这也适用于多子域共享 Cookie 的情况（例如，`sub.example.com` 和 `www.example.com`）。

#### 3. **HttpOnly + Domain**：

- 这种组合允许跨子域共享 Cookie，但客户端 JavaScript 无法访问。虽然它可以在 HTTP 或 HTTPS 协议下工作，但其作用主要是限制通过 JavaScript 的访问。

#### 4. **Secure + HttpOnly + Domain**：

- 这种组合是最安全的，要求使用 HTTPS，限制 JavaScript 访问 Cookie，同时允许跨子域传递 Cookie。这对于身份验证或敏感数据存储是非常适合的。

### 不成功的组合
1. **Secure + HttpOnly + Domain (但不使用 HTTPS)**：
    - 如果没有使用 HTTPS，但仍然设置了 `Secure` 属性，浏览器将不会发送 Cookie。`Secure` 属性要求只在 HTTPS 下工作，因此，如果你在 HTTP 下请求 Cookie，浏览器会忽略该 Cookie。
2. **HttpOnly 和 Domain 冲突**: 
    - 如果设置了 `Domain` 属性并且它不匹配当前域，或者该 `Domain` 属性指定了无法访问的父域或子域，浏览器也不会接受或发送该 Cookie。
3. **HttpOnly = False 和 Secure 结合不当**: 
    - 如果不设置 `Secure` 或 `Domain` 的话，`HttpOnly` 仍然会生效，但如果你期望通过 JavaScript 访问 Cookie，那么没有设置 `Secure` 和 `Domain` 可能会影响 Cookie 的发送行为，特别是在跨域请求时。

### 关于前端识别 `HttpOnly = False` 的 use_cookies 响应

`HttpOnly` 属性一旦被设置为 `True`，就会阻止 JavaScript 访问该 Cookie。由于 `HttpOnly` 是一个浏览器的安全机制，前端 JavaScript 是无法通过脚本读取该 Cookie 的。

如果你希望前端可以访问 Cookie，那么 `HttpOnly` 必须设置为 `False`。不过，设置为 `False` 时，你就失去了使用 `HttpOnly` 的安全保护（例如防止 XSS 攻击），因此这通常是在不处理敏感信息时才使用的。

如果你在 `use_cookies` 响应中想要让前端识别并使用 Cookie，必须将其配置为 `HttpOnly = False`，但这样做会导致该 Cookie 容易受到攻击。如果是用于存储会话信息、身份验证信息等，强烈建议不要设置 `HttpOnly = False`，而是通过服务器端逻辑安全地处理。

### 总结

4. **最安全组合**：`Secure + HttpOnly + Domain`，这能确保 Cookie 在 HTTPS 下传输，并且不能通过 JavaScript 访问，同时允许跨子域使用。
5. **前端传输的组合**：需要设置 `HttpOnly = False`，这样 JavaScript 才能访问 Cookie，但会暴露于 XSS 攻击等风险。
6. **避免设置冲突**：`Secure` 和 `HttpOnly` 的组合要求必须在 HTTPS 上工作，且无法通过 HTTP 使用。


**`SameSite` 属性**：`SameSite` 属性控制 Cookie 在跨站请求时的发送行为。`SameSite` 有三个设置： 
- **`Strict`**：只有当请求和 Cookie 所在的域名完全相同时，才会发送 Cookie。跨域请求将不会携带 Cookie。
- **`Lax`**：对于部分类型的跨站请求（如 GET 请求），浏览器会发送 Cookie，但其他类型的请求（如 POST）不会发送。
- **`None`**：允许跨域请求时发送 Cookie，但要求 `Secure` 属性被设置（即请求必须通过 HTTPS）。


![[attachments/Pasted image 20250214142102.png]]
