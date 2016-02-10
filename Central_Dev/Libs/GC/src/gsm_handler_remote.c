//#include "board_conf.h"
#include "utilities_new.h"


//#include <stdio.h>
//#include <string.h>





// Set of basic AT commands
const char atc0[] = "AT";                        // Every AT command starts with "AT"
const char atc1[] = "ATE0";                      // Disable command echo
const char atc2[] = "AT+CMGF=1";                 // TXT messages
      char atc3[] = "AT+CMGS=\"";                // sends SMS to desired number
const char atc4[] = "AT+CMGR=1";                 // Command for reading message from location 1 from inbox
const char atc5[] = "AT+CMGD=1,4";               // Erasing all messages from inbox
const char atc6[] = "AT+CMGL=\"ALL\"";           // Check status of received SMS
//

// Responses to parse
const uint8_t GSM_OK                       = 0;
const uint8_t GSM_Ready_To_Receive_Message = 1;
const uint8_t GSM_ERROR                    = 2;
const uint8_t GSM_UNREAD                   = 3;
//



// SMS Message string
char SMS_Message[300];

// ADC data string
char ADC1_data[6];

// phone number string
char phone_number[20];

// State machine control variables
char gsm_state = 0;
char response_rcvd = 0;
short responseID = -1, response = -1, rsp;
char set_stat = 0, clr_stat = 0;
char PORT_flag = 0;
char Digital_O = 0;
char gsm_number = 0;
char Unread_flag;
//
char status_req = 0; // Status request variable

// Send command or data to the Telit GM862 Module - (const)
void GM862_Send(const char *s)
{
// Send command or data string
   while(*s) {
     TM_USART_Putc(GSM_USART,*s++);
   }
// Terminatation by CR
   TM_USART_Putc(GSM_USART,0x0D);
}

// Send command or data to the Telit GM862 Module - (RAM)
void GM862_Send_Ram(char *s1)   //
{
// Send command or data string
   while(*s1) {
     TM_USART_Putc(GSM_USART,*s1++);
   }
// Terminatation by CR
    TM_USART_Putc(GSM_USART,0x0D);
}

// Get GSM response, if there is any
short Get_response() {
    if (response_rcvd) {
      response_rcvd = 0;
      return responseID;
    }
    else
      return -1;
}

// Wait for GSM response (infinite loop)
void Wait_response(char rspns) {
char test = 1;
  
  while (test){
  test = Get_response();
  if ((test == rspns) || (test == GSM_ERROR))
    test = 0;
  else
    test = 1;
  }
}

// Compose Status SMS
unsigned ComposeMessage(char* Message);

// Send Status SMS
void Send_Msg(char* Msg){
  char atc[33];
  
  atc[0] = 0;                        // clear atc string
  strcat(atc, atc3);                 // atc3 command for sending messages
  strcat(atc, phone_number);         // add phone number
  strcat(atc, "\"");                 // complete AT command
  GM862_Send_Ram(atc);               // send AT command for SMS sending
  Wait_response(GSM_Ready_To_Receive_Message); // Wait for appropriate ready signal

  GM862_Send_Ram(Msg);               // Send message content
  TM_USART_Putc(GSM_USART,0x1A);                 // Send CTRL + Z as end character
  TM_USART_Putc(GSM_USART,0x0D);                 // Send CR
  Wait_response(GSM_OK);             // Wait OK as confirmation that the message was sent
}

// Send status SMS to the cell phone number defined by the atc3 const string
void Send_Status(){
 ComposeMessage(SMS_Message);
 Send_Msg(SMS_Message);
}

// 3sec pause
void Wait(){
   Delayms(3000);
}


int gsmInit(void) {
  int status=-1;
  GSM_Control(GC_ON);
  while(1) {
    GM862_Send(atc0);                 // Send "AT" string until GSM862 sets up its baud rade
    Delayms(50);                    // and gets it correctly
    if (Get_response() == GSM_OK) {     // If GSM862 says "OK" on our baud rate we program can continue
      status=1;
      break;
    }
  }
  return status;
}

