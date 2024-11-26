# 最简步骤
- 在git上创建一个仓库最简的步骤, 首先在github上创建一个仓库并且获取https地址
```shell 
git init
git remote add origin https://github.com/FRIEDparrot/RemoteControl_Password_Lock.git 
git add *
git commit -m "test"
git branch  # 显示分支信息
git push origin master 
```

C:/Program%20Files/Git/mingw64/share/doc/git-doc

CLI -> command line interface
1.  创建公钥对应的SSH key

how to create the SSH key

```Bash
ssh-keygen -t rsa -C "your_email@example.com"
```

2. 提高代码的可读性
```Bash
git config --global color.ui auto
```
3. 查看对应的公钥和私钥文件
```Bash
cd ~/.ssh
ls

# github_id_rsa  github_id_rsa.pub  id_ed25519  id_ed25519.pub  known_hosts

cat ~/.ssh/github_id_rsa.pub
# 查看对应的SSH公钥
```

4. 克隆仓库
 ```Bash
 git clone git@github.com:hirocastest/Hello-World.git
 ```
 
```Bash
git status
# Changes not staged for commit:
#  (use "git add <file>..." to update what will be committed)
#  (use "git restore <file>..." to discard changes in working directory)
# (commit or discard the untracked or modified content in submodules)
#        modified:   pattern_identification_homework (modified content, untracked content)

# no changes added to commit (use "git add" and/or "git commit -a")
```

5. 查看提交日志
```Bash
git log
```


已经配置完毕所有的认证
```Bash
ssh -T git@github.com

ssh -T git@gitee.com
Hi FriedParrot! You've successfully authenticated, but GITEE.COM does not provide shell access.

Parrot@DESKTOP-4RUCE8G MINGW64 ~/Desktop
ssh -T git@github.com
Hi FRIEDparrot! You've successfully authenticated, but GitHub does not provide shell access.
```


更新仓库时，只需使用
```Bash
git push
```

仓库初始化
```Bash
mkdir git-tutorial
cd git-tutorial
git init
```

提交代码
```Bash
git commit -m "First commit"
# 查看日志
git log
```


```Bash

git add .  # 添加当前文件夹所有的文件到暂存区
# add -> track the files and changes
git status # show all the files you have modified in this folder
# including what you haven't commited yet
# 对于新修改之后，需要先添加到commit中
git commit -m "Commit Infomation"  # commit的是modified 的内容,
# -m is for message
# also we can 
git commit -m "Introduction" -m "description"

# commit -> save the files in Git
git push -u origin master 
git push -u origin master -f  # 强制 push
# upload Git commits to a remote repo


git pull
# download changes from remote repo
# 简化相应的表达
git log --pretty=short
# 查看某一特定文件日志
cd ....
git log README.md
```

```Bash
# 显示文件的改动
git log -p REAME.md
git diff # 查看工作数和缓存区的差别
git diff HEAD


```

#### 分支部分

```Bash
git branch # 显示分支
git checkout -b feature-A   
# 以master分支为基础 创建名字为future-A的分支

当feature-A分支被激活时，代码就会提交到future-A分支

# 使用
git checkout master  # 还原定位到master分支

git merge --no-ff feature-A # 合并feature-A分支到现有的master分支

git log --graph  # 图表查看日志内容
```

```Bash
git branch -d <分支名> # 删除本地分支
git branch -D <..> # 强制删除

git branch -a # 查看分支的信息

```

### 历史重设部分

```Bash
git reset # 回溯历史版本

git rest --hard <目标节点的哈希值>
git reset --hard fd0cbf0d4a25f747230694d95cac1be72d33441d
```

``` Bash
git remote remove origin
git remote add origin <仓库地址> # 添加目标服务端
git remote rm origin # 移除目标服务端
git remote -v # 查看远程仓库地址信息
git push origin master # 向远程服务端内添加内容

git reflog # 查看当前仓库操作日志
git commit --amend  # 修改提交信息

:wq 保存退出
```

```Bash
git reflog # 查看历史的哈希值

git commit -am "Fix typo"  # 提交后补充信息
```

