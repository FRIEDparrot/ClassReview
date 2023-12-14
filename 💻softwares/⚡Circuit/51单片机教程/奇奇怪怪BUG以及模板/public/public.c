#include "public.h"
#include <REGX52.H>
#include <INTRINS.H>

void Delay10us(u16 xus){ // @11.0592MHz 
    while (xus--); 
}

void Delay(u16 xms) // @11.0592MHz 
{
	while (xms > 0){
		unsigned char data i, j;
		_nop_();
		for (i = 2; i>0;i--){
			for (j = 199; j> 0; j--);
		}
		xms--;
	}
}
