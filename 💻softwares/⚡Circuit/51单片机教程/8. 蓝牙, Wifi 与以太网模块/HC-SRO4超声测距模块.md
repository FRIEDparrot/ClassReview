本超声测距是通过RCWL-9300解调测距芯片

当外部的MCU给模块一个大于10uS的高电平脉冲时, 模块会反馈一个与距离等比的高电平脉冲信号， 并计算距离为: 

$$s =  \frac{T * c}{2}$$
其中c为声速:
$$c \approx 342.62m/s$$
实际上如果想要精确测量距离， 必须和温度传感器进行配合使用。

![[attachments/Pasted image 20240122172601.png|1100]]


在超声模块中, Trig 引脚和Echo 引脚的电路图如图所示; Echo是超声发射控制, 而Trig是接受反射的超声波控制;

![[attachments/Pasted image 20240122172759.png]]
 触发信号Trig端发出10us的TTL电平信号，模块内部发出8个40KHZ的周期电平信号并用来检测回波。一旦检测到有超声波返回的信号则输出回响信号，回响信号的脉冲宽度与所测的距离成正比。

由此通过发射信号到收到的回响信号时间可以计算得到距离。（不过测量周期应为60ms以上，否则发射信号对回响信号产生影响）

需要说明的是, 为了更加精确， 可以使用定时器中断获取时间信息。 

测试代码如下:
```c
#include <REGX52.H>
#include "LED_controller.h"
#include "public.h"
#include "MATH.H"

// register according to the 
sbit Trig = P1^0;  // Trigger signal 
sbit Echo = P1^1;  //  

u16 t0 = 0;
u8  t0_add = 0;

void TIMER0_START(){
    TMOD &= ~T0_GATE_; TMOD &= ~T0_CT_; // use timer mode 
    TMOD |= T0_M0_; TMOD &= ~T0_M1_; // use mode 1 for timer 
    TH0 = 0xee; TL0 = 0x00;
    EA = 1; ET0 = 1; 
    TR0 = 1; // start_timer 
}

void TIMER0_STOP(){
    TR0 = 0; // stop timer;
}

void TIMER0_Func(void) interrupt 1{
    TH0 = 0xfc; TL0 = 0x18;
    if (t0_add) t0++;
}


float HC_SRO4_GetDis(void){
    float res = 0;
    Trig = 1;
    Delay10us(1);
    Trig = 0;
    while (!Echo);
    t0 = 0; t0_add = 1;
    while (Echo);
    t0_add = 0;
    res = (float)t0 * 342.62/2000;
    t0 = 0; // reset
    return res;
}

void HC_SRO4_Init(){
    TIMER0_START();
    Trig = 0;
    Echo = 0;
    Delay(10);
}

// 1000 meters as maximum and correct into the 3 digits. 
void Disp_Dis(float dis){
    float distance = dis;
    distance = fmod(distance, 1000);
    LED_ShowNum(0, (int)distance/100,0);
    distance = fmod(distance,100);
    LED_ShowNum(1, (int)distance/10,0);
    distance = fmod(distance,10);
    LED_ShowNum(2, (int)distance, 1);
    distance = fmod(distance,1) * 10;
    LED_ShowNum(3, (int)distance, 0);
    distance = fmod(distance,1) * 10;
    LED_ShowNum(4, (int)distance, 0);
    distance = fmod(distance,1) * 10;
    LED_ShowNum(5, (int)distance, 0);
    distance = fmod(distance,1) * 10;
    LED_ShowNum(6, (int)distance, 0);
}

void main(){
    float dis;
    HC_SRO4_Init();
    while (1){
        int i;
        dis = HC_SRO4_GetDis();
        for (i = 0; i < 50; i++){
            Disp_Dis(dis);
        }
    }
}
```