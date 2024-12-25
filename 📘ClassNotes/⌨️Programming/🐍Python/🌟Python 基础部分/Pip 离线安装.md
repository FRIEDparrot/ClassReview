要在离线环境中通过 `.whl` 文件安装 Python 包，并确保 Python 自动从一个指定的文件夹中查找和安装缺失的依赖包，可以采取以下步骤：

### 1. 将 `.whl` 包放在一个文件夹中

首先，确保你已经把所有需要的 `.whl` 包下载到一个文件夹中，比如 `whl_folder`。

### 2. 安装 `.whl` 包

如果你已经有所有需要的 `.whl` 包，可以通过以下命令手动安装它们：

```bash
pip install --no-index --find-links=path/to/whl_folder package_name
```

- `--no-index`: 禁用从 PyPI 或其他远程索引服务器下载包。
- `--find-links=path/to/whl_folder`: 指定 `.whl` 包所在的文件夹路径，`pip` 会在该路径下查找包。

### 3. 自动安装依赖

如果你希望 Python 自动从文件夹中查找并安装缺失的依赖，可以使用以下方法：

#### 3.1 使用 `requirements.txt` 文件

你可以创建一个 `requirements.txt` 文件，其中列出你需要的所有包和版本，然后使用以下命令批量安装：

1. 创建 `requirements.txt`，并在其中列出包名及其版本（如果已知）。

例如，`requirements.txt` 内容如下：

```txt
numpy==1.21.2
scipy==1.7.0
pandas==1.3.3
```

2. 使用 `pip` 安装：

```bash
pip install --no-index --find-links=path/to/whl_folder -r requirements.txt
```

#### 3.2 自动安装依赖的 `.whl` 包

如果没有 `requirements.txt` 文件，你可以使用脚本遍历 `.whl` 文件夹，手动安装每个包。这里是一个简单的 Python 脚本：

```python
import os
import subprocess

# 设置存放 .whl 包的文件夹路径
whl_folder = 'path/to/whl_folder'

# 获取该文件夹中的所有 .whl 文件
whl_files = [f for f in os.listdir(whl_folder) if f.endswith('.whl')]

# 遍历 .whl 文件并安装
for whl_file in whl_files:
    whl_path = os.path.join(whl_folder, whl_file)
    subprocess.run(['pip', 'install', '--no-index', '--find-links', whl_folder, whl_path])
```

此脚本会安装文件夹中所有的 `.whl` 包，并且不会从 PyPI 下载任何依赖。

### 4. 使用 `pip` 进行依赖解析

如果某个 `.whl` 包依赖其他包，而你已经将所有的 `.whl` 包都放在文件夹中，`pip` 会根据依赖关系自动从文件夹中查找并安装其他缺失的包。只要依赖包也位于该文件夹中，并且命名符合规范（即对应的 `.whl` 文件存在），`pip` 会顺利安装。

通过以上方式，即便电脑处于离线状态，你也可以使用本地的 `.whl` 文件安装 Python 包及其依赖。



torch gpu 配置方法 : 
```python
pip download torch==2.4.1 torchvision==0.19.1 --index-url https://download.pytorch.org/whl/cu121
```
