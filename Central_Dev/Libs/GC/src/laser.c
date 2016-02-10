#include "laser.h"


#define NS       				32         	// Number of samples to get from ADC
#define SR       				4          	// Samples removed after sorting, 4=(2 highest & 2 lowest)

volatile uint16_t ADC_Raw[NS] = {0};       	// Updated 2000 times per second by DMA
uint16_t Sample_ADC_Raw[NS]   = {0};       	// Non-volatile copy of ADC_Raw[NS]
uint32_t ADC_Average          = 0;         	// Average of the samples

float ADC_CurrentMultiplier   = 1.301489621489621;
int   offset			      = -2;








void Laser_GPIO_Conf(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOB, ENABLE); // (uint32_t)0x00000012
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; // (uint16_t)0xFF80
  GPIO_Init(GPIOE, &GPIO_InitStructure); 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10; // (uint16_t)0x0400
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}


void Laser_Update (void) {
  for (int i=0;i<10;i++) {
    if(TM_BKPSRAM_Read8(i)==0xAC)
      Laser_Control(i,LD_ON);
    else
      Laser_Control(i,LD_OFF);
  }
}


void Laser_Control (uint8_t channel, uint8_t state) {
  switch (channel) {
    case 0: {
        if (state==1)
          GPIO_SetBits(GPIOE, GPIO_Pin_7);
        else GPIO_ResetBits(GPIOE, GPIO_Pin_7);
        break;
    }
    case 1: {
        if (state==1)
          GPIO_SetBits(GPIOE, GPIO_Pin_8);
        else GPIO_ResetBits(GPIOE, GPIO_Pin_8);
        break;
    }
    case 2: {
        if (state==1)
          GPIO_SetBits(GPIOE, GPIO_Pin_9);
        else GPIO_ResetBits(GPIOE, GPIO_Pin_9);
        break;
    }
    case 3: {
        if (state==1)
          GPIO_SetBits(GPIOE, GPIO_Pin_10);
        else GPIO_ResetBits(GPIOE, GPIO_Pin_10);
        break;
    }
    case 4: {
        if (state==1)
          GPIO_SetBits(GPIOE, GPIO_Pin_11);
        else GPIO_ResetBits(GPIOE, GPIO_Pin_11);
        break;
    }
    case 5: {
        if (state==1)
          GPIO_SetBits(GPIOE, GPIO_Pin_12);
        else GPIO_ResetBits(GPIOE, GPIO_Pin_12);
        break;
    }
    case 6: {
        if (state==1)
          GPIO_SetBits(GPIOE, GPIO_Pin_13);
        else GPIO_ResetBits(GPIOE, GPIO_Pin_13);
        break;
    } 
    case 7: {
        if (state==1)
          GPIO_SetBits(GPIOE, GPIO_Pin_14);
        else GPIO_ResetBits(GPIOE, GPIO_Pin_14);
        break;
    }
    case 8: {
        if (state==1)
          GPIO_SetBits(GPIOE, GPIO_Pin_15);
        else GPIO_ResetBits(GPIOE, GPIO_Pin_15);
        break;
    }
    case 9: {
        if (state==1)
          GPIO_SetBits(GPIOB, GPIO_Pin_10);
        else GPIO_ResetBits(GPIOB, GPIO_Pin_10);
        break;
    }
    default:
    	  break;
  }
}









void Laser_ADC_GPIO_Conf()	{
  GPIO_InitTypeDef GPIO_InitStruct;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE); //0x00000007
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6; //0x0079
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; // 0x0003
  GPIO_Init(GPIOB, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3; //0x000D	
  GPIO_Init(GPIOC, &GPIO_InitStruct);
}



void Laser_ADC_Init() {
  Laser_ADC_GPIO_Conf();
  ADC_InitTypeDef ADC_InitDef;
  ADC_CommonInitTypeDef ADC_CommonInitDef;
  ADC_InitDef.ADC_ContinuousConvMode = DISABLE;
  ADC_InitDef.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitDef.ADC_ExternalTrigConv = DISABLE;
  ADC_InitDef.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitDef.ADC_NbrOfConversion = 1;
  ADC_InitDef.ADC_ScanConvMode = DISABLE;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  ADC_InitDef.ADC_Resolution = ADC_Resolution_12b;
  ADC_CommonInitDef.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitDef.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitDef.ADC_Prescaler = ADC_Prescaler_Div4;
  ADC_CommonInitDef.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_8Cycles;
  ADC_CommonInit(&ADC_CommonInitDef);
  ADC_Init(ADC1, &ADC_InitDef);
  ADC_Cmd(ADC1, ENABLE);
}


uint16_t Laser_ADC_Read(uint8_t channel) {
  ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_480Cycles);
  ADC_SoftwareStartConv(ADC1);
  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
  return ADC_GetConversionValue(ADC1);
}


float Get_Laser_Current(int channel)
{
  uint8_t i;
  float Current = 0;
  for(i = 0; i < NS; i++)
  {
    Sample_ADC_Raw[i] = Laser_ADC_Read(channel);
  }

  Sort_values(Sample_ADC_Raw, NS);

  ADC_Average = 0;
  for(i = SR/2; i < NS-SR/2; i++)
  {
    ADC_Average += Sample_ADC_Raw[i];
  }
  ADC_Average /= (NS-SR);

  Current = ADC_Average;
  Current *= ADC_CurrentMultiplier;

  Current += offset;

  return Current;
}



// not used in this project
float Laser_ADC_Read_Avg(uint8_t channel, uint8_t num_of_samples) {
  uint32_t avg_sample = 0;
  float result = 0;

  for (int index=0; index<num_of_samples; index++) {
    avg_sample += Laser_ADC_Read(channel);
  }
  result = avg_sample/num_of_samples; 
  result *= ADC_CurrentMultiplier;
  result += offset;

  return  result;
}



void Sort_values(uint16_t A[], uint8_t L)
{
  uint8_t i = 0;
  uint8_t status = 1;
  while(status == 1)
  {
    status = 0;
    for(i = 0; i < L-1; i++)
    {
      if (A[i] > A[i+1])
      {
        A[i]^=A[i+1];
        A[i+1]^=A[i];
        A[i]^=A[i+1];
        status = 1;    
      }
    }
  }
}
