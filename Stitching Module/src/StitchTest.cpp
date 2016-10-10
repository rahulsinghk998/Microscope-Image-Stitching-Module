#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

//RANSAC is used to select 4 pairs of matching points in a greater set or correspondences (i.e. when srcPoints.size() >= 4).
//So We need at least 4 correspondences simply because a Homography matrix H has 8 degrees of freedom,
//hence 8 linear equations are required to find a solution. Since each pairs of points generates 
//two linear equations (using x and y coordinates) we'll need a total of at least 4 correspondences.

//check when image of dimensions (both row and col) are added//Its important
//Behaiviour of image and speed of processing with the change in threshold and different detectors

using namespace cv;

class LaplacianBlending {
private:
	Mat_<Vec3f> left;
	Mat_<Vec3f> right;
	Mat_<float> blendMask;

	vector<Mat_<Vec3f> > leftLapPyr, rightLapPyr, resultLapPyr;
	Mat leftSmallestLevel, rightSmallestLevel, resultSmallestLevel;
	vector<Mat_<Vec3f> > maskGaussianPyramid; //masks are 3-channels for easier multiplication with RGB

	int levels;


	void buildPyramids() {
		buildLaplacianPyramid(left, leftLapPyr, leftSmallestLevel);
		buildLaplacianPyramid(right, rightLapPyr, rightSmallestLevel);
		buildGaussianPyramid();
	}

	void buildGaussianPyramid() {
		assert(leftLapPyr.size()>0);

		maskGaussianPyramid.clear();
		Mat currentImg;
		cvtColor(blendMask, currentImg, CV_GRAY2BGR);
		maskGaussianPyramid.push_back(currentImg); //highest level

		currentImg = blendMask;
		for (int l = 1; l<levels + 1; l++) {
			Mat _down;
			if (leftLapPyr.size() > l) {
				pyrDown(currentImg, _down, leftLapPyr[l].size());
			}
			else {
				pyrDown(currentImg, _down, leftSmallestLevel.size()); //smallest level
			}

			Mat down;
			cvtColor(_down, down, CV_GRAY2BGR);
			maskGaussianPyramid.push_back(down);
			currentImg = _down;
		}
	}

	void buildLaplacianPyramid(const Mat& img, vector<Mat_<Vec3f> >& lapPyr, Mat& smallestLevel) {
		lapPyr.clear();
		Mat currentImg = img;
		for (int l = 0; l<levels; l++) {
			Mat down, up;
			pyrDown(currentImg, down);
			pyrUp(down, up, currentImg.size());
			Mat lap = currentImg - up;
			lapPyr.push_back(lap);
			currentImg = down;
		}
		currentImg.copyTo(smallestLevel);
	}

	Mat_<Vec3f> reconstructImgFromLapPyramid() {
		Mat currentImg = resultSmallestLevel;
		for (int l = levels - 1; l >= 0; l--) {
			Mat up;

			pyrUp(currentImg, up, resultLapPyr[l].size());
			currentImg = up + resultLapPyr[l];
		}
		return currentImg;
	}

	void blendLapPyrs() {
		resultSmallestLevel = leftSmallestLevel.mul(maskGaussianPyramid.back()) +
			rightSmallestLevel.mul(Scalar(1.0, 1.0, 1.0) - maskGaussianPyramid.back());
		for (int l = 0; l<levels; l++) {
			Mat A = leftLapPyr[l].mul(maskGaussianPyramid[l]);
			Mat antiMask = Scalar(1.0, 1.0, 1.0) - maskGaussianPyramid[l];
			Mat B = rightLapPyr[l].mul(antiMask);
			Mat_<Vec3f> blendedLevel = A + B;

			resultLapPyr.push_back(blendedLevel);
		}
	}

public:
	LaplacianBlending(const Mat_<Vec3f>& _left, const Mat_<Vec3f>& _right, const Mat_<float>& _blendMask, int _levels) :
		left(_left), right(_right), blendMask(_blendMask), levels(_levels)
	{
		assert(_left.size() == _right.size());
		assert(_left.size() == _blendMask.size());
		buildPyramids();
		blendLapPyrs();
	};

	Mat_<Vec3f> blend() {
		return reconstructImgFromLapPyramid();
	}
};

Mat_<Vec3f> LaplacianBlend(const Mat_<Vec3f>& l, const Mat_<Vec3f>& r, const Mat_<float>& m) {
	LaplacianBlending lb(l, r, m, 4);
	return lb.blend();
}

