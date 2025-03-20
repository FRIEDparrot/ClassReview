
> [!error] 注意
> 除非是对于需要系统支持主题的部分， 否则不建议引入 tailwindcss 库， 对 vue 主题兼容性也不是很够。 因此不建议使用

首先最佳导入方法是使用 CDN 引入 tailwindcss 部分:
```html
<script src="https://unpkg.com/@tailwindcss/browser@4"></script>
```

注意: 很多对于引入 tailwindcss 之后的图片样式问题,  可以通过
```css 
style: display: inline-block 
```
进行解决 


## 深色模式切换 

你的 `setDarkMode` 函数已经在 `document.documentElement` 上设置了 `data-theme="dark"` 或 `data-theme="light"`，你可以利用 Tailwind CSS 的 `theme` 变量支持 `data-theme`，让 Tailwind 自动适配你的主题，而不需要单独写 `dark:` 相关的样式。

## **1. 在 `tailwind.config.js` 中配置 `data-theme` 变量**

Tailwind 默认支持 `dark` 伪类（`dark:`），但你的 `setDarkMode` 是基于 `data-theme` 切换的，所以你需要改用 `attribute` 选择器：

```js
module.exports = {
  darkMode: ['class', '[data-theme="dark"]'], // 让 Tailwind 识别 data-theme="dark"
  theme: {
    extend: {},
  },
  plugins: [],
};
```

这里 `darkMode: ['class', '[data-theme="dark"]']` 允许 `dark:` 伪类适用于 `data-theme="dark"` 的 HTML 结构，不需要 `documentElement.classList.toggle('dark')` 了。

---

## **2. 你的 API `setDarkMode` 直接控制 `data-theme`**

你的 API 现在已经能切换 `data-theme`，不需要修改：

```js
export function setDarkMode(mode: boolean) {
    const currentTheme = document.documentElement.getAttribute("data-theme");

    if ((mode && currentTheme !== "dark") || (!mode && currentTheme !== "light")) {
        store.commit("setDarkMode", mode);
        document.documentElement.setAttribute("data-theme", mode ? "dark" : "light");
    }
}
```

---

## **3. 你的 Tailwind CSS 组件支持 `data-theme`**

既然 Tailwind 现在支持 `data-theme` 了，你可以在 HTML / Vue / React 组件中这样写：

```html
<div class="bg-gray-200 text-gray-900 dark:bg-gray-800 dark:text-gray-100">
  这段文本在亮色模式下是深色，在暗色模式下是浅色
</div>
```

甚至，你可以让 `data-theme` 作用于 `bg-gray-200`：

```html
<div class="bg-gray-200 text-gray-900 [data-theme='dark']:bg-gray-800 [data-theme='dark']:text-gray-100">
  这段文本也能随主题切换
</div>
```

因为 `data-theme="dark"` 现在已经能被 Tailwind 识别，你可以直接用 `[data-theme="dark"]:` 前缀控制样式。

---

## **4. 结合 Vue/React 实现动态绑定**

如果你在 Vue 或 React 里，你可以用绑定的方式管理主题：

### **Vue 组件示例**

```vue
<template>
  <button @click="toggleDarkMode">
    切换暗色模式
  </button>
</template>

<script setup>
import { ref, watch } from 'vue';

const isDark = ref(false);

const toggleDarkMode = () => {
  isDark.value = !isDark.value;
  setDarkMode(isDark.value);
};

watch(isDark, (newVal) => {
  localStorage.setItem('darkMode', newVal ? 'dark' : 'light');
});
</script>
```

### **React 组件示例**

```jsx
import { useState, useEffect } from "react";

export default function DarkModeToggle() {
  const [darkMode, setDarkMode] = useState(
    localStorage.getItem("darkMode") === "dark"
  );

  useEffect(() => {
    setDarkMode(darkMode);
    localStorage.setItem("darkMode", darkMode ? "dark" : "light");
  }, [darkMode]);

  return <button onClick={() => setDarkMode(!darkMode)}>切换模式</button>;
}
```

---

## **5. 小结**

- **启用 `darkMode: ['class', '[data-theme="dark"]']`** 让 Tailwind 识别 `data-theme`
- **你的 `setDarkMode` API 保持不变**，仍然使用 `document.documentElement.setAttribute`
- **在 Tailwind 组件里用 `dark:` 或 `[data-theme='dark']:` 来切换颜色**
- **Vue/React 里绑定 `darkMode` 状态**，让主题切换生效

这样你的 Tailwind 主题切换就完全融入 `setDarkMode` 了，**既可以手动调用 API，也能让 Tailwind CSS 自动响应**！ 