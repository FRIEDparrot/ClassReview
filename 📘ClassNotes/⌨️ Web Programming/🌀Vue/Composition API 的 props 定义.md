在 Vue 3 的 Composition API 中，您可以使用 `defineProps` 来指定组件的 props。以下是一些基本的用法示例。

### 1. 基本用法

您可以通过 `defineProps` 函数来定义 props。在 `<script setup>` 中使用时，如下所示：

```vue
<template>
  <div>{{ message }}</div>
</template>

<script setup>
const props = defineProps({
  message: {
    type: String,
    required: true
  }
});
</script>
```

### 2. 类型推断

如果您的 props 不需要复杂的验证，您也可以简化为以下方式：

```vue
<template>
  <div>{{ title }}</div>
</template>

<script setup>
const props = defineProps(['title']);
</script>
```

### 3. 组合使用 `withDefaults`

您还可以使用 `withDefaults` 来给 props 设定默认值：

```vue
<template>
  <div>{{ title }}</div>
</template>

<script setup>
const props = withDefaults(defineProps(['title']), {
  title: '默认标题'
});
</script>
```

### 总结

您可以灵活运用 `defineProps` 来为您的组件提供所需的 props，并结合类型、必需性和默认值进行配置。如果您有更具体的需求或例子，欢迎告诉我！
