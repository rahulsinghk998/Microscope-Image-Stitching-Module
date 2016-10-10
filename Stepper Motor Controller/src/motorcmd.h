#pragma once

#include "stdafx.h"
#include "controllerdefs.h"
#include "scan.h"
#include "Motor_Serial_Comm.h"

/*Motor Movement Commands*/
int MoveToAbsPos(Motor *motor, int position);
//axisparameter	: MaximumPositioningSpeed: 4	: Should not be 0
//axisparameter	: TargetPositionReached  : 8	
//axisparameter	: MinimumSpeed			: 130   : Min Value 1

//axisparam		: AccelerationThreshold : 136 
//axisparam		: Actual Acceleration	: 135
//axisparam		: AccelerationDivisor	: 137
//asixparam		: AccelerationFactor	: 146	: Normally don't change

int SetMaxPosSpeed(Motor *motor, int velocity);
int GetMotorPosition(Motor *motor);
int MoveToRelPos(Motor *motor, int relPos);
int RotateLeft(Motor *motor, int velocity);
int RotateRight(Motor *motor, int velocity);
int StopMotor(Motor *motor);
int MoveToCoord(Motor *motor, ConnCommand cmd);

int GetLimitSwitchStatus(Motor *motor, int lim_switch);

int GetMaxCurrent(Motor *motor);
int SetMaxCurrent(Motor *motor);

//LEFT SWITCH -> REFERENCE SWITCH
//RIGHT SWITCH-> STOP SWITCH
//axis parameter:referencing mode:193
//axisparameter :ref search speed:194
//axisparameter :ref switch speed:195
int StartRefSeach(Motor *motor);
int StopRefSeach(Motor *motor);
int GetStatusRefSeach(Motor *motor);






//ACCELERATION CONFIGURATION in Position Mode and Velocity Mode
//READ ACTUAL ACCELERATION: AXIS PARAMETER 135

//VELOCITY CONFIGURATION
//MINIMUM SPEED: AXIS PARAMETER 130

//Function to wait while position is achieved
//Use of axis parameters

