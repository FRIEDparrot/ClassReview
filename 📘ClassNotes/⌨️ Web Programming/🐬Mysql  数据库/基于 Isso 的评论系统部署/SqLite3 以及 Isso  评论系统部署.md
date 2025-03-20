### (1) SqLite3 安装和配置
对于 Python,  sqlite3 是一个基于本地文件的， 极为轻量级的数据库，且python3 内置 sqlite3 模块，

由于 sqlite3 极为轻量级，一个 SQLite `.db` 文件只有一个主数据库，每个 SQLite 数据库文件都包含一个主数据库，这被称为 "main database"。在 SQL 语法中，你可以使用默认的数据库连接来查询和操作这个主数据库。 

可以直接使用一个 python 脚本创建 .db 文件
```python
import sqlite3  
  
# create comments.db file  
conn = sqlite3.connect('comments.db') 
```

在 windows 下, 可以直接从本地进行开发和运行 :  
![[attachments/Pasted image 20250228121616.png|600]] 

但是在实际的 Linux 运行环境中，为了仍然从文件获取， 并通过密码认证访问评论数据库， 我们需要配置 sqlite3-server : 

```sh
npm install -g sqlite3-server
```

具体可以参考 [[📘ClassNotes/⌨️ Web Programming/🐬Mysql  数据库/基于 Isso 的评论系统部署/利用 sqlite3-server 提供本地数据库文件访问|利用 sqlite3-server 提供本地数据库文件访问]]  

### (2) Isso 评论系统部署 
Isso 是一个开源的评论系统， 同时支持 server-hosting (服务器托管) ,   基于  SqlLite3 运行， 这意味着可以通过自研的独立注册机制下进行认证和评论。

对于后端 python 采用 Flask 框架支持， 可使用 : 
```python
pip install isso
```

创建一个 `isso.cfg` 文件 : 其中 filepath 直接配置为本地 `.db` 文件,  生产环境一般配置为  :
```sh
/var/lib/isso/comments.db
```

具体如下: 
```ini
[general]
; database location, check permissions, automatically created if it
; does not exist
dbpath = /var/lib/isso/comments.db

; for docker image: dbpath = /db/comments.db
;
; your website or blog (not the location of Isso!)
host = http://example.tld/

; you can add multiple hosts for local development
; or SSL connections. There is no wildcard to allow
; any domain.
host =
    http://localhost:1234/
    http://example.tld/
    https://example.tld/
```

运行 comment system, 只需要运行一行代码 : 
```sh
(venv) isso -c isso.cfg 
```
![[attachments/Pasted image 20250228123305.png|600]] 

然后在前端部分: 
```html
<script data-isso="//comments.example.tld/"
        src="//comments.example.tld/js/embed.min.js"></script>

<section id="isso-thread">
    <noscript>Javascript needs to be activated to view comments.</noscript>
</section>
```

具体可以参考 https://isso-comments.de/docs/reference/server-config/  进行配置 
