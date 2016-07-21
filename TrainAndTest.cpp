#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	Mat edges;
	namedWindow("edges", 1);
	for (;;)
	{
		Mat src;
		cap >> src; // get a new frame from camera
		Mat hsv;
		cvtColor(src, hsv, COLOR_BGR2HSV);

		Mat lower_hue_range;
		Mat upper_hue_range;

		inRange(hsv, Scalar(0, 100, 55), Scalar(15, 255, 180), lower_hue_range);
		inRange(hsv, Scalar(160, 100, 55), Scalar(179, 255, 180), upper_hue_range);

		Mat red_hue_image;
		addWeighted(lower_hue_range, 1.0, upper_hue_range, 1.0, 0.0, red_hue_image);

		imshow("Result", red_hue_image);
		waitKey(0);
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
	
	/*
	Mat src = imread("New Folder/asdf.jpg");
	//src.resize(400);
	Mat hsv;
	cvtColor(src, hsv, COLOR_BGR2HSV);

	Mat lower_hue_range;
	Mat upper_hue_range;

	inRange(hsv, Scalar(0, 100, 55), Scalar(15, 255, 180), lower_hue_range);
	inRange(hsv, Scalar(160, 100, 55), Scalar(179, 255, 180), upper_hue_range);

	Mat red_hue_image;
	addWeighted(lower_hue_range, 1.0, upper_hue_range, 1.0, 0.0, red_hue_image);

	imshow("Result", red_hue_image);
	waitKey(0);
	return 0;*/
}