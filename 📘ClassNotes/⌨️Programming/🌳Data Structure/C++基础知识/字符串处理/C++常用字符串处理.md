#### (1)memset 设置内存和清空字符串
参考: https://cplusplus.com/reference/cstring
清空字符串
```cpp 
memset(str1, 0, strlen(str1));
// 其中0表示设置为 \0,  strlen 取字符串长度
```


#### (2) 比较和复制部分字符串(strncmp和strncpy)





#### (3) strtok() : 按分隔符分割字符串
On a first call, the function expects a C string as argument for _str_, whose first character is used as the starting location to scan for tokens. In subsequent calls, the function expects a null pointer and uses the position right after the end of the last token as the new starting location for scanning.

第一次调用时, 首先给定需要扫描的字符串, 则自动将指针位置设置到起始部分, 而之后只需要传入一个空指针, 则strtok 会从上一次扫描的位置进行继续扫描, 得到下一个
```cpp
/* strtok example */
#include <stdio.h>
#include <string.h>

int main ()
{
  char str[] ="- This, a sample string.";
  char * pch;
  printf ("Splitting string \"%s\" into tokens:\n",str);
  pch = strtok (str," ,.-");
  while (pch != NULL)
  {
    printf ("%s\n",pch);
    pch = strtok (NULL, " ,.-");
  }
  return 0;
}
/* Output:
Splitting string "- This, a sample string." into tokens:
This
a
sample
string  */
```

