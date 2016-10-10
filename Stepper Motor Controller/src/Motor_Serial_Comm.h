#pragma once
#include "controllerdefs.h"

//Open serial interface
//Usage: ComHandle=OpenRS232("COM1", CBR_9600)
HANDLE OpenRS232(const char* ComName, DWORD BaudRate);

//Read the result that is returned by the module
//Parameters: Handle: handle of the serial port, as returned by OpenRS232
//Return value: 
ConnReply GetResult(HANDLE Handle);

//Send a binary TMCL command
//Parameters: Handle: Handle of the serial port (returned by OpenRS232).
BOOL SendCmd(HANDLE Handle, Motor *motor, ConnCommand cmd);

//Close the serial port
//Usage: CloseRS232(ComHandle);
void CloseRS232(HANDLE Handle);
