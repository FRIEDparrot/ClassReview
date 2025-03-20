**登录数据**（如身份验证令牌、用户会话等）通常建议存储在 **cookies** 中，而不是 `localStorage`，因为 cookies 本身可以设置 **自动过期时间**，并且更适合处理与服务器交互的身份验证数据。

### 为什么使用 cookies 而不是 localStorage 存储登录数据？

1. **自动过期**： Cookies 可以设置一个过期时间，浏览器会自动删除过期的 cookies，无需开发者手动处理。你可以指定一个固定的过期时间（例如 1 小时、1 天），或者设置为会话级别的 cookie，浏览器关闭时即删除。
    
2. **服务器交互**： Cookies 是与每个请求一起自动发送到服务器的，这使得它们非常适合存储会话信息（例如，JWT Token 或 session ID）。这样，服务器每次接收到请求时，都能自动读取请求头中的 cookies。
3. **安全性**：
    - **HTTPOnly**：可以通过设置 `HttpOnly` 标志来防止 JavaScript 访问 cookies，从而防止 XSS 攻击。
    - **Secure**：可以通过设置 `Secure` 标志，仅在通过 HTTPS 连接时发送 cookies，提高数据的安全性。
4. **跨标签页共享**： Cookies 是在浏览器范围内共享的，无论是刷新页面、打开新标签页，还是在不同的子域下，都会共享 cookies，而 `localStorage` 是在<b><mark style="background: transparent; color: orange">同一页面或同一窗口中共享的</mark></b>。

### 如何存储和读取登录数据（例如 token）使用 cookies

#### 1. **设置带过期时间的 Cookie**

你可以通过 `document.cookie` 或使用一个库（如 `js-cookie`）来设置带过期时间的 cookie。

```javascript
// 设置 cookie
function setCookie(name, value, days) {
    const expires = new Date();
    expires.setTime(expires.getTime() + days * 24 * 60 * 60 * 1000);
    document.cookie = `${name}=${value}; expires=${expires.toUTCString()}; path=/; secure; HttpOnly`;
}
```

#### 2. **读取 Cookie**

你可以通过 `document.cookie` 获取 cookies 中的数据，或者使用一个解析函数来读取特定的 cookie 值。

```javascript
// 获取 cookie 值
function getCookie(name) {
    const nameEQ = name + "=";
    const ca = document.cookie.split(';');
    for (let i = 0; i < ca.length; i++) {
        let c = ca[i];
        while (c.charAt(0) == ' ') c = c.substring(1, c.length);
        if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length, c.length);
    }
    return null;
}
```

#### 3. **删除 Cookie**

当用户退出登录时，你可能希望删除 cookies。

```javascript
// 删除 cookie
function eraseCookie(name) {
    document.cookie = name + '=; Max-Age=-99999999;';
}
```

### 示例：登录时存储 Token，设置过期时间

```javascript
// 登录成功后，设置带有过期时间的 cookie
const token = "your_jwt_token_here";
setCookie("auth_token", token, 1); // 1天过期
```

### 示例：读取 Token 并验证是否有效

```javascript
// 在后续的请求中，读取 token
const token = getCookie("auth_token");
if (token) {
    // 使用 token 进行 API 请求
    console.log("Token found:", token);
} else {
    console.log("Token not found or expired.");
}
```

### 示例：用户退出登录时删除 Token

```javascript
// 退出登录时删除 token
eraseCookie("auth_token");
```

### 安全性考虑：

1. **HttpOnly**： 如果使用 `HttpOnly` 标志，JavaScript 无法直接访问 cookie，因此可以防止 XSS 攻击窃取 cookie。例如：
    
    ```javascript
    document.cookie = "auth_token=your_jwt_token; expires=Thu, 18 Dec 2023 12:00:00 UTC; path=/; HttpOnly; secure";
    ```
    
2. **Secure**： 设置 `Secure` 标志，可以确保 cookies 仅通过 HTTPS 协议发送，增强数据传输的安全性：
    
    ```javascript
    document.cookie = "auth_token=your_jwt_token; expires=Thu, 18 Dec 2023 12:00:00 UTC; path=/; Secure";
    ```
    

### `localStorage` 和 `cookies` 的对比：

|特性|`localStorage`|`cookies`|
|---|---|---|
|**存储容量**|5MB 以内（浏览器限制）|大约 4KB（浏览器限制）|
|**过期时间**|无内置过期时间，需要手动实现|支持自动过期（`expires` 或 `max-age`）|
|**数据传输**|不自动发送到服务器|每次 HTTP 请求自动发送到服务器|
|**安全性**|易受 XSS 攻击（除非加密数据）|可以通过 `HttpOnly` 和 `Secure` 标志增强安全性|
|**跨标签页共享**|是（同一窗口和标签页）|是（同一浏览器，所有标签页共享 cookies）|

### 总结：

