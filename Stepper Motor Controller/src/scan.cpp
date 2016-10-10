#include "stdafx.h"
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <Windows.h>
#include "scan.h"
#include "controllerdefs.h"
#include "motorcmd.h"
#include "afxdialogex.h"

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

//int GetLimitSwitchStatus(Motor *motor, int ref_switch)
//StatusRefSearch(Motor *motor)
//Parameter:8 :Target Position is reached
//CREATE A FUNCTION FOR FINDING A START POINT//
//CREATE A FUNCTION <ConvertEncoderReadingToMicrometerDistance>

using namespace cv;
using namespace cv::detail;
static vector<Mat> vImg;
int testCount=0;

int ScanInit(Motor *motor1, Motor *motor2)
{
	StartRefSeach(motor1);
	StartRefSeach(motor2);
	while(GetMotorPosition(motor1)!=0 || GetMotorPosition(motor2)!=0);
	motor1->ref_position = 0;
	motor2->ref_position = 0;
	return 1;
}

int MoveWithConstVelocity(Motor *motor1, Motor *motor2, int direction, int velocity)
{
	//if(velocity<=0 || velocity>2047){
	//	return 0;
	//}
	switch(direction){
	case MOT_MOVE_LEFT: RotateLeft(motor1, velocity);
		break;
	case MOT_MOVE_RIGHT: RotateRight(motor1, velocity);
		break;
	case MOT_MOVE_UP: RotateLeft(motor2, velocity);
		break;
	case MOT_MOVE_DOWN:	RotateRight(motor2, velocity);
		break;
	default:
		DEBUG("INVALID DIRECTION COMMAND");
		return 0;
	}
	return 1;
}

int ConfigMotorAccelVelocity(Motor *motor1, Motor *motor2)
{
	return 1;
}

int MoveToXYCoordinate(Motor *motor1, Motor *motor2, CoordXY xyPoint)
{
	MoveToAbsPos(motor1, xyPoint.xaxis);
	MoveToAbsPos(motor2, xyPoint.yaxis);
	return 1;
}

int StartScan(Motor *motor1, Motor *motor2, ScanDim *slide){
	int x, y;
	CoordXY xyPoint;
	cv::Mat rImg; 
	cv::Mat img;
	
	int count=0;
	for (y = slide->scanStart.yaxis; y <= slide->scanEnd.yaxis; y = y + slide->pitch.yPitch){
		//Algo for change of y-axis 
		//if x=slide->scanEnd.xaxis then xpitch= xpitch*(-1); (-ve Pitch)
		//if x=slide->scanStart.xaxis then xpitch;  (+ve Pitch)
		//Change the velocity according to the pitch
		for (x = slide->scanStart.xaxis; x <= slide->scanEnd.xaxis; x = x + slide->pitch.xPitch){
			xyPoint.xaxis = x;
			xyPoint.yaxis = y;
			MoveToXYCoordinate(motor1, motor2, xyPoint);
			while(GetMotorPosition(motor1)!=x){										//Gets stuck in while loop, if command in not repeatedly given//
				MoveToXYCoordinate(motor1, motor2, xyPoint);
			}; 
			std::cout<<"MOT_X:: "<<GetMotorPosition(motor1)<<std::endl;
			std::cout<<"MOT_Y:: "<<GetMotorPosition(motor2)<<std::endl;

			Capture(&img);
			//vImg.push_back(img);
			//testCount++;
			//if(testCount>1){
			//	vImg.push_back(imread("Result.jpg"));
			//	vImg.push_back(imread("Next.jpg"));

			//	int num_images = static_cast<int>(vImg.size());
			//	Ptr<FeaturesFinder> finder;
	
			//	finder = new SurfFeaturesFinder();
			//	vector<ImageFeatures> features(num_images);
			//	vector<Mat> images(num_images);
			//	vector<cv::Size> full_img_size(num_images);
	
			//	Stitcher stitcher = Stitcher::createDefault();  
			//	stitcher.setFeaturesFinder(finder);
  
			//	unsigned long AAtime=0, BBtime=0;								//check processing time  
			//	AAtime = getTickCount();										
	  //
			//	printf("5");
			//	Stitcher::Status status = stitcher.stitch(vImg, rImg); 
			//	printf("6");
	  //
			//	BBtime = getTickCount();										   
			//	printf("%.2lf sec \n",  (BBtime - AAtime)/getTickFrequency() );   
			//	if (Stitcher::OK == status){   
			//		//imshow("Stitching Result",rImg);
			//		imwrite("Result.jpg",rImg);
			//	}
			//	else  
			//		printf("Stitching fail.");  
			//	//waitKey(0);
			//	//testCount=0;
			//	vImg.pop_back();
			//	vImg.pop_back();
			//}
		}
	}
	return 1;
}

int Capture(cv::Mat *camImg)
{
	static int i=1;
	//cv::Mat mat;
	cv::VideoCapture capture(CAM_PORT); // open the default camera
    if(!capture.isOpened())				// check if we succeeded
        return -1;
	else{
		Sleep(CAM_DELAY_MS);
		capture.read(*camImg);
	}
	//CString t = CTime::GetCurrentTime().Format(L"%H-%M-%S.jpg");
	//CString Path = L"C:\\Users\\Toshiba\\Desktop\\CapturedImage\\";
	//Path = Path + L"\\" + t;
	//const std::string savImg((CStringA(Path)));

	//if(testCount==0)
	//	cv::imwrite("Result.jpg",*camImg);
	//else
	//	cv::imwrite("Next.jpg",*camImg);

	std::string s = std::to_string(i);
	if(!(cv::imwrite(s.append(".jpg"), *camImg))){
		DEBUG("IMAGE CAPTURE FAILED");
		return 0;
	}
	else{
		capture.release();
		i++;
		return 1;
	}
}

