在你的 `vite.config.js` 中添加 `server` 配置项，强制指定 `host` 为 `127.0.0.1`。修改后的配置如下：

```javascript
import { defineConfig, loadEnv } from "vite";
import vue from "@vitejs/plugin-vue";
import path from "path";

const env = loadEnv(process.cwd(), process.env.NODE_ENV);
export default defineConfig(({ mode }) => {
    const env = loadEnv(mode, process.cwd());
    const vite_port = `${env.VITE_PORT ?? 5173}`; // 确保端口是 5173
    const vite_host = `${env.VITE_HOST ?? "127.0.0.1"}"; // 默认改为 127.0.0.1

    const resolvePath = (dir) => path.resolve(__dirname, dir);
    
    return {
        // 新增 server 配置
        server: {
            host: vite_host, // 使用环境变量或默认 127.0.0.1
            port: parseInt(vite_port, 10), // 确保端口是数字类型
            strictPort: true // 如果端口被占用直接报错，而不是自动更换
        },
        resolve: {
            alias: {
                /* 你原有的 alias 配置保持不变 */
            },
        },
        plugins: [vue()],
    };
});
```

### 配套操作：
1. **环境变量配置（可选）**  
   在项目根目录的 `.env` 文件中添加以下内容（若需覆盖默认值）：
   ```env
   VITE_HOST=127.0.0.1
   VITE_PORT=5173
   ```

2. **验证结果**  
   运行 `npm run dev`，控制台应输出：
   ```
   VITE v4.x.x ready in X ms
   ➜  Local:   http://127.0.0.1:5173/
   ```

### 注意事项：
- 如果仍出现 CORS 问题，可能是后端服务未配置允许 `127.0.0.1:5173` 的请求，需在后端设置 CORS 白名单（如 Access-Control-Allow-Origin）。
- `strictPort: true` 可防止端口被占用时 Vite 自动切换端口。
