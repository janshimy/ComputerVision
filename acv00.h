#ifndef HW00_H_
#define HW00_H_

#include <string>
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "util/DebugManager.h"
using namespace cv;
using namespace std;

class hw00{

public:
	/**
	 * This function is the top level function to perform hw00.
	 *
	 * @param inputImagePath: Color image as input.
	 * @param outputFilePath: Output file in which result is written.
	 */
	static void performHistEqualization(std::string inputImagePath, std::string outputFilePath);


	static void MatType( Mat inputMat )
	{
	    int inttype = inputMat.type();

	    string r, a;
	    uchar depth = inttype & CV_MAT_DEPTH_MASK;
	    uchar chans = 1 + (inttype >> CV_CN_SHIFT);
	    switch ( depth ) {
	        case CV_8U:  r = "8U";   a = "Mat.at<uchar>(y,x)"; break;
	        case CV_8S:  r = "8S";   a = "Mat.at<schar>(y,x)"; break;
	        case CV_16U: r = "16U";  a = "Mat.at<ushort>(y,x)"; break;
	        case CV_16S: r = "16S";  a = "Mat.at<short>(y,x)"; break;
	        case CV_32S: r = "32S";  a = "Mat.at<int>(y,x)"; break;
	        case CV_32F: r = "32F";  a = "Mat.at<float>(y,x)"; break;
	        case CV_64F: r = "64F";  a = "Mat.at<double>(y,x)"; break;
	        default:     r = "User"; a = "Mat.at<UKNOWN>(y,x)"; break;
	    }
	    r += "C";
	    r += (chans+'0');
	    cout << "Mat is of type " << r << " and should be accessed with " << a << endl;

	}

	static Mat getImageHistogram(Mat& inputImage){
		/// Separate the image in 3 places ( B, G and R )
		vector < Mat > bgr_planes;
		split(inputImage, bgr_planes);

		/// Establish the number of bins
		int histSize = 256;

		/// Set the ranges ( for B,G,R) )
		float range[] = { 0, 256 };
		const float *histRange = { range };

		bool uniform = true;
		bool accumulate = false;

		Mat b_hist, g_hist, r_hist;

		/// Compute the histograms:
		calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange,
				uniform, accumulate);
		calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange,
				uniform, accumulate);
		calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange,
				uniform, accumulate);

		// Draw the histograms for B, G and R
		int hist_w = 512;
		int hist_h = 400;
		int bin_w = cvRound((double) hist_w / histSize);

		Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

		/// Normalize the result to [ 0, histImage.rows ]
		normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
		normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
		normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

		/// Draw for each channel
		for (int i = 1; i < histSize; i++) {
			line(histImage,
					Point(bin_w * (i - 1),
							hist_h - cvRound(b_hist.at<float>(i - 1))),
					Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
					Scalar(255, 0, 0), 2, 8, 0);
			line(histImage,
					Point(bin_w * (i - 1),
							hist_h - cvRound(g_hist.at<float>(i - 1))),
					Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
					Scalar(0, 255, 0), 2, 8, 0);
			line(histImage,
					Point(bin_w * (i - 1),
							hist_h - cvRound(r_hist.at<float>(i - 1))),
					Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
					Scalar(0, 0, 255), 2, 8, 0);
		}
		return histImage;
	}

};


#endif
