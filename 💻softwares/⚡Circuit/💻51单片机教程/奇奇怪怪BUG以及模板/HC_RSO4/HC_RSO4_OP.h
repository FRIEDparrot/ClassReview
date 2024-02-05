#ifndef HC_RSO4_OP_H
#define HC_RSO4_OP_H

// register according to the 
sbit Trig = P1^0;  // Trigger signal 
sbit Echo = P1^1;  

void HC_SRO4_Init(void);
float HC_SRO4_GetDis(void);

#endif