#include "menu.h"

// definition commands word
#define _CMD_HELP   "help"
#define _CMD_GET    "get"
#define _CMD_SET    "set"
#define _CMD_SHOW    "show"


// arguments for get/set
	#define _SCMD_TIME  "time"
	#define _SCMD_DATA  "data"

// arguments for show
	#define _SHOW_SYS  "system"
	#define _SHOW_DDM  "ddmi"



#define _NUM_OF_CMD 4
#define _NUM_OF_GET_SET_SCMD 2
#define _NUM_OF_SHOW_SCMD 2
//available  commands
char * keyworld [] = {_CMD_HELP, _CMD_GET, _CMD_SET, _CMD_SHOW};
// 'set/clear' command argements
char * get_set_key [] = {_SCMD_TIME, _SCMD_DATA};
// 'show' command argements
char * show_key [] = {_SHOW_SYS, _SHOW_DDM};
// array for comletion
char * compl_world [_NUM_OF_CMD + 1];




microrl_t rl;
microrl_t * prl = &rl;

void help(void) {
  for (int i=0;i<_NUM_OF_CMD;i++)
    TM_USART_Puts(DEBUG_USART,keyworld[i]);
}
  
void set_time(int argc, const char * const * argv) {
  int tmp = 0;
  TM_RTC_Time_t time;
  TM_RTC_GetDateTime(&time, TM_RTC_Format_BIN);


  if (argc <= 4) {
    printf("You should provide additional arguments such as hours, minutes, seconds, example: set time 15 28 51 will set time to 15:28:51 [hh:mm:ss]\n\r");
  }
    
  if (argc == 5) {
    tmp=atoi(argv[2]);
      if(tmp>=0 && tmp <=24)
        time.hours=tmp;
      else printf("Argument %s sohuld represent hour in 24-hours format\n\r",argv[2]);
      
      tmp=atoi(argv[3]);
      if(tmp>=0 && tmp <=59)
        time.minutes=tmp;
      else printf("Argument %s sohuld represent minutes [0 - 59]\n\r",argv[3]);    
      
      tmp=atoi(argv[4]);
      if(tmp>=0 && tmp <=59)
        time.seconds=tmp;
      else printf("Argument %s sohuld represent seconds [0 - 59]\n\r",argv[4]);      
  }
  TM_RTC_SetDateTime(&time, TM_RTC_Format_BIN);

}
  
void time(void) {
  char buffer[50];
  TM_RTC_Time_t datatime;
  TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN);
  sprintf(buffer,"Current time: %02d:%02d:%02d, %02d.%02d.%02dr.\n\r",
          datatime.hours, datatime.minutes, datatime.seconds,
          datatime.date, datatime.month, datatime.year);
  TM_USART_Puts(DEBUG_USART,buffer);
}



int execute (int argc, const char * const * argv)
{
	int i = 0;
	// just iterate through argv word and compare it with your commands
	for (i=0; i<argc; i++) {
          
          if (strcmp (argv[i], _CMD_HELP) == 0) { 
            help();
            return 0;
          }
          
          
          else if (strcmp (argv[i], _CMD_SET) == 0) {
            if (strcmp (argv[i+1], _SCMD_TIME) == 0) {
              set_time(argc,argv);
              return 0;
            }
          }
          
          
          else if (strcmp (argv[i], _CMD_GET) == 0) {
            if (strcmp (argv[i+1], _SCMD_TIME) == 0) {
              time();
              return 0;
            }
          }
          else 	printf ("command: %s not found\n\r",(char*)argv[i]);
	}     
  return 0;
}


void print_cli(const char * ch) {   
    TM_USART_Puts(DEBUG_USART,(char*)ch);
}




char ** complete (int argc, const char * const * argv)
{
	int j = 0;

	compl_world [0] = NULL;

	// if there is token in cmdline
	if (argc == 1) {
		// get last entered token
		char * bit = (char*)argv [argc-1];
		// iterate through our available token and match it
		for (int i = 0; i < _NUM_OF_CMD; i++) {
			// if token is matched (text is part of our token starting from 0 char)
			if (strstr(keyworld [i], bit) == keyworld [i]) {
				// add it to completion set
				compl_world [j++] = keyworld [i];
			}
		}
	}	
        else if ((argc > 1) && ((strcmp (argv[0], _CMD_GET)==0) || (strcmp (argv[0], _CMD_SET)==0))) { // if command needs subcommands
		// iterate through subcommand
		for (int i = 0; i < _NUM_OF_GET_SET_SCMD; i++) {
			if (strstr (get_set_key [i], argv [argc-1]) == get_set_key [i]) {
				compl_world [j++] = get_set_key [i];
			}
		}
          
	}
        else if ((argc > 1) && (strcmp (argv[0], _CMD_SHOW)==0)) { // if command needs subcommands
		// iterate through subcommand
		for (int i = 0; i < _NUM_OF_SHOW_SCMD; i++) {
			if (strstr (show_key [i], argv [argc-1]) == show_key [i]) {
				compl_world [j++] = show_key [i];
			}
		}
          
	}          
        
        else { // if there is no token in cmdline, just print all available token
		for (; j < _NUM_OF_CMD; j++) {
			compl_world[j] = keyworld [j];
		}
	}

	// note! last ptr in array always must be NULL!!!
	compl_world [j] = NULL;
	// return set of variants
	return compl_world;
}




void menu(void) {
  
  microrl_init (prl, print_cli);
  microrl_set_execute_callback (prl, execute);
  microrl_set_complete_callback (prl, complete);
  char d;

  while (1) {
    d=TM_USART_Getc(DEBUG_USART);
    if(d == 0x03) {
      printf("\n\rCTRL+C was detected, leaving menu\n\r");
      break;
    }
    Delay(100);
    microrl_insert_char (prl, d);
  }
}