在 Vue 应用中，使用事件通信（如事件总线或 Vuex）可以轻松实现组件间的通信。以下是实现点击导航栏图标发送通知到主窗口并触发背景切换的示例：

### 导航栏组件
在导航栏中，可以通过触发一个自定义事件来通知主窗口。这里我们使用 Vue 提供的 `$emit` 方法。

#### 修改导航栏代码
```javascript
methods: {
    updateShowArrow() {
        let shouldShowArrow = (window.innerWidth >= 1150); // show arrow on screens with width >= 1000px
        this.navItems.forEach(element => {
            element.show_arrow = shouldShowArrow;
        });
    },
    setDarkMode() { 
        let dark_icon = document.getElementById("theme-toggle-dark");
        let light_icon = document.getElementById("theme-toggle-light");
        if (this.darkmode) {
            document.documentElement.setAttribute("data-theme", "dark");
            light_icon.style.display = "block";
            dark_icon.style.display = "none";
        } else {
            document.documentElement.setAttribute("data-theme", "light");
            light_icon.style.display = "none";
            dark_icon.style.display = "block";
        }
        this.$emit("theme-changed", this.darkmode); // 通知主窗口
    }
},
mounted() {
    this.updateShowArrow();
    window.addEventListener("resize", this.updateShowArrow);

    let dark_icon = document.getElementById("theme-toggle-dark");
    let light_icon = document.getElementById("theme-toggle-light");
    
    dark_icon.addEventListener("click", (event) => {
        this.darkmode = true;
        this.setDarkMode();
    });
    light_icon.addEventListener("click", (event) => {
        this.darkmode = false;
        this.setDarkMode();
    });
    this.setDarkMode();
}
```

### 主窗口
主窗口可以监听来自导航栏的事件，并根据通知更新背景。

#### 主窗口代码示例
```javascript
<template>
    <div :class="{ 'dark-mode': isDarkMode, 'light-mode': !isDarkMode }">
        <NavBar @theme-changed="onThemeChanged" />
        <!-- 其他内容 -->
    </div>
</template>

<script>
import NavBar from './NavBar.vue';

export default {
    components: {
        NavBar
    },
    data() {
        return {
            isDarkMode: false
        };
    },
    methods: {
        onThemeChanged(isDark) {
            this.isDarkMode = isDark;
            console.log("Theme changed:", isDark ? "Dark" : "Light");
        }
    }
};
</script>

<style>
.dark-mode {
    background-color: #121212;
    color: #ffffff;
}

.light-mode {
    background-color: #ffffff;
    color: #000000;
}
</style>
```

### 工作流程
1. 导航栏组件通过 `$emit` 发出 `theme-changed` 事件，附带当前的 `darkmode` 状态。
2. 主窗口监听该事件，并通过 `onThemeChanged` 方法接收数据。
3. 主窗口根据接收的数据动态更新 `isDarkMode` 的状态，从而切换背景样式。

### 优化建议
如果项目复杂，可以引入 Vuex 来管理主题状态，方便跨组件同步和维护。例如，将 `darkmode` 状态存入 Vuex 的状态树，并通过 Vuex 的 `commit` 和 `dispatch` 来管理和分发主题切换逻辑。