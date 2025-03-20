
> [!NOTE] 项目用
> `sphinx-apidoc -o ./docs/source/ `
> `./docs make html`

Sphinx : 
- `Sphinx` 是一个文档生成工具，能够将 Python 的 docstring 和额外的文档文件转换为 HTML、PDF 等多种格式。
- 它支持自动提取代码中的 docstring，并格式化成漂亮的文档。
- `scikit-learn` 使用了 Sphinx 及其插件（如 `numpydoc` 和 `sphinx-gallery`）。

此外, 除了 Sphinx，还有许多其他强大的文档生成工具，可以根据项目需求选择。以下是一些流行的工具及其案例:

**1. MkDocs**
- **特点**:
    - 专为静态网站生成文档设计，简单易用。
    - 使用 Markdown 编写文档，适合快速上手。
    - 配合 `mkdocs-material` 主题，可以生成美观的文档。

## **Sphinx 生成 Python 文档完整教程**
Sphinx 是一个强大的文档生成工具，广泛用于 Python 项目，如 Django、Flask 和 NumPy 等。以下是使用 Sphinx 生成 Python 文档的完整流程，包括安装、配置、编写文档、生成 HTML/PDF 文档等。
## **1. 安装 Sphinx**

Sphinx 需要 Python 环境，首先安装它：

```bash
pip install sphinx
```

如果你要支持 Markdown 文件，还需要安装 `myst-parser`：

```bash
pip install myst-parser
```

## **2. 创建文档目录**
建议在你的 Python 项目的根目录下创建 `docs` 目录：

```bash
mkdir docs
cd docs
sphinx-quickstart
```

然后按照提示配置：

- **Separate source and build directories?** (Y) → 选择 `Yes`，将 `source` 和 `build` 分开
- **Project name** → 输入你的项目名称
- **Author name** → 输入你的名字
- **Project release** → 输入版本号（如 `1.0`）
- **Docstring format** → 建议选择 `reStructuredText`，但如果你用 `Google-style` 或 `NumPy-style`，可以安装 `sphinx.ext.napoleon`

## **3. 配置 `conf.py`**

Sphinx 生成的 `docs/source/conf.py` 是文档配置文件，修改它以启用一些扩展：

```python
import os
import sys
sys.path.insert(0, os.path.abspath('../'))  # 让 Sphinx 可以找到 Python 代码

extensions = [
    'sphinx.ext.autodoc',       # 自动从 docstring 生成文档
    'sphinx.ext.napoleon',      # 支持 Google 和 NumPy 风格 docstring
    'sphinx.ext.viewcode',      # 在 HTML 页面显示源代码
    'myst_parser',              # 允许解析 Markdown 文件
]

napoleon_google_docstring = True  # 允许 Google 风格
napoleon_numpy_docstring = True   # 允许 NumPy 风格

html_theme = "alabaster"  # 主题，可改为 "sphinx_rtd_theme" 等
```

**如果你用 Markdown 而不是 reST (`.rst`)，请确保 `myst_parser` 在 `extensions` 里。**

主题安装: 
```sh
pip install sphinx_rtd_theme
#  然后 extensions 加  "sphinx_rtd_theme",
```

## **4. 自动生成 API 文档**

Sphinx 可以从 Python 源代码自动提取 `docstrings` 生成 API 文档。

**在 `docs/source` 目录下创建 `index.rst`**，并修改如下：

```rst
.. MyProject documentation master file, created by
   sphinx-quickstart on Thu Jan 1 00:00:00 2025.

Welcome to MyProject's documentation!
=====================================

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   modules

API Documentation
=================
.. automodule:: my_module
   :members:
   :undoc-members:
   :show-inheritance:
```

然后运行以下命令，自动生成 API 文档 (这个是指定 project 路径和 `/docs` 的相对路径)
```bash
sphinx-apidoc -o source ../my_project   # 我们认为 docs 和 my_project  在同一目录下 
``` 

这会在 `source/` 目录下生成 `modules.rst` 等文件。然后直接采用 : 
```bash
make html
```

然后在 `docs/build/html/index.html` 里查看生成的网页。

