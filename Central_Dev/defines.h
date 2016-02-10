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

  /* Put here your global defines for all my libraries used in your project */
  #define TM_DELAY_TIM							TIM3
  #define TM_DELAY_TIM_IRQ						TIM3_IRQn
  #define TM_DELAY_TIM_IRQ_HANDLER	        	TIM3_IRQHandler
  #define TM_I2C_TIMEOUT                        2000
  #define TM_I2C3_ACK							I2C_Ack_Enable
  #define TM_USART2_USE_CUSTOM_IRQ 				// custom USART handler for Remote unit GSM module - definition at the end of gsm.c source file



  /* Private defines */
  #define OUTPUT_USART                     		USART3
  #define OUTPUT_USART_SPEED               		115200
  #define OUTPUT_USART_PINS                		TM_USART_PinsPack_2

  #define MENU_USART                      		USART1
  #define MENU_USART_SPEED                		115200
  #define MENU_USART_PINS                 		TM_USART_PinsPack_1

  #define GSM_USART                      	 	USART2
  #define GSM_USART_SPEED                	 	115200
  #define GSM_USART_PINS                  		TM_USART_PinsPack_2

  #define SFP_I2C_PORT							I2C3
  #define SFP_I2C_PINS            				TM_I2C_PinsPack_1
  #define SFP_I2C_SPEED           				200000
  #define SFP_I2C_ADDRESS						0xA2



#endif
