```c
// 正确
void main(){
    int ch = 0;
    P0 = 0xff;   // reset the LED_port
}

// 报错 "syntax error near 'int', expected '__asm'",
void main(){
    P0 = 0xff;   // reset the LED_port
    int ch = 0;
}
```


注意c文件 一定要放在source Group下，不然会有
WARNING : REFERENCE MADE TO UNRESOLVED EXTERNAL 报错
![[attachments/Pasted image 20231211201138.png]]