#ifndef IRED_CONTROLLER_H
#define IRED_CONTROLLER_H
#include "PUBLIC.H"

void IRED_INIT(void);
u8 IRED_Get_Command(u8 *cmd);
u8 IRED_CMD_Mapping(u8 *cmd, u8 mode); // mode 0: real; mode 1: simulation

#endif 