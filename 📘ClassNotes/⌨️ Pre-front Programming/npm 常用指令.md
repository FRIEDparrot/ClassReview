Node.js 是基于 ChormeV8 引擎的 JavaScript 运行环境,  可以基于 Express 框架构建 Web APP, 或者 Electron 框架构建应用,  基于restify 接口构建 API 接口项目等等, API 参考[nodejs api](https://nodejs.org/docs/latest/api/console.html)。

全局包:`npm@9.2.0 C:\Program Files\nodejs\node_global\node_modules\npm` 

## 一、npm 指令类型总结
### (1) 基础指令
npm 设置镜像源:
```sh
npm config set registry=https://registry.npmmirror.com
```
启动  app 只需要采用
```sh
cd app1
npm install   # use cnpm install  instead (yarn install  # 仍然不行)
sudo npm install -g typescript
npm start
npm  run dev
```

**在项目下安装 electron**(这个是必备的):
```sh
cnpm install electron --save-dev
```

```sh
# npm 通过 make 构建整个包的项目, 例如如下 可以进行 build 
npm run make
```

查看当前或者全局某个包的版本:
```sh
npm list express
npm list -g express 
npm show express version
```

采用 electron-forge 构建工程(需要同时安装 electron):
 ```sh
npm init electron-app@latest test1
npm init electron-app@latest my-app --template=webpack
```
![[attachments/Pasted image 20241023154042.png]]
利用 npm fund 来查看当前的工程结构信息:
```sh
PS E:\workpack\JS\Test1\test1> npm fund
test1@1.0.0
`-- https://github.com/sponsors/malept
  | `-- @electron-forge/cli@7.5.0, @electron-forge/core@7.5.0, @malept/cross-spawn-promise@2.0.0
  `-- https://github.com/chalk/chalk?sponsor=1
      `-- chalk@4.1.2
```

如果在 git 上进行发行, 则可以采用 electron-forge/publisher 包自动生成 publish 脚本
```sh
cnpm install --save-dev @electron-forge/publisher-github
cnpm run publish
```

### (2) 全局包相关
查看全局包:
```sh
npm list -g
npm root -g    # 查看全局包的安装路径 
sudo npm update -g   # 更新所有全局包
npm config list
```
缓存相关:
```sh
sudo npm cache verify  # 自动清除缓存, 验证 npm 缓存并报告缓存的状态
sudo npm cache clean --force  # 强制清除缓存
```
要更新所有全局安装的 npm 包，可以使用 `npm update -g` 

`npm update -g`：可以更新全局包到符合版本范围的版本。
`npm-check-updates`：可以更新全局包到最新可用版本。
`npm outdated` : 查看过期的包部分

> 使用 `npm-check-updates` 更新到最新版本
1. 安装 `npm-check-updates` 包（全局安装）：
```bash
 npx npm-check-updates
 npm install -g npm-check-updates
```
2. 更新全局包：
```bash
ncu -g -u
```
   - `ncu`: 命令是 `npm-check-updates` 的缩写。
   - `-g`: 针对全局包。
   - `-u`: 更新到最新版本。
```bash
npm install -g # 全局模块更新到最新版本
```

> [!CAUTION] Security
> Common web vulnerabilities, such as Cross-Site Scripting (XSS), have a higher security impact on Electron applications hence it is highly recommended to adopt secure software development best practices and perform security testing.
> 
> **To display remote content, use the [`<webview>`](https://www.electronjs.org/docs/latest/api/webview-tag) tag or a [`WebContentsView`](https://www.electronjs.org/docs/latest/api/web-contents-view) and make sure to disable the `nodeIntegration` and enable `contextIsolation`.**

see more at [security tutorial](https://www.electronjs.org/docs/latest/tutorial/security)
![[attachments/Pasted image 20241023160307.png]]

### (3) 构建基于 Electron-forge 的 App 应用
```sh
cnpm init electron-app@latest test1
cnpm install electron 
cnpm install electron-squirrel-startup
cnpm install glob@9
npm install @electron-forge/plugin-fuses
npm install @electron/packager@latest 
npm install rimraf@4
npm  start 
```

### (4) 常用的 require 包设置
一般使用 express 包可以进行方便地<mark style="background: transparent; color: red">创建Web网站的服务器和API接口的服务器</mark>, 
```js
npm -i  express 
```

```js title:创建服务器
const http = require('http')    /* 导入 http 模块*/
server = http.createServer()   /* 创建实例  */ 
server.on('request', (req,res)=>{console.log('visit detect')})    /* 绑定 request 事件*/
server.listen(8090,()=>{console.log('http server at 127.0.0.1')})      /* 启动监听 */
server.on('request', (req,res)=>{const str=`${req.url}`; res.end(str)})  /*  */
server.close()
```

```js
const fs = require('fs')
```


## 二、 Electron 和 C++ 底层进行集成化使用 
### (1) 集成用的包
在Electron中，可以通过多种方式集成C++或C#代码，例如将它们编译为动态链接库（DLL），然后通过JavaScript调用这些本地模块。这种方式通常用于提升性能或利用现有的C++/C#代码库。以下是几种常见的方法和后端插件，适用于集成C++或C#代码到Electron中: 
#### 1. **`ffi-napi` 和 `ref-napi`**
   - **语言**: C, C++, C#（通过编译成DLL）
   - **适用场景**: 需要调用现有的C/C++或C#编译的动态链接库（DLL）时。
   - **简介**: `ffi-napi` 和 `ref-napi` 是Node.js模块，用于从JavaScript直接调用编译好的本地库（如`.dll`文件或`.so`文件）。

   - **集成步骤**:
     1. 编写并编译C++或C#代码，生成动态链接库（如`mydll.dll`）。
     2. 使用`ffi-napi`在Electron中调用这些DLL的函数。

   - **示例**:
     ```bash
     npm install ffi-napi ref-napi
     ```
     在Electron项目中编写调用代码：
     ```javascript
     const ffi = require('ffi-napi');
     const ref = require('ref-napi');

     const myLib = ffi.Library('path/to/mydll', {
       'myFunction': ['int', ['int', 'int']]
     });

     console.log(myLib.myFunction(1, 2)); // 调用C++/C#编写的函数
     ```

   - **优点**: 代码更简单，不需要深度了解V8，适合调用已有的DLL文件。
   - **缺点**: 由于是跨语言调用，性能可能不如直接编写Node.js插件。 
#### 2. **Edge.js**
   - **语言**: C# 和 .NET
   - **适用场景**: 需要与C#和.NET库进行集成时。
   - **简介**: `Edge.js` 是一个允许Node.js和.NET交互的库，可以直接在Node.js中运行C#代码。

   - **集成步骤**:
     1. 安装`edge-js`模块：
        ```bash
        npm install edge-js
        ```
     2. 使用`edge-js`在JavaScript中调用C#代码或DLL：
        ```javascript
        const edge = require('edge-js');

        const hello = edge.func(function() {/*
          async (input) => { 
              return ".NET welcomes " + input.ToString(); 
          }
        */});

        hello('Electron', function(error, result) {
            if (error) throw error;
            console.log(result); // 输出: .NET welcomes Electron
        });
        ```

     - **调用现有的C# DLL**:
       ```javascript
       const edge = require('edge-js');

       const myLib = edge.func({
           assemblyFile: 'path/to/mydll.dll',
           typeName: 'MyNamespace.MyClass',
           methodName: 'MyMethod' // 这个方法必须是异步的
       });

       myLib('someInput', function(error, result) {
           if (error) throw error;
           console.log(result);
       });
       ```

   - **优点**: 很适合现有的.NET项目或库的复用，API相对简单。
   - **缺点**: 需要.NET环境，可能不适合对性能要求特别高的场景。

### (2) dll 动态链接库生成办法

在 C++ 和 C# 中生成动态链接库（DLL）文件的过程有些不同，但两者都可以通过 Visual Studio 实现。以下是它们各自的步骤：
#### 1. 在 C++ 中生成 DLL
1. **创建项目**：
   - 打开 Visual Studio。
   - 选择“新建项目”（New Project）。
   - 选择“`C++`” -> “`Windows`” -> “`动态链接库（DLL）`”模板。
   - 选择“`Windows 桌面 DLL`”，并为项目命名。
2. **定义导出函数**：
   - 在生成的 `.cpp` 文件中，定义你想导出的函数。
   - 使用 `__declspec(dllexport)` 关键字来标记导出的函数。例如
```cpp
#include "pch.h" // 如果使用预编译头文件
extern "C" __declspec(dllexport) int Add(int a, int b) {
	return a + b;
}
```
3. **编译项目**：
   - 点击“生成”菜单，然后选择“生成解决方案”。
   - 生成的 DLL 文件将位于项目的输出目录（通常在 `Debug` 或 `Release` 文件夹下）。
#### 2. 在 C# 中生成 DLL
1. **创建项目**：
   - 打开 Visual Studio。
   - 选择“新建项目”（New Project）。
   - 选择“`C#`” -> “`类库（Class Library）`”模板。
   - 为项目命名并创建。
2. **编写类和方法**：
   - 在生成的 `Class1.cs` 文件中，编写你想导出的类和方法。例如：
```csharp
using System;
public class MathOperations
{
	 public int Add(int a, int b)
	 {
	 return a + b;
	 }
}
```
3. **编译项目**：
- 点击“生成”菜单，然后选择“生成解决方案”。
- 生成的 DLL 文件将位于项目的输出目录（通常在 `bin\Debug` 或 `bin\Release` 文件夹下）。

说明:
- **C++ DLL**：通常用于需要与低级系统组件交互或使用 C/C++ 编写的库时。导出时需要注意函数调用约定。
- **C# DLL**：主要用于 .NET 环境，适合创建类库供其他 .NET 应用程序调用。

在导出函数时，如果你希望在不同语言中调用这些 DLL 函数（如在 C# 中调用 C++ DLL），需要注意函数的导出方式和调用约定，例如使用 `extern "C"` 来避免名称修饰（name mangling）。
