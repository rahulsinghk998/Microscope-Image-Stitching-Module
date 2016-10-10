#pragma once

#include "stdafx.h"
#include "controllerdefs.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/stitching/stitcher.hpp"  
#include <opencv2/stitching/detail/autocalib.hpp>
#include <opencv2/stitching/stitcher.hpp>
#include <opencv2/stitching/detail/blenders.hpp>
#include <opencv2/stitching/detail/camera.hpp>
#include <opencv2/stitching/detail/exposure_compensate.hpp>
#include <opencv2/stitching/detail/matchers.hpp>
#include <opencv2/stitching/detail/motion_estimators.hpp>
#include <opencv2/stitching/detail/seam_finders.hpp>
#include <opencv2/stitching/detail/util.hpp>
#include <opencv2/stitching/detail/util_inl.hpp>
#include <opencv2/stitching/detail/warpers.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2\opencv_modules.hpp>
#include <vector>

#define		CAM_PORT			0
#define		CAM_DELAY_MS		100			//Delay required for opening i.e. starting the camera and taking the first pic
#define		CAM_FPS				15
#define		CAM_FPS_DELAY_MS	(1000/CAM_FPS)	
#define		CAM_RESOLUTION		480X640

#define		FRAME_OVERLAP_PCENT 20			//Overlapping section of two frame for Image Processing
#define		OBJ_NUM				1			//ObjectiveNumber
#define		SCALE_X				10			//IN uM
#define		SCALE_Y				10			//In uM
#define		STAGE_LIMITX		3000		//In uM
#define		STAGE_LIMITY		2500		//In uM
//#define		TOLERANCE					//How much +- error in position should be considered

#define		MOT_DELAY_REPLY_MS	20			//Depends on Baudrate
//#define		MOT_DELAY_PER_COUNT 1		//Need to be configured as it depends on speed and motor
//#define		MOT_MAX_ENCODER_COUNT
//#define		MOT_MIN_ENCODER_COUNT
//#define		MOT_MAX_DIS_COVER
//#define		MOT_MAX_POS_VEL
//#define		MOT_MIN_POS_VEL

#define		MOT_STOP			5
#define		MOT_MOVE_LEFT		4
#define		MOT_MOVE_RIGHT		6
#define		MOT_MOVE_UP			8
#define		MOT_MOVE_DOWN		2
#define		MOT_SPEED_UP		7
#define		MOT_SPEED_DOWN		9

#define		MOTOR_COM_PORT		"COM8"
#define		BAUD_RATE			9600

#define		REF_SWITCH_L		0
#define		REF_SWITCH_R		1

typedef struct MotPosStruct
{
	int xaxis;
	int yaxis;
}CoordXY;

typedef struct PitchStruct{
	int xPitch;
	int yPitch;
}PitchDim;

typedef struct SlideDimStruct
{
	CoordXY	scanStart;
	CoordXY	scanEnd;
	PitchDim pitch;
}ScanDim;

int MoveToXYCoordinate(Motor *motor1, Motor *motor2, CoordXY pointXY);
int ConfigMotorAccelVelocity(Motor *motor1, Motor *motor2);
int StartScan(Motor *motor1, Motor *motor2, ScanDim *slide);
int MoveWithConstVelocity(Motor *motor1, Motor *motor2, int direction, int velocity);

/*@Function
	To Initialize the motor before scan
	Arguments	:: Pass the both motor parameter struct
	Return		:: 1 if successful
				   0 if unsuccessful
*/
int ScanInit(Motor *motor1, Motor *motor2);

/*@Function
	To Capture an image from camera
	Call the function it stores the image
TO DO:: Pass an agrument which specifies the cam number and the location where the file will be stored
*/
int Capture(cv::Mat *camImg);

