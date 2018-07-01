#include <opencv2\opencv.hpp>
#include <string>
#include <iostream>
#include "EdgePreservingFilter.h"

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
	float delta = 10 + denoiseLeval * denoiseLeval * 5;
	filter_by_localMeanSquareFilter(inFrame, outFrame, delta);
	
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