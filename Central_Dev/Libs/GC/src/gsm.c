#include "gsm.h"




// Set of basic AT commands
const char atc0[] = "AT";                        // Every AT command starts with "AT"
const char atc1[] = "ATE0";                      // Disable command echo
const char atc2[] = "AT+CMGF=1";                 // TXT messages
      char atc3[] = "AT+CMGS=\"";                // sends SMS to desired number
const char atc4[] = "AT+CMGR=1";                 // Command for reading message from location 1 from inbox
const char atc5[] = "AT+CMGD=1,4";               // Erasing all messages from inbox
const char atc6[] = "AT+CMGL=\"ALL\"";           // Check status of received SMS
const char check_reg[] = "AT+CREG=1";            // Check status of network registration
const char read_sms_on_receipt[] = "AT+CNMI=1,2,0,0,0"; // After issuing when sms is received it is imidiately displayed from memory
const char get_info_sms_on_receipt[] = "AT+CNMI=1,1,0,2,0"; // after issuing when sms is received modem sends for example: +CMTI: "SM",1




// Responses to parse
const uint8_t GSM_OK                       = 0;
const uint8_t GSM_Ready_To_Receive_Message = 1;
const uint8_t GSM_ERROR                    = 2;
const uint8_t GSM_UNREAD                   = 3;
const uint8_t GSM_REGISTERED               = 4;
const uint8_t NEW_MESSAGE                  = 5;
const uint8_t NO_CARRIER                   = 6;



// SMS Message string
char SMS_Message[300];



// phone number string
char phone_number[20];

// State machine control variables
char gsm_state = 0;
char reg_stat = 0; 
char response_rcvd = 0;
short responseID = -1, response = -1, rsp;
char set_stat = 0, clr_stat = 0;
char PORT_flag = 0;
char Digital_O = 0;
char gsm_number = 0;
char Unread_flag;
char delete_messages = 0;
char status_req = 0; // Status request variable


int laser_update_flag=0;
int delete_flag=0;

int gsm_reg_stat=0;


uint32_t reg_check_timer=0;









int gsm_Init(void) {
  GSM_Init();
  if(GSM_Registered(20000)) {
    TM_USART_Puts(MENU_USART,"Connection to GSM network: \033[32mOK \033[0m\r\n");
    return 1;
     // add LED control here for GSM Network Status
  }
  else {
    TM_USART_Puts(MENU_USART,"Connection to GSM network: \033[31mERROR\033[0m\r\n");
    return 0;
    // add LED control here for GSM Network Status
  }
}







int GSM_Init(void) {
	 TM_USART_Init(GSM_USART, GSM_USART_PINS, GSM_USART_SPEED);
  int status=-1;
  uint32_t start_time=TM_DELAY_Time(); 
  TM_USART_Puts(GSM_USART,"ATQ0\r"); // enable result codes appending with OK or ERROR messages
  while ((TM_DELAY_Time() - start_time) < 10000) {
    GM862_Send(atc0);                 // Send "AT" string until GSM862 sets up its baud rade
    Delayms(50);                    // and gets it correctly
    if (Get_response() == GSM_OK) {     // If GSM862 says "OK" on our baud rate we program can continue
      status=1;
      break;
    }
  } 
  GM862_Send(atc2);
  Wait_response(GSM_OK);
 return status;
}


int GSM_Reg_Status(int interval) {
  int tmp = gsm_reg_stat;
  if (TM_DELAY_Time() >= (reg_check_timer + interval)) {
    if (tmp == 0 || tmp == 2 || tmp == 3 || tmp == 4) {
      TM_GPIO_SetPinHigh(GPIOD,GPIO_Pin_9); // GSM OK LED
      TM_GPIO_SetPinLow(GPIOD,GPIO_Pin_8); // GSM Problem LED
    }
    if (tmp == 1) {
      TM_GPIO_SetPinLow(GPIOD,GPIO_Pin_9); // GSM OK LED
      TM_GPIO_SetPinHigh(GPIOD,GPIO_Pin_8); // GSM Problem LED
    }
    
    gsm_reg_stat=0;
    reg_check_timer = TM_DELAY_Time();
    GM862_Send("AT+CREG?");
  }

  return tmp;
}