```Bash
_git commit_ [-a | --interactive | --patch] [-s] [-v] [-u<mode>] [--amend]
	   [--dry-run] [(-c | -C | --squash) <commit> | --fixup [(amend|reword):]<commit>)]
	   [-F <file> | -m <msg>] [--reset-author] [--allow-empty]
	   [--allow-empty-message] [--no-verify] [-e] [--author=<author>]
	   [--date=<date>] [--cleanup=<mode>] [--[no-]status]
	   [-i | -o] [--pathspec-from-file=<file> [--pathspec-file-nul]]
	   [(--trailer <token>[(=|:)<value>])…​] [-S[<keyid>]]
	   [--] [<pathspec>…​]
```

[[https://git-scm.com/docs/git-commit|commit document]]


```Bash
git rebase -i # 压缩历史

# 用上述方式执行 git rebase命令，可以选定当前分支中包含
# HEAD（最新提交）在内的两个最新历史记录为对象，并在编辑器中
# 打开。
```

### Submodule Use methods

```Bash

git submodule add git@github.com:url_to/awesome_submodule.git path_to_awesome_submodule
```

With default configuration, this will check out the **code** of the `awesome_submodule.git` repository to the `path_to_awesome_submodule` directory

After this operation, if you do a `git status` you'll see two files in the `Changes to be committed` list: the `.gitmodules` file and the path to the submodule. When you commit and push these files you commit/push the submodule to the origin.

Then you'll have to initiate them with:
```
git submodule init
```


If you've cloned a repository that makes use of submodules, you should also run this command to get the submodule's code. This is not automatically done by `git clone`. However, if you add the `--recurse-submodules` flag, it will.

[Pushing Updates in the submodule](https://gist.github.com/gitaarik/8735255#pushing-updates-in-the-submodule)

```Bash
git submodule update -init

usage: git submodule [--quiet] [--cached]
   or: git submodule [--quiet] add [-b <branch>] [-f|--force] [--name <name>] [--reference <repository>] [--] <repository> [<path>]
   or: git submodule [--quiet] status [--cached] [--recursive] [--] [<path>...]
   or: git submodule [--quiet] init [--] [<path>...]
   or: git submodule [--quiet] deinit [-f|--force] (--all| [--] <path>...)
   or: git submodule [--quiet] update [--init [--filter=<filter-spec>]] [--remote] [-N|--no-fetch] [-f|--force] [--checkout|--merge|--rebase] [--[no-]recommend-shallow] [--reference <repository>] [--recursive] [--[no-]single-branch] [--] [<path>...]
   or: git submodule [--quiet] set-branch (--default|--branch <branch>) [--] <path>
   or: git submodule [--quiet] set-url [--] <path> <newurl>
   or: git submodule [--quiet] summary [--cached|--files] [--summary-limit <n>] [commit] [--] [<path>...]
   or: git submodule [--quiet] foreach [--recursive] <command>
   or: git submodule [--quiet] sync [--

```

```Bash
start a working area (see also: git help tutorial)
   clone     Clone a repository into a new directory
   init      Create an empty Git repository or reinitialize an existing one

work on the current change (see also: git help everyday)
   add       Add file contents to the index
   mv        Move or rename a file, a directory, or a symlink
   restore   Restore working tree files
   rm        Remove files from the working tree and from the index

examine the history and state (see also: git help revisions)
   bisect    Use binary search to find the commit that introduced a bug
   diff      Show changes between commits, commit and working tree, etc
   grep      Print lines matching a pattern
   log       Show commit logs
   show      Show various types of objects
   status    Show the working tree status

grow, mark and tweak your common history
   branch    List, create, or delete branches
   commit    Record changes to the repository
   merge     Join two or more development histories together
   rebase    Reapply commits on top of another base tip
   reset     Reset current HEAD to the specified state
   switch    Switch branches
   tag       Create, list, delete or verify a tag object signed with GPG

collaborate (see also: git help workflows)
   fetch     Download objects and refs from another repository
   pull      Fetch from and integrate with another repository or a local branch
   push      Update remote refs along with associated objects

'git help -a' and 'git help -g' list available subcommands and some
concept guides. See 'git help <command>' or 'git help <concept>'
to read about a specific subcommand or concept.
See 'git help git' for an overview of the system.

```


```
# 使用链接可以查看master分支7天前后的差别
https://github.com/rails/rails/compare/master@{7.day.ago}...master
```

-  day
- week
- month
- year

**查看与指定日期之间的差别**

```Bash
# 示例网址
https://github.com/rails/rails/compare/master@{2013-01-01}...master
```

[x] 在Github中使用t可以开启搜索,可以通过文件的目录和文件部分名称来搜索文件

![[Pasted image 20221129193539.png|400]]

点击文件的某一行高亮之后
将“#L10”改成“#L10-15”，则会标记该文件的第
10～15 行
```ruby
puts 'Hello World'
```


## Issue 的管理

Issue可以通过添加标签来进行管理，通过添加标签后，可以单独显示带有某个标签的issue

Issue可以通过设置里程碑(Milestone)来对Issue进行管理

![[Pasted image 20221129195350.png|450]]
![[Pasted image 20221129195431.png]]

注：这个是贡献问题的规则
```ruby
%% Tasklist 语法
- [ ] 
- [x]
- [ ] 
```
%%
如果一个处于 Open 状态的 Issue 已经处理完毕，只要在***该提交中***以
下列任意一种格式描述提交信息，对应的 Issue 就会被 Close。
● fix #24
● fixes #24
● fixed #24
● close #24
● closes #24
这个免去再次到github中寻找相关的问题
%%

● 将特定的 Issue 转换为 Pull Request

在 GitHub 上，如果给 Issue 添加源代码，它就会变成我们马上要讲
到的 Pull Request. Issue 与 Pull Request 的编号相互通用，通过 GitHub
的 API 可以将特定的 Issue 转换为 Pull Request，能够完成这一操作的
hub 命令将在本书的 8.1 节中讲解。在这里，各位只要先记住 Issue 与
Pull Request 的编号通用即可。

### 5.7　Pull request 的创建

这个是进行自己修改源码并申请提交基本步骤: 

```Bash
# 1. fork别人的repository 到自己的repository
# 2. clone自己的repository 到自己的电脑

# 3. 添加别人仓库的远程链接（在使用pull request时
git remote add <远程链接名称> <git 仓库地址>

# 4. 将文件修改后提交到自己fork过的repository中
# 5. 在自己的repository上点击pull requests并进入comparing changes界面,进行create pull request

# 此时另一端就会有pull requests的显示,另一端将对应的分支merge入自己的分支即可

```

```Ruby
获取 diff 格式与 patch 格式的文件
对长期投身于软件开发的人来说，有时可能会希望以 diff 格
式文件和 patch 格式文件的形式来处理 Pull Request。
举个例子，假设 Pull Request 的 URL 如下所示。
　　 https://github.com/用户名/仓库名/pull/28
如果想获取 diff 格式的文件，只要像下面这样在 URL 末尾
添加 .diff 即可。
　　 https://github.com/用户名/仓库名/pull/28.diff
同 理， 想 要 patch 格 式 的 文 件， 只 需 要 在 URL 末 尾 添
加 .patch 即可。
```


-  Conversation
可以查看与当前 Pull Request 相关的所
有评论以及提交的历史记录。人们在这里添加评论互相探讨，发送提交
落实讨论内容的整个过程会按时间顺序列出


```Ruby
# 引用评论
在 Conversation 中人们通过添加评论进行对话。这里有一
个简单方法可以帮您引用某个人的评论。选中想引用的评论然
后按 R 键，被选择的部分就会自动以评论语法写入评论文本框
（图 a）。
```

- Commits
Commits 标签页中，按时间顺序列表显示了与当前 Pull Request
相关的提交（图 5.21）。标签上的数字为提交的次数。每个提交右侧的
哈希值可以连接到该提交的代码

在评论中输入“:”（冒号）便会启动表情自动补全功能
[emoji](http://www.emoji-cheat-sheet.com/ 查)

在行号左侧的加号可以插入评论


## Wiki 的使用

Wiki也支持GFM语法
使用New Page创建

通过clone可以获取wiki仓库，本地创建并编辑页面，提交之后再进行push,完成对wiki的创建与编辑

- 在 Wiki 中显示侧边栏
所有 Wiki 页面都可以显示侧边栏。做法很简单，只要创建名为`_sidebar`的页面即可
此时在编辑各页面时页面下部会附加 Sidebar 段

#### 5.9 Pulse: 
体现该仓库软件开发活跃度的功能（图 5.26）。近期该仓库
创建了多少 Pull Request 或 Issue，有多少人参与了这个仓库的开发等

#### 5.10 Graphs