## **6. 生成 PDF 文档**

首先安装 LaTeX 相关工具：

```bash
sudo apt install texlive-latex-base texlive-fonts-recommended texlive-latex-extra
```

然后执行：

```bash
make latexpdf
```

PDF 文件会生成在 `docs/build/latex/` 里。

---

## **7. 扩展功能**

### **支持 Google 和 NumPy Docstrings**

确保 `sphinx.ext.napoleon` 在 `conf.py` 里：

```python
extensions = ['sphinx.ext.napoleon']
```

然后 Sphinx 允许你使用：

#### **Google-style docstrings**

```python
def add(a: int, b: int) -> int:
    """
    Adds two numbers.

    Args:
        a (int): The first number.
        b (int): The second number.

    Returns:
        int: The sum of the two numbers.
    """
    return a + b
```

#### **NumPy-style docstrings**

```python
def add(a, b):
    """
    Adds two numbers.

    Parameters
    ----------
    a : int
        The first number.
    b : int
        The second number.

    Returns
    -------
    int
        The sum of the two numbers.
    """
    return a + b
```

---

## **8. 发布文档**

如果你想在 [Read the Docs](https://readthedocs.org/) 发布：

1. 在 GitHub 上托管代码
2. 在 `docs/` 目录下添加 `requirements.txt`：
    
    ```
    sphinx
    sphinx-rtd-theme
    myst-parser
    ```
    
3. 在 Read the Docs 创建项目，链接 GitHub 仓库，Read the Docs 会自动构建文档。

---

## **总结**

1. **安装 Sphinx** (`pip install sphinx myst-parser`)
2. **初始化** (`sphinx-quickstart`)
3. **修改 `conf.py`** 启用 `autodoc` 和 `napoleon`
4. **编写 `.rst` 或 `.md` 文档**
5. **自动生成 API 文档** (`sphinx-apidoc -o source ../my_project`)
6. **生成 HTML/PDF** (`make html` / `make latexpdf`)
7. **发布到 Read the Docs**

这就是完整的 Sphinx 文档生成流程 🚀。如果有具体问题，可以继续问！

## 二、 添加对 Flask  +  SqlAlchemy 项目的支持 

在 Sphinx 文档中自动解析 Flask 的路由 (`routes`) 和 SQLAlchemy 的模型 (`models`) 需要一些额外的配置和工具支持。默认情况下，Sphinx 的 `autodoc` 扩展无法直接解析 Flask 的路由和 SQLAlchemy 的模型，因为这些内容通常是通过动态方式生成的，而不是静态定义的。

以下是解决这个问题的步骤和方法：

---

### 1. **确保 Sphinx 配置正确**
在 `docs/conf.py` 文件中，确保启用了 `autodoc` 扩展，并添加了 Flask 和 SQLAlchemy 的相关配置。

```python
# conf.py

import os
import sys
sys.path.insert(0, os.path.abspath('..'))  # 将项目根目录添加到 Python 路径

extensions = [
    'sphinx.ext.autodoc',       # 启用 autodoc
    'sphinx.ext.viewcode',      # 启用源代码链接
    'sphinx.ext.napoleon',      # 支持 Google 和 NumPy 风格的文档字符串
]

# 如果需要支持 SQLAlchemy
autodoc_default_options = {
    'members': True,
    'undoc-members': True,
    'show-inheritance': True,
    'special-members': '__init__',
}
```

---

### 2. **解析 Flask 的路由**
Flask 的路由是通过装饰器动态生成的，因此默认情况下 Sphinx 无法直接解析它们。你可以使用以下方法将路由信息添加到文档中：

#### 方法 1：手动编写路由文档
在 `index.rst` 或单独的路由文档文件中，手动列出所有路由及其描述。例如：

```rst
Flask Routes
============

.. http:get:: /api/v1/users
   :synopsis: Get a list of users.

   This route returns a list of all users.

.. http:post:: /api/v1/users
   :synopsis: Create a new user.

   This route creates a new user.
```

#### 方法 2：使用 `sphinxcontrib-httpdomain` 扩展
安装 `sphinxcontrib-httpdomain` 扩展，它可以更好地支持 HTTP API 文档。

1. 安装扩展：

   ```bash
   pip install sphinxcontrib-httpdomain
   ```

2. 在 `conf.py` 中启用扩展：

   ```python
   extensions = [
       'sphinxcontrib.httpdomain',  # 启用 HTTP 域支持
   ]
   ```

3. 在文档中使用 `http` 指令描述路由：

   ```rst
   .. http:get:: /api/v1/users
      :synopsis: Get a list of users.

      This route returns a list of all users.
   ```

#### 方法 3：自动生成路由文档
你可以编写一个脚本，从 Flask 应用中提取路由信息并生成 `.rst` 文件。以下是一个示例脚本：

```python
from flask import Flask
import inspect

app = Flask(__name__)

def generate_routes_docs(app, filename='routes.rst'):
    with open(filename, 'w') as f:
        f.write("Flask Routes\n")
        f.write("============\n\n")
        for rule in app.url_map.iter_rules():
            f.write(f".. http:get:: {rule}\n")
            f.write(f"   :synopsis: {rule.endpoint}\n\n")
            f.write(f"   This route handles {rule.endpoint}.\n\n")

# 生成路由文档
generate_routes_docs(app)
```

运行此脚本后，会生成一个 `routes.rst` 文件，你可以在 `index.rst` 中包含它：

```rst
.. toctree::
   :maxdepth: 2

   routes
```

---

### 3. **解析 SQLAlchemy 模型**
SQLAlchemy 的模型类通常是通过 `declarative_base` 动态生成的，因此 Sphinx 的 `autodoc` 可能无法直接解析它们。以下是解决方法：

#### 方法 1：确保模型类被导入
在 `conf.py` 中，确保 SQLAlchemy 模型类被正确导入。例如：

```python
# conf.py

import os
import sys
sys.path.insert(0, os.path.abspath('..'))

# 导入 SQLAlchemy 模型
from my_package.models import User, Post  # 替换为你的模型类
```

#### 方法 2：使用 `sphinx-autodoc` 手动编写模型文档
在 `index.rst` 或单独的模型文档文件中，手动编写模型文档：

```rst
SQLAlchemy Models
=================

User Model
----------

.. autoclass:: my_package.models.User
   :members:
   :undoc-members:
   :show-inheritance:

Post Model
----------

.. autoclass:: my_package.models.Post
   :members:
   :undoc-members:
   :show-inheritance:
```

#### 方法 3：使用 `sphinx-autodoc` 自动生成模型文档
你可以编写一个脚本，自动生成 SQLAlchemy 模型的文档。以下是一个示例脚本：

```python
from my_package.models import Base
import inspect

def generate_models_docs(models, filename='models.rst'):
    with open(filename, 'w') as f:
        f.write("SQLAlchemy Models\n")
        f.write("=================\n\n")
        for model in models:
            f.write(f"{model.__name__} Model\n")
            f.write("-" * (len(model.__name__) + "--------\n\n")
            f.write(f".. autoclass:: {model.__module__}.{model.__name__}\n")
            f.write("   :members:\n")
            f.write("   :undoc-members:\n")
            f.write("   :show-inheritance:\n\n")

# 生成模型文档
generate_models_docs([User, Post])  # 替换为你的模型类
```

运行此脚本后，会生成一个 `models.rst` 文件，你可以在 `index.rst` 中包含它：

```rst
.. toctree::
   :maxdepth: 2

   models
```

---

### 4. **重新生成文档**
完成上述配置后，运行以下命令重新生成文档：

```bash
make clean
make html
```

生成的文档将包含 Flask 的路由和 SQLAlchemy 的模型信息。

---

### 总结
- 对于 Flask 路由，可以使用 `sphinxcontrib-httpdomain` 扩展或编写脚本自动生成路由文档。
- 对于 SQLAlchemy 模型，确保模型类被正确导入，并使用 `autoclass` 指令生成文档。
- 通过编写脚本，可以自动化生成路由和模型的文档，减少手动工作量。

希望这些方法能帮助你成功将 Flask 路由和 SQLAlchemy 模型集成到 Sphinx 文档中！ 