Node.js 是基于 ChormeV8 引擎的 JavaScript 运行环境,  可以基于 Express 框架构建 Web APP, 或者 Electron 框架构建应用,  基于restify 接口构建 API 接口项目等等。

通过require可以获取其中指定的模块
```js title:创建服务器
const http = require('http')    /* 导入 http 模块*/
server = http.createServer()   /* 创建实例  */ 
server.on('request', (req,res)=>{console.log('visit detect')})    /* 绑定 request 事件*/
server.listen(8090,()=>{console.log('http server at 127.0.0.1')})      /* 启动监听 */
server.on('request', (req,res)=>{const str=`${req.url}`; res.end(str)})  /*  */
server.close()
```

一般使用 express 包可以进行方便地<mark style="background: transparent; color: red">创建Web网站的服务器和API接口的服务器</mark>, 
```js
npm -i  express 
```

```js
const fs = require('fs')
```
