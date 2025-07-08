对于采用vue 和 vite 构建的前端网页, 需要 npx vite 才能运行, 直接打开 index 显然是没有 网页的, 那么采用 nginx 实际上是打开 html 显然会错误 ， 如何把 vite 项目部署到服务器上呢

如果直接通过浏览器打开 `file:///` 协议访问 `dist/index.html` 文件，但浏览器的 **CORS（跨域资源共享）** 限制阻止了加载本地资源。这是因为通过 `file:///` 协议加载文件时，浏览器认为不同的文件是跨域的，导致脚本和样式表无法加载。

> [!HINT] 通过  server 浏览器预览
> serve 可以直接 cd 进  dist  目录运行 serve 可以直接运行可执行 html 文件. 此时可以忽略电脑的禁止运行警告.

### **解决方法**

#### 1. **使用本地 HTTP 服务器**
浏览器要求 HTML 文件和其依赖的资源（如 JS 和 CSS 文件）必须通过 HTTP 或 HTTPS 协议访问，而不能直接通过 `file:///` 打开。

运行以下命令，启动一个本地 HTTP 服务器：
- **方法 1：使用 Vite 的 `preview` 命令**
  ```bash
  npx vite preview
  ```
  这会启动一个服务器，通常在 `http://localhost:4173`（端口可能不同）提供构建后的文件。
- **方法 2：使用通用 HTTP 服务器**
  你可以使用任何简易服务器来提供文件服务:
  - Node.js
```bash
npx serve dist
```
  - Python
    ```bash
    python -m http.server --directory dist 8080
    ```
    然后通过 `http://localhost:8080` 访问你的页面。

---

#### 2. **调整资源路径**
Vite 默认构建的文件路径是相对于 HTML 文件的相对路径。如果你计划将文件放在某个服务器的子路径中，需要调整 `vite.config.js` 中的 `base` 配置。例如：

```javascript
import { defineConfig } from 'vite';

export default defineConfig({
  base: './', // 设置为相对路径
});
```
然后重新运行：
```bash
npx vite build
```

---

#### 3. **解决 favicon 加载问题**
`parrot_web-DkGNz76-.ico` 文件加载失败，说明该文件可能不存在于 `dist/assets/` 中，或路径配置错误。

- 确认 `favicon.ico` 或 `.ico` 文件是否存在于源代码目录中，通常应放在 `public/` 文件夹中。
- 如果没有，则在项目根目录添加一个 `favicon.ico` 文件，重新构建。

---

#### 4. **使用浏览器禁用 CORS（开发环境专用）**
如果你仅仅是为了调试，而不想启动服务器，可以通过禁用浏览器的 CORS 来解决此问题（不推荐用于生产环境）。

- **谷歌 Chrome 禁用 CORS**  
  启动 Chrome 时添加命令行参数：
  ```bash
  chrome.exe --disable-web-security --user-data-dir="C:/ChromeDev"
  ```
  然后通过该实例访问 `index.html`。

### **推荐**
最好的方式是使用 HTTP 服务器（方法 1 或 2），避免手动修改浏览器设置，也确保环境接近生产环境。


最终:   采用  import 解决!!! 



注意打包得到的文件大小, 一般 > 200kb 就需要考虑服务器带宽和优化问题