int GSM_Registered (uint32_t timeout) {
  GM862_Send("ATQ0");
  Delayms(50);
  GM862_Send(check_reg);
  //Wait_response(GSM_OK); 
  uint32_t start_time=TM_DELAY_Time();
  Delayms(30);
  GM862_Send("ATQ1");
  Delayms(30);
  while ((TM_DELAY_Time() - start_time) < timeout) {
    GM862_Send("AT+CREG?");
    Delayms (1000);
    if (Get_response() == GSM_REGISTERED) {
      gsm_reg_stat=1;
      GM862_Send("ATQ0");
      TM_GPIO_SetPinLow(GPIOD,GPIO_Pin_9); // GSM OK LED
      TM_GPIO_SetPinHigh(GPIOD,GPIO_Pin_8); // GSM Problem LED
      break;
    }
  }
  GM862_Send("ATQ0");
  Delayms(50);
  //Wait_response(GSM_OK);
  //Delayms(2000);
  GM862_Send(read_sms_on_receipt);
  Delayms(50);
  return gsm_reg_stat;
}



// Send command or data to the Telit GM862 Module - (const)
void GM862_Send(const char *s) {
  while(*s) {
   TM_USART_Putc(GSM_USART,*s++);
  }
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

// Send Status SMS
void Send_Msg(char* Msg, char* number) {
  char atc[33];
  atc[0] = 0;                                   // clear atc string
  strcat(atc, atc3);                            // atc3 command for sending messages
  strcat(atc, number);                     // add phone number
  strcat(atc, "\"");                            // complete AT command
  GM862_Send(atc);                              // send AT command for SMS sending
  Wait_response(GSM_Ready_To_Receive_Message);  // Wait for appropriate ready signal
  GM862_Send(Msg);                              // Send message content
  TM_USART_Putc(GSM_USART,0x1A);                // Send CTRL + Z as end character
  TM_USART_Putc(GSM_USART,0x0D);                // Send CR
  Wait_response(GSM_OK);                        // Wait OK as confirmation that the message was sent
}







void delete_read_gsm_messages(void) {
   if(delete_messages) {
      //Delayms(20);
      delete_messages=0;
      GM862_Send("AT+CMGD=1,4"); // delete all messages 
    }
}




// Reading the data from GSM_USART in the interuppt routine
void GSM_Handler(uint8_t c) {
  char tmp=c;

  #ifdef GSM_DEBUG
       TM_USART_Putc(MENU_USART,tmp);
  #endif


    switch (gsm_state) {
      case  0: {
                response = -1;                   // Clear response
                if (tmp == 'O')                  // We have 'O', it could be "OK" or "On" or "Off"
                  gsm_state = 1;                 // Expecting 'K'
                if (tmp == '>')                  // We have '>', it could be "> "
                  gsm_state = 10;                // Expecting ' '
                if (tmp == 'E')                  // We have 'E', it could be "ERROR"
                  gsm_state = 30;                // Expecting 'R'
                if (tmp == 'U')                  // We have 'U', it could be "UNREAD"
                  gsm_state = 50;                // Expecting 'N'
                if (tmp == 'C')                  // We have 'C', it could be "CREG" or "CMTI"
                  gsm_state = 60;                // Expecting 'R' or "M"
                if (tmp == 'N')                  // We have 'N', it could be "NO CARRIER"
                  gsm_state = 80;                // Expecting 'O'
                if (tmp == 'M')
                  gsm_state = 210;
                break;
               }
      case  1: {
                if (tmp == 'K') {                // We have 'K' ->
                  response = GSM_OK;             // We have "OK" response
                  gsm_state = 20;                // Expecting CR+LF
                }
                else
                  if (tmp == 'n')                  // We have 't', it could be "Onx"
                    gsm_state = 41;                // Expecting 'x' with value between 0-9
                else
                  if (tmp == 'f')                // We have 'e'. it could be "Set"
                    gsm_state = 42;
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
//________________________________________________ "On" or "Off" ____________________________________________________________________________________

//_________________________________________________ "On" ____________________________________________________________________________________________
      case 41: {
                if (tmp >= '0' && tmp <= '9') {   // We have pin number, enable Laser x
                  TM_BKPSRAM_Write8(tmp-0x30,0xAC);
                  Laser_Control(tmp-0x30,LD_ON);
                  delete_messages = 1;
                  gsm_state = 0;
                }
                else
                  gsm_state = 0;                 // Reset state machine
                break;
               }
//_________________________________________________ "Off" ___________________________________________________________________________________________
      case 42: {
                if (tmp == 'f')                  // We have 'f', it could be "Off"
                  gsm_state = 43;                // Expecting 'x' with value between 0-9
                else
                  gsm_state = 0;                 // Reset state machine
                break;
               }

       case 43: {
                 if (tmp >= '0' && tmp <= '9')  { // We have pin number, disable Laser x
                  TM_BKPSRAM_Write8(tmp-0x30,0x00);
                  Laser_Control(tmp-0x30,LD_OFF);
                  delete_messages = 1;
                  gsm_state = 0;
                 }
                else
                  gsm_state = 0;                 // Reset state machine
                break;
               }
//________________________________________________ "UNREAD" _________________________________________________________________________________________
       case 50: {
                  if (tmp == 'N')
                    gsm_state = 51;
                  else
                    gsm_state = 0;
                  break;
                 }
       case 51: {
                  if (tmp == 'R')
                    gsm_state = 52;
                  else
                    gsm_state = 0;
                  break;
                 }
       case 52: {
                  if (tmp == 'E')
                    gsm_state = 53;
                  else
                    gsm_state = 0;
                  break;
                 }
       case 53: {
                  if (tmp == 'A')
                    gsm_state = 54;
                  else
                    gsm_state = 0;
                  break;
                 }
       case 54: {
                  if (tmp == 'D'){
                    response_rcvd = 1;             // We have "UNREAD" response
                    response = GSM_UNREAD;         // Set reception flag
                    responseID = response;         // Set response ID
                    Unread_flag = 1;
                  }
                  gsm_state = 0;
                  break;
                 }
//_________________________________________"CREG" or "CMTI"__________________________________________________________________________________________
        case 60: {
                  if (tmp == 'R') {             // We have 'R', it could be "CREG"
                     gsm_state = 61;
                   }
                   else if (tmp == 'M')
                     gsm_state = 70;
                   else
                     gsm_state = 0;
                   break;
                 }


//_____________________________________________________"CREG: 1,1" or "CREG 0,1"_____________________________________________________________________
       case 61: {
                  if (tmp == 'E')
                    gsm_state=62;
                  else 
                    gsm_state=0;
                  break;
                  }
       case 62: {
                  if (tmp == 'G')
                    gsm_state=63;
                  else 
                    gsm_state=0;
                  break;
                  }
        case 63: {
                   if (tmp == ':')
                     gsm_state=64;
                   else 
                    gsm_state=0;
                   break;
                  }
        case 64: {
                   if (tmp == ' ')
                    gsm_state=65;
                   else 
                    gsm_state=0;
                   break;
                  }
        case 65: {
                  if (tmp == '0' || tmp == '1') {
                    gsm_state=66;
                    gsm_reg_stat = 0;
                  }
                   else 
                    gsm_state=0;
                   break;
                  } 
        case 66: {
                   if (tmp == ',') {
                     gsm_state=67;
                   }
                   else 
                    gsm_state=0;
                   break;
                  }
        case 67: {
                   if (tmp == '1') {
                    response_rcvd = 1;             // We have "CREG: 1,1" - NETWORK REGISTERED response
                    response = GSM_REGISTERED;
                    gsm_reg_stat = 1;
                    responseID = response;         // Set response ID
                   }
                   else if (tmp == '2') {
                    response_rcvd = 1;             // We have "CREG: 1,1" - ME is searching for GSM provider
                    response = GSM_ERROR;
                    gsm_reg_stat = 2;
                    responseID = response;         // Set response ID
                   }
                   else if (tmp == '3') {
                    response_rcvd = 1;             // We have "CREG: 1,3" - REGISTRATION DENIEDe
                    response = GSM_ERROR;
                    gsm_reg_stat = 3;
                    responseID = response;         // Set response ID
                   }
                   if (tmp == '5') {
                    response_rcvd = 1;             // We have "CREG: 1,1" - NETWORK REGISTERED in ROAMING response
                    response = GSM_REGISTERED;
                    gsm_reg_stat = 5;
                    responseID = response;         // Set response ID
                   }
                   else {
                    gsm_state=0;
                    //gsm_reg_stat = 0;
                   }
                    break;
                  }
//_____________________________________________________"CMTI: "SM", x" ______________________________________________________________________________
        case 70: {
                   if (tmp == 'T') {
                    gsm_state=71;
                   }
                   else
                     gsm_state=0;
                   break;
                  }
        case 71: {
                   if (tmp == 'I')
                    gsm_state=72;
                   else
                     gsm_state=0;
                   break;
                  }
        case 72: {
                   if (tmp == ':')
                    gsm_state=73;
                   else
                     gsm_state=0;
                   break;
                  }
        case 73: {
                   if (tmp == ' ') {

                    gsm_state=74;
                   }
                   else
                     gsm_state=0;
                   break;
                  }
        case 74: {
                   if (tmp == '"')
                    gsm_state=75;
                   else
                     gsm_state=0;
                   break;
                  }
        case 75: {
                   if (tmp == 'S')
                    gsm_state=76;
                   else
                     gsm_state=0;
                   break;
                  }
        case 76: {
                   if (tmp == 'M')
                    gsm_state=77;
                   else
                     gsm_state=0;
                   break;
                  }
        case 77: {
                   if (tmp == '"')
                    gsm_state=78;
                   else
                     gsm_state=0;
                   break;
                  }
        case 78: {
                   if (tmp == ',')
                    gsm_state=79;
                   else
                     gsm_state=0;
                   break;
                  }
        case 79: {
                   if (tmp >= '0' && tmp <= '9') {
                    laser_update_flag=1;
                    response_rcvd = 1;             // We have "CMTI: "SM", x" - new message received
                    response = NEW_MESSAGE;
                    responseID = response;         // Set response ID
                   }
                   else
                     gsm_state=0;
                   break;
                  }
//_____________________________________________________"NO CARRIRER" ________________________________________________________________________________
        case 80: {
                   if (tmp == 'O')
                     gsm_state=81;
                   else
                     gsm_state=0;
                   break;
                  }
        case 81: {
                   if (tmp == ' ')
                     gsm_state=82;
                   else
                     gsm_state=0;
                   break;
                  }
        case 82: {
                   if (tmp == 'C')
                     gsm_state=83;
                   else
                     gsm_state=0;
                   break;
                  }
        case 83: {
                   if (tmp == 'A')
                     gsm_state=84;
                   else
                     gsm_state=0;
                   break;
                  }
        case 84: {
                   if (tmp == 'R')
                     gsm_state=85;
                   else
                     gsm_state=0;
                   break;
                  }
        case 85: {
                   if (tmp == 'R')
                     gsm_state=86;
                   else
                     gsm_state=0;
                   break;
                  }
        case 86: {
                   if (tmp == 'I')
                     gsm_state=87;
                   else
                     gsm_state=0;
                   break;
                  }
        case 87: {
                   if (tmp == 'E')
                     gsm_state=88;
                   else
                     gsm_state=0;
                   break;
                  }
        case 88: {
                  if (tmp == 'R') {
                    response_rcvd = 1;             // We have "CREG: 1,1" - NETWORK REGISTERED response
                    response = NO_CARRIER;
                    responseID = response;         // Set response ID
                  }
                   else
                     gsm_state=0;
                   break;
                  }

        default: {                                 // Unwanted character
                gsm_state = 0;                   // Reset state machine
               }
    }
}

// custom handler for USART2 (GSM USART)
void TM_USART2_ReceiveHandler(uint8_t c) {
  GSM_Handler(c);
}
