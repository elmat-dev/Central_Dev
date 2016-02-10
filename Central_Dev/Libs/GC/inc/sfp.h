#ifndef SFP_H
#define SFP_H

/*
 ******************************************************************************
 * @file    sfp.h
 * @author  Elmat Team
 * @version V0.1b
 * @date    31-October-2014
 * @brief   Header file for Library for SFP module support
 ******************************************************************************
 */

#include <math.h>
#include <stdio.h>
#include "tm_stm32f4_i2c.h"
#include "tm_stm32f4_delay.h"







typedef struct
{
	float temp;
	float voltage;
	float current;
	float tx_power_dbm;
	float rx_power_dbm;
	float tx_power_mw;
	float rx_power_mw;
    int present;
} SFP_Data;

typedef enum
{
	temperature = 0x60,
	voltage = 0x62,
	current = 0x64,
	tx_mw = 0x66,
	rx_mw = 0x68,
	tx_dbm = 0x10,
	rx_dbm = 0x20,
} SFP_READ_DDMI_Type_t;


int sfpInit(void);
int selecPortSFP (uint8_t port);
float SFP_DDMI_RAW2Float (SFP_READ_DDMI_Type_t data_type, uint16_t raw_value);
void SFP_READ_DDMI_All_8bit (uint8_t* ddmi_buffer);
void SFP_READ_DDMI_All_16bit (uint16_t* ddmi_buffer);
float SFP_READ_DDMI_Single_float (SFP_READ_DDMI_Type_t type);
uint16_t SFP_READ_DDMI_Single_uint16 (SFP_READ_DDMI_Type_t type);
int get_ddmi_1(SFP_Data * ddmi, uint8_t port);
int get_ddmi_2(SFP_Data * ddmi, uint8_t port);
void show_ddmi(short module_id);
float mw_2_dbm (float raw_value);

#endif