int GetPerspectiveMat(cv::Mat &image1, cv::Mat &image2, cv::Mat&H){
	cv::Mat gray_image1;
	cv::Mat gray_image2;
	// Convert to Grayscale
	cvtColor(image1, gray_image1, CV_RGB2GRAY);
	cvtColor(image2, gray_image2, CV_RGB2GRAY);

	if (!gray_image1.data || !gray_image2.data){
		std::cout << " --(!) Error reading images " << std::endl; return -1;
	}
	//-- Step 1: Detect the keypoints using SURF Detector
	// Original threshold was kept 400. But it gave opencv error: assertion failed (count>=4) in cvFindHomography
	int minHessian = 387;
	SurfFeatureDetector detector(minHessian);
	//minHessian max value is 53 for fastfeaturedetector
	//ORB feathure detector can also be used
	//int minHessian = 10;
	//FastFeatureDetector detector(minHessian);

	std::vector< KeyPoint > keypoints_object, keypoints_scene;
	detector.detect(gray_image1, keypoints_object);
	detector.detect(gray_image2, keypoints_scene);

	//-- Step 2: Calculate descriptors (feature vectors)
	SurfDescriptorExtractor extractor;
	Mat descriptors_object, descriptors_scene;
	extractor.compute(gray_image1, keypoints_object, descriptors_object);
	extractor.compute(gray_image2, keypoints_scene, descriptors_scene);

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors_object, descriptors_scene, matches);
	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_object.rows; i++){
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	//printf("-- Max dist : %f \n", max_dist);
	//printf("-- Min dist : %f \n", min_dist);

	//-- Use only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector< DMatch > good_matches;
	for (int i = 0; i < descriptors_object.rows; i++){
		if (matches[i].distance < 3 * min_dist){
			good_matches.push_back(matches[i]);
		}
	}
	std::vector< Point2f > obj;
	std::vector< Point2f > scene;
	for (int i = 0; i < good_matches.size(); i++){
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}
	// Find the Homography Matrix
	//Gives error when the matching points are less than 4 as for a distinct feature min 8 points are req. and for linear equation 4 variables are required
	//http://stackoverflow.com/questions/14430184/opencv-cv-findhomography-assertion-error-counter-4
		H = findHomography(obj, scene, CV_RANSAC);								
	return 1;
}

int StitchImageCol(cv::Mat &image1, cv::Mat &image2, cv::Mat &result){
	Mat H;
	GetPerspectiveMat(image1, image2, H);
	Matx33f mn(1, 0, H.at<double>(0,2),
		0, 1, H.at<double>(1, 2),
		0, 0, 1);
	warpPerspective(image1, result, mn, cv::Size(image2.cols, H.at<double>(1, 2) + image1.rows), INTER_NEAREST, BORDER_REPLICATE);	
	cv::Rect roi_rect = cv::Rect(0, H.at<double>(1, 2), image1.cols, image2.rows - H.at<double>(1, 2) + 1);
	std::cout << "col: " << H.at<double>(1, 2) << std::endl;
	std::cout << "row: " << H.at<double>(0, 2) << std::endl;
	std::cout << "H Transform Matrix: " << H << std::endl;

	Mat l8u = image2(roi_rect);
	Mat r8u = result(roi_rect);
	Mat_<Vec3f> l; l8u.convertTo(l, CV_32F, 1.0 / 255.0);
	Mat_<Vec3f> r; r8u.convertTo(r, CV_32F, 1.0 / 255.0);
	Mat_<float> m(l.rows, l.cols, 0.0);
	m(Range(0, m.rows / 2), Range::all()) = 1.0;
	Mat_<Vec3f> blend = LaplacianBlend(l, r, m);
	
	Mat temp;
	blend.convertTo(temp, CV_8UC3, 255.0);
	cv::Mat half(result, cv::Rect(0, 0, image2.cols, image2.rows));
	image2.copyTo(half);
	temp.copyTo(result(cv::Rect(0, H.at<double>(1, 2), temp.cols, temp.rows)));
	return 1;
}