// Main
void gsm_init(){
  GSM_Control(GC_ON);
  TM_USART_Init(GSM_USART, GSM_USART_PINS, GSM_USART_SPEED);
  //GSM_Enable();
  while(1) {
    GM862_Send(atc0);                 // Send "AT" string until GSM862 sets up its baud rade
    Delayms(100);                    // and gets it correctly
    if (Get_response() == GSM_OK)     // If GSM862 says "OK" on our baud rate we program can continue
      break;
  }

 GM862_Send(atc1);        // Disable command echo
 Wait_response(GSM_OK);

 GM862_Send(atc2);        // Set message type as TXT
 Wait_response(GSM_OK);

 while(1){
   GM862_Send(atc5);      // Delete all messages (if any)
   if (get_response() == GSM_OK) // If messages are deleted
     break; // break from while
   Delayms(500);
 }
 // blink as a sign that initialization is successfully completed
 TM_USART_Puts(DEBUG_USART,"Wiadomosci SMS zosta³y usiniete\n\r");

  // infinite loop
  while(1) {
    GM862_Send(atc6);        // Read status of the messages and read message it self
    Delayms(100);           // Wait until the message is read

    while(1) {
      GM862_Send(atc0);      // Wait until the module is ready
      Delayms(50);
      if (Get_response() == GSM_OK)
        break;
    }

    if (Unread_flag){

      }

      while(1) {
        GM862_Send(atc0);    // Wait until the module is ready
        Delayms(50);
        if (Get_response() == GSM_OK)
          break;
      }

      if (status_req){       // Send status SMS if it's been requested
        status_req = 0;
        Send_Status();
      }

      Unread_flag = 0;
    }

    while(1){
      GM862_Send(atc5);  // Delete all messages (if any)
      Delayms(50);
      if (get_response() == GSM_OK) // If messages are deleted
        break;           // break from while
      Delayms(50);
      if (Unread_flag){  // if we have received message in mean time
        Unread_flag = 0;
        break;           // break from while
      }
    }
    Wait();

}


/******************************************************************************/

// Compose Status SMS
unsigned ComposeMessage(char* Message){
  unsigned adc_value1, adc_value2;
  char temp_txt[15];
  char PORTB_temp, i, PORTD_temp;

  //RC1IE_bit = 0;                // Disable Rx1 intterupts

  Message[0] = '\0';

  // SMS header
  strcat(Message, "INFO:");
  strcat(Message, "\r\n");      // Add new line (CR + LF)
  //
  
  temp_txt[0] = 0;
  strcat(temp_txt, "PORTB ");

  temp_txt[14] = 0;
  strcat(Message, temp_txt);
  strcat(Message, "\r\n");      // Add new line (CR + LF)*/
  //

  temp_txt[0] = 0;
  strcat(temp_txt, "PORTD ");


  temp_txt[14] = 0;
  strcat(Message, temp_txt);
  strcat(Message, "\r\n");      // Add new line (CR + LF)*/
  //

  // Adding ADC values to the SMS
  strcat(Message, "AN2 - ");  // AN1
  strcat(Message, ADC1_data); // Add ADC1 data
  strcat(Message, "\r\n");    // Add new line (CR + LF)
  //

  // SMS footer
  strcat(Message, "End.");
  strcat(Message, "\r\n");      // Add new line (CR + LF)
  //

  //RC1IE_bit = 1;                // Enable Rx1 intterupts

  return strlen(Message);
}




