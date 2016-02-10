/*
 ******************************************************************************
 * @file    main.c
 * @author  Elmat Team
 * @version V0.2
 * @date    31-October-2014
 * @brief   Application for Central Device
 ******************************************************************************
 */

#include "main.h"



/* Private defines ------------------------------------------------------------*/
#define SYSTEMTICK_PERIOD_MS  	10
#define DATA_OUT_INTERVAL		5000	// collected valid data will be send to Data USART in every X milliseconds
#define GSM_CHECK_INTERVAL		10000	// check GSM registration status in every X milliseconds
#define LASER_LOCK_TIMEOUT		20000	// Laser Lock timeout - if there will be no new packet from remote device with particular ID,
										// after given period of time, then laser with the same ID will be disabled and data will not be displayed for


volatile uint32_t LocalTime = 0;                                                /* this variable is used to create a time reference incremented by 10ms */
Remote_Data_t Remote_Data_Array[10];                                            /* this variable is a global scope variable defined in main.h, used in udp_echoserver.c and main.c
                                                                                   it is holding information from packets received from remote devices */

/* Pointers to custom timers */
TM_DELAY_Timer_t* GSM_Status_Update_Timer;
TM_DELAY_Timer_t* Print_Results_Timer;
TM_DELAY_Timer_t* LaserLock_Timer;




/* This function is used to periodically check GSM status */
void GSM_Status_Update_Timer_Task(void* UserParameters) {
  GSM_Reg_Status(0);
}

/* This function is used to periodically print data collected from remote devices */
void Print_Results_Timer_Task(void* UserParameters) {
  print_results();
}

/* This function is used to periodically update LaserLock variables */
void LaserLock_Timer_Task(void* UserParameters) {
  uint32_t current_time=TM_DELAY_Time();
    for (int i=0;i<10;i++)
      if (current_time > Remote_Data_Array[i].rcv_system_time + LASER_LOCK_TIMEOUT) {
    	  Remote_Data_Array[i].laser=0;
        Laser_Control(i,LD_OFF);
        TM_BKPSRAM_Write8(i,0);
      }

}


int main(void)
{
  SystemInit();						// initialize MCU clocks and registers
  TM_DELAY_Init();					// initialize Delay library
  TM_DELAY_SetTime(0);				// Reset couter for systime
  Laser_GPIO_Conf();				// configure GPIO for laser control (to be able to enable/disable lasers via software
  TM_BKPSRAM_Init();				// initialize BKP RAM access library
  Laser_Update();			// load laser statuses saved in BKP RAM
  TM_USART_Init(OUTPUT_USART, OUTPUT_USART_PINS, OUTPUT_USART_SPEED);		// initialize UART used for collected Data output
  TM_USART_Init(MENU_USART, MENU_USART_PINS, MENU_USART_SPEED);				// initialize UART used for configuration
  TM_RTC_Init(TM_RTC_ClockSource_External);									// initialize RTC library
  TM_GPIO_Init(GPIOD, GPIO_Pin_8, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Low); // configure GPIO for GSM status indication (RED LED)
  TM_GPIO_Init(GPIOD, GPIO_Pin_9, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Low); // configure GPIO for GSM status indication (GREEN LED)
  Laser_ADC_Init();					// initialize ADC peripherals
  Menu_Init();						// initialize CLI library
  sfpInit();						// configure GPIO for SFP modules
  gsm_Init();						// initialize GSM module


  /* configure and initialize Ethernet hardware and LwIP stack */

  ETH_BSP_Config();					// configure ETH GPIOs
  printf("Ethernet MAC and PHY configured successfully!\n");
  LwIP_Init();						// start LwIP stack
  printf("LwIP stack initialized successfully!\n");
  UDP_Server_Init();				// start UDP Server
  printf("UDP Server initialized successfully!\n");

  //start periodic tasks

  /* GSM Status update "task" */
  GSM_Status_Update_Timer = TM_DELAY_TimerCreate(GSM_CHECK_INTERVAL, 1, 1, GSM_Status_Update_Timer_Task, NULL);
  printf("GSM status check task created!\n");
  /* Print results from remote devices "task" */
  Print_Results_Timer = TM_DELAY_TimerCreate(DATA_OUT_INTERVAL, 1, 1, Print_Results_Timer_Task, NULL);
  printf("Print collected data task created!\n");
  /* LaserLock status update "task" */
  LaserLock_Timer = TM_DELAY_TimerCreate(1000, 1, 1, LaserLock_Timer_Task, NULL);
  printf("Laser lock check task created!\n");

  while (1) {
	  /* CLI menu update */
	  Menu_Update();
      /* check if any packet received */
	  if (ETH_CheckFrameReceived())
	  {
		  /* process received ethernet packet */
		  LwIP_Pkt_Handle();
	  }
    /* handle periodic timers for LwIP */
    LwIP_Periodic_Handle(LocalTime);
    /* update laser statuses */
    Laser_Update();
    /* remove SMS messages which were read by system */
    delete_read_gsm_messages();
  }
} 


























//Updates the system local time for LwIP periodic function such as ARP Table updates
void Time_Update(void) {
  static unsigned short ms_ticks = 0;
  LocalTime += SYSTEMTICK_PERIOD_MS;
  ms_ticks += SYSTEMTICK_PERIOD_MS;
  if (ms_ticks >= 1000)
    {
      ms_ticks = 0;
    }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif
