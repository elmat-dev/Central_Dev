#include "utilities_new.h"






short GC_BKPSRAM_StringWrite (char * str, short start, short length) {

  while (length--) {
    TM_BKPSRAM_Write8(start++,*str++);
  }
  TM_BKPSRAM_Write8(start,0);
  return length; // should be equal to 0 when all chars are written
}

short GC_BKPSRAM_StringRead (char * buffer, short start, short length) {
  short i=0;
  while (i <= (length)) {
    buffer[i] = TM_BKPSRAM_Read8(start++);
    if (buffer[i] == 0) {
      i++;
      break;
    }
    else i++;
  }
  buffer[i]=0;
  return i; // should be equal to 0 when all chars are written
}

char* GC_BKPSRAM_StringReadPtr (short start, short length) {
  short i=0;
  char *str = malloc(sizeof(char) * length+1);
  if (str == NULL)
    TM_USART_Puts(MENU_USART,"Can't allocate memory for return string\n\r");
  else {
    while (i <= (length)) {
      *str++ = TM_BKPSRAM_Read8(start++);
      if (*str == 0) {
        i++;
        break;
      }
      else i++;
    }
  *str=0;
  }
  return str; // should be equal to 0 when all chars are written
}




int setPhoneNumber (int number) {
  char str_number[10]; 
  sprintf(str_number,"%d",number); // int to string
  for (int i=0;i<9;i++)
    TM_BKPSRAM_Write8(i,str_number[i]); 
  TM_BKPSRAM_Write8(0x09, 0x01); //set flat to indicate that number is set  
  
  return 0;
}


int getPhoneNumberAsInt (void) {
  char str_number[10];
  for (int i=0;i<9;i++)
  str_number[i] = TM_BKPSRAM_Read8(i); 
  
  return atoi(str_number); // string to int
}

int getPhoneNumberAsStr (char* number) {

  for (int i=0;i<9;i++)
  (*number++) = TM_BKPSRAM_Read8(i); 
  
  return 0;
}

char* getPhoneNumberAsStrPtr (void) {
  char *str = malloc(sizeof(char) * 9+1);
  if (str == NULL)
    TM_USART_Puts(MENU_USART,"Can't allocate memory for return string\n\r");
  else {
  for (int i=0;i<9;i++)
  (*str++) = TM_BKPSRAM_Read8(i); 
  }
  *str=0;
  return str;
}

















float uintToFloat (uint8_t u0, uint8_t u1, uint8_t u2, uint8_t u3) {
  FloatOrUInt temp;
  temp.fInt[3]=u3;
  temp.fInt[2]=u2;
  temp.fInt[1]=u1;
  temp.fInt[0]=u0;
return temp.f;
}

void floatToUint (float f, uint8_t* u0, uint8_t* u1, uint8_t* u2, uint8_t* u3) {
  FloatOrUInt temp;
  temp.f=f;
  *u0=temp.fInt[0];
  *u1=temp.fInt[1];
  *u2=temp.fInt[2];
  *u3=temp.fInt[3];
}











int loadMACaddress(macAddress* mac) {
  mac->address1=TM_BKPSRAM_Read8(1);
  mac->address2=TM_BKPSRAM_Read8(2);
  mac->address3=TM_BKPSRAM_Read8(3);
  mac->address4=TM_BKPSRAM_Read8(4);
  mac->address5=TM_BKPSRAM_Read8(5);
  mac->address6=TM_BKPSRAM_Read8(6);
  return 0;
}

int saveMACaddress(macAddress mac) {
  TM_BKPSRAM_Write8(1,mac.address1);
  TM_BKPSRAM_Write8(2,mac.address2);
  TM_BKPSRAM_Write8(3,mac.address3);
  TM_BKPSRAM_Write8(4,mac.address4);
  TM_BKPSRAM_Write8(5,mac.address5);
  TM_BKPSRAM_Write8(6,mac.address6);  
  return 0;
}



uint8_t loadIP(IPAddress* address, short position) {
  address->ip1 = TM_BKPSRAM_Read8(position++);
  address->ip2 = TM_BKPSRAM_Read8(position++);
  address->ip3 = TM_BKPSRAM_Read8(position++);
  address->ip4 = TM_BKPSRAM_Read8(position++);
  address->port = TM_BKPSRAM_Read8(position);
return 0; 
}

uint8_t saveIP(IPAddress* address, short position) {
  TM_BKPSRAM_Write8(position++, address->ip1);
  TM_BKPSRAM_Write8(position++, address->ip2);
  TM_BKPSRAM_Write8(position++, address->ip3);
  TM_BKPSRAM_Write8(position++, address->ip4);
  TM_BKPSRAM_Write8(position, address->port);
return 0; 
}



