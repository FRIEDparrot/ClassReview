在 Vue 项目中，你可以通过路由参数（如 `/blogs/:uuid` 中的 `uuid`）来获取数据，并将数据渲染到组件中的文本框中。以下是实现这一需求的步骤 :

### 1. **定义路由**
在路由配置文件（通常是 `router/index.ts`）中，定义带有参数的路由：

```typescript
import { createRouter, createWebHistory } from 'vue-router';
import BlogDetail from '@/views/BlogDetail.vue'; // 你的组件

const router = createRouter({
  history: createWebHistory(),
  routes: [
    {
      path: '/blogs/:uuid', // 动态路由参数
      component: BlogDetail,
    },
  ],
});

export default router;
```

### 2. **在组件中获取路由参数**
在目标组件（如 `BlogDetail.vue`）中，使用 Vue Router 的 `useRoute` 钩子来获取路由参数。

```vue
<template>
  <div>
    <input v-model="blogData.title" placeholder="Title" />
    <input v-model="blogData.content" placeholder="Content" />
  </div>
</template>

<script lang="ts">
import { defineComponent, ref, onMounted } from 'vue';
import { useRoute } from 'vue-router';
import { fetchBlogByUuid } from '@/api/blog'; // 假设有一个 API 函数用于获取数据

export default defineComponent({
  name: 'BlogDetail',
  setup() {
    const route = useRoute();
    const blogData = ref({
      title: '',
      content: '',
    });

    // 获取路由参数
    const uuid = route.params.uuid as string;
    
    // 根据 uuid 获取数据
    onMounted(async () => {
      try {
        const response = await fetchBlogByUuid(uuid);
        blogData.value = response.data; // 假设返回的数据结构是 { title: string, content: string }
      } catch (error) {
        console.error('Failed to fetch blog data:', error);
      }
    });

    return {
      blogData,
    };
  },
});
</script>
```

---

### 3. **获取数据并渲染到文本框**
在上面的代码中：
- 使用 `useRoute` 获取当前路由的参数 `uuid`。
- 在 `onMounted` 生命周期钩子中，调用 API 函数（如 `fetchBlogByUuid`）获取数据。
- 将获取到的数据绑定到 `blogData`，并通过 `v-model` 将数据渲染到文本框中。

---

### 4. **API 函数示例**
假设你有一个 API 函数 `fetchBlogByUuid`，用于根据 `uuid` 获取博客数据：

```typescript
import axios from 'axios';

export const fetchBlogByUuid = async (uuid: string) => {
  const response = await axios.get(`/api/blogs/${uuid}`);
  return response.data; // 假设返回的数据结构是 { title: string, content: string }
};
```

---

### 5. **处理路由参数变化**
如果路由参数（如 `uuid`）可能发生变化（例如，从 `/blogs/1` 跳转到 `/blogs/2`），你需要监听路由参数的变化并重新获取数据。

```typescript
import { watch } from 'vue';

// 监听路由参数变化
watch(
  () => route.params.uuid,
  async (newUuid) => {
    try {
      const response = await fetchBlogByUuid(newUuid as string);
      blogData.value = response.data;
    } catch (error) {
      console.error('Failed to fetch blog data:', error);
    }
  }
);
```

---

### 6. **完整代码示例**
以下是完整的 `BlogDetail.vue` 组件代码：

```vue
<template>
  <div>
    <input v-model="blogData.title" placeholder="Title" />
    <input v-model="blogData.content" placeholder="Content" />
  </div>
</template>

<script lang="ts">
import { defineComponent, ref, onMounted, watch } from 'vue';
import { useRoute } from 'vue-router';
import { fetchBlogByUuid } from '@/api/blog';

export default defineComponent({
  name: 'BlogDetail',
  setup() {
    const route = useRoute();
    const blogData = ref({
      title: '',
      content: '',
    });

    // 获取路由参数
    const uuid = route.params.uuid as string;

    // 根据 uuid 获取数据
    const fetchData = async (uuid: string) => {
      try {
        const response = await fetchBlogByUuid(uuid);
        blogData.value = response.data;
      } catch (error) {
        console.error('Failed to fetch blog data:', error);
      }
    };

    // 初始化获取数据
    onMounted(() => {
      fetchData(uuid);
    });

    // 监听路由参数变化
    watch(
      () => route.params.uuid,
      (newUuid) => {
        fetchData(newUuid as string);
      }
    );

    return {
      blogData,
    };
  },
});
</script>
```

---

### 7. **总结**
- 使用 `useRoute` 获取路由参数。
- 在 `onMounted` 中根据参数获取数据并渲染到文本框。
- 使用 `watch` 监听路由参数变化，确保数据动态更新。
- 通过 `v-model` 实现数据的双向绑定。

通过以上步骤，你可以实现根据路由参数获取数据并渲染到文本框的功能。