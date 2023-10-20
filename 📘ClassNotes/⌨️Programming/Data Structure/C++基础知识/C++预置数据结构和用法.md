常用的数据结构有 : 
```cpp 
array  // 数组
vector // 向量
list   // 链表
queue  // 队列
stack  // 栈
map    // 图
set    // 集合
```

#### (1) Array使用
[array的使用方法](https://blog.csdn.net/weixin_45826022/article/details/102893386?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522168005856216800182131404%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=168005856216800182131404&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-102893386-null-null.142^v76^insert_down38,201^v4^add_ask,239^v2^insert_chatgpt&utm_term=C%2B%2B%20array&spm=1018.2226.3001.4187)
`array<类型名, 元素个数> 数组名;`

![[attachments/Pasted image 20230329110103.png|600]]
`[]` 与`at()`的区别在于，`[]`不会进行检查数字是否越界，而 `at()` 会进行检查（时间开销很少），如果越界则抛出 `std::out_of_rang` 异常。  
因此，除非确定访问没有越界，否则应该尽量使用更安全的 `at()` 。

其中迭代器的表示为：
![[attachments/Pasted image 20230329110439.png|600]]

[cbegin, cend使用](https://blog.csdn.net/u013271656/article/details/113560304?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522168005916416800215073054%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=168005916416800215073054&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~baidu_landing_v2~default-1-113560304-null-null.142^v76^insert_down38,201^v4^add_ask,239^v2^insert_chatgpt&utm_term=C%2B%2B%20crbegin&spm=1018.2226.3001.4187)


#### (2) List使用方法
[list使用方法](https://blog.csdn.net/u_nravel/article/details/109674797)
