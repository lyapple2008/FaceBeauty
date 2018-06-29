#include "preprocess.h"

#define MIN2(a, b) ((a) < (b) ? (a) : (b))  
#define MAX2(a, b) ((a) > (b) ? (a) : (b))  
#define CLIP3(x, a, b) MIN2(MAX2(a,x), b)

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

void frame_enhance_with_mask(cv::Mat & outImg, cv::Mat & inImg, cv::Mat & maskImg, float coef)
{
	int nr = inImg.rows;
	int nc = inImg.cols;
	int nChannels = inImg.channels();

	if (nr != outImg.rows ||
		nc != outImg.cols ||
		nChannels != outImg.channels() ||
		nChannels != 3) {
		return;
	}

	for (int i = 0; i < nr; i++) {
		uint8_t *inData = inImg.ptr<uint8_t>(i);
		uint8_t *outData = outImg.ptr<uint8_t>(i);
		uint8_t *maskData = maskImg.ptr<uint8_t>(i);
		for (int j = 0; j < nc; j++) {
			if (maskData[j] == 255) {
				outData[0] = CLIP3(outData[0] * 0.8 + inData[0] * 0.2, 0, 255);
				outData[1] = CLIP3(outData[1] * 0.8 + inData[1] * 0.2, 0, 255);
				outData[2] = CLIP3(outData[2] * 0.8 + inData[2] * 0.2, 0, 255);
			}
			else {
				outData[0] = CLIP3(inData[0] * 0.8 + inData[0] * 0.2, 0, 255);
				outData[1] = CLIP3(inData[1] * 0.8 + inData[1] * 0.2, 0, 255);
				outData[2] = CLIP3(inData[2] * 0.8 + inData[2] * 0.2, 0, 255);
			}
			
			inData += 3;
			outData += 3;
		}
	}
}

