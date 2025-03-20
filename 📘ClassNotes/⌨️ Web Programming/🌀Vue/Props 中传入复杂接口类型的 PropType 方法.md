
> [!NOTE] 补充
> 在前端， `const localTime = new Date(utcTime.getTime());` 这种方式会自动将 UTC 时间转换为本地时间。  

为了在 ts 中保证安全性， 同时在属性中传入接口，  应当遵循如下方法: 

我们以传入一个接口和传入一个接口数组为例进行讲解: 
```ts 
export interface IstdMenuItem {
    titleKey?: string;
    title?: string;
    type?: string; // for 'divider', 'subheader'
    priotity?: number;
    icon?: string; // prepend icon (or append icon)
    badge?: boolean; // show badge
    badgeCount?: number; // number to show in badge(if badge is true)
    url?: string;
    callback?: () => void;
}

export interface IstdMenuList {
    items: Array<IstdMenuItem>;
}
```

而有一个组件需要引用，  如果采用 istdMenuList 传入， 采用 `PropType<Object>` 声明类型 :
```ts 
import { defineComponent, PropType } from 'vue'; 
 menus: {
	type: Object as PropType<IstdMenuList>, // 使用 Object 而不是 Array
	default: () => ({ items: [] } as IstdMenuList), // 默认值是一个对象
},
```

如果采用 IstdMenuItem， 则使用 Array 声明类型: 

```ts
menus: {
	type: Array as PropType<IstdMenuItem[]>, // 使用 PropType 包装
	default: () => [] as IstdMenuItem[],
},
```

注意以下关键点 : 
1. **`PropType` 的作用**：
   - `PropType` 是 Vue 提供的一个工具类型，用于将 TypeScript 的静态类型与 Vue 的运行时类型系统结合起来。
   - **对于复杂类型（如接口、接口数组、联合类型等），必须使用 `PropType` 来明确类型**。

2. **为什么不能直接使用 `IstdMenuItem[]`**：
   - Vue 的 `props` 类型系统是基于 JavaScript 的运行时类型检查（如 `Array`、`String`、`Number` 等）。
   - TypeScript 的**静态类型（如 `IstdMenuItem[]`）无法直接映射到 Vue 的运行时类型系统，因此需要通过 `PropType` 来显式声明`**。

3. **默认值的函数形式**：
   - 对于数组或对象的默认值，建议使用函数形式（`default: () => []`），以避免多个组件实例共享同一个引用类型的数据。
