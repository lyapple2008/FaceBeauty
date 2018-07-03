#include <opencv2\opencv.hpp>
#include <string>
#include <iostream>
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

int main(int argc, char* argv[])
{
	Timer timer;
	std::string picPath = "../TestSets/face_003.jpg";
	cv::Mat inFrame = cv::imread(picPath);
	if (inFrame.empty()) {
		std::cout << "Fail to open " << picPath << std::endl;
		return -1;
	}

	timer.start();

	cv::Mat outFrame;
	int denoiseLeval = 10;
	cv::Mat ycbcrFrame;
	cv::cvtColor(inFrame, ycbcrFrame, CV_BGR2YCrCb);
	std::vector<cv::Mat> channelsFrame;
	cv::split(ycbcrFrame, channelsFrame);
	cv::Mat yFrame = channelsFrame.at(0);
	cv::Mat yFrame_filtered;
	int filterLevel = 8;
	filter_by_localMeanSquareFilter_grayscale(yFrame, yFrame_filtered, filterLevel);
	std::vector<cv::Mat> newChannelsFrame;
	newChannelsFrame.push_back(yFrame_filtered);
	newChannelsFrame.push_back(channelsFrame.at(1));
	newChannelsFrame.push_back(channelsFrame.at(2));
	cv::merge(newChannelsFrame, outFrame);
	cv::imshow("denoise", outFrame);


	float level = 0.5;
	skinWhiten_brightness(outFrame, level);
	cv::imshow("skinWhiten", outFrame);

	cv::Mat skinMask;
	skinSegment_ycbcr_cbcr(outFrame, skinMask);
	cv::imshow("skinMask", skinMask);
	
	cv::blur(skinMask, skinMask, cv::Size(5, 5));
	cv::imshow("blur skinMask", skinMask);

	blendImageWithMask(outFrame, inFrame, skinMask);
	cv::imshow("blend image", outFrame);

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