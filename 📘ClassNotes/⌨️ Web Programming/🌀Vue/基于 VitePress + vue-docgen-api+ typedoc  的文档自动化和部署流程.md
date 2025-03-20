
> [!warning]  TypeDoc 对 options  API 支持
>  TypeDoc 对 Options API 支持不佳， 难以推断 router, emit等等关键字， 因此对于 vue 项目，我们采用 vue-docgen-api 生成组件的文档， 而通过 typedoc + glob 库 生成 api 和 

可以使用 VitePress 和一些插件实现从 Vue 项目代码到文档生成的全自动化流程。这在一定程度上可以减少手动编写文档的工作，尽量实现从代码注释到文档的自动化。以下是实现这一目标的步骤及建议：
#### 优点：
- 基于 Vite，构建速度快。
- 支持 Vue 3 和 Options API。
- 可自定义文档主题和布局。 
#### 缺点：
- 需要手动配置 `vue-docgen-api` 解析组件。
- 对 TypeScript 的支持需要额外配置。  

### 步骤概述

1. **设置 VitePress**:
   - 首先，安装并初始化 VitePress。你可以通过以下命令安装 VitePress：

     ```bash
     npm install -D vitepress
     ```

采用如下方法: 
```sh
npx vitepress init
```
然后将 Where to initialize 放到 ./docs 下即可

1. **使用 JSDoc 或其他注释解析工具**:
   - 使用 JSDoc 注释来描述你的 Vue 组件和 API。这样可以通过 JSDoc 将注释提取到 JSON 格式的文档中。
```sh
npm install jsdoc --save-dev
npm install jsdoc-vuejs --save-dev
sudo npm install jsdoc-api -D  
```

然后根据如下设定根目录的 `jsdoc.json` 文件以配置 jsdoc  
```json
{
  "source": {
    "include": ["src"],
    "exclude": ["node_modules"]
  },
  "plugins": ["jsdoc-vuejs"],
  "opts": {
    "destination": "docs/api",
    "recurse": true,
    "template": "templates/default"
	}
} 
```

   - 可以使用插件自动将 JSDoc 生成的 JSON 文档转化为 Markdown 格式，进而集成到 VitePress 中。例如，可以使用 [`jsdoc-to-markdown`](https://github.com/jsdoc2md/jsdoc-to-markdown) 来从 JSDoc JSON 文件生成 Markdown。

```bash
npm install -D jsdoc-to-markdown
```

   - 然后编写一个脚本，自动运行 JSDoc，并生成文档。你可以在 `package.json` 中添加一个脚本：

```json
"scripts": {
	"docs:generate": "jsdoc2md src/**/*.vue src/**/*.ts > docs/api/README.md",
}
```

4. **VitePress 配置**:
   - 在你的 VitePress 项目中，你可以在 `/docs/.vitepress/config.js` 中配置文档路由和其他设置。确保你的生成文档包含在 VitePress 的视图中。

5. **自动化构建**:
   - 将上述步骤结合在一起，可以通过 CI/CD 管道（如 GitHub Actions）实现完全的自动化。当代码更新时，自动运行文档生成脚本并构建 VitePress 文档。

是的，您当前的配置代码可以通过一些调整来支持 TypeScript 的文档注释生成。`vue-docgen-api` 支持解析 TypeScript 文件中的文档注释，这样您就可以提取组件的属性和事件文档。

要使 `vue-docgen-api` 更好地支持 TypeScript 解析，您可能需要安装一些额外的依赖，并确保您的 TypeScript 配置是正确的。以下是您可以采取的几个步骤：

1. **确保依赖完整**:
   确保安装了必要的依赖。除了 `vue-docgen-api`，您可能还需要安装 `typescript` 和 `vue-docgen-api` 所需的类型定义：
   ```bash
   npm install --save-dev typescript vue-docgen-api @types/node
   ```

2. **更新 TypeScript 配置**:
   确保 `tsconfig.json` 中的配置可以支持 Vue 文件。至少需要设置如下内容：
   ```json
   {
     "compilerOptions": {
       "target": "ESNext",
       "module": "ESNext",
       "moduleResolution": "node",
       "jsx": "preserve",
       "strict": true,
       "esModuleInterop": true,
       "skipLibCheck": true,
       "compilerOptions": {
         "types": ["vite/client"]
       },
       "paths": {
         "@/*": ["src/*"]
       }
     },
     "include": ["src/**/*.ts", "src/**/*.d.ts", "src/**/*.tsx", "src/**/*.vue"]
   }
   ```

3. **解析 Vue 组件**:
   使用 `parseComponent` 解析 Vue 组件时，确保您传递的路径是正确的。使用 TypeScript 时，您的 `.vue` 文件中的 `<script lang="ts">` 标签能够被正确解析。

4. **生成文档**:
   您的文档生成代码大致可以保持不变，确保组件中的文档注释（如 JSDoc）提供足够信息以供提取。您可以考虑提升生成的 Markdown 文档的格式，使其更易于阅读。

5. **运行文档生成**:
   使用 Node.js 运行您的文档生成代码，以自动生成 Markdown 文档。

6. **VitePress 集成**:
   将生成的 Markdown 文档放置在适当的目录下，并配置 VitePress 以使其能够正确识别并展示这些文档。 
### 示例更新后的代码
这里有个更新后的示例代码，以确保可以用于带有 TypeScript 的 Vue 组件：
```javascript
import { parseComponent } from "vue-docgen-api";
import fs from "fs";

const componentPath = "./src/components/MyComponent.vue";
const componentInfo = parseComponent(componentPath);

fs.writeFileSync(
    "./docs/components/MyComponent.md",
    `
# ${componentInfo.displayName}

## Props
${componentInfo.props.map((prop) => `- **${prop.name}**: ${prop.description || '无描述'}`).join("\n")}

## Events
${componentInfo.events.map((event) => `- **${event.name}**: ${event.description || '无描述'}`).join("\n")}
`,
);
```
通过这些调整，您应当能够成功生成包含 TypeScript 文档注释的 Vue 组件文档。需要特别注意确保您的组件代码遵循标准的文档注释格式，以便 `vue-docgen-api` 能够准确解析信息。

## 基于 JsDoc 和 jsdoc-plugin-typescript 的 api 文档生成
对于我们希望生成 api, utils 文档，同时保持采用 jsdoc 

```sh
sudo npm install jsdoc -D 
sudo npm install --save-dev jsdoc-plugin-typescript
```
