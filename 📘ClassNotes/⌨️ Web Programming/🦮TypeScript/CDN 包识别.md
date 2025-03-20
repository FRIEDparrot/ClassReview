在 TypeScript 项目中，通过 CDN 引入的库（如 GSAP）默认不会被 TypeScript 识别，因为没有安装相应的类型声明包。以下是配置方法，使 TypeScript 能正确识别 GSAP：

### 解决方法 1: 安装类型声明包

GSAP 提供了官方的类型声明包，您可以通过 npm 或 yarn 安装：

```bash
npm install gsap @types/gsap --save-dev
```

安装后，TypeScript 会自动识别 `gsap`。

### 解决方法 2: 创建全局类型声明（适用于 CDN 引入）

如果您使用 CDN 引入 GSAP，而不通过 npm 安装，可以手动创建一个全局的类型声明文件。

1. **创建类型声明文件** 在项目根目录下创建一个 `gsap.d.ts` 文件（或者放在 `src/types/gsap.d.ts` 中，确保路径被 TypeScript 包含）。
2. **添加以下内容**
    
    ```typescript
    // src/types/gsap.d.ts
    declare module "gsap" {
        export * from "gsap/all";
    }
    
    declare module "gsap/all" {
        export const gsap: typeof import("gsap");
        export const TweenLite: typeof import("gsap").TweenLite;
        export const TweenMax: typeof import("gsap").TweenMax;
        export const TimelineLite: typeof import("gsap").TimelineLite;
        export const TimelineMax: typeof import("gsap").TimelineMax;
    }
    
    declare const gsap: typeof import("gsap").gsap;
    ```
3. **引入 CDN 的 GSAP** 在 HTML 文件中引入 GSAP：
```html
<script src="https://cdnjs.cloudflare.com/ajax/libs/gsap/3.12.2/gsap.min.js"></script>
```
4. **在 TypeScript 文件中使用 GSAP**
```typescript
gsap.to(".box", { duration: 1, x: 100, rotation: 360 });
```

### 解决方法 3: 配置 TypeScript 全局变量
直接声明全局变量以避免 TypeScript 报错（适合简单项目）。 
1. 在 `src/types/gsap.d.ts` 中添加：
    
    ```typescript
    declare const gsap: {
        to: (target: any, vars: Record<string, any>) => void;
        from: (target: any, vars: Record<string, any>) => void;
        fromTo: (target: any, fromVars: Record<string, any>, toVars: Record<string, any>) => void;
    };
    ```
    
2. 使用时无需额外配置，直接调用 `gsap`：
    
    ```typescript
    gsap.to(".box", { x: 100, duration: 1 });
    ```

### 注意事项

- **优先选择安装类型声明包**：这是推荐的做法，特别是在复杂项目中。
- **确保 CDN 版本匹配**：手动声明类型时，需参考 GSAP 的文档或类型定义，以保持一致性。
- **配置 `tsconfig.json`**：确保 `types` 包含自定义类型声明文件的路径：
    
    ```json
    {
        "compilerOptions": {
            "typeRoots": ["./node_modules/@types", "./src/types"]
        }
    }
    ```
    

这样配置后，TypeScript 应该能够正确识别通过 CDN 引入的 GSAP。

```json
{
  "compilerOptions": {
    "target": "esnext",
    "module": "esnext",
    "moduleResolution": "node",
    "strict": true,
    "esModuleInterop": true,
    "skipLibCheck": true,
    "forceConsistentCasingInFileNames": true,
    "jsx": "preserve",
    "outDir": "./dist",
    "baseUrl": "./src",
    "paths": {
      "*": ["node_modules/*", "src/types/*"]
    },
    "typeRoots": [
      "./node_modules/@types",
      "./src/types"  // 确保包含 src/types 路径
    ]
  },
  "include": [
    "src/**/*.ts",
    "src/**/*.tsx",
    "src/types/**/*.d.ts"  // 确保类型声明文件被包含
  ],
  "exclude": [
    "node_modules"
  ]
}

```