### (1) Electron 简介
Electron 是一种基于 **Chroumium, node.js 和 Native api** 的跨平台应用开发框架. 常见的开发程序包括 VSCode, Discord 等等都是基于 Electron 进行开发的。

可以进行和 Angular, React, Vue 等等进行集成,
主要的日常应用, 除了与操作系统通讯之外, 可以**通过 chromium 进行浏览器格式的 css 等(es6, es7 等等语法特性, 并通过 html 定制界面)**

对于文件的读写操作, 可以通过 node.js 的后端来进行. 实现跨平台的桌面应用构建。而  Native API 提供统一的原生的界面能力。

### (2) Electron 主要架构
主要的 Main Process 部分包括 MainProcess, Render process 和 Native APIs 几个部分.

![[Excalidraw/Electron 基本应用开发简介 2024-10-22 22.37.56]]
Electron 内部集成了 IPC 和 RPC 等不同窗口的数据传输 API, 例如从渲染进程获取点击信息, 然后将输入传递给主进程 (通过 IPC 进行进程信息传递)

主进程一般是 package.json 中, main 属性所对应的文件.
需要说明的是, **只有主进程才能进行 GUI 的 API 操作**, 而渲染进程需要先建立信息通信才能进行调用 Native  API 通信

electron 网站: https://www.electronjs.org/ 
界面支持 dom 操作和 node api 调用操作, 而一个应用可以具有多个不同的渲染进程. 

参考 https://www.electronforge.io/ 如果采用 electron forge, 则需要带有 electron-app 参数
npm 包是在开发过程中广泛使用的,采用如下方法创建一个初始的app工程，可以利用 [Squirrel](https://github.com/Squirrel) 包进行相应的自动应用程序更新:
```sh
npm init electron-app@latest test1
npm init electron-app@latest my-app -- --template=webpack
```

**==任何  electron 有关包都可以从 npm 进行安装==**

### (3) Electron 项目基本组成
初始项目参考 [quick start](https://github.com/electron/electron-quick-start) 
A basic Electron application needs just these files:
- `package.json` - Points to the app's main file and lists its details and dependencies.
- `main.js` - Starts the app and creates a browser window to render HTML. This is the app's **main process**.
- `index.html` - A web page to render. This is the app's **renderer process**.
- `preload.js` - A content script that runs before the renderer process loads.

其中 main.js 首先需要 require('electron') 包部分


```json title:package.json
{
  "name": "electron-quick-start",
  "version": "1.0.0",
  "description": "A minimal Electron application",
  "main": "main.js",
  "scripts": {
    "start": "electron ."    /* this specifiies the  program to run(this project)*/
  },
  "repository": "https://github.com/electron/electron-quick-start",
  "keywords": [
    "Electron",
    "quick",
    "start",
    "tutorial",
    "demo"
  ],
  "author": "GitHub",
  "license": "CC0-1.0",
  "devDependencies": {
    "electron": "^33.0.2"
  }
}
```


```js title:main.js
// Modules to control application life and create native browser window
const { app, BrowserWindow } = require('electron')
const path = require('node:path')

function createWindow () {
  // Create the browser window.
  const mainWindow = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      preload: path.join(__dirname, 'preload.js')
    }
  })

  // and load the index.html of the app.
  mainWindow.loadFile('index.html')
  // Open the DevTools.
  // mainWindow.webContents.openDevTools()
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.whenReady().then(() => {
  createWindow()

  app.on('activate', function () {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (BrowserWindow.getAllWindows().length === 0) createWindow()
  })
})

// Quit when all windows are closed, except on macOS. There, it's common
// for applications and their menu bar to stay active until the user quits
// explicitly with Cmd + Q.
app.on('window-all-closed', function () {
  if (process.platform !== 'darwin') app.quit()
})

// In this file you can include the rest of your app's specific main process
// code. You can also put them in separate files and require them here.
```

```js title:preload.js
/**
 * The preload script runs before `index.html` is loaded
 * in the renderer. It has access to web APIs as well as
 * Electron's renderer process modules and some polyfilled
 * Node.js functions.
 *
 * https://www.electronjs.org/docs/latest/tutorial/sandbox
 */
window.addEventListener('DOMContentLoaded', () => {
  const replaceText = (selector, text) => {
    const element = document.getElementById(selector)
    if (element) element.innerText = text
  }

  for (const type of ['chrome', 'node', 'electron']) {
    replaceText(`${type}-version`, process.versions[type])
  }
})
```

```html title:index.html
<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8">
    <!-- https://developer.mozilla.org/en-US/docs/Web/HTTP/CSP -->
    <meta http-equiv="Content-Security-Policy" content="default-src 'self'; script-src 'self'; style-src 'self' 'unsafe-inline'">
    <link href="./styles.css" rel="stylesheet">
    <title>Hello World!</title>
  </head>
  <body>
    <h1>Hello World!</h1>
    We are using Node.js <span id="node-version"></span>,
    Chromium <span id="chrome-version"></span>,
    and Electron <span id="electron-version"></span>.
    <!-- You can also require other files to run in this process -->
    <script src="./renderer.js"></script>
  </body>
</html>
```


## 导入 node.js 相关包
首先 node.js 包是采用 require 命令进行导入的
```js
const { app, BrowserWindow } = require('electron')
const path = require('node:path')
const url = require('node:url')
```
