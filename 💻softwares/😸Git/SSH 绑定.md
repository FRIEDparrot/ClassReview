要将自己的电脑 SSH 添加到 GitHub 账号，通常需要完成以下几个步骤：

1. **确认 SSH 密钥文件**：确保你已经生成了 SSH 密钥，并且知道其路径。默认情况下，密钥文件通常位于 `~/.ssh/` 目录下，文件名可能是 `id_rsa` 和 `id_rsa.pub`。

2. **添加 SSH 密钥到 SSH 代理**：
   先确认 SSH 代理是否正在运行，可以使用以下命令启动 SSH 代理：

   ```bash
   eval "$(ssh-agent -s)"
   ```

   然后使用以下命令添加你的 SSH 私钥（你提到的 `github_id_rsa`）：

   ```bash
   ssh-add ~/.ssh/github_id_rsa
   ```

   如果这一步成功，你应该没有看到错误提示。

3. **复制公钥**：接下来，你需要将公钥添加到 GitHub。首先，复制你的 SSH 公钥，可以用以下命令：

   ```bash
   cat ~/.ssh/github_id_rsa.pub
   ```

   将输出的内容复制到剪贴板。

4. **在 GitHub 上添加 SSH 密钥**：
   - 登录你的 GitHub 账号。
   - 点击右上角的头像，选择 **Settings**。
   - 在左侧菜单中选择 **SSH and GPG keys**。
   - 点击 **New SSH key** 或 **Add SSH key** 按钮。
   - 在 **Title** 一栏填上一个名称（可以是你的电脑名称），然后在 **Key** 一栏粘贴你之前复制的公钥。
   - 点击 **Add SSH key**。

5. **测试 SSH 连接**：完成以上步骤后，可以用以下命令测试 SSH 连接是否成功：

   ```bash
   ssh -T git@github.com
   ```

   如果一切设置正确，你应该会看到类似以下的信息：

   ```
   Hi username! You've successfully authenticated, but GitHub does not provide shell access.
   ```

如果你在操作过程中遇到任何问题，欢迎随时询问！
