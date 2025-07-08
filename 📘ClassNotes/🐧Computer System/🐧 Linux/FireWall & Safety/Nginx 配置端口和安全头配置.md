根据你提供的安全报告截图，主要问题是检测到 **Open Ports in Nginx**（开放了不必要的端口），同时你需要一个完整的 **Nginx HTTP 安全头配置** 示例。以下是具体解决方案：

---

### **一、解决 `Open Ports Found in Nginx` 问题**
#### **原因分析**
- 安全工具检测到你的 Nginx 服务器开放了不必要的端口（如 22/SSH、21/FTP 等），可能被误认为潜在攻击面。
- 常见误判场景：
  - 服务器开放了默认的 `80（HTTP）` 和 `443（HTTPS）` 之外的端口。
  - 未正确配置防火墙（如 UFW、iptables），导致扫描工具误报。

#### **解决方案**
1. **检查当前开放的端口**：
   ```bash
   sudo netstat -tulnp | grep nginx
   sudo ss -tulnp | grep nginx
   ```
   - 正常情况下应仅显示 `80` 和 `443`。

2. **关闭不必要的端口**：
   - **方法1：通过防火墙限制**（推荐）：
     ```bash
     # 如果使用 UFW（Ubuntu）
     sudo ufw allow 80/tcp
     sudo ufw allow 443/tcp
     sudo ufw enable
     sudo ufw status  # 确认仅允许 80/443
     ```
   - **方法2：Nginx 配置中禁用非必要监听**：
     检查 `/etc/nginx/sites-enabled/` 下的配置文件，确保没有类似以下冗余监听：
     ```nginx
     server {
         listen 8080;  # 关闭非标准端口
         ...
     }
     ```

2. **验证端口是否关闭**：
   ```bash
   sudo nmap -sT -p- your-server-ip
   ```
   - 确保输出中只有 `80/http` 和 `443/https` 是 `open` 状态。

### **二、Nginx HTTP 安全头完整配置示例**
将以下配置添加到 `/etc/nginx/nginx.conf` 或站点配置文件（如 `/etc/nginx/sites-available/your-site`）的 `server` 块中：

```nginx
server {
    listen 443 ssl http2;
    listen [::]:443 ssl http2;
    server_name your-domain.com;

    # SSL 配置（根据实际路径修改）
    ssl_certificate /path/to/cert.pem;
    ssl_certificate_key /path/to/privkey.pem;
    ssl_protocols TLSv1.2 TLSv1.3;
    ssl_prefer_server_ciphers on;

    # 强制 HTTPS（如果同时监听 80 端口）
    if ($scheme = http) {
        return 301 https://$host$request_uri;
    }

    # 安全头配置
    add_header X-Frame-Options "DENY" always;
    add_header X-Content-Type-Options "nosniff" always;
    add_header X-XSS-Protection "1; mode=block" always;
    add_header Referrer-Policy "strict-origin-when-cross-origin" always;
    add_header Content-Security-Policy "default-src 'self'; script-src 'self' 'unsafe-inline' https://trusted.cdn.com; style-src 'self' 'unsafe-inline'; img-src 'self' data: https:; connect-src 'self'; frame-ancestors 'none';" always;

    # 其他安全优化
    server_tokens off;  # 隐藏 Nginx 版本号
    client_max_body_size 10M;  # 限制上传文件大小

    # 静态文件缓存和 MIME 类型安全
    location ~*\.(js|css|png|jpg|jpeg|gif|ico)$ {
        expires 1y;
        add_header Cache-Control "public, no-transform";
    }

    # 禁止访问敏感路径
    location ~ /\.(ht|git|env) {
        deny all;
        return 404;
    }

    # 主站点配置
    root /var/www/html;
    index index.html;
}
```

---

### **三、关键安全头说明**
| 安全头                  | 作用                                                                 |
|-------------------------|----------------------------------------------------------------------|
| `X-Frame-Options`       | 禁止页面被嵌入到 `<iframe>` 中，防止点击劫持。                       |
| `X-Content-Type-Options`| 禁止浏览器嗅探 MIME 类型，强制遵守服务器声明的类型。                 |
| `X-XSS-Protection`      | 启用浏览器内置的 XSS 过滤器。                                        |
| `Referrer-Policy`       | 控制 Referer 头的信息泄露。                                          |
| `Content-Security-Policy`| 限制资源加载来源，防止 XSS/数据注入（需根据实际需求调整 `script-src`）。 |

---

### **四、验证配置**
1. **测试 Nginx 配置语法**：
   ```bash
   sudo nginx -t
   ```
2. **重载 Nginx**：
   ```bash
   sudo systemctl reload nginx
   ```
