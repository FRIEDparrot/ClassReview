#include "stm32f10x.h"
#include "Float_to_string.h"
#include "math.h"

/*功   能： 浮点数保留小数位数四舍五入*/
/*输入参数: 浮点数指针 ， 小数位数*/
void PrecisionHandle(float *value,unsigned char Wrange)
{
	int sign = 1;
	if(*value < 0)
	{
		sign = -1;
	}
 
	*value += ((sign * pow((float)10, (float)((-1) * (Wrange + 1)))) * 5);
}

/*功   能：浮点数转换成字符串*/
/*输入参数:浮点数 ，输出整数位数，小数位数，输出字符串指针，字符串长度*/
/*返回值  ：-1 错误	0 正常*/
signed char FloatToString(float value,UINT8 int_width,UINT8 Wrange,UINT8 * data,UINT8 len)
{
    signed char result = 0;
    signed char sign = 1;
    UINT8 i = 0;
    int int_value = 0;
	if(value < 0)
	{
		sign = -1;
	}
	
    if(Wrange == 0)
    {
       if( (int_width + 1) > len || int_width == 0)
       {
         result = -1;
       }
       int_value = (int)(sign * value );
       if(sign == -1)
       {
           for(i = int_width;i > 0 ;i--)
           {
                data[i] = (int_value % 10) + '0';
                int_value = int_value / 10;
           }
           data[0] = '-';
       }
       else
       {
           for(i = (int_width-1);i > 0 ;i--)
           {
                data[i] = (int_value % 10) + '0';
                int_value = int_value / 10;
           }
           data[0] = (int_value % 10) + '0';
       }          
    }
    else
    {
        if( (int_width + Wrange +1) > len || int_width == 0)  
        {
            result = -1;
        }
        PrecisionHandle(&value,Wrange);
        int_value = (int)(sign * value * (pow((float)10,(float)Wrange)));
        if(sign == -1)
        {
            for(i = (int_width +Wrange +1);i > 0 ;i--)
            {
                if(i == int_width + 1)
                {
                    data[i] = '.'; 
                    continue;
                }
                data[i] = (int_value % 10) + '0';
                int_value = int_value / 10;  
            }
            data[0] = '-';
        }
        else
        {
           for(i = (int_width +Wrange);i > 0 ;i--)
           {  
              if(i == int_width)
              {
                data[i] = '.'; 
                continue;
              }
              data[i] = (int_value % 10) + '0';
              int_value = int_value / 10;
           }
           data[0] = (int_value % 10) + '0';
        }                      
    }
    
    return result;
}
