##  实际情况
Isso 仍然在 `/js/embed.min.js` 提供静态资源，说明 **Isso 并不会因为 `listen = http://127.0.0.1:9000/comments` 就把自身 API 和静态资源放到 `/comments/` 下**。Isso 的静态资源和 API 仍然是默认根路径 `/`，所以 `/comments/` 只是 URL 的逻辑前缀，而 Isso 依然按 `/` 解析路径。 

## **为什么 Isso 仍然在 `/js/embed.min.js` 提供资源？**

Isso **不支持** 直接在 `listen` 或 `public-endpoint` 里更改自己的静态资源和 API 路径，而是**默认在 `/` 目录下运行**。即使你配置 `base-url` 为 `/comments`，Isso 的 `embed.min.js` 仍然会加载 `/js/embed.min.js`，API 仍然会走 `/new` 而不是 `/comments/new`。

Isso **不会自己修改 URL 路径**，你需要**手动配置反向代理或 URL 重写**。

---

## **解决方案**

### **方法 1：让 Nginx 反向代理 Isso 并重写路径（推荐）**

如果你有 Nginx，可以使用**路径重写**让 Isso 运行在 `/comments`，而不会影响它的 API 和静态文件路径。

#### **Nginx 配置**

在你的 Nginx 配置文件中，增加：

```nginx
location /comments/ {
    proxy_pass http://127.0.0.1:9000/;
    proxy_set_header Host $host;
    proxy_set_header X-Real-IP $remote_addr;
    proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
    rewrite ^/comments(/.*)$ $1 break;
}
```

#### **如何生效**

- 这样 `http://yourdomain.com/comments/js/embed.min.js` **实际会转发到** `http://127.0.0.1:9000/js/embed.min.js`
- Isso 仍然以 **根路径** 处理所有请求，但外部 URL 会有 `/comments/` 作为前缀

✅ **最终效果**

- `http://yourdomain.com/comments/js/embed.min.js` **能正确加载**
- `http://yourdomain.com/comments/new` **Isso 能正确响应**
- Isso 自己的 API **不会因为路径问题报错**

---

### **方法 2：修改 Isso 代码（复杂，不推荐）**

Isso **内部没有提供参数** 让它修改 `/js/` 路径，而是**写死在代码里的**。如果你不想用 Nginx，你需要手动修改 Isso 源码：

1. **克隆 Isso 源码**
    
    ```bash
    git clone https://github.com/posativ/isso.git
    cd isso
    ```
    
2. **修改 `isso/views.py`**
    - 找到静态文件路径
    - 手动加上 `/comments` 作为前缀
3. **重新构建 Isso**
    
    ```bash
    python setup.py install
    ```
    

✅ **这个方法可以修改 Isso 的默认路径**，但 **不推荐**，因为每次 Isso 更新你都要手动改代码。

---

### **方法 3：前端 Vue 组件手动改路径**

如果你无法使用 Nginx，但 Isso 仍然跑在 `/`，你可以在 Vue 里手动指定 `embed.min.js` 的路径：

```ts
script.dataset.isso = 'https://hellofriedparrot.club/comments/';
script.src = 'https://hellofriedparrot.club/js/embed.min.js'; // 仍然加载根路径
```

✅ **这样 Isso 仍然在 `/` 运行，但前端访问时仍然能正确加载 `/comments/` 的 API**。

---

## **最终建议**

- **推荐方案**：用 **Nginx 反向代理**，让 Isso 运行在 `/`，但对外呈现 `/comments/`
- **临时解决方案**：前端手动改 `embed.min.js` 的 URL，但 Isso API 仍然会在 `/`
- **不建议**：改 Isso 源码，因为 Isso 设计上就是运行在 `/` 的，改源码容易导致后续升级麻烦

如果你没有 Nginx，考虑是否可以用 Flask 反向代理 Isso。🚀