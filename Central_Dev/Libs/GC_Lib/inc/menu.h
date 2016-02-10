#ifndef MENU_H
#define MENU_H




#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "microrl.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_rtc.h"


#define DEBUG_USART           USART6







int execute (int argc, const char * const * argv);
void print_cli(const char * ch);
char ** complet (int argc, const char * const * argv);
void menu(void);



#endif