int StitchImageRow(cv::Mat &image1, cv::Mat &image2, cv::Mat &result){
	Mat H;
	GetPerspectiveMat(image1, image2, H);

	//////////////////////////////////////////////////////////////////////////////////
	//This part is hardcoded if their is not much feature difference and the translation along x is not there//
	//For future implementation a more defined approach needs to adopted//
	std::cout << "row: " << H.at<double>(0, 2) << std::endl;
	if ((int)H.at<double>(0, 2) < 100 || (int)H.at<double>(0, 2) > 2000 )
		H.at<double>(0, 2) = 317.775 *((int)((image2.cols - 640) / 310) + 1);
	//////////////////////////////////////////////////////////////////////////////////

	// Use the Homography Matrix to warp the images
	Matx33f mn(1, 0, H.at<double>(0,2),									
		0, 1, 0, 
		0, 0, 1);
	warpPerspective(image1, result, mn, cv::Size(image1.cols + H.at<double>(0, 2) , image1.rows), INTER_NEAREST, BORDER_REPLICATE);		
	cv::Rect roi_rect = cv::Rect(H.at<double>(0,2), 0, image2.cols-H.at<double>(0,2)+1, image2.rows);

	Mat l8u = image2(roi_rect);
	Mat r8u = result(roi_rect);;
	Mat_<Vec3f> l; l8u.convertTo(l, CV_32F, 1.0 / 255.0);
	Mat_<Vec3f> r; r8u.convertTo(r, CV_32F, 1.0 / 255.0);
	Mat_<float> m(l.rows, l.cols, 0.0);
	m(Range::all(), Range(0, m.cols / 2)) = 1.0;																		
	Mat_<Vec3f> blend = LaplacianBlend(l, r, m);

	Mat temp;
	blend.convertTo(temp, CV_8UC3, 255.0);
	cv::Mat half(result, cv::Rect(0, 0, image2.cols, image2.rows));
	image2.copyTo(half);
	temp.copyTo(result(cv::Rect(H.at<double>(0, 2), 0, temp.cols, temp.rows)));
	return 1;
}

int ResizeImage(cv::Mat &image1, cv::Mat &image2){
	//if(image1.rows==image2.rows && image1.cols==image2.cols){
	//	return 1;
	//}
	//if(image1.rows==image2.rows || image1.cols==image2.cols){
	//	if(image1.rows>image2.rows)
	//		resize(image2);
	//	else
	//		resize(image1);
	//	if(image1.cols>image2.cols)
	//		resize(image2);
	//	else
	//		resize(image1);
	//}
	//Mat image1, image2;
	//if(img1.rows==img2.rows && img1.cols==img2.cols){
	//	image1 = img1;
	//	image2 = img2;
	//	//return 1;
	//}
	//if (img1.rows == img2.rows || img1.cols == img2.cols){
	//	if (img1.rows > img2.rows)
	//		resize(img2, image1, img2.size(), 0, 0, INTER_LINEAR);
	//	else
	//		resize(img1, image2, img1.size(), 0, 0, INTER_LINEAR);
	//	if (img1.cols > img2.cols)
	//		resize(img2, image1, img2.size(), 0, 0, INTER_LINEAR);
	//	else
	//		resize(img1, image2, img1.size(), 0, 0, INTER_LINEAR);
	//}
	return 1;
}

int main(int argc, char** argv)
{
	int a, b, start;
	cv::Mat result;

	int imgNumOfset	=	0;
	int imgSize		=	8;
	int rowSize		=	8;		//Number of images in 1 Row of Grid
	int colSize		=	1;		//Number of images in 1 Col of Grid
	
	for (start = 0; start < imgSize ; start = start + rowSize){
		for (a = 1 + start, b = (start + rowSize); a < b; a = a + 2){
			if ((b-a-1) % 2){
				Mat img1 = imread(std::to_string(b+imgNumOfset).append(".jpg"));
				Mat img2 = imread(std::to_string(b-1 + imgNumOfset).append(".jpg"));
				int x;
				if (img1.rows >= img2.rows)
					x = img2.rows;
				else
					x = img1.rows;
				Mat image2(img2, cv::Rect(0, 0, img2.cols, x));
				Mat image1(img1, cv::Rect(0, 0, img1.cols, x));
				StitchImageRow(image1, image2, result);
				imwrite(std::to_string(b-1 + imgNumOfset).append(".jpg"), result);
				b = b - 1;
			}

			Mat img1 = imread(std::to_string(a + 1 + imgNumOfset).append(".jpg"));
			Mat img2 = imread(std::to_string(a + imgNumOfset).append(".jpg"));
			int x;
			if (img1.rows >= img2.rows)
				x = img2.rows;
			else
				x = img1.rows;
			Mat image2(img2, cv::Rect(0, 0, img2.cols, x));
			Mat image1(img1, cv::Rect(0, 0, img1.cols, x));
			StitchImageRow(image1, image2, result);
			imwrite(std::to_string((a + start) / 2 + 1 + imgNumOfset).append(".jpg"), result);
			waitKey(0);
			if (a == (b - 1)) {
				if (b == (start + 2))
					imwrite(std::to_string(start / rowSize + 1 + imgNumOfset ).append("row.jpg"), result);
				a = -1 + start;
				b = (b + start) / 2;
			}
		}
	}
	for (a = 1, b = colSize; a < b; a = a + 2){
		if (b % 2){
			Mat img1 = imread(std::to_string(b).append("row.jpg"));
			Mat img2 = imread(std::to_string(b-1).append("row.jpg"));
			int y;
			if (img1.cols >= img2.cols)
				y = img2.cols;
			else
				y = img1.cols;

			Mat image2(img2, cv::Rect(0, 0, y, img2.rows));
			Mat image1(img1, cv::Rect(0, 0, y, img1.rows));
			StitchImageCol(image1, image2, result);
			imwrite(std::to_string(b-1).append("row.jpg"), result);
			b = b - 1;
		}	
		Mat img1 = imread(std::to_string(a + 1).append("row.jpg"));
		Mat img2 = imread(std::to_string(a).append("row.jpg"));
		int y;
		if (img1.cols >= img2.cols)
			y = img2.cols;
		else
			y = img1.cols;

		Mat image2(img2, cv::Rect(0, 0, y, img2.rows));
		Mat image1(img1, cv::Rect(0, 0, y, img1.rows));
		StitchImageCol(image1, image2, result);
		imwrite(std::to_string(a / 2 + 1).append("row.jpg"), result);
		if (a == (b - 1)) {
			a = -1;
			b = b / 2;
		}
	}
	return 0;
}

