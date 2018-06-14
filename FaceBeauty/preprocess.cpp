#include "preprocess.h"

void whiteBalance(cv::Mat& in, cv::Mat& out)
{
	std::vector<cv::Mat> imageRGB;
	cv::split(in, imageRGB);

	double R, G, B;
	B = cv::mean(imageRGB[0])[0];
	G = cv::mean(imageRGB[1])[0];
	R = cv::mean(imageRGB[2])[0];

	double KR, KG, KB;
	double meanRGB = (R + G + B) / 3;
	KB = meanRGB / B;
	KG = meanRGB / G;
	KR = meanRGB / R;

	imageRGB[0] *= KB;
	imageRGB[1] *= KG;
	imageRGB[2] *= KR;

	cv::merge(imageRGB, out);
}

