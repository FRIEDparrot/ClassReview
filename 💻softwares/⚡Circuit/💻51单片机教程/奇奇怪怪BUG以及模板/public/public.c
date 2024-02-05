#include "public.h"
#include <REGX52.H>
#include <INTRINS.H>

void Delay10us(u16 xus){ // @11.0592MHz 
    while (xus--); 
}

void Delay(u16 xms) // copied from module 
{
	u16 i,j;
	for(i= xms;i>0;i--)
		for(j=110;j>0;j--);
}