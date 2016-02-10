/*
 ******************************************************************************
 * @file    sfp.c
 * @author  Elmat Team
 * @version V0.1b
 * @date    31-October-2014
 * @brief   Library for SFP module support
 ******************************************************************************
 */

#include "sfp.h"






int sfpInit(void){
  TM_I2C_Init(SFP_I2C_PORT, SFP_I2C_PINS, SFP_I2C_SPEED);
  if (TM_I2C_IsDeviceConnected(SFP_I2C_PORT, 0xE8) && TM_I2C_IsDeviceConnected(SFP_I2C_PORT, 0xEC)) return 0;
  else return -1;
}

int selecPortSFP (uint8_t port) {
  if ( (TM_I2C_IsDeviceConnected(SFP_I2C_PORT, 0xE8) == 0 ) || (TM_I2C_IsDeviceConnected(SFP_I2C_PORT, 0xEC) == 0) )
    return -1;
  
  else {
      TM_I2C_WriteNoRegister(SFP_I2C_PORT, 0xE8, 0x00);
      TM_I2C_WriteNoRegister(SFP_I2C_PORT, 0xEC, 0x00);
   switch (port) {
    case 0: 	{
        TM_I2C_WriteNoRegister(SFP_I2C_PORT, 0xE8, 0x01);
        if (TM_I2C_IsDeviceConnected(SFP_I2C_PORT, 0xA2)) return 10;
        break;
    }
    case 1:     {
        TM_I2C_WriteNoRegister(SFP_I2C_PORT, 0xE8, 0x02);
        if (TM_I2C_IsDeviceConnected(SFP_I2C_PORT, 0xA2)) return 10;
        break;
    }   
    case 2:     {
        TM_I2C_WriteNoRegister(SFP_I2C_PORT, 0xE8, 0x04);
        if (TM_I2C_IsDeviceConnected(SFP_I2C_PORT, 0xA2)) return 10;
        break;
    } 
    case 3:     {
        TM_I2C_WriteNoRegister(SFP_I2C_PORT, 0xE8, 0x08);
        if (TM_I2C_IsDeviceConnected(SFP_I2C_PORT, 0xA2)) return 10;
        break;
    } 
    case 4:     {
        TM_I2C_WriteNoRegister(SFP_I2C_PORT, 0xE8, 0x10);
        if (TM_I2C_IsDeviceConnected(SFP_I2C_PORT, 0xA2)) return 10;
        break;
    }
    case 5:     {
        TM_I2C_WriteNoRegister(SFP_I2C_PORT, 0xE8, 0x20);
        if (TM_I2C_IsDeviceConnected(SFP_I2C_PORT, 0xA2)) return 10;
        break;
    }
    case 6:     {
        TM_I2C_WriteNoRegister(SFP_I2C_PORT, 0xE8, 0x40);
        if (TM_I2C_IsDeviceConnected(SFP_I2C_PORT, 0xA2)) return 10;
        break;
    }
    case 7:     {
        TM_I2C_WriteNoRegister(SFP_I2C_PORT, 0xE8, 0x80);
        if (TM_I2C_IsDeviceConnected(SFP_I2C_PORT, 0xA2)) return 10;
        break;
    }
    case 8:     {
        TM_I2C_WriteNoRegister(SFP_I2C_PORT, 0xEC, 0x01);
        if (TM_I2C_IsDeviceConnected(SFP_I2C_PORT, 0xA2)) return 10;
        break;
    }
    case 9:     {
        TM_I2C_WriteNoRegister(SFP_I2C_PORT, 0xEC, 0x02);
        if (TM_I2C_IsDeviceConnected(SFP_I2C_PORT, 0xA2)) return 10;
        break;
    }
    case 100:   {
      TM_I2C_WriteNoRegister(SFP_I2C_PORT, 0xE8, 0x00);
      TM_I2C_WriteNoRegister(SFP_I2C_PORT, 0xEC, 0x00);
        break;
    }    
    default:	{
      TM_I2C_WriteNoRegister(SFP_I2C_PORT, 0xE8, 0x00);
      TM_I2C_WriteNoRegister(SFP_I2C_PORT, 0xEC, 0x00);
       break;
    }      
  }
  }
 return 0;
}

