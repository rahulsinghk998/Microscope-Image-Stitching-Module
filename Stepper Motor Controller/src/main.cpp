#include "stdafx.h"
#include <iostream>
#include <string>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <Windows.h>
#include "Motor_Serial_Comm.h"
#include "controllerdefs.h"
#include "motorcmd.h"
#include "scan.h"
#include <conio.h>
#include <process.h>

/*TO DO
	Velocity control in Position Mode - DONE->CHECK THE FUNCTION
	Calibration Part
	Small GUI for Calibration
	Need to check the initialization part
	Add an extra argument for velocity in Position Mode
	Change return values and argument according to the function
	Make some more commands for acceleration, velocity and left and right value
	Add function for current setting for motor
	Take care of the boundary conditions in every function and the DEBUG log for any error
	Take care of the Reply delay of controller and put #define for that

	Use distance,velocity and acceleration formula for finding the timeDelay To reach from 1 position to another.
	If possible error associated to it

	How a multithreading works and its basic programming
	//WHAT IS MICROSTEPPING? STEPPER MOTOR ERROR? AND ITS RESOLUTION? ITS CONTROL
	Espected time of completion can be shown in GUI.
*/

CoordXY move;
int xVel;
int yVel;

void test(void *param)
{
	move.xaxis=0;
	move.yaxis=0;
	xVel=1024;
	yVel=1024;
	char keystroke;
	while(keystroke=_getch()){
		//Use time information and create a time varing incrementing function
		switch(keystroke){
		case 'a':	move.xaxis=move.xaxis-10;
					if(move.xaxis<0) move.xaxis=0;
					break;
		case 'd':	move.xaxis=move.xaxis+10;	break;
		case 'w':	move.yaxis=move.yaxis+10;	break;
		case 's':	move.yaxis=move.yaxis-10;	
					if(move.yaxis<0) move.yaxis=0;
					break;					
		case 'K':	xVel--;
					if(xVel<1) xVel=1;
					break;
		case 'M':	xVel++;			break;
		case 'H':	yVel++;			break;
		case 'P':	yVel--;
					if(yVel<1) yVel=1;
					break;
		default:	break;
		}
	}
}

void init_motor(Motor *motor1, Motor *motor2)
{
	//ADD MORE DECLEARATIONS//
	motor1->address=1;
	motor1->number=1;
	motor1->num_refswitches=2;
	motor1->position=GetMotorPosition(motor1);

	motor2->address=1;
	motor2->number=2;
	motor2->num_refswitches=2;
	motor2->position=GetMotorPosition(motor2);
}

void init_slide(ScanDim *slide)
{
	slide->scanStart.xaxis=0000;
	slide->scanStart.yaxis=0000;
	slide->scanEnd.xaxis=300000;
	slide->scanEnd.yaxis=240000;
	slide->pitch.xPitch=15000;
	slide->pitch.yPitch=12000;
}

int main(int arg, char* argv)
{
	Motor *motor1, *motor2;
	ScanDim *slide;
	//HANDLE hThread;

	motor1=(Motor *)malloc(sizeof(Motor));
	motor2=(Motor *)malloc(sizeof(Motor));
	slide=(ScanDim *)malloc(sizeof(ScanDim));
	
	init_motor(motor1, motor2);
	SetMaxPosSpeed(motor1, 1024);
	SetMaxPosSpeed(motor2, 1024);
	ScanInit(motor1,motor2);
	//hThread=(HANDLE)_beginthread(test,0,NULL);

	//WaitForSingleObject(hThread,INFINITE);

	init_slide(slide);
	StartScan(motor1,motor2,slide);

	//while(1){
	//	//MoveWithConstVelocity(motor1,motor2,MOT_MOVE_LEFT,500);
	//	RotateRight(motor1,100);
	//	Capture();
	//	if(GetMotorPosition(motor1)>100000){
	//		StopMotor(motor1);
	//		MoveToXYCoordinate(motor1,motor2,slide->scanStart);
	//		ScanInit(motor1,motor2);
	//		//while(GetMotorPosition(motor1)!=slide->scanStart.xaxis){
	//		//	MoveToXYCoordinate(motor1,motor2,slide->scanStart);
	//		//}
	//	}
	//}

	free(motor1);
	free(motor2);
	free(slide);
	return 0;
}
//
//		/*Sleep(250);
//		std::cout<<"X:: "<<move.xaxis<<std::endl;
//		std::cout<<"Y:: "<<move.yaxis<<std::endl;
//		std::cout<<"X_VEL:: "<<xVel<<std::endl;
//		std::cout<<"Y_VEL:: "<<yVel<<std::endl;
//		MoveToXYCoordinate(motor1,motor2,move);*/
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
////for(j=100; j<31001; j=j+1000){
//	//	MoveToAbsPos(motor1, j);
//	//	while(j!=GetMotorPosition(motor1));
//	//	std::cout<<GetMotorPosition(motor1)<<std::endl;
//	//	Capture();
//
//		//switch(keystroke){
//		//case 'a': {
//		//	RotateLeft(motor1,xVel);
//		//	break;
//		//	}
//		//case 'd': {
//		//	RotateRight(motor1,xVel);
//		//	break;
//		//	}
//		//case 'w':{
//		//	RotateLeft(motor2,yVel);
//		//	break;
//		//	}
//		//case 's' :{
//		//	RotateRight(motor2,yVel);
//		//	break;
//		//	}
//		//case 32 :{
//		//	StopMotor(motor1);
//		//	StopMotor(motor2);
//		//	break;
//		//	}





