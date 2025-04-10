libHSL package is the ultimate sparse linear algebra collection for large-scale scientific computing. It contains more than 160 HSL packages and aims to facilitate the use of HSL in Julia, Fortran, and C. 

具体从 https://licences.stfc.ac.uk/product/libhsl-2024_11_28 获取 

文档部分参考:  https://www.hsl.rl.ac.uk/archive/index.html  


首先获取预编译的 windows 版本的 `_binaries`  libhsl 部分(不需要采用源码用 menson 编译), 然后只需将 bin (动态链接部分) 添加到系统 PATH 中即可。本机的安装路径如下:

```sh
E:\libHSL\bin 
```


• LD_LIBRARY_PATH on Linux; 
• DYLD_LIBRARY_PATH or DYLD_FALLBACK_LIBRARY_PATH on macOS;
• PATH on Windows.  

首先需要保证编译平台正确， **采用 Debug, x64 平台**， 

对于一个例如采用: 
![[attachments/Pasted image 20250329123006.png|350]]
选中项目，点击右上角扳手，修改 include directories, 然后包含 libHSL 的 include 路径 , 注意平台一定需要采用 x64 并且匹配: 添加 include 到 general 部分: 
![[attachments/Pasted image 20250329134855.png]]


找到 Linker 下的  Additional Library Directories， 添加 lib 路径
![[attachments/Pasted image 20250329134947.png|550]]

然后在 Input > additional dependencies 中添加 libhsl.dll.a 部分(不需要添加.a，由于采用的是动态构建, .a 则需要完整的, 较大): 
![[attachments/Pasted image 20250329135212.png|550]]

主要3个步骤: 
1. **附加包含目录**：  
    `E:\libHSL\include`    
2. **附加库目录**：  
    `E:\libHSL\lib`
3. **附加依赖项**：  
    `libhsl.dll.a` 


## 二、 简单测试用例 

首先在 hsi_mi20d.h 中, 我们需要手动定义一下缺失的 `_Bool` : 
```cpp
typedef unsigned char _Bool;  /* C99 _Bool type */ 
```

此时运行程序会出现以下问题:  
![[attachments/Pasted image 20250329144817.png]]
此时我们需要添加 bin 目录到 Additional Library Directories 中，然后即可找到对应的  libhsl.dll. 重新运行报错如下 : 
![[attachments/Pasted image 20250329150715.png]]  

需要下载:  [fortran 运行时库](https://www.intel.com/content/www/us/en/developer/articles/tool/compilers-redistributable-libraries-by-version.html) 即 Intel® Fortran Compiler Runtime for Windows* (IFX/IFORT) , 这样就能找到给的 libgfortran-5.dll 了. 如果再找不到重新添加下 bin 路径到对应设置中. 

采用如下的代码进行测试 : 
```cpp 
#include <iostream>

using namespace std;

extern "C" {
#include "hsl_mi20d.h"
}

int main() {
    // 初始化控制结构和信息结构
    struct mi20_control_d control;
    struct mi20_info_d info;
    mi20_default_control_d(&control);

    // 设置控制参数
    control.print_level = 1;  // 输出基本信息
    control.error = 6;        // 错误输出到stderr
    control.print = 6;        // 常规输出到stdout

    // 示例矩阵 (CSR格式)
    const int n = 3;
    int rowptr[] = { 0, 2, 4, 5 };  // 调整指针
    int colind[] = { 0, 1, 0, 1, 2 };
    double val[] = { 1.0, -2.0, -3.0, 4.0, 5.0 };  // 确保有负的非对角元素

    // 创建预处理数据
    void* keep = nullptr;
    cout << "Setting up preconditioner..." << endl;
    mi20_setup_csr_d(n, rowptr, colind, val, &keep, &control, &info);

    if (info.flag != 0) {
        cerr << "Error in mi20_setup_csr_d, flag = " << info.flag << endl;
        return 1;
    }

    // 测试向量
    double rhs[] = { 1.0, 1.0, 1.0 };  // 右侧向量
    double sol[] = { 0.0, 0.0, 0.0 };  // 解向量

    // 应用预处理
    cout << "Applying preconditioner..." << endl;
    mi20_precondition_d(rhs, sol, &keep, &control, &info);

    if (info.flag != 0) {
        cerr << "Error in mi20_precondition_d, flag = " << info.flag << endl;
        return 1;
    }

    // 输出预处理结果
    cout << "Preconditioned solution:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "sol[" << i << "] = " << sol[i] << endl;
    }

    // 清理
    cout << "Cleaning up..." << endl;
    mi20_finalize_d(&keep, &control, &info);

    return 0;
}
```

输出结果如下: 

```cpp 
Setting up preconditioner...
Applying preconditioner...
Preconditioned solution:
sol[0] = -15.6562
sol[1] = -11.4922
sol[2] = 0.2
Cleaning up...
```


