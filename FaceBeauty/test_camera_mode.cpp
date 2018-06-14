#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

#include "skinSegment.h"
#include "EdgePreservingFilter.h"
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
	cv::VideoCapture inVideo(0);
	if (!inVideo.isOpened()) {
		std::cout << "Fail to open camera." << std::endl;
		return -1;
	}

	cv::Mat inFrame, outFrame;
	Timer timer;
	while (1) {
		char key = cv::waitKey(10);
		if (key == 27) {
			break;
		}

		inVideo >> inFrame;
		if (inFrame.empty()) {
			break;
		}

		//-----------------------------------------------
		timer.start();

		whiteBalance(inFrame, outFrame);

		cv::Mat outFrame;
		//int d = 7;
		//filter_by_bilaterFiler_opencv(inFrame, outFrame, d, d * 2, d * 2);
		float sigma_spatial = 0.01f;
		float sigma_range = 0.09f;
		filter_by_rbf(inFrame, outFrame, sigma_spatial, sigma_range);

		//cv::Mat skinMask;
		//skinSegment_hsv(inFrame, skinMask);
		//skinSegment_ycbcr_cbcr(inFrame, skinMask);
		//cv::imshow("skinMask", skinMask);

		float level = 0.5;
		skinWhiten_brightness(outFrame, level);

		//cv::Mat mergeFrame = outFrame.clone();
		//std::function<bool(uint8_t)> func1 = [](uint8_t x) {return x == 255; };
		//mergeFrameMask(mergeFrame, inFrame, skinMask, func1);
		//cv::imshow("mergeFrame", mergeFrame);

		//float coef = 0.3;
		//frameEnhance(outFrame, inFrame, coef);

		float consumeTime = timer.elapsedTime();
		float fps = 1.0 / consumeTime;
		std::cout << "image -- " << inFrame.cols << "X" << inFrame.rows;
		std::cout << " -- " << consumeTime * 1000 << "ms";
		std::cout << " -- " << fps << "fps" << std::endl;

		cv::imshow("before", inFrame);
		cv::imshow("after", outFrame);
	}

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
