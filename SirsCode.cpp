#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/operations.hpp"

using namespace cv;
using namespace std;

Mat red_hue_image,src,hsv,src_gray;


int thresh = 100;
int max_thresh = 255;
RNG rng(12345);
int i = 0;

/// Function headers
void thresh_callback(int, void*);
string getImageType(int);

int main(int, char**)
{
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;
	cap >> src;
	Mat OrMat(src.rows, src.cols, 0);
	Mat AndMat(src.rows, src.cols, 0);

	int x = 1;
	int fps = 10;
	int flag = 0;

	for (;;)
	{
		cap >> src;
		imshow("Original", src);
		if (waitKey(30) >= 0)
			break;
		cvtColor(src, hsv, COLOR_BGR2HSV);	//save hsv
		Mat lower_hue_range;
		Mat upper_hue_range;
		inRange(hsv, Scalar(0, 25, 55), Scalar(15, 255, 230), lower_hue_range);
		inRange(hsv, Scalar(160, 100, 55), Scalar(179, 255, 230), upper_hue_range);
		addWeighted(lower_hue_range, 1.0, upper_hue_range, 1.0, 0.0, red_hue_image);	//by now you have the reds in red_hue_image
		GaussianBlur(red_hue_image, red_hue_image, Size(9, 9), 1, 1);	//blur the red matrix
		imshow("Only Red", red_hue_image);	//show the red matrix
		thresh_callback(0, 0);
	}

	for (int i=0;i<40;i++)
	{
		cap >> src;	
		cvtColor(src, hsv, COLOR_BGR2HSV);	//save hsv
		Mat lower_hue_range;
		Mat upper_hue_range;
		inRange(hsv, Scalar(0, 25, 55), Scalar(15, 255, 230), lower_hue_range);
		inRange(hsv, Scalar(160, 100, 55), Scalar(179, 255, 230), upper_hue_range);
		addWeighted(lower_hue_range, 1.0, upper_hue_range, 1.0, 0.0, red_hue_image);	//by now you have the reds in red_hue_image
		GaussianBlur(red_hue_image, red_hue_image, Size(9, 9), 1, 1);	//blur the red matrix
		thresh_callback(0, 0);
		//imshow("OrMat", OrMat);
		bitwise_or(OrMat, red_hue_image, OrMat);
		bitwise_and(AndMat, red_hue_image, AndMat);
	}

	//bitwise_not(OrMat, OrMat);
	imshow("OrMat", OrMat);
	imshow("AndMat", AndMat);

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
	catch(Exception e)
	{
		cout << e.err;
	}
	

	waitKey(0);
	return 0;
}

void thresh_callback(int, void*)
{
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using canny
	Canny(red_hue_image, canny_output, thresh, thresh * 2, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		drawContours(drawing, contours, i, Scalar(255, 255, 255), 2, 8, hierarchy, 0, Point());
	}

	/// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
}

string getImgType(int imgTypeInt)
{
	int numImgTypes = 35; // 7 base types, with five channel options each (none or C1, ..., C4)

	int enum_ints[] = { CV_8U,  CV_8UC1,  CV_8UC2,  CV_8UC3,  CV_8UC4,
		CV_8S,  CV_8SC1,  CV_8SC2,  CV_8SC3,  CV_8SC4,
		CV_16U, CV_16UC1, CV_16UC2, CV_16UC3, CV_16UC4,
		CV_16S, CV_16SC1, CV_16SC2, CV_16SC3, CV_16SC4,
		CV_32S, CV_32SC1, CV_32SC2, CV_32SC3, CV_32SC4,
		CV_32F, CV_32FC1, CV_32FC2, CV_32FC3, CV_32FC4,
		CV_64F, CV_64FC1, CV_64FC2, CV_64FC3, CV_64FC4 };

	string enum_strings[] = { "CV_8U",  "CV_8UC1",  "CV_8UC2",  "CV_8UC3",  "CV_8UC4",
		"CV_8S",  "CV_8SC1",  "CV_8SC2",  "CV_8SC3",  "CV_8SC4",
		"CV_16U", "CV_16UC1", "CV_16UC2", "CV_16UC3", "CV_16UC4",
		"CV_16S", "CV_16SC1", "CV_16SC2", "CV_16SC3", "CV_16SC4",
		"CV_32S", "CV_32SC1", "CV_32SC2", "CV_32SC3", "CV_32SC4",
		"CV_32F", "CV_32FC1", "CV_32FC2", "CV_32FC3", "CV_32FC4",
		"CV_64F", "CV_64FC1", "CV_64FC2", "CV_64FC3", "CV_64FC4" };

	for (int i = 0; i<numImgTypes; i++)
	{
		if (imgTypeInt == enum_ints[i]) return enum_strings[i];
	}
	return "unknown image type";
}