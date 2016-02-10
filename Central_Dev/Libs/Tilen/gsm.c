#include "gsm.h"



void sendATCommand(char* command){
TM_USART_Puts(USART_GSM,command);
TM_USART_Puts(USART_GSM,"\r\n");
}





bool parseFind(const char* const input, const char* search) {
  if (!input) return 0; // check if input is not NULL
  return strstr(input,search);
}


char * readResponse (uint16_t timeout, uint16_t datasize, uint32_t delay) {
  char * response;
  uint16_t time=TM_DELAY_Time();
  malloc(sizeof(char) * (datasize));
  
  if (response==NULL) {
    return 0;
  }
  
  if(!TM_USART_BufferEmpty(USART_GSM)){
    TM_USART_Gets(USART_GSM, response, sizeof(response));
  }
                  
}
  