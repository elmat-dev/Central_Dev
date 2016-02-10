#include "vt100.h"



char helper[100];



void vt100Init(USART_TypeDef* USARTx) {
 // initializes terminal to "power-on" settings
// ESC c
TM_USART_Puts(USARTx,"\x1B\x63");
}

void vt100ClearScreen(USART_TypeDef* USARTx) {
  // ESC [2J
  TM_USART_Puts(USARTx,"\x1B[2J");
}


void vt100CursorHome(USART_TypeDef* USARTx) {
  // ESC [H
  TM_USART_Puts(USARTx,"\x1B[H");
}

void vt100SetAttr(USART_TypeDef* USARTx, uint8_t attr) {
  // ESC [Ps m
  sprintf(helper, "\x1B[%dm",attr);
  TM_USART_Puts(USARTx, helper);
}

void vt100SetCursorMode(USART_TypeDef* USARTx, uint8_t visible) {
  if(visible)
  // ESC [ ? 25 h
  TM_USART_Puts(USARTx,"\x1B[?25h");
  else
  // ESC [ ? 25 l
  TM_USART_Puts(USARTx,"\x1B[?25l");
}

void vt100characterSet(USART_TypeDef* USARTx, uint8_t character_set) {
  if(character_set == 0)
  // ESC (B
  TM_USART_Puts(USARTx,"\x1B(B");
  else if (character_set == 1)
  // ESC (0
  TM_USART_Puts(USARTx,"\x1B)0");
}





void vt100SetCursorPos(USART_TypeDef* USARTx, uint8_t line, uint8_t col) {
  // ESC [ Pl ; Pc H
  sprintf(helper, "\x1B[%d;%dH",line,col);
  TM_USART_Puts(USARTx, helper);
}

