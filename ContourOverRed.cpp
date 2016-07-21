#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/operations.hpp"

using namespace cv;
using namespace std;

Mat red_hue_image, src, hsv, src_gray;
Mat kernel = getStructuringElement(MORPH_RECT,Size(20,20));

int thresh = 100;
int max_thresh = 255;
RNG rng(12345);
int i = 0;

int main(int, char**)
{
	VideoCapture cap(0);  // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;
	cap >> src;

	Mat OrMat(src.rows, src.cols, 0, Scalar(0, 0, 0));	//A matrix which stores the OR of the captures
	Mat ContourDrawing(src.rows, src.cols, 0);
	
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	int x = 1;
	int fps = 10;
	int flag = 0;

	for (;;)	//Run infinitely till a key is pressed. This loop is for test
	{
		cap >> src;

		imshow("Original", src);
		if (waitKey(30) >= 0)
			break;
		
		cvtColor(src, hsv, COLOR_BGR2HSV);	//save hsv of the source
		
		Mat lower_hue_range;
		Mat upper_hue_range;
		inRange(hsv, Scalar(0, 25, 55), Scalar(15, 255, 230), lower_hue_range);		//Color calibration for hsv
		inRange(hsv, Scalar(160, 100, 55), Scalar(179, 255, 230), upper_hue_range);	//These thresholds will help in detection of red color

		addWeighted(lower_hue_range, 1.0, upper_hue_range, 1.0, 0.0, red_hue_image);	//by now you have the reds in red_hue_image
		GaussianBlur(red_hue_image, red_hue_image, Size(9, 9), 1, 1);					//blur the red matrix

		imshow("Only Red", red_hue_image);	//show the red matrix

		try //Canny Contours over the red result
		{
			Canny(red_hue_image, ContourDrawing, thresh, thresh * 2, 3);
			findContours(ContourDrawing, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
			Mat drawing = Mat::zeros(ContourDrawing.size(), CV_8UC3);
			for (int i = 0; i< contours.size(); i++)
				drawContours(drawing, contours, i, Scalar(255, 255, 255), 2, 8, hierarchy, 0, Point());
		}
		catch (Exception e)
		{
			cout << "Problem with Canny\n\n" << e.err;
		}

		//imshow("Contourasdf", ContourDrawing);
		dilate(ContourDrawing, kernel, 1);
		imshow("Contour", ContourDrawing);
	}

								// Once the previous for loop exits, this one runs where 100 frames are captured and an OR is taken for the same
								// The base code is same as the previous FOR loop. The difference here is that each result is OR'd together here. 
								// Finally, a row mean and a column mean is taken and this can be calibrated with expected results to tell whether 
	for (int i = 0; i<100; i++)	// or not ther is an error in the red potting
	{
		cap >> src;	
		cvtColor(src, hsv, COLOR_BGR2HSV);	//save hsv
		Mat lower_hue_range;
		Mat upper_hue_range;
		inRange(hsv, Scalar(0, 25, 55), Scalar(15, 255, 230), lower_hue_range);
		inRange(hsv, Scalar(160, 100, 55), Scalar(179, 255, 230), upper_hue_range);
		addWeighted(lower_hue_range, 1.0, upper_hue_range, 1.0, 0.0, red_hue_image);	//by now you have the reds in red_hue_image
		GaussianBlur(red_hue_image, red_hue_image, Size(9, 9), 1, 1);	//blur the red matrix
		
		try //Canny Contours
		{
			Canny(red_hue_image, ContourDrawing, thresh, thresh * 2, 3);
			findContours(ContourDrawing, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
			Mat drawing = Mat::zeros(ContourDrawing.size(), CV_8UC3);
			for (int i = 0; i< contours.size(); i++)
				drawContours(drawing, contours, i, Scalar(255, 255, 255), 2, 8, hierarchy, 0, Point());
		}
		catch (Exception e)
		{
			cout << "Problem with Canny\n\n" <<e.err;
		}		
		dilate(OrMat, kernel, 100);
		bitwise_or(OrMat, ContourDrawing, OrMat);
	}

	//bitwise_not(OrMat, OrMat);
	dilate(OrMat, kernel, 1);
	imshow("OrMat", OrMat);
	//imshow("One of the contour drawings...", ContourDrawing);

	//now take row sum and column sum
	Mat rowsum = Mat(1, OrMat.rows, OrMat.type());
	Mat colsum = Mat(1, OrMat.cols, OrMat.type());

	Scalar rowmean,colmean;
	Scalar rowstdv, colstdv;
	try
	{	
		reduce(OrMat, rowsum, 0, CV_REDUCE_SUM, CV_32SC1);
		reduce(OrMat, colsum, 1, CV_REDUCE_SUM, CV_32SC1);
		//cout << rowsum << endl << endl << colsum;
		meanStdDev(rowsum,rowmean,rowstdv);
		meanStdDev(colsum,colmean,colstdv);
		cout << "Row Mean - " << rowmean[0];
		cout << "\nRow Standard Deviation - " << rowstdv[0];
		cout << "\nColumn Mean - "<< colmean[0];
		cout << "\nColumn Standard Deviation - " << colstdv[0];		

	}
	catch (Exception e)
	{
		cout << e.err;
	}

	waitKey(0);
	return 0;
}