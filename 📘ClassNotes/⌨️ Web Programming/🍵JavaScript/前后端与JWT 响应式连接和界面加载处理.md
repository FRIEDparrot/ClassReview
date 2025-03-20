在前端接收到后端返回的用户信息后，可以使用以下步骤保存用户信息并实现权限验证和动态界面调整。以下是一个示例方案，结合了 **Vue.js** 和 **JWT** 的前端实践。

**验证成功**
- **行为**：跳转到主页面，保存 token; 

### **前端登录处理逻辑**

#### 示例：前端登录表单与后端交互

```html
<template>
  <div>
    <form @submit.prevent="login">
      <input v-model="username" type="text" placeholder="Username" required />
      <input v-model="password" type="password" placeholder="Password" required />
      <button type="submit">Login</button>
    </form>
    <p v-if="errorMessage" class="error">{{ errorMessage }}</p>
  </div>
</template>

<script>
import axios from "axios";

export default {
  data() {
    return {
      username: "",
      password: "",
      errorMessage: ""
    };
  },
  methods: {
    async login() {
      try {
        const response = await axios.post("http://your-backend-api/uname", {
          username: this.username,
          password: this.password
        });

        // 假设后端返回 token 和用户数据
        const { token, username } = response.data.data;

        // 将 token 和用户信息保存到 localStorage 或 sessionStorage
        localStorage.setItem("authToken", token);
        localStorage.setItem("username", username);

        // 重定向到用户主页
        this.$router.push("/dashboard");
      } catch (error) {
        // 处理登录失败的情况
        if (error.response) {
          this.errorMessage = error.response.data.message;
        } else {
          this.errorMessage = "An error occurred. Please try again later.";
        }
      }
    }
  }
};
</script>
```

---

### **保存用户信息**
使用 `localStorage` 或 `sessionStorage` 将登录成功后的 `token` 和用户信息临时保存到浏览器中。

#### 示例: 
```javascript
// 保存 token 和用户信息
localStorage.setItem("authToken", token);
localStorage.setItem("username", username);

// 获取 token 和用户信息
const authToken = localStorage.getItem("authToken");
const username = localStorage.getItem("username");
```

- **`localStorage`**: 数据持久化保存，即使浏览器关闭也会保留。
- **`sessionStorage`**: 会话级存储，浏览器关闭后数据清除。

---

### **权限验证**

设置一个 **Axios 拦截器**，在每次向后端请求时自动附加 `Authorization` 头部，验证用户身份。

#### Axios 拦截器示例：

```javascript
import axios from "axios";

// 创建 Axios 实例
const apiClient = axios.create({
  baseURL: "http://your-backend-api",
  timeout: 10000
});

// 请求拦截器：附加 Authorization 头部
apiClient.interceptors.request.use(
  (config) => {
    const token = localStorage.getItem("authToken");
    if (token) {
      config.headers.Authorization = `Bearer ${token}`;
    }
    return config;
  },
  (error) => Promise.reject(error)
);

// 响应拦截器：处理权限相关的错误
apiClient.interceptors.response.use(
  (response) => response,
  (error) => {
    if (error.response && error.response.status === 401) {
      // 处理未授权错误，例如重定向到登录页
      localStorage.removeItem("authToken");
      localStorage.removeItem("username");
      window.location.href = "/login";
    }
    return Promise.reject(error);
  }
);

export default apiClient;
```

---

### **动态界面调整**

根据用户的权限，动态调整界面，例如显示/隐藏导航栏项。

#### 示例：权限验证和动态界面

```html
<template>
  <div>
    <nav v-if="isLoggedIn">
      <a href="/dashboard">Dashboard</a>
      <a href="/profile">Profile</a>
      <a @click="logout">Logout</a>
    </nav>
    <p v-else>
      <a href="/login">Login</a>
    </p>
  </div>
</template>

<script>
export default {
  computed: {
    isLoggedIn() {
      return !!localStorage.getItem("authToken");
    }
  },
  methods: {
    logout() {
      // 清除用户信息并重定向到登录页
      localStorage.removeItem("authToken");
      localStorage.removeItem("username");
      this.$router.push("/login");
    }
  }
};
</script>
```

---

### **路由守卫**

使用 Vue Router 的 **导航守卫**，确保未登录用户无法访问需要身份验证的页面。

#### 示例：

```javascript
import Vue from "vue";
import Router from "vue-router";
import Dashboard from "@/components/Dashboard.vue";
import Login from "@/components/Login.vue";

Vue.use(Router);

const router = new Router({
  routes: [
    { path: "/login", component: Login },
    { 
      path: "/dashboard", 
      component: Dashboard, 
      meta: { requiresAuth: true } 
    }
  ]
});

router.beforeEach((to, from, next) => {
  const isLoggedIn = !!localStorage.getItem("authToken");

  if (to.matched.some((record) => record.meta.requiresAuth) && !isLoggedIn) {
    next("/login"); // 未登录时重定向到登录页
  } else {
    next(); // 继续导航
  }
});

export default router;
```

---

### **完整用户体验流程**

1. 用户提交登录表单，发送用户名和密码到后端。
2. 后端返回 token 和用户信息，前端保存到 `localStorage`。
3. Axios 请求拦截器为每次请求附加 `Authorization` 头部。
4. 路由守卫限制访问，仅登录用户可访问受保护页面。
5. 动态界面调整，显示与权限匹配的功能选项。

通过这些步骤，可以有效地实现前后端协作的权限管理和用户界面动态控制。 
