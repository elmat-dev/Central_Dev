#include "stm32f4xx.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_timer_properties.h"
#include "tm_stm32f4_gps.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_i2c.h"
#include "tm_stm32f4_rtc.h"
#include "tm_stm32f4_adc.h"
#include "tm_stm32f4_low_power.h"
//#include "tm_stm32f4_stdio.h"
#include "tm_stm32f4_bkpsram.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


//const char atc0[] = "AT";                        // Every AT command starts with "AT"
//const char atc1[] = "ATE0";                      // Disable command echo
//const char atc2[] = "AT+CMGF=1";                 // TXT messages
//      char atc3[] = "AT+CMGS=\"";                // sends SMS to desired number
//const char atc4[] = "AT+CMGR=1";                 // Command for reading message from location 1 from inbox
//const char atc5[] = "AT+CMGD=1,4";               // Erasing all messages from inbox
//const char atc6[] = "AT+CMGL=\"ALL\"";           // Check status of received SMS




#define AT_RESP_NOT_RCVD                               9
#define AT_RESP_RCVD                                   1
#define AT_RESP_TMOUT                                  2











//#define TM_DISCO_LedOn(led)            (TM_DISCO_LED_PORT->BSRRL = led)










typedef struct {
	int ID;
	float Longitude;
	float Latitude;
	float Battery_Voltage;
	float Temperature;
	float Humidity;
        float Input_Power;
        float Output_Power;
} Transmitter_Data;









void IO_Config (void);
void ZSPM_GPIO_Config (void);
void SFP_GPIO_Config (void);
void PHY_GPIO_Config (void);
void GSM_GPIO_Config (void);
void GPS_GPIO_Config (void);
void ZSPM_On (void);
void ZSPM_Off (void);
void PHY_On (void);
void PHY_Off (void);
void SFP_On (void);
void SFP_Off (void);
int GSM_On (void);
void GSM_Off (void);
void GPS_On (void);
void GPS_Off (void);
void GPS_Reset (void);
TM_GPS_Result_t GPS_CheckPosition(TM_GPS_Data_t*);
void RTC_Start (void);
int enable_rtc(void);
int GSM_AT_wConfirm (char * , char * ) ;
float tcTemp2float (uint8_t, uint8_t);
float battery_voltage (void);