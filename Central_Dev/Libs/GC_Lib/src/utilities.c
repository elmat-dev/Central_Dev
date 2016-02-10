// I/O Initialization 
// all floating pins set as analog inputs
//ethernet, i2c, usarts not set

#include "utilities.h"



// all floating pins as analog inputs, enabled wakeup pin PA0;
void IO_Config(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC |
                          RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE , ENABLE);
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_11 | GPIO_Pin_13;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_15;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_13;
   GPIO_Init(GPIOC, &GPIO_InitStructure);
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 |
                                 GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
   GPIO_Init(GPIOD, &GPIO_InitStructure);
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 |
                                GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
   GPIO_Init(GPIOE, &GPIO_InitStructure);							 
   
   
   
   //PA0 - PA0_WAKEUP_PIN
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);							 
}

void dis_all (void) {
 /**
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE , ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
**/
  //IO_Config();
  //SFP_GPIO_Config();
  //PHY_GPIO_Config();
  //GSM_GPIO_Config();
  //GPS_GPIO_Config();
  SFP_Off();
  PHY_Off();
  GSM_Off();
  GPS_Off();
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE , DISABLE);

  RCC_AHB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
TM_LOWPOWER_EnableWakeUpPin();
  while (1)
  {
    //PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
    //TM_LOWPOWER_SleepUntilInterrupt(1);
    TM_LOWPOWER_StopUntilInterrupt();
  }
    
}


// inicjalizacja GPIO do w³¹czania i wy³¹czania przetwornicy PV
void ZSPM_GPIO_Config (void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB, GPIO_Pin_14);
}
//inicjalizacja GPIO odpowiedzialnego za wlaczanie i wylaczanie modu³u SFP, domyslnie po zainicjalizowaniu modul jest wylaczony
void SFP_GPIO_Config (void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOE, GPIO_Pin_1);
}
//inicjalizacja GPIO odpowiedzialnego za wlaczanie i wylaczanie PHY oraz zegara 50MHz, domyslnie po zainicjalizowaniu wylaczony, komunikacja z MAC nie inicjowana
void PHY_GPIO_Config (void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);
  
}
//inicjalizacja GPIO odpowiedzialnego za wlaczanie i wylaczanie GSM, domyslnie po zainicjalizowaniu wylaczony, interfejs USART nie inicjowany
void GSM_GPIO_Config (void)     {
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE , ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      //PB9 - GSM_PWR_En (Low - enable)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
      //PD6 - GSM_WAKEUP (Hi for few ms - enable)
      //PD7 - GSM_RESET (Hi for few ms - enable)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
      //PE0 - GSM_ON/OFF (Low for 200ms - enable)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}
//inicjalizacja GPIO odpowiedzialnego za wlaczanie i wylaczanie GPS, domyslnie po zainicjalizowaniu wylaczony, interfej USART nie inicjowany
void GPS_GPIO_Config (void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      //PC12 - GPS_RESET
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
      //PD0 - GPS_PWR_En (Hi - enable)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOD, &GPIO_InitStructure);  
}
  





// obsluga przetwornicy PV
void ZSPM_On (void) {
  ZSPM_GPIO_Config();
  GPIO_SetBits(GPIOB, GPIO_Pin_14);
}
  
void ZSPM_Off (void) {
  GPIO_ResetBits(GPIOB, GPIO_Pin_14);
}


void PHY_On(void) {
  //PHY_GPIO_Config();
  GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);
  GPIO_ResetBits(GPIOE, GPIO_Pin_2);
  for (int i = 0; i < 200000; i++)
  GPIO_SetBits(GPIOE, GPIO_Pin_2);
  GPIO_ResetBits(GPIOE, GPIO_Pin_2);
}

void PHY_Off(void) {
  GPIO_ResetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);
}

void SFP_On (void) {
  //SFP_GPIO_Config();

  //TM_DELAY_Init();
  GPIO_SetBits(GPIOE, GPIO_Pin_1);
  //Delayms(100); // SFP MSA Standard says 300ms for full SFP initialization from power on
}

void SFP_Off (void) {
  GPIO_ResetBits(GPIOE, GPIO_Pin_1);
}


