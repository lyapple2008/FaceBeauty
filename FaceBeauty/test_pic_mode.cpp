#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <math.h>
#include "EdgePreservingFilter.h"
#include "skinSegment.h"
#include "skinWhiten.h"

class Timer {
private:
	unsigned long begTime;
public:
	void start() { begTime = clock(); }
	float elapsedTime() { return float((unsigned long)clock() - begTime) / CLOCKS_PER_SEC; }
};

typedef std::function<bool(uint8_t)> checkPixelFunc;

void mergeFrameMask(cv::Mat& outFrame, cv::Mat& inFrame, cv::Mat& maskFrame, checkPixelFunc func);

int main(int argc, char* argv[])
{
	Timer timer;
	std::string picPath = "../TestSets/face_008.jpg";
	cv::Mat inFrame = cv::imread(picPath);
	if (inFrame.empty()) {
		std::cout << "Fail to open " << picPath << std::endl;
		return -1;
	}
	
	//-----------------------------------------------
	timer.start();

	cv::Mat outFrame;
	int d = 7;
	filter_by_bilaterFiler_opencv(inFrame, outFrame, d, d * 2, d * 2);
	//float sigma_spatial = 0.01f;
	//float sigma_range = 0.09f;
	//filter_by_rbf(inFrame, outFrame, sigma_spatial, sigma_range);
	
	cv::Mat skinMask;
	skinSegment_hsv(inFrame, skinMask);
	cv::imshow("skinMask", skinMask);

	float level = 0.5;
	skinWhiten_brightness(outFrame, level);

	cv::Mat mergeFrame = outFrame.clone();
	std::function<bool(uint8_t)> func1 = [](uint8_t x) {return x == 255; };
	mergeFrameMask(mergeFrame, inFrame, skinMask, func1);
	cv::imshow("mergeFrame", mergeFrame);

	float consumeTime = timer.elapsedTime();
	float fps = 1.0 / consumeTime;
	std::cout << "image -- " << inFrame.cols << "X" << inFrame.rows;
	std::cout << " -- " << consumeTime * 1000 << "ms";
	std::cout << " -- " << fps << "fps" << std::endl;

	cv::imshow("before", inFrame);
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
