//What is the use of getPort Function?//

#include "stdafx.h"
#include "controllerdefs.h"
#include "Motor_Serial_Comm.h"

HANDLE OpenRS232(const char* ComName, DWORD BaudRate)
{
	HANDLE ComHandle;
	DCB CommDCB;
	COMMTIMEOUTS CommTimeouts;

	ComHandle=CreateFile((CString)ComName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(GetLastError()!=ERROR_SUCCESS) return INVALID_HANDLE_VALUE;
	else
	{
		GetCommState(ComHandle, &CommDCB);

		CommDCB.BaudRate=BaudRate;
		CommDCB.Parity=NOPARITY;
		CommDCB.StopBits=ONESTOPBIT;
		CommDCB.ByteSize=8;

		CommDCB.fBinary=1;  //Binary Mode only
		CommDCB.fParity=0;
		CommDCB.fOutxCtsFlow=0;
		CommDCB.fOutxDsrFlow=0;
		CommDCB.fDtrControl=0;
		CommDCB.fDsrSensitivity=0;
		CommDCB.fTXContinueOnXoff=0;
		CommDCB.fOutX=0;
		CommDCB.fInX=0;
		CommDCB.fErrorChar=0;
		CommDCB.fNull=0;
		CommDCB.fRtsControl=RTS_CONTROL_TOGGLE;
		CommDCB.fAbortOnError=0;

		SetCommState(ComHandle, &CommDCB);

		//Set buffer size
		SetupComm(ComHandle, 100, 100);

		//Set up timeout values (very important, as otherwise the program will be very slow)
		GetCommTimeouts(ComHandle, &CommTimeouts);

		CommTimeouts.ReadIntervalTimeout=MAXDWORD;
		CommTimeouts.ReadTotalTimeoutMultiplier=0;
		CommTimeouts.ReadTotalTimeoutConstant=0;

		SetCommTimeouts(ComHandle, &CommTimeouts);

		return ComHandle;
  }
}

ConnReply GetResult(HANDLE Handle)
{
	UCHAR RxBuffer[9];
	DWORD Errors, BytesRead;
	COMSTAT ComStat;
	ConnReply reply;

	ClearCommError(Handle, &Errors, &ComStat);
    if(ComStat.cbInQue>8)
	{
		ReadFile(Handle, RxBuffer, 9, &BytesRead, NULL);						//NEED TO TAKE CARE OF THE NUMBER OF BYTES OVERLAPPED WHICH HAPPENS IN EXTREME SITUATIONS

		reply.reply_address  =  RxBuffer[0];
		reply.module_address = RxBuffer[1];
		reply.status   =  RxBuffer[2];
		reply.value    = (RxBuffer[4] << 24) | (RxBuffer[5] << 16) | (RxBuffer[6] << 8) | RxBuffer[7];
		reply.checksum = RxBuffer[8];
	} 
	else{
		reply.status = TMCL_STATUS_INVALID_VALUE;
		return reply;
	}

	return reply;
}

BOOL SendCmd(HANDLE Handle, Motor *motor, ConnCommand cmd)
{
	UCHAR TxBuffer[9];
	DWORD BytesWritten;
	int i;

	TxBuffer[0]=motor->address;
	TxBuffer[1]=cmd.command;
	TxBuffer[2]=cmd.type;
	TxBuffer[3]=motor->number;
	TxBuffer[4]=cmd.value >> 24;
	TxBuffer[5]=cmd.value >> 16;
	TxBuffer[6]=cmd.value >> 8;
	TxBuffer[7]=cmd.value & 0xff;
	TxBuffer[8]=0;
	for(i=0; i<8; i++)
		TxBuffer[8]+=TxBuffer[i];

	//Send the datagram//
	//CHECK WHETHER DATA IS WRITTEN CORRECTLY OR NOT// PROBABLE SEND THE &BYTESWRITTEN
	WriteFile(Handle, TxBuffer, 9, &BytesWritten, NULL);

	return 1;
}

//Close the serial port
//Usage: CloseRS232(ComHandle);
void CloseRS232(HANDLE Handle)
{
	CloseHandle(Handle);
}
