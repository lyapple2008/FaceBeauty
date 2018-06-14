#include <opencv2/opencv.hpp>
#include <string>
#include "guidedfilter.h"

class Timer {
private:
	unsigned long begTime;
public:
	void start() { begTime = clock(); }
	float elapsedTime() { return float((unsigned long)clock() - begTime) / CLOCKS_PER_SEC; }
};

int main(int argc, char* argv[])
{
	std::string picPath = "../TestSets/face_004.jpg";
	cv::Mat inFrame = cv::imread(picPath);
	if (inFrame.empty()) {
		std::cout << "Fail to open " << picPath << std::endl;
		return -1;
	}

	Timer timer;
	timer.start();
	cv::Mat tmpFrame = inFrame;

	int r = 4; // try r=2, 4, or 8
	double eps = 0.1 * 0.1; // try eps=0.1^2, 0.2^2, 0.4^2

	eps *= 255 * 255;   // Because the intensity range of our images is [0, 255]

	cv::Mat outFrame = guidedFilter(inFrame, tmpFrame, r, eps);

	//cv::Mat enhancedFrame = (inFrame - outFrame) * 2 + outFrame;
	//cv::imshow("enhanceFrame", enhancedFrame);

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