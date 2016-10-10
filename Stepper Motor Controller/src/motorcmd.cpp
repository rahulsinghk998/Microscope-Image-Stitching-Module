#include "stdafx.h"
#include <iostream>
#include "motorcmd.h"
#include "controllerdefs.h"
#include "Motor_Serial_Comm.h"

//Need to Fix the Repetion of the HANDLE initialization in every function//

int MoveToAbsPos(Motor *motor, int position)
{
	HANDLE portHandle;
	ConnCommand cmd;
	ConnReply reply;

	cmd.command = TMCL_MVP;
	cmd.type	= TMCL_MVP_ABS;
	cmd.value	= position;

	portHandle = OpenRS232(MOTOR_COM_PORT, BAUD_RATE);
	if (portHandle == INVALID_HANDLE_VALUE){
		DEBUG("ERROR OPENING COM PORT");
	}
	else{
		SendCmd(portHandle, motor, cmd);
		Sleep(10);
		reply = GetResult(portHandle);
		//if error=1 return 0;
	}
	CloseRS232(portHandle);
	return 1;
}

int SetMaxPosSpeed(Motor *motor, int velocity)
{
	HANDLE portHandle;
	ConnCommand cmd;
	ConnReply reply;

	cmd.command = TMCL_SAP;
	cmd.type	= TMCL_AP_MAX_POS_SPEED;
	cmd.value	= velocity;

	portHandle = OpenRS232(MOTOR_COM_PORT, BAUD_RATE);
	if (portHandle == INVALID_HANDLE_VALUE){
		DEBUG("ERROR OPENING COM PORT");
	}
	else{
		SendCmd(portHandle, motor, cmd);
		Sleep(10);
		reply = GetResult(portHandle);
		//if error=1 return 0;
	}
	CloseRS232(portHandle);
	return 1;
}

int GetMotorPosition(Motor *motor){
	HANDLE portHandle;
	ConnCommand cmd;
	ConnReply reply;

	cmd.command = TMCL_GAP;
	cmd.type	= TMCL_AP_CURR_POS;
	cmd.value	= 0;

	portHandle = OpenRS232(MOTOR_COM_PORT, BAUD_RATE);
	if (portHandle == INVALID_HANDLE_VALUE){
		DEBUG("ERROR OPENING COM PORT");
	}
	else{
		SendCmd(portHandle, motor, cmd);
		Sleep(200);
		reply = GetResult(portHandle);
		//Check For error and checksum//
		//if error=1 return 0;
	}
	CloseRS232(portHandle);
	return reply.value;
}

int MoveToRelPos(Motor *motor, int relValue)
{
	HANDLE portHandle;
	ConnCommand cmd;
	ConnReply reply;

	cmd.command = TMCL_MVP;
	cmd.type	= TMCL_MVP_REL;
	cmd.value	= relValue;

	portHandle = OpenRS232(MOTOR_COM_PORT, BAUD_RATE);
	if (portHandle == INVALID_HANDLE_VALUE){
		DEBUG("ERROR OPENING COM PORT");
	}
	else{
		SendCmd(portHandle, motor, cmd);
		Sleep(200);
		reply = GetResult(portHandle);
		//Check For error and checksum//
		//if error=1 return 0;
	}
	CloseRS232(portHandle);
	return 1;
}

int StartRefSeach(Motor *motor)
{
	HANDLE portHandle;
	ConnCommand cmd;
	ConnReply reply;

	cmd.command = TMCL_RFS;
	cmd.type	= TMCL_RFS_START;
	cmd.value	= 0;

	portHandle = OpenRS232(MOTOR_COM_PORT, BAUD_RATE);
	if (portHandle == INVALID_HANDLE_VALUE){
		DEBUG("ERROR OPENING COM PORT");
	}
	else{
		SendCmd(portHandle, motor, cmd);
		Sleep(200);
		reply = GetResult(portHandle);
		//Check For error and checksum//
		//if error=1 return 0;
	}
	CloseRS232(portHandle);
	return 1;
}

int StopRefSeach(Motor *motor)
{
	HANDLE portHandle;
	ConnCommand cmd;
	ConnReply reply;

	cmd.command	= TMCL_RFS;
	cmd.type	= TMCL_RFS_STOP;
	cmd.value	= 0;

	portHandle = OpenRS232(MOTOR_COM_PORT, BAUD_RATE);
	if (portHandle == INVALID_HANDLE_VALUE){
		DEBUG("ERROR OPENING COM PORT");
	}
	else{
		SendCmd(portHandle, motor, cmd);
		Sleep(200);
		reply = GetResult(portHandle);
		//Check For error and checksum//
		//if error=1 return 0;
	}
	CloseRS232(portHandle);
	return 1;
}

