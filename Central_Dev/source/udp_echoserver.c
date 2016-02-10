/*
 ******************************************************************************
 * @file    udp_echoserver.c
 * @author  Elmat Team
 * @version V0.1b
 * @date    31-October-2014
 * @brief   UDP echo server application
 ******************************************************************************
 */
#include "udp_echoserver.h"



/* This variable is used to store DOM Data from local SFP modules*/
SFP_Data local;


void UDP_Server_Init(void)
{
   struct udp_pcb *upcb;
   err_t err;
   /* Create a new UDP control block  */
   upcb = udp_new();
   if (upcb) {
     /* Bind the upcb to the UDP_PORT port */
     /* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
      err = udp_bind(upcb, IP_ADDR_ANY, UDP_SERVER_PORT);
      if (err == ERR_OK) {
        /* Set a receive callback for the upcb */
        udp_recv(upcb, udp_echoserver_receive_callback, NULL);
      } else {
        printf("LwIP UDP_Server_Init: Can't bind PCB\n");
      }
   } else {
     printf("LwIP UDP_Server_Init: Can't create PCB\n");
   }
}










void udp_echoserver_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{
	//char *data = malloc (100 * sizeof(char));
	float tmp;
	int id;
	uint8_t * ptr;
	char * data;
	data = (char*) p->payload;

	/* Get ID, it is the first element in payload, size of uint8_t */
	id = (uint8_t) data[0];

	/* Update DOM reading for local SFP module with same ID as ID of remote device */
	get_ddmi_1(&local,id);

	/* Save ID of remote device to Remote Data Array with location defined by ID number */
	Remote_Data_Array[id].id = id;

	/* Set variable which is checked to indicated that new data was received */
	Remote_Data_Array[id].enabled = 1;

	/* Update timeout counter */
	Remote_Data_Array[id].rcv_system_time = TM_DELAY_Time();

	/* Update remote device SFP TX power, units of [dBm] */
	ptr = (uint8_t*) &data[1];
	Remote_Data_Array[id].tx = mw_2_dbm(byteArray2Float(ptr));

	/* Update remote device SFP RX power, units of [dBm] */
	ptr = (uint8_t*) &data[6];
	Remote_Data_Array[id].rx = mw_2_dbm(byteArray2Float(ptr));

	/* Update remote device SFP temperature, units of [C] */
	ptr = (uint8_t*) &data[10];
	Remote_Data_Array[id].sfp_temp = byteArray2Float(ptr);

	/* Update remote device Li-Ion Batter voltage, units of [V] */
	ptr = (uint8_t*) &data[14];
	Remote_Data_Array[id].v_bat = byteArray2Float(ptr);

	/* Update remote device Temperature from Si7021 [if available], units of [C] */
	ptr = (uint8_t*) &data[18];
	tmp = byteArray2Float(ptr);
    if ( (tmp < -80.0) || ( tmp > 120.0) )
    	tmp = -500.0;
    Remote_Data_Array[id].temp = tmp;

    /* Update remote device Humidity from Si7021 [if available], units of [C] */
    ptr = (uint8_t*) &data[22];
    tmp = byteArray2Float(ptr);
    if ( (tmp < 0.0) || ( tmp > 100.0) )
    	tmp = -500.0;
    Remote_Data_Array[id].humid = tmp;

    /* Update remote device GPS location - latitude */
  	ptr = (uint8_t*) &data[26];
  	tmp = byteArray2Float(ptr);
    if ( (tmp < -90.0 ) || ( tmp > 90.0) )
    	tmp = -500.0;
  	Remote_Data_Array[id].latitude = tmp;

  	/* Update remote device GPS location - longitude */
  	ptr = (uint8_t*) &data[30];
  	tmp = byteArray2Float(ptr);
    if ( (tmp < -180.0 ) || ( tmp > 180.0) )
    	tmp = -500.0;
    Remote_Data_Array[id].longitude = tmp;

    /* Update remote device laser status */
    Remote_Data_Array[id].laser = (uint8_t) data[34];

    /* Check if remote device has sent request to enable Power Laser of give ID */
    Remote_Data_Array[id].laser_enable_remote_request = (uint8_t) data[35];

    /* Update optical power received by Power Over Fiber Optical receiver */
    ptr = (uint8_t*) &data[37];
    tmp = byteArray2Float(ptr);
    Remote_Data_Array[id].optical_power = tmp;


    /* If remote device doesn't receive power via power fiber then disable laser and write state to BKP RAM memory */
    if (Remote_Data_Array[id].laser == 0)
    {
    	TM_BKPSRAM_Write8(id,0);
    	Laser_Update();
    }
    /* If remote device has sent request to enable laser for power fiber then enable laser and write state to BKP RAM memory */
    if (Remote_Data_Array[id].laser_enable_remote_request == 'L')
    {
    	TM_BKPSRAM_Write8(id,0xAC);
    	Laser_Update();
    }
    //udp_connect(upcb, addr, UDP_CLIENT_PORT);
    /* Tell the client that we have accepted it */
    //udp_send(upcb, p);
    /* free the UDP connection, so we can accept new clients */
    //udp_disconnect(upcb);
    /* Free the p buffer */
    pbuf_free(p);
  	//free(data);
}




void print_results(void)
{
	char print_data[256];
	uint32_t timeout = TM_DELAY_Time();

	for (int i=0;i<10;i++)
	{
		if (timeout > Remote_Data_Array[i].rcv_system_time + DEVICE_AGING_INTERVAL)
			Remote_Data_Array[i].enabled = 0;	// if there is no new packet for 25 seconds then stop printing data from that device
		if (Remote_Data_Array[i].enabled == 1)  // if the new data was received within 25 seconds, then print results on the Data Interface
		{
			sprintf(print_data, "%d\t%2.5f\t%2.5f\t%2.2f\t%2.2f\t%2.2f\t%2.2f\t%3.6f\t%3.6f\t%d\t%2.2f\n",
				Remote_Data_Array[i].id,
				Remote_Data_Array[i].tx,
				Remote_Data_Array[i].rx,
				Remote_Data_Array[i].sfp_temp,
				Remote_Data_Array[i].v_bat,
				Remote_Data_Array[i].temp,
				Remote_Data_Array[i].humid,
				Remote_Data_Array[i].latitude,
				Remote_Data_Array[i].longitude,
				Remote_Data_Array[i].laser,
				Remote_Data_Array[i].optical_power);
			TM_USART_Puts(OUTPUT_USART,print_data);
		}
	}
}



float byteArray2Float (uint8_t * bytes) {
  FloatOrUInt tmp;
  for (int i=0;i<4;i++)
    tmp.fInt[i]= *bytes++;
return tmp.f;
}
