#ifndef __MAIN_H
#define __MAIN_H

/*
 ******************************************************************************
 * @file    main.h
 * @author  Elmat Team
 * @version V0.2
 * @date    31-October-2014
 * @brief   Header file for main source file
 ******************************************************************************
 */



#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#include "tm_stm32f4_timer_properties.h"
#include "tm_stm32f4_low_power.h"
#include "tm_stm32f4_bkpsram.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_gps.h"
#include "tm_stm32f4_i2c.h"
#include "tm_stm32f4_rtc.h"
#include "tm_stm32f4_adc.h"
#include "tm_stm32f4_id.h"

#include "stm32f4x7_eth_bsp.h"
#include "stm32f4x7_eth.h"
#include "lwipopts.h"
#include "netconf.h"
#include "udp_echoserver.h"



#include "defines.h"
#include "laser.h"
#include "gsm.h"
#include "sfp.h"
#include "menu.h"




typedef struct
{
	uint8_t id;
	uint8_t laser;
	uint8_t laser_enable_remote_request;
	float tx;
	float rx;
	float sfp_temp;
	float v_bat;
	float temp;
   	float humid;
   	float latitude;
   	float longitude;
   	uint8_t enabled;
   	uint8_t counter;
   	float optical_power;
   	uint32_t rcv_system_time;
   	uint8_t command;
   	uint8_t command_data;
} Remote_Data_t;

typedef union {
  float f;
  uint8_t fInt[4];
} FloatOrUInt;

typedef union {
  uint16_t uint16;
  uint8_t bytes[2];
} u16_bytes;




extern Remote_Data_t Remote_Data_Array[10];



#define RMII_MODE						/* Connection type between MAC and PHY */
#define UDP_SERVER_PORT    7            /* define the UDP local connection port */
#define UDP_CLIENT_PORT    7   			/* define the UDP remote connection port, not used here */


/* MAC Address of the central device */
#define MAC_ADDR0	0x00
#define MAC_ADDR1	0x1b
#define MAC_ADDR2	0xc5
#define MAC_ADDR3	0x16
#define MAC_ADDR4	0x17
#define MAC_ADDR5	0x18


/* Static Local IP address of central device: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   0
#define IP_ADDR3   2


/* NETMASK */
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0


/* Gateway IP Address */
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   0
#define GW_ADDR3   254


/* Exported functions ------------------------------------------------------- */
void Time_Update(void);
void Delay(uint32_t nCount);


#ifdef __cplusplus
}
#endif


#endif /* __MAIN_H */