// obsluga moduly GSM
int GSM_On (void) {
  uint8_t is_enabled = 0;
  char buffer[32];
  GSM_GPIO_Config();
  //TM_DELAY_Init();
  TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
  //TM_USART_Init(USART6, TM_USART_PinsPack_1, 115200);
  GPIO_ResetBits(GPIOB, GPIO_Pin_9); // PWR_EN
  GPIO_ResetBits(GPIOE, GPIO_Pin_0); //GSM_ON/OFF
  Delayms(300);
  GPIO_SetBits(GPIOE, GPIO_Pin_0);
  //Delayms(2000);
  printf("Proba uruchomienia modulu GSM\r\n");
  
  //memset(buffer, 0, sizeof(buffer));
  //for (int i=0;i<32;i++) printf("%c",TM_USART_Getc(USART1) );
  for (int i=0;i<sizeof buffer;i++) buffer[i]=TM_USART_Getc(USART1);
  //for (int i=0;i<32;i++) printf("%d",TM_USART_Getc(USART1) );
  
    printf("Zawartosc bufora : %s\r\n", buffer);
  if (!strncmp(buffer,"AT command ready\r\n",15))
    is_enabled = 1;
  else is_enabled = 0;
  
  
return is_enabled;
}

void GSM_Off (void) {
  GSM_GPIO_Config();
  TM_DELAY_Init();
  GPIO_ResetBits(GPIOE, GPIO_Pin_0);
  Delayms(3200);
  GPIO_SetBits(GPIOE, GPIO_Pin_0);
  GPIO_SetBits(GPIOB, GPIO_Pin_9);
  GPIO_SetBits(GPIOE, GPIO_Pin_0);
}



// obs³uga GPS
void GPS_On (void) {
  //GPS_GPIO_Config();
  GPIO_SetBits(GPIOC, GPIO_Pin_12);
  GPIO_SetBits(GPIOD, GPIO_Pin_0);
  
}

void GPS_Off (void) {
  GPIO_ResetBits(GPIOD, GPIO_Pin_0);
}

void GPS_Reset (void) {
  TM_DELAY_Init();
  GPIO_ResetBits(GPIOC, GPIO_Pin_12);
  Delayms(20);
  GPIO_SetBits(GPIOC, GPIO_Pin_12);
}


 TM_GPS_Result_t GPS_CheckPosition(TM_GPS_Data_t * GPS_Data) {
   TM_GPS_Result_t result;
   GPS_On();
   TM_DELAY_Init();
   TM_DELAY_SetTime(0);
   GPS_Data->Fix=0;
   TM_GPS_Init(GPS_Data, 9600);
      do {
        result = TM_GPS_Update(GPS_Data);
        if (GPS_Data->Fix==1) break;
      } while (TM_DELAY_Time() < 60000);
  GPS_Off();
  return result;
}












int GSM_AT_wConfirm (char * command, char * expected_response) 
  {
    char * token = malloc(100*sizeof(char)); 
    char buffer[32];
    int result=0;
    //clear USART1 buffer
    for (int i=0;i<sizeof buffer;i++) buffer[i]=TM_USART_Getc(USART1);
    memset(buffer, 0, sizeof(buffer));
    // send AT command to GSM_Module
    TM_USART_Puts(USART1,command);
    Delayms(180);
    //read back GMS Module response
    for (int i=0;i<sizeof buffer;i++) buffer[i]=TM_USART_Getc(USART1);
    token = strtok (buffer,"\n\r");
      while (token != NULL)
        {
          if (strncmp(token,expected_response,sizeof token)==0)
          result=1;
          printf("Potwierdzam wykonanie komendy %s, otrzymano potwierdzenie: %s\r\n", command, expected_response);
          printf ("%s\n\r",token);
          token = strtok (NULL, "\n\r");

        }
      free(token);
  return result;    
  }


int AT_CMD_WITH_RESP (USART_TypeDef* USARTx, char * command, char * expected_response , long timeout) {
  char * buffer;
  long timeout_cnt_start=TM_DELAY_Time();
  int ret_val=AT_RESP_NOT_RCVD;
  do {
    TM_USART_Gets(USARTx,buffer,sizeof *buffer);
    if(strstr(buffer,expected_response)) {
      ret_val=AT_RESP_RCVD;
      break;
    }
    else ret_val=AT_RESP_NOT_RCVD;
  } while ((TM_DELAY_Time() - timeout_cnt_start) < timeout);
  return ret_val;
}



void AT_Echo (USART_TypeDef* USARTx, char on_off) {
  char *buf;
  int buf_len=5;
  buf = (char*)malloc(buf_len * sizeof(*buf));
  
  if (on_off==0 || on_off==1) {
    sprintf(buf,"ATE%d\r",on_off);
    TM_USART_Puts(USARTx,buf);
    free(buf);
  }
}
  









float battery_voltage (void)  {
  float voltage=0;
  float reference=0.000836;
  TM_ADC_Init(ADC1, ADC_Channel_4);
  for (int i=0;i<5;i++)
  voltage=2*(TM_ADC_Read(ADC1, ADC_Channel_4))*reference;
  return voltage;
}







//void TM_RTC_RequestHandler() {
//  printf("Zg³oszenie od  RTC \r\n");
//  NVIC_SystemReset();
//}





