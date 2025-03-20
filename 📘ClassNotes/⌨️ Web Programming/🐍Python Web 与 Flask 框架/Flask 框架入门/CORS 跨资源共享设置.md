 **CORS**（跨源资源共享）策略引起的。简而言之，它是一个安全机制，浏览器默认不允许网页从一个源（例如 `http://127.0.0.1:5174`）请求另一个源的资源（例如 `http://127.0.0.1:5000`），除非目标资源的服务器明确允许这种请求。 


**在前端请求中添加头部**:
- 一般在前端发起请求时不需要特别添加 CORS 头，因为这是浏览器会自动处理的事情。但如果你需要添加特殊的头部，可以通过在请求中设置：

如果你的后端和前端不在同一个域下（即存在跨域请求），你还需要在服务器端响应中设置 `Access-Control-Allow-Credentials: true`，否则浏览器不会发送 Cookie。

注意: 如果设置了 `Access-Control-Allow-Credentials: true`，则不能使用通配符（`*`）来设置 `Access-Control-Allow-Origin`。



问题出在你使用了 `params` 作为请求的参数，这些参数会被序列化到 URL 中，而不是放在请求的正文里。`withCredentials: true` 的设置**会使浏览器尝试发送 cookies，但它依赖于你发送的请求的类型和结构。如果请求被发送为 `GET`（默认情况下，`params` 是用在 `GET` 请求中），那么浏览器会将 cookies 附加到请求头中**。

因此方法一是采用 get 请求, 

然而，如果你期望发送的是 `POST` 请求，并且需要携带 cookies，那么你应该将请求的参数放到请求体中，而不是通过 `params` 传递。你可以使用 `data` 代替 `params`。修改后的代码如下：
```javascript
tokenVerifyApi() {
    return request.post("/auth/token/verify", {
        withCredentials: true, // send cookies along with the request
        data: {
            lang: getPreferedLangCode() || "en",  // 将 lang 放在请求体中
        },
    });
}
```

这样，后端应该能够正确接收到请求的 `data`，并且浏览器应该会发送 cookies。 