3. **在线检查安全头**：
   - 使用 [SecurityHeaders.com](https://securityheaders.com/) 扫描你的网站，确认所有安全头生效。

---

### **五、额外建议**
- **定期扫描漏洞**：
  ```bash
  sudo apt install lynis  # 安全审计工具
  sudo lynis audit system
  ```
- **监控日志**：
  ```bash
  tail -f /var/log/nginx/access.log | grep -E '404|403|500'
  ```

完成以上步骤后，Guardio 等工具应不再标记你的网站为恶意。如果问题仍存在，可能是缓存或历史记录导致，建议通过 Guardio 官方渠道申诉并提供修复证据。



以下是每个 HTTP 安全头的详细解释及其作用，按你的配置逐条分析： 

### **1. `X-Frame-Options "DENY"`**
- **作用**：防止网页被嵌入到 `<iframe>` 中，避免点击劫持（Clickjacking）攻击。
- **参数**：
  - `DENY`：完全禁止任何页面嵌入 iframe。
  - `SAMEORIGIN`：仅允许同域名下的页面嵌入（更宽松的选项）。
- **示例场景**：阻止恶意网站将你的登录页面嵌入到他们的 iframe 中窃取用户操作。

### **2. `X-Content-Type-Options "nosniff"`**
- **作用**：强制浏览器遵守服务器声明的 `Content-Type`，禁止 MIME 类型嗅探。
- **参数**：
  - `nosniff`：对脚本（JS/CSS）和图片等资源，严格按声明类型解析。
- **示例场景**：防止上传的 `.jpg` 文件被篡改为 `.js` 并执行。

---

### **3. `X-XSS-Protection "1; mode=block"`**
- **作用**：启用浏览器内置的 XSS 过滤器（旧版浏览器支持，现代浏览器已逐步废弃，由 CSP 替代）。
- **参数**：
  - `1`：启用 XSS 过滤。
  - `mode=block`：检测到 XSS 攻击时直接阻止页面加载（而非仅过滤）。
- **注意**：新版浏览器推荐用 CSP 的 `script-src` 替代此功能。

---


下面两个网站不添加
---

### **4. `Referrer-Policy "strict-origin-when-cross-origin"`**
- **作用**：控制浏览器在导航时发送 `Referer` 头的策略，平衡隐私与功能。
- **参数**：
  - `strict-origin-when-cross-origin`：
    - 同源请求：发送完整 URL（如 `https://example.com/page`）。
    - 跨源请求：仅发送域名（如 `https://example.com`）。
- **示例场景**：用户从你的网站跳转到外部链接时，避免泄露路径参数。

---

### **5. `Content-Security-Policy` (CSP)**
- **作用**：定义资源加载的白名单，是防御 XSS/数据注入的核心策略。
- **你的配置解析**：
  ```nginx
  default-src 'self';                  # 默认所有资源只能从当前域名加载
  script-src 'self' 'unsafe-inline' https://trusted.cdn.com;  # 脚本允许：当前域名、内联脚本、指定CDN
  style-src 'self' 'unsafe-inline';    # 样式允许：当前域名和内联样式
  img-src 'self' data: https:;         # 图片允许：当前域名、Data URL、所有HTTPS源
  connect-src 'self';                  # AJAX/WebSocket 仅允许当前域名
  frame-ancestors 'none';              # 等效于 X-Frame-Options: DENY
  ```
- **关键指令**：
  - `'self'`：当前域名（含协议和端口）。
  - `'unsafe-inline'`：允许内联脚本/样式（存在风险，建议逐步移除）。
  - `data:`：允许 Data URL（如 Base64 图片）。
  - `https:`：允许所有 HTTPS 域（需谨慎，建议明确域名）。

---

### **为什么用 `always`？**
- Nginx 的 `add_header` 默认只在 `2xx/3xx` 响应中添加头，`always` 确保在所有响应（如 `404`、`500`）中强制生效，避免漏网之鱼。

---

### **各头部协作关系**
| 安全头                  | 主要防御目标               | 补充说明                     |
|-------------------------|----------------------------|------------------------------|
| `X-Frame-Options`       | 点击劫持                   | 已被 CSP 的 `frame-ancestors` 替代（建议优先用 CSP）。 |
| `X-Content-Type-Options`| MIME 混淆攻击              | 不可或缺，无替代方案。       |
| `X-XSS-Protection`      | 反射型 XSS                 | 旧浏览器兼容，现代用 CSP。   |
| `Referrer-Policy`       | 隐私泄露                   | 独立功能，无替代。           |
| `Content-Security-Policy`| XSS/数据注入/资源劫持      | 最全面的防护核心。           |

---

### **优化建议**
1. **逐步移除 `unsafe-inline`**：
   - 将内联脚本（如 `<script>alert()</script>`）改为外部文件引用。
   - 使用 `nonce` 或 `hash` 替代（需配合 CSP 动态生成）。
2. **收紧 `img-src`**：
   - 将 `https:` 改为具体域名（如 `https://trusted.cdn.com`）。
3. **监控 CSP 违规**：
   - 在 `Content-Security-Policy` 后添加 `report-uri /csp-report-endpoint;`，收集违规日志。

---

通过以上配置，你的网站已具备基础安全防护能力。如需进一步强化，可参考 [Mozilla CSP 指南](https://developer.mozilla.org/en-US/docs/Web/HTTP/CSP)。