// state machine
// Reading the data from UART in the interuppt routine
void TM_USART1_ReceiveHandler(uint8_t c) {
  char i;
  char tmp = USART2->DR;        // Get received byte
  //TM_USART_INT_InsertToBuffer(&TM_USART2, (uint8_t)tmp);

// Process reception through state machine
// We are parsing only "OK" and "> " responses
    switch (gsm_state) {
      case  0: {
                response = -1;                   // Clear response
                if (tmp == 'O')                  // We have 'O', it could be "OK"
                  gsm_state = 1;                 // Expecting 'K'
                if (tmp == '>')                  // We have '>', it could be "> "
                  gsm_state = 10;                // Expecting ' '
                if (tmp == 'E')                  // We have 'E', it could be "> "
                  gsm_state = 30;                // Expecting 'R'
                if (tmp == 'S')                  // We have 'S', it could be "Status?" or "Set"
                  gsm_state = 40;                // Expecting 't'
                if (tmp == 'R')                  // We have 'R', it could be "RDx" or
                  gsm_state = 50;                // Expecting D
                if (tmp == 'C')                  // We have 'C', it could be "Clear"
                  gsm_state = 110;               // Expecting l
                if (tmp == 'U')                  // We have 'U', it could be "UNREAD"
                  gsm_state = 120;               // Expecting l
                break;
               }
      case  1: {
                if (tmp == 'K') {                // We have 'K' ->
                  response = GSM_OK;             // We have "OK" response
                  gsm_state = 20;                // Expecting CR+LF
                }
                else
                  gsm_state = 0;                 // Reset state machine
                break;
               }
      case 10: {
                if (tmp == ' ') {
                  response_rcvd = 1;             // We have "> " response
                  response = GSM_Ready_To_Receive_Message; // Set reception flag
                  responseID = response;         // Set response ID
                }
                gsm_state = 0;                   // Reset state machine
                break;
                }

      case 20: {
                if (tmp == 13)                   // We have 13, it could be CR+LF
                  gsm_state = 21;                // Expecting LF
                else
                  gsm_state = 0;                 // Reset state machine
                break;
               }
      case 21: {
                if (tmp == 10) {                 // We have LF, response is complete
                  response_rcvd = 1;             // Set reception flag
                  responseID = response;         // Set response ID
                }
                gsm_state = 0;                   // Reset state machine
                break;
               }

      case 30: {
                if (tmp == 'R')                  // We have 'R', it could be "ERROR"
                  gsm_state = 31;                // Expecting 'R'
                else
                  gsm_state = 0;                 // Reset state machine
                break;
               }
      case 31: {
                if (tmp == 'R')                  // We have 'R', it could be "ERROR"
                  gsm_state = 32;                // Expecting 'O'
                else
                  gsm_state = 0;                 // Reset state machine
                break;
               }
      case 32: {
                if (tmp == 'O')                  // We have 'O', it could be "ERROR"
                  gsm_state = 33;                // Expecting 'R'
                else
                  gsm_state = 0;                 // Reset state machine
                break;
               }
      case 33: {
                if (tmp == 'R'){                 // We have 'R'
                  response_rcvd = 1;             // We have "ERROR" response
                  response = GSM_ERROR;          // Set reception flag
                  responseID = response;         // Set response ID
                }
                gsm_state = 0;                   // Reset state machine
                break;
                }
      case 40: {
                if (tmp == 't')                  // We have 't', it could be "Status?"
                  gsm_state = 41;                // Expecting 'a'
                else
                  if (tmp == 'e')                // We have 'e'. it could be "Set"
                    gsm_state = 100;
                  else
                    gsm_state = 0;               // Reset state machine
                  break;
               }
      case 41: {
                if (tmp == 'a')                  // We have 'a', it could be "Status?"
                  gsm_state = 42;                // Expecting 't'
                else
                  gsm_state = 0;                 // Reset state machine
                break;
               }
      case 42: {
                if (tmp == 't')                  // We have 't', it could be "Status?"
                  gsm_state = 43;                // Expecting 'u'
                else
                  gsm_state = 0;                 // Reset state machine
                break;
               }

       case 43: {
                if (tmp == 'u')                  // We have 'u', it could be "Status?"
                  gsm_state = 44;                // Expecting 's'
                else
                  gsm_state = 0;                 // Reset state machine
                break;
               }

       case 44: {
                if (tmp == 's')                  // We have 's', it could be "Status?"
                  gsm_state = 45;                // Expecting '?'
                else
                  gsm_state = 0;                 // Reset state machine
                break;
               }

       case 45: {
                if (tmp == '?'){                 // We have '?'
                  status_req = 1;                // Status has been requested!
                }
                gsm_state = 0;                   // Reset state machine
                break;
                }

       case 50: {
                if (tmp == 'D')                  // We have 'D', it could be "RDx"
                  gsm_state = 51;                // Expecting number
                else
                  gsm_state = 0;                 // Reset state machine
                break;
               }
       case 51: {
                if (tmp >= '0' && tmp <= '7'){   // We have pin number, it could be "RDx"
                  if (set_stat)
                    Digital_O |= 1 << (tmp - '0');
                  if (clr_stat)
                    Digital_O &= (0xFF ^(1 << (tmp - '0')));
                }
                  PORT_flag = 1;
                  gsm_state = 0;                 // Reset state machine
               }; break;

       case 100: {
                   if (tmp == 't'){              // We have 't', we received set command
                     set_stat = 1;               // Set following bits
                     clr_stat = 0;               // Do not clear following bits
                   }
                   gsm_state = 0;
                   break;
                 }
       case 110: {
                   if (tmp == 'l'){              // We have 'l', it could be Clear
                     gsm_state = 111;
                   }
                   else
                     gsm_state = 0;
                   break;
                 }
       case 111: {
                   if (tmp == 'e'){              // We have 'e', it could be Clear
                     gsm_state = 112;
                   }
                   else
                     gsm_state = 0;
                   break;
                 }
       case 112: {
                   if (tmp == 'a'){              // We have 'a', it could be Clear
                     gsm_state = 113;
                   }
                   else
                     gsm_state = 0;
                   break;
                 }
       case 113: {
                   if (tmp == 'r'){              // We have 'r', we have received Clear
                     clr_stat = 1;               // Clear following bits
                     set_stat = 0;               // Do not set following bits
                   }
                   gsm_state = 0;
                   break;
                 }
       case 120: {
                  if (tmp == 'N')
                    gsm_state = 121;
                  else
                    gsm_state = 0;
                  break;
                 }
       case 121: {
                  if (tmp == 'R')
                    gsm_state = 122;
                  else
                    gsm_state = 0;
                  break;
                 }
       case 122: {
                  if (tmp == 'E')
                    gsm_state = 123;
                  else
                    gsm_state = 0;
                  break;
                 }
       case 123: {
                  if (tmp == 'A')
                    gsm_state = 124;
                  else
                    gsm_state = 0;
                  break;
                 }
       case 124: {
                  if (tmp == 'D'){
                    response_rcvd = 1;             // We have "ERROR" response
                    response = GSM_UNREAD;         // Set reception flag
                    responseID = response;         // Set response ID
                    Unread_flag = 1;
                  }
                  gsm_state = 0;
                  break;
                 }
      default: {                                 // Unwanted character
                gsm_state = 0;                   // Reset state machine
               }
    }
    // parse phone number on which we should reply
    switch (gsm_number) {
      case 0 :{
               if (tmp == '"'){
                 gsm_number = 1;
                 i = 0;
               }
              }; break;
      case 1 :{
               if (tmp == '+'){
                 phone_number[i] = tmp;
                 i ++;
                 gsm_number = 2;
               }
               else{
                 gsm_number = 0;
                 i = 0;
               }
              }; break;
      case 2 :{
               if (tmp >= '0' && tmp <= '9'){
                 phone_number[i] = tmp;
                 i ++;
               }
               else 
                 if (tmp == '"'){
                   phone_number[i] = 0;
                   i = 0;
                   gsm_number = 0;
                 }
                 else{
                   phone_number[0] = 0;
                   i = 0;
                   gsm_number = 0;
                 }
               break;
              }
      default : {
                 phone_number[0] = 0;
                 i = 0;
                 gsm_number = 0;
                }
    }
}