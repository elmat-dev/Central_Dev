#ifndef GSM_H
#define GSM_H


#include <string.h>
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_delay.h"
#include "laser.h"


/* uncomment line below to observe GSM module output data on MENU USART */
//#define GSM_DEBUG


int gsm_Init(void);
void GM862_Send(const char*);
short Get_response(void);
void Wait_response(char);
void Send_Msg(char*, char*);
int GSM_Init(void);
int GSM_Registered (uint32_t);
void delete_read_gsm_messages(void);
void GSM_Handler(uint8_t);
int GSM_Reg_Status(int);


#endif // end of GSM_H file
