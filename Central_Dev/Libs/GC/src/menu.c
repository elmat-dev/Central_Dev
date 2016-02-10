#include "menu.h"



/*------------------------------ Console Message -----------------------------*/
#define MESSAGE0   "***************************************\n"
#define MESSAGE1   "*     Central Monitoring System       *\n"
#define MESSAGE2   "*      Designed by @Elmat Team        *\n"
#define MESSAGE3   "*          Copyright 2015             *\n"
#define MESSAGE4   "***************************************\n"
/*----------------------------------------------------------------------------*/


short current_adjust_program = 0;

  
Laser_Data_t Laser_Data_Array[10];
  


/* Print Welcome message */
void welcome_message (void)
{
  TM_USART_Puts(MENU_USART,MESSAGE0);
  TM_USART_Puts(MENU_USART,MESSAGE1);
  TM_USART_Puts(MENU_USART,MESSAGE2);
  TM_USART_Puts(MENU_USART,MESSAGE3);
  TM_USART_Puts(MENU_USART,MESSAGE4);
}



/* Command Line Interface print character wrapper function */
void cmdlineOutputInterface(uint8_t c) {
  TM_USART_Putc(MENU_USART,c);
}



/* Command Line Interface setup and initialization */
void Menu_Init(void) {
  /* Pass the pointer to the function used to print characters over user terminal */
  cmdlineSetOutputFunc(cmdlineOutputInterface);
  /* Initialize command line interface library */
  cmdlineInit();
  
  /* Register commands in the command line interface library */
  cmdlineAddCommand("set", set);
  cmdlineAddCommand("show", show);
  cmdlineAddCommand("help", help);
  /* Enable support for VT100 terminal characters and encoding */
  vt100Init(MENU_USART);
  /* Print command line prompt string. String is defined in cmdline.c file as CmdlinePrompt */
  cmdlinePrintPrompt();
}








void current_adj (void) {
  char buffer[50];
  int i = 0;
  //vt100ClearScreen(MENU_USART);
  vt100CursorHome(MENU_USART);

  for (i=0;i<10;i++) {
	  Laser_Data_Array[i].current = Get_Laser_Current(i);
  }
  
    TM_USART_Puts(MENU_USART,"*****************************************\n\r");
    TM_USART_Puts(MENU_USART,"*********LASER CURRENT ADJUSTMENT********\n\r");
    TM_USART_Puts(MENU_USART,"!!        Units in milli Amperes       !!\n\r");
    TM_USART_Puts(MENU_USART,"*****************************************\n\r");
    TM_USART_Puts(MENU_USART,"*   L0  *   L1  *   L2  *   L3  *   L4  *\n\r");
    TM_USART_Puts(MENU_USART,"*****************************************\n\r");
    sprintf(buffer,"* %04.0f  * %04.0f  * %04.0f  * %04.0f  * %04.0f  *\n\r",
    		Laser_Data_Array[0].current,
    		Laser_Data_Array[1].current,
    		Laser_Data_Array[2].current,
    		Laser_Data_Array[3].current,
    		Laser_Data_Array[4].current);
    TM_USART_Puts(MENU_USART,buffer);
    TM_USART_Puts(MENU_USART,"*****************************************\n\r");
    TM_USART_Puts(MENU_USART,"*****************************************\n\r");
    TM_USART_Puts(MENU_USART,"*   L5  *   L6  *   L7  *   L8  *   L9  *\n\r");
    TM_USART_Puts(MENU_USART,"*****************************************\n\r");
    sprintf(buffer,"* %04.0f  * %04.0f  * %04.0f  * %04.0f  * %04.0f  *\n\r",
    		Laser_Data_Array[5].current,
    		Laser_Data_Array[6].current,
    		Laser_Data_Array[7].current,
    		Laser_Data_Array[8].current,
    		Laser_Data_Array[9].current);
    TM_USART_Puts(MENU_USART,buffer);
    TM_USART_Puts(MENU_USART,"*****************************************\n\r");

  TM_USART_Puts(MENU_USART,"\n\r");
}





void help (void)
{
	printf("Currently supported commands are:\n");
	printf("\tshow system       - display basic information about system configuration\n");
	printf("\tshow ddmi [x]     - display DOM information from SFP module installed in [<0-9>] slot of central device\n");
	printf("\tshow laser status - display softwawre status of all lasers (ON/OFF state). Doesn't display status of Keylocks\n");
	printf("\tset laser current - run subprogram for live update of laser current for all enabled and powered lasers\n");
	printf("\t                    !! Please be careful not to exceed maximum current for used lasers: 2300mA !!\n");
	printf("\tset laser [x] on  - enable (in software) laser with given id[<0-9>] !!\n");
	printf("\t                    Please remember to power on/off the driver of given laser by Kelock with same ID\n");
	printf("\tset laser [x] off - disable (in software) laser with given id[<0-9>] !!\n");
	printf("\t                    Please remember to power on/off the driver of given laser by Kelock with same ID\n");
}