- 对于 **登录数据**（如 JWT token 或会话 ID），**cookies** 是更好的选择，特别是当你需要与服务器进行身份验证交互时。使用 cookies，你可以轻松实现自动过期、自动随请求发送、提高安全性（如使用 `HttpOnly` 和 `Secure`）等功能。
- `localStorage` 更适合存储一些不会随每个请求一起发送的客户端数据，而不适合存储需要长期验证的敏感信息（如用户登录状态）。




在你的场景中（暗色主题需要跨页面共享，且可能跳转出当前页面后返回），使用 **`localStorage`** 或 **`cookies`** 存储主题偏好都有其优点，但推荐的选择取决于以下几个因素：


### **1. 使用 `localStorage` 的情况**

#### **优点**:

1. **简单易用**：`localStorage` 的操作较为直观，直接在前端管理。
2. **跨页面共享**：在同一域名下的所有页面都可以访问 `localStorage`，可以很好地满足你的需求。
3. **性能较优**：数据存储在客户端，不会在每个 HTTP 请求中自动发送，避免增加请求负担。

#### **适用场景**:

- 主题偏好是仅在客户端使用的数据。
- 偏好数据没有必要传递到服务器。
- 需要一个轻量、快速的存储方式。

#### **实现示例**:

```javascript
// 保存主题到 localStorage
function saveThemePreference(theme) {
    localStorage.setItem('theme', theme);
}

// 加载主题偏好
function loadThemePreference() {
    return localStorage.getItem('theme') || 'light'; // 默认主题为 light
}

// 应用主题
function applyTheme(theme) {
    document.documentElement.setAttribute('data-theme', theme);
}

// 初始化时调用
const savedTheme = loadThemePreference();
applyTheme(savedTheme);
```

---

### **2. 使用 `cookies` 的情况**

#### **优点**:

1. **服务器端同步**：如果需要在服务器端获取用户的主题偏好（例如，用于 SSR 或个性化响应），`cookies` 是一个更好的选择。
2. **更持久的存储**：`cookies` 支持设置过期时间，适合用于长期存储偏好。
3. **跨子域共享**：如果你的应用有多个子域，`cookies` 可以通过 `domain` 属性实现跨域共享。

#### **缺点**:

1. **请求开销**：`cookies` 会随着每个 HTTP 请求发送到服务器，可能导致额外的网络开销（虽然主题偏好通常是小数据，不会对性能有太大影响）。
2. **操作稍复杂**：需要引入库（如 `js-cookie`）或手动实现 `cookie` 的读取和写入逻辑。

#### **适用场景**:

- 需要在服务器端使用主题偏好。
- 需要跨子域共享主题偏好。

#### **实现示例（使用 `js-cookie` 库）**:

```javascript
import Cookies from 'js-cookie';

// 保存主题到 cookie
function saveThemePreference(theme) {
    Cookies.set('theme', theme, { expires: 365 }); // 设置为 1 年后过期
}

// 加载主题偏好
function loadThemePreference() {
    return Cookies.get('theme') || 'light'; // 默认主题为 light
}

// 应用主题
function applyTheme(theme) {
    document.documentElement.setAttribute('data-theme', theme);
}

// 初始化时调用
const savedTheme = loadThemePreference();
applyTheme(savedTheme);
```

#### 安装 `js-cookie`:

```bash
npm install js-cookie
```

---

### **3. 比较：`localStorage` vs `cookies`**

|**特性**|**localStorage**|**cookies**|
|---|---|---|
|**数据持久性**|持久（除非用户手动清除）|可设置过期时间|
|**跨页面共享**|是|是|
|**服务器可访问性**|否|是（会在请求头中发送）|
|**适用场景**|仅前端使用|需要后端也知道偏好|
|**数据大小限制**|~5MB（每个域名）|~4KB（每个 `cookie`）|
|**操作复杂性**|简单|需要额外实现读写逻辑或引入库|

---

### **4. 推荐选择**

#### **使用 `localStorage` 的场景**:

- 如果主题偏好只需要在客户端前端共享，`localStorage` 是更简单和高效的选择。

#### **使用 `cookies` 的场景**:

- 如果需要后端参与处理用户的主题偏好（例如用于个性化内容渲染或跨设备同步），使用 `cookies` 更合适。

---

### **5. 结合 Vuex 管理状态（可选）**

无论选择 `localStorage` 还是 `cookies`，可以在 Vuex 中管理主题状态，并在初始化时从存储中加载偏好：

```javascript
import { createStore } from 'vuex';
import Cookies from 'js-cookie'; // 如果使用 cookies

export default createStore({
    state: {
        theme: Cookies.get('theme') || 'light', // 或从 localStorage 加载
    },
    mutations: {
        setTheme(state, theme) {
            state.theme = theme;
            Cookies.set('theme', theme, { expires: 365 }); // 或 localStorage.setItem('theme', theme);
        },
    },
    actions: {
        toggleTheme({ commit, state }) {
            const newTheme = state.theme === 'light' ? 'dark' : 'light';
            commit('setTheme', newTheme);
            document.documentElement.setAttribute('data-theme', newTheme);
        },
    },
});
```

通过这种方式，你可以方便地在 Vue 组件中响应主题切换操作，同时持久化用户的主题偏好。