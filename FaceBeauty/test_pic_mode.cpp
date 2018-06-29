#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <math.h>
#include "EdgePreservingFilter.h"
#include "skinSegment.h"
#include "skinWhiten.h"
#include "preprocess.h"

class Timer {
private:
	unsigned long begTime;
public:
	void start() { begTime = clock(); }
	float elapsedTime() { return float((unsigned long)clock() - begTime) / CLOCKS_PER_SEC; }
};

typedef std::function<bool(uint8_t)> checkPixelFunc;

void mergeFrameMask(cv::Mat& outFrame, cv::Mat& inFrame, cv::Mat& maskFrame, checkPixelFunc func);
void frameEnhance(cv::Mat& outFrame, cv::Mat& inFrame, float coef);

int main(int argc, char* argv[])
{
	Timer timer;
	std::string picPath = "../TestSets/face_003.jpg";
	cv::Mat inFrame = cv::imread(picPath);
	if (inFrame.empty()) {
		std::cout << "Fail to open " << picPath << std::endl;
		return -1;
	}
	cv::imshow("before", inFrame);

	//-----------------------------------------------
	timer.start();

	cv::Mat outFrame = inFrame.clone();

	//whiteBalance(inFrame, outFrame);
	//cv::imshow("whiteBalance", outFrame);

	//int d = 7;
	//filter_by_bilaterFiler_opencv(inFrame, outFrame, d, d * 2, d * 2);
	float sigma_spatial = 0.01f;
	float sigma_range = 0.1f;
	int width = inFrame.cols;
	int height = inFrame.rows;
	int channel = inFrame.channels();
	const int width_height = width * height;
	const int width_channel = width * channel;
	const int width_height_channel = width * height * channel;
	float *buffer = new float[(width_height_channel + width_height
		+ width_channel + width) * 2];
	filter_by_rbf(outFrame, outFrame, sigma_spatial, sigma_range, buffer);
	delete[] buffer;
	//float lambda = 0.63;
	//int K = 10;
	//int niters = 2;
	//filter_by_AnisotropicFilter(inFrame, outFrame, lambda, K, niters);
	std::cout << timer.elapsedTime() * 1000 << std::endl;


	//cv::imwrite("result.jpg", outFrame);
	cv::Mat skinMask;
	skinSegment_ycbcr_cbcr(outFrame, skinMask);
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
	cv::dilate(skinMask, skinMask, element);
	cv::imshow("skinMask", skinMask);

	cv::Mat mergeFrame = outFrame.clone();
	//std::function<bool(uint8_t)> func1 = [](uint8_t x) {return x == 255; };
	//mergeFrameMask(mergeFrame, inFrame, skinMask, func1);
	//cv::seamlessClone(outFrame, inFrame, skinMask, cv::Point(outFrame.cols / 2, outFrame.rows / 2), mergeFrame, cv::NORMAL_CLONE);
	frame_enhance_with_mask(mergeFrame, inFrame, skinMask, 1.0);
	cv::imshow("mergeFrame", mergeFrame);

	float level = 0.5;
	skinWhiten_brightness(outFrame, level);

	float coef = 0.3;
	frameEnhance(outFrame, inFrame, coef);
	//frame_enhance_with_mask(outFrame, inFrame, skinMask, coef);


	float consumeTime = timer.elapsedTime();
	float fps = 1.0 / consumeTime;
	std::cout << "image -- " << inFrame.cols << "X" << inFrame.rows;
	std::cout << " -- " << consumeTime * 1000 << "ms";
	std::cout << " -- " << fps << "fps" << std::endl;

	
	cv::imshow("after", outFrame);
	cv::waitKey(0);

	return 0;
}

void mergeFrameMask(cv::Mat& outFrame, cv::Mat& inFrame, cv::Mat& maskFrame, checkPixelFunc func)
{
	int nr = inFrame.rows;
	int nc = inFrame.cols;
	int nChannels = inFrame.channels();

	if (nr != outFrame.rows ||
		nc != outFrame.cols ||
		nChannels != outFrame.channels() ||
		nr != maskFrame.rows ||
		nc != maskFrame.cols ||
		nChannels != maskFrame.channels() ||
		nChannels != 3) {
		return;
	}

	for (int i = 0; i < nr; i++) {
		uint8_t *inData = inFrame.ptr<uint8_t>(i);
		uint8_t *outData = outFrame.ptr<uint8_t>(i);
		uint8_t *maskData = maskFrame.ptr<uint8_t>(i);
		for (int j = 0; j < nc; j++) {
			if (func(maskData[j])) {
				outData[0] = inData[0];
				outData[1] = inData[1];
				outData[2] = inData[2];
			}
			inData += 3;
			outData += 3;
		}
	}
}

void frameEnhance(cv::Mat& outFrame, cv::Mat& inFrame, float coef)
{
	int nr = inFrame.rows;
	int nc = inFrame.cols;
	int nChannels = inFrame.channels();

	if (nr != outFrame.rows ||
		nc != outFrame.cols ||
		nChannels != outFrame.channels() ||
		nChannels != 3) {
		return;
	}

	for (int i = 0; i < nr; i++) {
		uint8_t *inData = inFrame.ptr<uint8_t>(i);
		uint8_t *outData = outFrame.ptr<uint8_t>(i);
		for (int j = 0; j < nc; j++) {
			int r = (inData[0] - outData[0]) * coef + outData[0];
			int g = (inData[1] - outData[1]) * coef + outData[1];
			int b = (inData[2] - outData[2]) * coef + outData[2];
			if (r < 0) {
				r = 0;
			}
			else if (r > 255) {
				r = 255;
			}

			if (g < 0) {
				g = 0;
			}
			else if (g > 255) {
				g = 255;
			}

			if (b < 0) {
				b = 0;
			}
			else if (b > 255) {
				b = 255;
			}

			outData[0] = r;
			outData[1] = g;
			outData[2] = b;

			inData += 3;
			outData += 3;
		}
	}

}