float mw_2_dbm (float raw_value) {
  return 10 * log10(raw_value);
}

float dbm_2_mw (float raw_value) {
  return pow(10,raw_value/10);
}


float SFP_DDMI_RAW2Float (SFP_READ_DDMI_Type_t data_type, uint16_t raw_value) {
	float value =  -50000.0;
	switch (data_type) {
		case temperature: {
			if (raw_value > 32767)
				raw_value -= 65536;
			value=raw_value/256.000;
			break;
			
		}
		case voltage: {
			value = raw_value/10000.000;
			break;
		}
		case current: {
			value = raw_value/500.000;
			break;
		}
		case tx_mw: {
			value = raw_value/10000.000;
			break;
		}
		case rx_mw: {
			value = raw_value/10000.000;
			break;
		}		
		case tx_dbm: {
			value = 10 * (log10(raw_value/10000.000));
			break;
		}
		case rx_dbm: {
			value = 10*(log10(raw_value/10000.000));
			break;
		}		
	}
return value;
}

void SFP_READ_DDMI_All_8bit (uint8_t* ddmi_buffer) {
  TM_I2C_ReadMulti(SFP_I2C_PORT, SFP_I2C_ADDRESS, temperature, ddmi_buffer, 10);
  TM_I2C_ReadMulti(SFP_I2C_PORT, SFP_I2C_ADDRESS, temperature, ddmi_buffer, 10);
}

void SFP_READ_DDMI_All_16bit (uint16_t* ddmi_buffer) {
    uint8_t buffer[10];
	SFP_READ_DDMI_All_8bit(buffer);
	for (int i=0; i<10; i+=2)
	*ddmi_buffer++ = (buffer[i] << 8) | buffer[i+1];	
}

float SFP_READ_DDMI_Single_float (SFP_READ_DDMI_Type_t type) {
  uint8_t buffer[2];
  TM_I2C_ReadMulti(SFP_I2C_PORT, SFP_I2C_ADDRESS, type, buffer, 2);
  TM_I2C_ReadMulti(SFP_I2C_PORT, SFP_I2C_ADDRESS, type, buffer, 2);
  return SFP_DDMI_RAW2Float(type,((buffer[0] << 8) | buffer[1]));
}

uint16_t SFP_READ_DDMI_Single_uint16 (SFP_READ_DDMI_Type_t type) {
  uint8_t buffer[2];
  TM_I2C_ReadMulti(SFP_I2C_PORT, SFP_I2C_ADDRESS, type, buffer, 2);
  TM_I2C_ReadMulti(SFP_I2C_PORT, SFP_I2C_ADDRESS, type, buffer, 2);
  return ((buffer[0] << 8) | buffer[1]);
}



