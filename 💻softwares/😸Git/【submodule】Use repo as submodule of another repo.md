将一个仓库作为子仓库放在另外一个大仓库中, 可以采用 submodule 模块。

```git
git submodule add https://github.com/FRIEDparrot/GBK-code-table code_table_resources  # 后面的是 path/to/submodule
```

子模块的初始化和更新如下:
```git
git submodule update --init --recursive  # 初始化和更新仓库的子模块

git add .gitmodules code_table_resources  # 将某个路径作为模块添加到仓库中 
git commit -m "commit comment" 
git push 
```
