这与在 Keil 中混用 C 和 C++ 有关。

首先，Keil 工程同时包含 C 文件和 CPP 文件时，C 文件中的函数在 CPP 文件中是无法直接访问的。因此，即使你已经将源文件添加到工程中，也会出现未定义错误。解决这个问题的方法是将 `#include "esp8266_driver.h"` 和 `#include "esp8266_command_func.h"` 改为以下形式：

```c
#ifdef __cplusplus
extern "C" {
#endif

#include "esp8266_driver.h"
#include "esp8266_command_func.h"

#ifdef __cplusplus
}
#endif
```

[这样，C++ 文件就能正确访问 C 文件中的函数了。重新编译一下，应该就不会再出现 `Error: L6218E: Undefined symbol` 错误了](https://blog.csdn.net/zhizuwei/article/details/86017618)[1](https://blog.csdn.net/zhizuwei/article/details/86017618)[2](https://devpress.csdn.net/v1/article/detail/86017618)。
