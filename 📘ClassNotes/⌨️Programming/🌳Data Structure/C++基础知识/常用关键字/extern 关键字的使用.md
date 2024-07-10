例如, 在 SDCard.h 中，提供了 `_sd_combine_dir` 等等几个函数, 但是我们希望在包含 user_sd_func.h 时, 不包含以 SDCard__ 开头的一堆函数, 但是包含共享的几个字符串处理函数，这时即可在头文件中利用 extern 进行预声明(实际默认就是extern)。 

![[attachments/Pasted image 20240627170359.png]]

此时只要包含 user_sd_func.h 即可包含后面定义的几个函数。
