
#ifndef CMDLINE_H
#define CMDLINE_H



// constants/macros/typdefs
typedef void (*CmdlineFuncPtrType)(void);

// functions

//! initalize the command line system
void cmdlineInit(void);

//! add a new command to the database of known commands
// newCmdString should be a null-terminated command string with no whitespace
// newCmdFuncPtr should be a pointer to the function to execute when
//   the user enters the corresponding command tring
void cmdlineAddCommand(char* newCmdString, CmdlineFuncPtrType newCmdFuncPtr);

//! sets the function used for sending characters to the user terminal
void cmdlineSetOutputFunc(void (*output_func)(unsigned char c));

//! call this function to pass input charaters from the user terminal
void cmdlineInputFunc(unsigned char c);

//! call this function in your program's main loop
void cmdlineMainLoop(void);

// internal commands
void cmdlineRepaint(void);
void cmdlineDoHistory(uint8_t action);
void cmdlineProcessInputString(void);
void cmdlinePrintPrompt(void);
void cmdlinePrintError(void);
void cmdlineHelp(void);
// argument retrieval commands
//! returns a string pointer to argument number [argnum] on the command line
char* cmdlineGetArgStr(uint8_t argnum);
//! returns the decimal integer interpretation of argument number [argnum]
long cmdlineGetArgInt(uint8_t argnum);
//! returns the hex integer interpretation of argument number [argnum]
long cmdlineGetArgHex(uint8_t argnum);

#endif