//#include "stdafx.h"
//#include <iostream>
//#include <fstream>
////#include "opencv2/imgcodecs/imgcodecs.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/stitching/stitcher.hpp"
//
//using namespace std;
//using namespace cv;
//
//bool try_use_gpu = false;
//vector<Mat> imgs;
//string result_name = "C:\\Users\\Toshiba\\Desktop\\7.jpg";
//
//void printUsage();
//int parseCmdArgs(int argc, char** argv);
//
//int main(int argc, char* argv[])
//{
//    int retval = parseCmdArgs(argc, argv);
//    if (retval) return -1;
//
//    Mat pano;
//
//	//cout <<"1"<< endl;
//	//imgs[1]=imread("C:\\Users\\Toshiba\\Desktop\\2.jpg");
//	//cout << "2"<< endl;
//	//imgs[2]=imread("C:\\Users\\Toshiba\\Desktop\\3.jpg");
//	//cout << "3"<< endl;
//
//	//imshow("1", imgs[1]);
//	//imshow("2", imgs[2]);
//
//    Stitcher stitcher = Stitcher::createDefault(try_use_gpu);
//	cout << "4"<< endl;
//    Stitcher::Status status = stitcher.stitch(imgs, pano);
//	cout << "5"<< endl;
//
//    if (status != Stitcher::OK)
//    {
//        cout << "Can't stitch images, error code = " << int(status) << endl;
//        return -1;
//    }
//
//    imwrite(result_name, pano);
//    return 0;
//}
//
//
//void printUsage()
//{
//    cout <<
//        "Rotation model images stitcher.\n\n"
//        "stitching img1 img2 [...imgN]\n\n"
//        "Flags:\n"
//        "  --try_use_gpu (yes|no)\n"
//        "      Try to use GPU. The default value is 'no'. All default values\n"
//        "      are for CPU mode.\n"
//        "  --output <result_img>\n"
//        "      The default is 'result.jpg'.\n";
//}
//
//
//int parseCmdArgs(int argc, char** argv)
//{
//    if (argc == 1)
//    {
//        printUsage();
//        return -1;
//    }
//    for (int i = 1; i < argc; ++i)
//    {
//        if (string(argv[i]) == "--help" || string(argv[i]) == "/?")
//        {
//            printUsage();
//            return -1;
//        }
//        else if (string(argv[i]) == "--try_use_gpu")
//        {
//            if (string(argv[i + 1]) == "no")
//                try_use_gpu = false;
//            else if (string(argv[i + 1]) == "yes")
//                try_use_gpu = true;
//            else
//            {
//                cout << "Bad --try_use_gpu flag value\n";
//                return -1;
//            }
//            i++;
//        }
//        else if (string(argv[i]) == "--output")
//        {
//            result_name = argv[i + 1];
//            i++;
//        }
//        else
//        {
//            Mat img = imread(argv[i]);
//			imshow("1", img);
//			Sleep(2000);
//            if (img.empty())
//            {
//                cout << "Can't read image '" << argv[i] << "'\n";
//                return -1;
//            }
//            imgs.push_back(img);
//        }
//    }
//    return 0;
//}