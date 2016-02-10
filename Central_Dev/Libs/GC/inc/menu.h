#ifndef MENU_H
#define MENU_H

#include "cmdline.h"
#include "vt100.h"
#include "main.h"



void cmdlineOutputInterface(uint8_t c);
void Menu_Init(void);
void welcome_message (void);

void Menu_Update(void);

void current_adj (void);
void show (void);
void set (void);
void help (void);


#endif
