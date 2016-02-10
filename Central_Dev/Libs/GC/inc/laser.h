#ifndef LASER_H
#define LASER_H

#include "stm32f4xx.h"
#include "tm_stm32f4_adc.h"
#include "tm_stm32f4_bkpsram.h"

#define LD_ON                           1
#define LD_OFF                          0



typedef struct {
  float current;
  uint8_t status;
} Laser_Data_t;


/* @brief: Configure GPIOs for laser control
 * @param: None
 * @retval: None
 */
void Laser_GPIO_Conf(void);

/* @brief: Control the enable pin for laser drivers
 * @param: number of channel which should be enabled/disabled (ID of laser, between 0-9
 * @param: state of laser: 1-ON, 0-OFF
 * @retval: None
 */
void Laser_Control (uint8_t, uint8_t);

/* @brief: Update status of lasers based on values written to BKP RAM memory
 * @param: None
 * @retval: None
 */
void Laser_Update (void);


/* @brief: Configure ADC GPIOs for reading of current set for lasers
 * @param: None
 * @retval: None
 */
void Laser_ADC_GPIO_Conf();


/* @brief: Configure ADCs for reading of current set for lasers
 * @param: None
 * @retval: None
 */
void Laser_ADC_Init();


/* @brief: Issue ADC read for current set for laser with given ID (channel)
 * @param channel - number of laser for which set current should be read
* 					Caution: Laser must be enabled and powered for this reading, otherwise value close to 0 will be returned
 * @retval: Raw value read from ADC connected to laser driver with given ID
 */
uint16_t Laser_ADC_Read(uint8_t);


/* @brief: Read current set for laser with given ID (channel) using normal averaging
 * @param channel - number of laser for which set current should be read
 * @param num_of_samples - number of samples which should be read from ADC to calculate average
 * 					Caution: Laser must be enabled and powered for this reading, otherwise value close to 0 will be returned
 * @retval: float representation of current set for laser with given ID. Units of milliAmps
 */
float Laser_ADC_Read_Avg(uint8_t, uint8_t);


/* @brief: Read current set for laser with given ID (channel) using sorting and removing lowest and highest values
 * @param channel - number of laser for which set current should be read
 * 					Caution: Laser must be enabled and powered for this reading, otherwise value close to 0 will be returned
 * @retval: float representation of current set for laser with given ID. Units of milliAmps
 */
float Get_Laser_Current(int);


/* @brief: Function used for array sorting
 * @param A[] - array of uint16_t values which should be sorted from lowest to highest
 * @param L - number of values in the array
 * @retval: None
 */
void Sort_values(uint16_t *, uint8_t);

#endif  // end of LASER_H file