int get_ddmi_1(SFP_Data * ddmi, uint8_t port) {
        ddmi->present=0;
        uint16_t buffer[8];
        int test=0;
        float tmp_tx_pwr = 0;
        if ( selecPortSFP(port) == -1) { 
          ddmi->present = 0;
          return -1;
        }
        if (!TM_I2C_IsDeviceConnected(SFP_I2C_PORT, 0xA2)){ 
          ddmi->present = -2;
          return -2;
        }
        else {
          ddmi->temp = SFP_READ_DDMI_Single_float(temperature);
          ddmi->voltage = SFP_READ_DDMI_Single_float(voltage);
          ddmi->current = SFP_READ_DDMI_Single_float(current);
          ddmi->rx_power_mw = SFP_READ_DDMI_Single_float(rx_mw);
          
          for (int i=0;i<8;i++) {
                buffer[i]=SFP_READ_DDMI_Single_uint16(tx_mw);
                //Delayms(70);
          }
                //heapsort(buffer,8);
          
                for (int i=0;i<8;i++) test += buffer[i];
                  //printf("wartosc buffora %d\n\r",buffer[i]);
                  //printf("srednia rx %2.3f\n\r",mw_2_dbm(SFP_DDMI_RAW2Float(rx_mw,test/8)));
                  
                  
                
//            float tolerance = 0.03;
//            float tx_tmp=0.0;
//            float tx_new=0.0;
//            tx_tmp = SFP_READ_DDMI_Single_float(rx_mw);
//
//            printf("wartosc ddmi odczyt %2.5f\n\r",i,tx_tmp);
//            tmp_tx_pwr+=tmp;
//          }
//            tmp_tx_pwr/=1;
          //printf("wartosc ddmi po usrednieniu : %2.5fdBm\n\r",mw_2_dbm(tmp_tx_pwr));
          //printf("%2.2f\n\r",mw_2_dbm( ddmi->rx_power_mw));
          //ddmi->tx_power_mw = SFP_READ_DDMI_Single_float(tx_mw);
          ddmi->tx_power_mw = tmp_tx_pwr;
          ddmi->tx_power_mw = SFP_READ_DDMI_Single_float(tx_mw);
          
          ddmi->tx_power_dbm = mw_2_dbm(ddmi->tx_power_mw);
          ddmi->rx_power_dbm = mw_2_dbm(ddmi->rx_power_mw);
          
          ddmi->present = 1;    
        }
  return 0;
}




int get_ddmi_2(SFP_Data * ddmi, uint8_t port) {
	uint16_t tmp[5];
        ddmi->present=0;
        
        if ( selecPortSFP(port) == -1) { 
          ddmi->present = 0;
          return -1;
        }
        if (!TM_I2C_IsDeviceConnected(SFP_I2C_PORT, 0xA2)){ 
          ddmi->present = -2;
          return -2;
        }
          else {
            SFP_READ_DDMI_All_16bit(tmp);
            ddmi->temp = SFP_DDMI_RAW2Float(temperature,tmp[0]);
            ddmi->voltage = SFP_DDMI_RAW2Float(voltage,tmp[1]);
            ddmi->current = SFP_DDMI_RAW2Float(current,tmp[2]);
            ddmi->tx_power_dbm = SFP_DDMI_RAW2Float(tx_dbm,tmp[3]);
            ddmi->rx_power_dbm = SFP_DDMI_RAW2Float(rx_dbm,tmp[4]);
            ddmi->tx_power_mw = SFP_DDMI_RAW2Float(tx_mw,tmp[3]);
            ddmi->rx_power_mw = SFP_DDMI_RAW2Float(rx_mw,tmp[4]);
            ddmi->present = 1;
          }
    return 0;
}


void show_ddmi(short module_id) {
 SFP_Data ddmi1;
 SFP_Data ddmi2;
 
 if(get_ddmi_1(&ddmi1,module_id) == 0)
   printf("read ddmi by port: %2.2f, %2.2f, TX: %2.2f dBm (%2.2fmW), RX: %2.3fdBm (%2.3fmW)\n\r", ddmi1.temp,ddmi1.voltage, ddmi1.tx_power_dbm,ddmi1.tx_power_mw,ddmi1.rx_power_dbm,ddmi1.rx_power_mw ); 
 else    printf("Module DDMI 1 not found\n\r");  
 
 if(get_ddmi_2(&ddmi2,module_id) == 0)
   printf("read ddmi by port: %2.2f, %2.2f,  %2.2f, %2.2f\n\r", ddmi2.temp,ddmi2.voltage, ddmi2.tx_power_dbm,ddmi2.tx_power_mw); 
 else    printf("Module DDMI 2 not found\n\r");

}