void set (void)
{
  char * buffer = cmdlineGetArgStr(1);
  char * parameter;
  int laser_id;

  parameter = strtok(buffer," ");
  if ((!strcmp(parameter, "?")) || (!strcmp(parameter, "/?")) || (!strcmp(parameter, "--help"))) {
	  printf("\nSupported 'set' subcommands:\n");
	  printf("\tlaser current - set current for powered and enabled lasers\n");
	  printf("\t                For this process laser must be enabled (via software) and powered (via keylock)\n");
      printf("\tlaser [x] on    - enables laser [x] ( where x <0-9> )\n");
      printf("\tlaser [x] off   - disables laser [x] ( where x <0-9> ) \n");
  }
  
  else if ((!strcmp(parameter, "laser"))) {
	  parameter = strtok(NULL," ");
	     if ((!strcmp(parameter, "current")))
	     {
	    	 vt100ClearScreen(MENU_USART);
	         vt100CursorHome(MENU_USART);
	         vt100SetCursorMode(MENU_USART,0);
	         current_adjust_program = 1;
	     }
	     else
	     {
	    	 laser_id = atoi(parameter);
	    	 if (laser_id < 0 || laser_id > 9)
	    	 {
	    		 printf("Please provide correct laser ID <0 - 9>\n");
	    	 }
	    	 parameter = strtok(NULL," ");
	    	 if (!strcmp(parameter, "on"))
	    	 {
	    		TM_BKPSRAM_Write8(laser_id, 0xAC);
				Laser_Update();
	    	 }
	    	 else if (!strcmp(parameter, "off"))
	    	 {
	    		 TM_BKPSRAM_Write8(laser_id, 0x00);
	    	 }
	     }
  }
}





void show (void)
{
  char * buffer = cmdlineGetArgStr(1);
  char * parameter;
  int tmp;
  int sfp_id;
  parameter = strtok(buffer," ");
  if ((!strcmp(parameter, "?")) || (!strcmp(parameter, "/?")) || (!strcmp(parameter, "--help")))
  {
	  printf("\nSupported 'show' subcommands:\n");
	  printf("\tshow laser status - show lasers status information\n");
	  printf("\tshow system       - show system basic configuration\n");
	  printf("\tshow ddmi [x]       - show DDMI of local SFP (where [x] <0-9> )\n");
  }

  else if ((!strcmp(parameter, "system")))
  {
	  vt100characterSet(MENU_USART,1);
	  printf("System Informations:\n");
      printf("\tLocal IP address set to static: %d.%d.%d.%d\n",IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
      printf("\tNetwork mask set to static: %d.%d.%d.%d\n",NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
      printf("\tGateway IP address set to static: %d.%d.%d.%d\n",GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
      printf("\tServer is listening for data from remote devices at UDP port number:%d\n",UDP_SERVER_PORT);
      printf("\tGSM connection status:%d\n",GSM_Reg_Status(0));
  }
  
  else if ((!strcmp(parameter, "ddmi")))
  {
	  SFP_Data ddmi;
		parameter = strtok(NULL," ");
		sfp_id = atoi(parameter);
		if ( (sfp_id < 0) || (sfp_id > 9) )
			printf("Please provide correct number of SFP slot <0 - 9>\n");
		else
		{
			get_ddmi_1(&ddmi, sfp_id);
			printf("\t\t+----------+----------+----------+-------------+\n");
			printf("\t| TX (dBm) | RX (dBm) | Temp (C) | Voltage (V) |\n");
			printf("\t+----------+----------+----------+-------------+\n");
			printf("\t| %2.3f   | %2.3f   | %2.2f   | %2.2f       |\n",
				 ddmi.tx_power_dbm,
				 ddmi.rx_power_dbm,
				 ddmi.temp,
				 ddmi.voltage);
			printf("\t+----------+----------+----------+-------------+\n");
		}

  }
  
  else if ((!strcmp(parameter, "laser")))
  {
	  parameter = strtok(NULL," ");
	  if ((!strcmp(parameter, "status")))
	  {
		  printf("\n\n | ID | State |\n --------------\n");
		  for (int i=0;i<10;i++)
		  {
			  tmp = TM_BKPSRAM_Read8(i);
			  if( tmp == 0xAC)
				  printf(" | %02d |    ON |\n",i);
			  else if (tmp == 0x00)
				  printf(" | %02d |   OFF |\n",i);
		  }
      printf(" --------------\n\n");
    }
  }
}



void Menu_Update(void)
{
	char terminal_input;
	terminal_input = TM_USART_Getc(MENU_USART);		// get character from user terminal

	if (terminal_input == 0x03) 					// if CTRL+Z is detected then exit to main menu
	{
		current_adjust_program = 0;					// reset variable to exit current_adj() program
		vt100SetCursorMode(MENU_USART,1);			// show cursor in the terminal
	}
  
	cmdlineInputFunc(terminal_input);				// pass the character received over user termianl to the CLI stack
	cmdlineMainLoop(); 								// process the characters in the CLI stack
  
	if (current_adjust_program)
		current_adj();								// run Current adjustment subroutine
}