int loadConfig (Device_Config * config) {
  uint8_t temp[4];
  
  config->ID=getID();
  
  loadMACaddress(&config->macaddr);
  
  loadIP(&config->localip,7);
  loadIP(&config->remoteip,12);
  loadIP(&config->netmask,17);
  loadIP(&config->gateway,22);
  
  temp[0]=TM_BKPSRAM_Read8(27);
  temp[1]=TM_BKPSRAM_Read8(28);
  temp[2]=TM_BKPSRAM_Read8(29);
  temp[3]=TM_BKPSRAM_Read8(30);
  config->latitude = uintToFloat(temp[0],temp[1],temp[2],temp[3]);
  
  temp[0]=TM_BKPSRAM_Read8(31);
  temp[1]=TM_BKPSRAM_Read8(32);
  temp[2]=TM_BKPSRAM_Read8(33);
  temp[3]=TM_BKPSRAM_Read8(34);
  config->longitude = uintToFloat(temp[0],temp[1],temp[2],temp[3]);
  
  for (int i=0;i<12;i++) config->phone_number[i]= TM_BKPSRAM_Read8(35 + i);

return 0;
}


int saveConfig(Device_Config config) {
  TM_BKPSRAM_Write8(0x0A, config.ID);
  //TM_BKPSRAM_Write8(0x0B, 0x01);
  saveMACaddress(config.macaddr);
  
  saveIP(&config.localip,7);
  saveIP(&config.remoteip,12);
  saveIP(&config.netmask,17);
  saveIP(&config.gateway,22);
  
  for (int i=0;i<12;i++) TM_BKPSRAM_Write8(35 + i, config.phone_number[i]);
  //saveIPaddress(config.localip, config.netmask, config.gateway, config.remoteip);
  return 0;
}








//////////////// IP ADDRESS FUNCTIONS //////////////////////////////


/* Reads IP addresses from BKP SRAM  to IP structures*/
void loadIPaddress(IPAddress* local, IPAddress* mask, IPAddress* gw, IPAddress* serv) {
  loadIP(local,7);
  loadIP(serv,12);
  loadIP(mask,17);
  loadIP(gw,22);
  
  /*
  local->ip1=TM_BKPSRAM_Read8(7);
  local->ip2=TM_BKPSRAM_Read8(8);
  local->ip3=TM_BKPSRAM_Read8(9);
  local->ip4=TM_BKPSRAM_Read8(10);
  local->port=TM_BKPSRAM_Read8(11);
  
  serv->ip1=TM_BKPSRAM_Read8(12);
  serv->ip2=TM_BKPSRAM_Read8(13);
  serv->ip3=TM_BKPSRAM_Read8(14);
  serv->ip4=TM_BKPSRAM_Read8(15);
  serv->port=TM_BKPSRAM_Read8(16);
  
  mask->ip1=TM_BKPSRAM_Read8(17);
  mask->ip2=TM_BKPSRAM_Read8(18);
  mask->ip3=TM_BKPSRAM_Read8(19);
  mask->ip4=TM_BKPSRAM_Read8(20);
  
  gw->ip1=TM_BKPSRAM_Read8(22);
  gw->ip2=TM_BKPSRAM_Read8(23);
  gw->ip3=TM_BKPSRAM_Read8(24);
  gw->ip4=TM_BKPSRAM_Read8(25);
  */
}

/* Writes IP addresses from structures to BKP SRAM */
void saveIPaddress(IPAddress local, IPAddress mask, IPAddress gw, IPAddress serv) {
  TM_BKPSRAM_Write8(7,local.ip1);
  TM_BKPSRAM_Write8(8,local.ip2);
  TM_BKPSRAM_Write8(9,local.ip3);
  TM_BKPSRAM_Write8(10,local.ip4);
  TM_BKPSRAM_Write8(11,local.port);
  
  TM_BKPSRAM_Write8(12,serv.ip1);
  TM_BKPSRAM_Write8(13,serv.ip2);
  TM_BKPSRAM_Write8(14,serv.ip3);
  TM_BKPSRAM_Write8(15,serv.ip4);
  TM_BKPSRAM_Write8(16,serv.port);

  TM_BKPSRAM_Write8(17,mask.ip1);
  TM_BKPSRAM_Write8(18,mask.ip2);
  TM_BKPSRAM_Write8(19,mask.ip3);
  TM_BKPSRAM_Write8(20,mask.ip4);

  TM_BKPSRAM_Write8(21,gw.ip1);
  TM_BKPSRAM_Write8(22,gw.ip2);
  TM_BKPSRAM_Write8(23,gw.ip3);
  TM_BKPSRAM_Write8(24,gw.ip4);
}







