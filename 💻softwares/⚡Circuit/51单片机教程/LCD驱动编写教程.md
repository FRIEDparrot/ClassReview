
```c
#if (LCD1602_4OR8_DATA_INTERFACE==0)//8位LCD
void lcd1602_init(void)
{
	lcd1602_write_cmd(0x38);//数据总线8位，显示2行，5*7点阵/字符
	lcd1602_write_cmd(0x0c);//显示功能开，无光标，光标闪烁
	lcd1602_write_cmd(0x06);//写入新数据后光标右移，显示屏不移动
	lcd1602_write_cmd(0x01);//清屏	
}
#else
void lcd1602_init(void)
{
	lcd1602_write_cmd(0x28);//数据总线4位，显示2行，5*7点阵/字符
	lcd1602_write_cmd(0x0c);//显示功能开，无光标，光标闪烁
	lcd1602_write_cmd(0x06);//写入新数据后光标右移，显示屏不移动
	lcd1602_write_cmd(0x01);//清屏	
}
#endif
```