//addWeighted(roi, 0.5, roi1, 0.5, 0.0, output, -1);
//cvSetImageROI(result, cvRect(x, y, width, height));
//cvSetImageROI(src2, cvRect(x, y, width, height));
//cvAddWeighted(src1, 0.5, src2, 0.5, 0.0, src1);
/*std::cout << H << std::endl;
for (int i = 0; i < H.rows; i++){
	for (int j = 0; j < H.cols; j++){
		std::cout << "i: " << i << "j: " << j << H.at<double>(i, j) << std::endl;
	}
}
*/
//Mat imgResult(image1.rows, 2 * image1.cols, image1.type());
//Mat roiImgResult_Left = imgResult(Rect(0, 0, image1.cols, image1.rows));
//Mat roiImgResult_Right = imgResult(Rect(image1.cols, 0, image2.cols, image2.rows));
//Mat roiImg1 = image1(Rect(0, 0, image1.cols, image1.rows));
//Mat roiImg2 = warpresult2(Rect(0, 0, image2.cols, image2.rows));
//roiImg1.copyTo(roiImgResult_Left); //Img1 will be on the left of imgResult
//roiImg2.copyTo(roiImgResult_Right); //Img2 will be on the right of imgResult
//resize(img1, image2, img1.size(), 0, 0, INTER_LINEAR);

//cv::Mat H = cv::findHomography(obj, scene, CV_RANSAC);
//
////-- Get the corners from the image_1 ( the object to be "detected" )
//std::vector<cv::Point2f> obj_corners(4);
//obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(img_object.cols, 0);
//obj_corners[2] = cvPoint(img_object.cols, img_object.rows); obj_corners[3] = cvPoint(0, img_object.rows);
//std::vector<cv::Point2f> scene_corners(4);
//
//cv::perspectiveTransform(obj_corners, scene_corners, H);
//
////-- Draw lines between the corners (the mapped object in the scene - image_2 )
//cv::line(img_matches, scene_corners[0] + cv::Point2f(img_object.cols, 0), scene_corners[1] + cv::Point2f(img_object.cols, 0), cv::Scalar(0, 255, 0), 4);
//cv::line(img_matches, scene_corners[1] + cv::Point2f(img_object.cols, 0), scene_corners[2] + cv::Point2f(img_object.cols, 0), cv::Scalar(0, 255, 0), 4);
//cv::line(img_matches, scene_corners[2] + cv::Point2f(img_object.cols, 0), scene_corners[3] + cv::Point2f(img_object.cols, 0), cv::Scalar(0, 255, 0), 4);
//cv::line(img_matches, scene_corners[3] + cv::Point2f(img_object.cols, 0), scene_corners[0] + cv::Point2f(img_object.cols, 0), cv::Scalar(0, 255, 0), 4);
//
////-- Show detected matches
//cv::imshow("Good Matches & Object detection", img_matches);
