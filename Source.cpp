#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/types_c.h"
#include <iostream>
#include<stdlib.h>
#include<string.h>

using namespace std;
using namespace cv;


/** @function main */
int main()
{
	/// Declare variables
	Mat src, dst1, dst2, dst3, dst4, dst5, dst6, dst7, dst8;
	Mat horim, verim, rdiag, ldiag;
	Mat kernel1;
	Mat kernel2;
	Mat kernel3;
	Mat kernel4;
	Mat kernel5;
	Mat kernel6;
	Mat kernel7;
	Mat kernel8;
	Mat data1, data2, data3, data4, data5;
	const int kNewWidth = 4;
	const int kNewHeight = 4;
	Mat new_mat1, new_mat2, new_mat3, new_mat5, new_mat4;
	int MAX_ROW = 4, MAX_COL = 4;
	//	int one1[100];

	Point anchor;
	double delta;
	int ddepth;
	//int kernel_size;
	char* window_name1 = "horizontal1";
	char* window_name2 = "horizontal2";
	char* window_name3 = "vertical1";
	char* window_name4 = "vertical2";
	char* window_name5 = "rightdiag1";
	char* window_name6 = "rightdiag2";
	char* window_name7 = "leftdiag1";
	char* window_name8 = "leftdiag2";


	char* window1 = "resized1";
	char* window2 = "resized2";
	char* window3 = "resized3";
	char* window4 = "resized4";
	char* window5 = "resized5";


	int c;
	float horizontal1[3][3] =
	{
		{ 5, -3, -3 },
		{ 5, 0, -3 },
		{ 5, -3, -3 }
	};

	float horizontal2[3][3] =
	{
		{ 5, 5, 5 },
		{ -3, 0, -3 },
		{ -3, -3, -3 }
	};

	float vertical1[3][3] =
	{
		{ -3, -3, 5 },
		{ -3, 0, 5 },
		{ -3, -3, 5 }
	};

	float vertical2[3][3] =
	{
		{ 5, -3, -3 },
		{ 5, 0, -3 },
		{ 5, -3, -3 }
	};

	float rightdiag1[3][3] =
	{
		{ -3, 5, 5 },
		{ -3, 0, 5 },
		{ -3, -3, -3 }
	};

	float rightdiag2[3][3] =
	{
		{ -3, -3, -3 },
		{ 5, 0, -3 },
		{ 5, 5, -3 }
	};

	float leftdiag1[3][3] =
	{
		{ -3, -3, -3 },
		{ -3, 0, 5 },
		{ -3, 5, 5 }
	};

	float leftdiag2[3][3] =
	{
		{ 5, 5, -3 },
		{ 5, 0, -3 },
		{ -3, -3, -3 }
	};

	/// Load an image
	char filepath[200];
	int rowcount = 1;
	float dataAll[61][80];
	for (int i = 1; i < 61; i++)
	{
		strcpy(filepath, "threshold/");
		char filename[20];
		sprintf(filename, "%d.jpg", i);
		strcat(filepath, filename);
		src = imread(filepath);
		//cout << filepath << endl;
		strcpy(filepath, "");

		/*
		if (!src.data)
		{
		return -1;
		}

		/// Create window
		namedWindow(window_name1, CV_WINDOW_AUTOSIZE);
		*/
		/// Initialize arguments for the filter
		anchor = Point(-1, -1);
		delta = 0;
		ddepth = -1;
		Mat dest;
		// thresholding the image
		Mat rc;
		cvtColor(src, rc, CV_BGR2GRAY);
		threshold(rc, dest, 39, 255, 0);
		/*  0-implies normal binary thresholding;
		1-inverse thresholding
		2: Truncate
		3: To Zero
		4: To Zero Inverted";
		reference link http://docs.opencv.org/2.4/doc/tutorials/imgproc/threshold/threshold.html
		*/
		imshow("thresh", dest);
		/// Update kernel size for a normalized box filter


		kernel1 = Mat(3, 3, CV_32FC1, horizontal1);
		kernel2 = Mat(3, 3, CV_32FC1, horizontal2);
		kernel3 = Mat(3, 3, CV_32FC1, vertical1);
		kernel4 = Mat(3, 3, CV_32FC1, vertical2);
		kernel5 = Mat(3, 3, CV_32FC1, rightdiag1);
		kernel6 = Mat(3, 3, CV_32FC1, rightdiag2);
		kernel7 = Mat(3, 3, CV_32FC1, leftdiag1);
		kernel8 = Mat(3, 3, CV_32FC1, leftdiag2);

		/// Apply filter
		filter2D(dest, dst1, ddepth, kernel1, anchor, delta, BORDER_DEFAULT);
		filter2D(dest, dst2, ddepth, kernel2, anchor, delta, BORDER_DEFAULT);
		filter2D(dest, dst3, ddepth, kernel3, anchor, delta, BORDER_DEFAULT);
		filter2D(dest, dst4, ddepth, kernel4, anchor, delta, BORDER_DEFAULT);
		filter2D(dest, dst5, ddepth, kernel5, anchor, delta, BORDER_DEFAULT);
		filter2D(dest, dst6, ddepth, kernel6, anchor, delta, BORDER_DEFAULT);
		filter2D(dest, dst7, ddepth, kernel7, anchor, delta, BORDER_DEFAULT);
		filter2D(dest, dst8, ddepth, kernel8, anchor, delta, BORDER_DEFAULT);

		/*
		imshow(window_name1, dst1);
		imshow(window_name2, dst2);
		imshow(window_name3, dst3);
		imshow(window_name4, dst4);
		imshow(window_name5, dst5);
		imshow(window_name6, dst6);
		imshow(window_name7, dst7);
		imshow(window_name8, dst8);
		*/

		// choosing the maximum of respective features horizontal,vertical,right diagonal,left diagonal
		horim = max(dst1, dst2);
		verim = max(dst3, dst4);
		rdiag = max(dst5, dst6);
		ldiag = max(dst7, dst8);

		//Resizing to 4x4 image
		resize(horim, new_mat1, cvSize(kNewWidth, kNewHeight));
		resize(verim, new_mat2, cvSize(kNewWidth, kNewHeight));
		resize(rdiag, new_mat3, cvSize(kNewWidth, kNewHeight));
		resize(ldiag, new_mat4, cvSize(kNewWidth, kNewHeight));
		resize(dest, new_mat5, cvSize(kNewWidth, kNewHeight));

		// displaying images
		imshow(window1, new_mat1);
		imshow(window2, new_mat2);
		imshow(window3, new_mat3);
		imshow(window4, new_mat4);
		imshow(window5, new_mat5);
		imshow("indow", src);

		// converting into a 1D array of data\
												
		data1 = new_mat1.reshape(16, 1);
		data2 = new_mat2.reshape(16, 1);
		data3 = new_mat3.reshape(16, 1);
		data4 = new_mat4.reshape(16, 1);
		data5 = new_mat5.reshape(16, 1);
		/*
		// Displaying data
		cout << data1 << endl;
		cout << data2 << endl;
		cout << data3 << endl;
		cout << data4 << endl;
		cout << data5;
		*/


		// inverted data ie 255-x  because in order for the characters to be in white

		CvMat inv1, inv2, inv3, inv4, inv5;

		bitwise_not(data1, inv1);
		bitwise_not(data2, inv2);
		bitwise_not(data3, inv3);
		bitwise_not(data4, inv4);
		bitwise_not(data5, inv5);

		//displaying inverted matrices
		/*
		cout << inv1 << endl;
		cout << inv2 << endl;
		cout << inv3 << endl;
		cout << inv4 << endl;
		cout << inv5 << endl;
		*/
		// CONCATINATE to get single feature vector

		/*vector<Mat> matrices = {


		(1, 16, CV_32FC1, inv1),
		(1, 16, CV_32FC1, inv2),
		(1, 16, CV_32FC1, inv3),
		(1, 16, CV_32FC1, inv4),
		(1, 16, CV_32FC1, inv5)


		};*/

		//Mat arry[] = { inv1, inv2, inv3, inv4, inv5 };
		//Mat tempout;


		//hconcat(arry, 5, tempout);
		//cout << "Elements" << tempout;
		//cout << endl;
		//cout << "Contents are" << endl;

		int k = 0;
		float data[80];

		inv1.convertTo(inv1, CV_32FC1);
		for (int i = 0; i < 16; i++)
			data[k++] = (float) inv1.data[i];

		inv2.convertTo(inv2, CV_32FC1);
		for (int i = 0; i < 16; i++)
			data[k++] = (float)inv2.data[i];

		inv3.convertTo(inv3, CV_32FC1);
		for (int i = 0; i < 16; i++)
			data[k++] = (float)inv3.data[i];

		inv4.convertTo(inv4, CV_32FC1);
		for (int i = 0; i < 16; i++)
			data[k++] = (float)inv4.data[i];

		inv5.convertTo(inv5, CV_32FC1);
		for (int i = 0; i < 16; i++)
			data[k++] = (float)inv5.data[i];

		Mat out = Mat(1, 80, CV_32FC1, data);
		//cout <<"abskjvhfbvajlbrbvhb"<< out<<endl;
		for (int j = 0; j < 80; j++)
			dataAll[rowcount][j] = out.data[j];

		/*

		for (int i = 0; i < 80; i++)
		dataAll[rowcount][i] = out.data[i];
		*/


		// combining matrices to get 61x80
		/*float nullarry[1][80] =
		{
		{ 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0,
		0, 0, 0 }

		}; */
		/*
		try
		{
		cv::vconcat(matric, out);
		}
		catch (Exception e)
		{
		cout<<endl<<e.what()<<endl;
		}



		*/
		/*Mat1f m(1, 80);
		cout << m;
		*/
		//Deleting first row (keep outside for loop)



		//consider as functioon
		rowcount++;
	}

	Mat AllFeatures = Mat(60, 80, CV_32FC1, dataAll);
	cout <<"\n\n\n\n\n\n\n\n\n\n\n"<< AllFeatures;
	c = waitKey(0);
	return 0;


}