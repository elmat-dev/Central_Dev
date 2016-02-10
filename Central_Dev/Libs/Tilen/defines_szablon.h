/**
 *  Defines for your entire project at one place
 * 
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@version 	v1.0
 *	@ide		Keil uVision 5
 *	@license	GNU GPL v3
 *	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#ifndef TM_DEFINES_H
#define TM_DEFINES_H

/* Put your global defines for all libraries here used in your project */


// decide which timer to use for Delay library
 #define TM_DELAY_TIM				TIM2
 #define TM_DELAY_TIM_IRQ			TIM2_IRQn
 #define TM_DELAY_TIM_IRQ_HANDLER	        TIM2_IRQHandler
 #define TM_I2C_TIMEOUT                         200

 #define TM_I2C3_ACK						I2C_Ack_Enable
 #define TM_USART2_USE_CUSTOM_IRQ // for Remote unit GSM module
 #define TM_USART3_USE_CUSTOM_IRQ // for v2224g module  
   
   /* Private defines */
   
  #define GPS_LONGITUDE_ADDRESS                 0x0C
  #define GPS_LONGITUDE_LENGTH                  12
  #define GPS_LATITUDE_ADDRESS                  0x1A
  #define GPS_LATITUDE_LENGTH                   12
  #define PHONE_ADDRESS                         0x00
  #define PHONE_LENGTH                          9
  #define PHONE_SET_ADDRESS                     0x09
  #define ID_ADDRESS                            0x0A
  #define ID_LENGTH                             1
  #define ID_SET_ADDRESS                        0x0B
   
   
#endif
