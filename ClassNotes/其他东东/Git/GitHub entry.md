[[GitHub入门与实践.pdf]]

[[Git使用教程.pdf]]

1. [Proxy configuration](https://githubtraining.github.io/training-manual/#/01_getting_ready_for_class?id=proxy-configuration)

- Git proxy
If the organization uses a proxy, we would need to configure the proxy settings in Git by typing the following command in the Git Bash:
```Bash
git config --global http.proxy https://YOUR.PROXY.SERVER:8080
```
where YOUR.PROXY.SERVER is the url of the proxy 

```Bash
git config --global http.proxy https://YOUR_PROXY_USERNAME:YOUR_PROXY_PASSWORD@YOUR.PROXY.SERVER:8080
```

You can use almost any text editor, but we have the best success with the following:

-   [Atom](https://atom.io/)
-   [Visual Studio Code](https://code.visualstudio.com/)
-   Notepad
-   Vi or Vim
-   Sublime
-   Notepad++
-   GitPad

For more information on integrations, check out [https://github.com/integrations](https://github.com/integrations).

	Git takes a snapshot of your repository at a given point in time. This snapshot is called a commit.
	When you clone a copy of a repository to your local machine, you receive a copy of the entire repository and its history.

#### Branches

	When you create a new branch in Git, you are actually just creating a pointer that corresponds to the most recent commit in a line of work. Git keeps the commits for each branch separate until you explicitly tell it to merge those commits into the main line of work.

### Repository

>A repository is the most basic element of GitHub,unlike an ordinary folder on your laptop, a GitHub repository offers simple yet powerful tools for collaborating with others.
>
> A repository contains all the project files (including documentation) and ==stores each file's revision history==.

[Repository navigation](https://githubtraining.github.io/training-manual/#/02_getting_started?id=repository-navigation)

Issues can be assigned to specific team members and are designed to encourage discussion and collaboration.

#### [Pull requests](https://githubtraining.github.io/training-manual/#/02_getting_started?id=pull-requests)

A Pull Request represents a change, such as adding, modifying, or deleting files, which the author would like to make to the repository. ==Pull Requests help you write better software by facilitating code review and showing the status of any automated tests==.

[Using GitHub Issues](https://githubtraining.github.io/training-manual/#/02_getting_started?id=using-github-issues)

[Projects](https://githubtraining.github.io/training-manual/#/02_getting_started?id=projects)

[Wiki](https://githubtraining.github.io/training-manual/#/02_getting_started?id=wiki)

### Necessary Introduction Files
#### 1. README.md
	The README.md is a special file that we recommend all repositories contain. GitHub looks for this file and helpfully displays it below the repository.

#### 2.CONTRIBUTING.md
	used to describe the process for collaborating on the repository.

#### 3.ISSUE_TEMPLATE.md
	The ISSUE_TEMPLATE.md (and its twin the pull request template) are used to generate templated starter text for your project issues. Any time someone opens an issue, the content in the template will be pre-populated in the issue body.


[How to create a GitHub issue](https://githubtraining.github.io/training-manual/#/02_getting_started?id=activity-creating-a-github-issue)

```
YOUR-USERNAME will update a file by replacing a caption.

- [ ] Create a branch
- [ ] Edit the file
- [ ] Commit the changes
- [ ] Create a Pull Request
- [ ] Request a Review
- [ ] Make more changes
- [ ] Get an approval
- [ ] Merge the Pull Request
```

@mention
When you @mention someone in an issue, they will receive a notification - even if they are not currently subscribed to the issue or watching the repository.

if you mentioned the name of the organization, then all the members of the organization will receive a notification.

A `#` followed by the number of an issue or ==pull request (without a space) in the same repository will create a cross-link==.


. By default, the "blessed" or "canonical" version of your project lives on a branch called `main`. This branch can actually be named anything,

每一个功能更改或修正都对应一个 Issue，讨论或修正都以这个 Issue 为中心进行。只要查看 Issue，就能知道和这个更改相关的一切信 息，并以此进行管理

Git 的提交信息中写上 Issue 的 ID（例如“#7”），GitHub 就会自 动生成从 Issue 到对应提交的链接。

### GitHub中的公开许可协议：

在GitHub上，有修正BSD许可协议，Apache许可协议，多选用MIT许可协议
实 际 使 用 时， 只 需 将 LICENSE 文 件 加 入 仓 库， 并 在
README.md 文件中声明使用了何种许可协议即可。

## GitHub功能详解

GitHub 有相应的快捷键 可以使用 shift + / 查看