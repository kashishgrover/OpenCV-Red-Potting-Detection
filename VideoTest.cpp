#include "opencv2/opencv.hpp"

using namespace cv;

int main(int, char**)
{
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;
	int x = 1;
	int fps = 10;
	for (;;)
	{
		Mat src;

		cap >> src; // get a new frame from camera
		cap.set(CV_CAP_PROP_FPS, fps);

		Mat hsv;
		cvtColor(src, hsv, COLOR_BGR2HSV);
		
		Mat lower_hue_range;
		Mat upper_hue_range;

		inRange(hsv, Scalar(0, 25, 55), Scalar(15, 255, 230), lower_hue_range);
		inRange(hsv, Scalar(160, 100, 55), Scalar(179, 255, 230), upper_hue_range);

		Mat red_hue_image;
		addWeighted(lower_hue_range, 1.0, upper_hue_range, 1.0, 0.0, red_hue_image);
		//GaussianBlur(red_hue_image, red_hue_image, Size(9, 9), 1, 1);
		
		std::vector<Vec3f> circles;

		//void HoughCircles(InputArray image,
		//					OutputArray circles,
		//					int method,
		//					double dp,
		//					double minDist,
		//					double param1 = 100,
		//					double param2 = 100,
		//					int minRadius = 0,
		//					int maxRadius = 0)
		/*
		HoughCircles(red_hue_image, circles, CV_HOUGH_GRADIENT, 1, 100, 100, 40, 0, 10000);

		/// Draw the circles detected
		for (size_t i = 0; i < circles.size(); i++)
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2])+10;
			// circle outline
			circle(red_hue_image, center, radius, Scalar(0, 255, 255), 3, 8, 0);
		}
		*/
		imshow("Hough Circle Transform Demo", red_hue_image);
		imshow("Original", src);

		if (circles.size() == 1)
			std::cout << "Potting correct!\n\n";
		else std::cout << "\tPotting Incorrect!\n\n";


		if(waitKey(30) >= 0) break;
	}

	return 0;
}