int GetStatusRefSeach(Motor *motor)
{
	HANDLE portHandle;
	ConnCommand cmd;
	ConnReply reply;

	cmd.command = TMCL_RFS;
	cmd.type	= TMCL_RFS_STATUS;
	cmd.value	= 0;

	portHandle = OpenRS232(MOTOR_COM_PORT, BAUD_RATE);
	if (portHandle == INVALID_HANDLE_VALUE){
		DEBUG("ERROR OPENING COM PORT");
	}
	else{
		SendCmd(portHandle, motor, cmd);
		Sleep(200);
		reply = GetResult(portHandle);
		//Check For error and checksum//
		//if error=1 return 0;
	}
	CloseRS232(portHandle);
	return reply.value;
}

int RotateLeft(Motor *motor, int velocity)
{
	HANDLE portHandle;
	ConnCommand cmd;
	ConnReply reply;

	cmd.command = TMCL_ROL;
	cmd.type	= NULL;
	cmd.value	= velocity;

	portHandle = OpenRS232(MOTOR_COM_PORT, BAUD_RATE);
	if (portHandle == INVALID_HANDLE_VALUE){
		DEBUG("ERROR OPENING COM PORT");
	}
	else{
		SendCmd(portHandle, motor, cmd);
		Sleep(10);
		reply = GetResult(portHandle);
		//if error=1 return 0;
	}
	CloseRS232(portHandle);
	return 1;
}

int RotateRight(Motor *motor, int velocity)
{
	HANDLE portHandle;
	ConnCommand cmd;
	ConnReply reply;

	cmd.command = TMCL_ROR;
	cmd.type	= NULL;
	cmd.value	= velocity;

	portHandle = OpenRS232(MOTOR_COM_PORT, BAUD_RATE);
	if (portHandle == INVALID_HANDLE_VALUE){
		DEBUG("ERROR OPENING COM PORT");
	}
	else{
		SendCmd(portHandle, motor, cmd);
		Sleep(10);
		reply = GetResult(portHandle);
		//if error=1 return 0;
	}
	CloseRS232(portHandle);
	return 1;
}

int StopMotor(Motor *motor)
{
	HANDLE portHandle;
	ConnCommand cmd;
	ConnReply reply;

	cmd.command = TMCL_MST;
	cmd.type	= NULL;
	cmd.value	= NULL;

	portHandle = OpenRS232(MOTOR_COM_PORT, BAUD_RATE);
	if (portHandle == INVALID_HANDLE_VALUE){
		DEBUG("ERROR OPENING COM PORT");
	}
	else{
		SendCmd(portHandle, motor, cmd);
		Sleep(10);
		reply = GetResult(portHandle);
		//if error=1 return 0;
	}
	CloseRS232(portHandle);
	return 1;
}

int MoveToCoord(Motor *motor, ConnCommand cmd)
{
	return 0;
}
/*To Find the status of the reference switch. For left switch pass 0 and for right pass 1*/
int GetLimitSwitchStatus(Motor *motor, int ref_switch)
{
	HANDLE portHandle;
	ConnCommand cmd;
	ConnReply reply;    
	
	cmd.command = TMCL_GAP;
	cmd.value	= 0;

	switch (ref_switch){
    case REF_SWITCH_L:
		cmd.type = TMCL_AP_LIMIT_L; 
        break;
    case REF_SWITCH_R:
        cmd.type = TMCL_AP_LIMIT_R; 
        break;
    default:
        return -1;
	}

	portHandle = OpenRS232(MOTOR_COM_PORT, BAUD_RATE);
	if (portHandle == INVALID_HANDLE_VALUE){
		DEBUG("ERROR OPENING COM PORT");
	}
	else{
		SendCmd(portHandle, motor, cmd);
		Sleep(200);
		reply = GetResult(portHandle);
	}
	CloseRS232(portHandle);

	return reply.value;
}

int GetMaxCurrent(Motor *motor)
{
	return 0;
}

int SetMaxCurrent(Motor *motor)
{
	return 0;
}

