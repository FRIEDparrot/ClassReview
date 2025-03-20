在 Vue.js 中，prop 的命名不支持使用连字符（`-`）作为变量名的一部分。连字符一般用于在模板中标识 prop，而在 JavaScript 中，变量名（包括 props）必须遵循有效的命名规则，因此不能包含连字符。

不过，你可以通过使用 `camelCase` 或 `PascalCase` 来定义 Props 名称，然后在模板中以短横线格式调用它。比如，在组件的 script 部分定义 prop 如下：

### 定义 Props

```javascript
export default defineComponent({
    name: "BriefIteratorList",
    props: {
        itemsLength: { type: Number, required: true }, // 使用 camelCase 命名
    },
    // 其他代码...
});
```

### 在模板中使用 
在模板中，你可以使用短横线格式来传递这个 prop，例如：

```vue
<template>
    <div>
        <brief-iterator-list :items-length="totalItems" />
    </div>
</template>
```

### 解析

- **定义 Prop**：在 `props` 中使用 `camelCase`，如 `itemsLength`。
- **使用 Prop**：在父组件中，以短横线格式 `items-length` 将这个 prop 绑定到值。

### 完整示例

以下是完整的代码示例，展示如何使用 `camelCase` 来定义 prop，然后在模板中以短横线格式使用它：

```vue
<template>
    <div class="brief-iterator-list">
        <brief-iterator-list :items-length="totalItems" />
    </div>
</template>

<script>
import BriefIteratorList from './BriefIteratorList.vue';

export default {
    components: {
        BriefIteratorList,
    },
    data() {
        return {
            totalItems: 100, // 假设这是你要传递的值
        };
    },
};
</script>
```

这个方式能够确保代码清晰、符合 JavaScript 命名规则，同时也支持 Vue 的模板语法。
