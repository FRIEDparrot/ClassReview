安装 :
```sh
npm install typedoc --save-dev
# 如果是 vue 项目， 则需要如下包: 
npm install  typedoc-plugin-vue --save-dev 
```

首先在 ts-config.json 中需要 skip lib check : 
```json
{
  "compilerOptions": {
	"types": ["vite/client", "vue"], // 需要正确声明类型
    "moduleResolution": "node",
    "esModuleInterop": true,
    "skipLibCheck": true
  }
}
```

然后采用
```sh
npm run type-check 
```

保证没有检查语法错误之后， 一般地 ，  我们使用其生成在项目的根目录 `/docs` 下的文档 

```python
npx typedoc --plugin typedoc-plugin-vue src/main.ts
```

```json
{
    "entryPoints": ["src/main.ts"],
    "out": "docs",
    "exclude": ["**/node_modules/**"],
    "includeVersion": true
}
```

```sh
npx typedoc --out ./docs ./src/main.ts  
```

需要使用 `typedoc` 支持的选项来生成文档。`typedoc` 的配置项通常和 `tsconfig.json` 不完全一样，因此请确保使用正确的字段。 

```json
{
  "out": "docs",                          // 输出文档的目录
  "entryPoints": ["src/main.ts"],  // 输入点部分
  "exclude": "**/node_modules/**",        // 排除 node_modules 目录
  "plugin": ["typedoc-plugin-vue"],       // 加载 vue 插件
  "theme": "default",                     // 使用默认的主题
  "readme": "none",                       // 不生成 README
  "name": "My Project",                   // 项目名称
  "version": true,                        // 输出版本信息
  "watch": true,                          // 启用监视模式
  "useTsLinkResolution": true,            // 启用 TypeScript 的链接解析
  "hideGenerator": true,                  // 隐藏自动生成的注释
  "disableSources": true                  // 禁用源代码的显示
}
```

- **`out`**: 指定生成的文档输出目录（例如 `docs`）。
- **`exclude`**: 排除指定的文件夹或文件（例如 `node_modules`）。
- **`plugin`**: 使用插件 `typedoc-plugin-vue`，使 `typedoc` 能识别 Vue 组件。
- **`theme`**: 使用 `typedoc` 默认的主题（你也可以使用其他主题）。
- **`readme`**: 指定是否生成 `README.md` 文件，`none` 表示不生成。
- **`name`**: 项目的名称，显示在文档顶部。
- **`version`**: 输出当前项目的版本号。
- **`watch`**: 启用文件监视模式，当文件更改时自动重新生成文档。
- **`useTsLinkResolution`**: 启用 TypeScript 的链接解析功能，适用于一些引用的类型或接口。
- **`hideGenerator`**: 隐藏文档底部的自动生成信息。
- **`disableSources`**: 禁用源代码的显示，只生成 API 文档。

然后，运行 `typedoc`: 
```bash
npx typedoc --options typedoc.json 
```
这时，`typedoc` 会使用 `typedoc.json` 配置文件中的设置来生成文档。
