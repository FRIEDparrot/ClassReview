### **问题分析**
1. **GSAP 插件未注册**    
    - 你的错误信息提示 **"Missing plugin? gsap.registerPlugin()"**。
    - 可能是 **`gsap.registerPlugin()` 没有正确调用** 或 **使用的 `effect` 依赖于某些 GSAP 插件**（如 `ScrollTrigger`、`MotionPathPlugin` 等）。
    - 你需要检查 `gsap.effects.floatingAlertWindow(alert_obj);` 内部是否用到了 **`y` 或 `opacity`**，而这些动画可能依赖 **GSAP 插件**。
2. **`this.$refs.alertRef` 失败** 
    - Vue `ref` 绑定的组件在 `setup()` 中需要 `defineExpose({ alertRef })` 暴露。
    - 但你这里是 `Options API`，**this.$refs.alertRef 实际上是 Vue 组件对象，而不是 DOM 元素**。
    - GSAP **适用于 DOM 元素**，所以你应该在 **`this.$refs.alertRef.$el`** 获取实际 DOM。
3. **`document.getElementById` vs `$refs`** 
    - **`document.getElementById` 可能导致重复 ID**：
        - 在 **多个页面中使用** `FloatingAlert` 组件时，`id="floating-alert"` 可能会重复，导致 `getElementById` 获得的是 **错误的元素**。
    - **正确方式**：
        - **如果是 Vue 组件**，应使用 `$refs.alertRef.$el`。
        - **如果是普通 DOM**，则可以使用 `document.getElementById`，但需确保 **ID 唯一**。

---

### **修正代码**

#### **1. 确保 GSAP 插件已正确注册**

在 `main.js` 或 `animation_main.js` **手动注册 GSAP 插件**：

```js
import gsap from "gsap";
import { CSSPlugin } from "gsap/CSSPlugin"; 

gsap.registerPlugin(CSSPlugin);
```

如果 `floatingAlertWindow` 依赖其他插件（如 `ScrollTrigger`、`MotionPathPlugin`），请确保它们也被 `registerPlugin`。

---

#### **2. Vue 组件：使用 `$refs.alertRef.$el` 作为 GSAP 目标**
注意 el 是对应的 dom 元素;
```vue
<template>
    <v-alert
        v-if="show"
        max-width="500"
        ref="alertRef"
        id="floating-alert"
        style="
            opacity: 0;
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            z-index: 100;
            pointer-events: none;
        "
        :type="type"
        :title="title"
        :text="text"
    >
    </v-alert>
</template>

<script>
import gsap from "gsap";

export default {
    data() {
        return {
            show: false,
            type: "success",
            title: "",
            text: "",
        };
    },
    methods: {
        showAlert(type = "success", title = "Success", text = "") {
            this.type = type;
            this.title = title;
            this.text = text;
            this.show = true;

            this.$nextTick(() => {
                // 这里使用 `$refs.alertRef.$el` 作为 DOM 目标
                gsap.effects.floatingAlertWindow(this.$refs.alertRef.$el);
            });

            // 3 秒后消失
            setTimeout(() => {
                this.show = false;
            }, 3000);
        },
    },
};
</script>
```

---

#### **3. `Login.vue` & `Register.vue` 调用方式**

```vue
<FloatingAlert ref="emailAlert" />
```

```js
methods: {
    show_email_alert(success = true, msg = "Email sent successfully") {
        this.$refs.emailAlert.showAlert(
            success ? "success" : "error",
            success ? "Success" : "Error",
            msg
        );
    },
}
```

---

### **✅ 这样做的优势**

1. **修复 `$refs` 不能直接用于 GSAP**（必须获取 `$el`）。
2. **避免 `document.getElementById` 可能的 ID 冲突问题**。
3. **确保 GSAP 插件正确注册，避免 `opacity` 和 `y` 的报错**。
4. **让 `FloatingAlert` 组件完全可复用**，多个页面都能调用 `showAlert()`。

---

### **🚀 总结**

- **问题1**（GSAP 插件未注册） ➜ **手动 `gsap.registerPlugin(CSSPlugin)`**
- **问题2**（`this.$refs.alertRef` 不能直接用） ➜ **改成 `this.$refs.alertRef.$el`**
- **问题3**（避免 `document.getElementById` 可能的 ID 冲突） ➜ **使用 `$refs`**
- **问题4**（优化 `FloatingAlert` 组件，使其可复用）

这样，`FloatingAlert` 组件就可以 **在不同页面** **直接调用 `showAlert()`**，不会出现 Vue `$refs` 失效的问题 🚀。