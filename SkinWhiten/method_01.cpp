#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

#define PI	3.1415926

void skinWhiten_01(cv::Mat& img, float level);

int main(int argc, char* argv[])
{
	std::string picPath = "../TestSets/face_003.jpg";
	cv::Mat inFrame = cv::imread(picPath);
	if (inFrame.empty()) {
		std::cout << "Fail to open " << picPath << std::endl;
		return -1;
	}

	cv::Mat resultFrame = inFrame.clone();

	float level = 0.5;
	skinWhiten_01(resultFrame, level);

	cv::imshow("before", inFrame);
	cv::imshow("after", resultFrame);
	cv::waitKey(0);

	return 0;
}

void skinWhiten_01(cv::Mat& img, float level)
{
	int brightness = (int)(level * 100);
	uint8_t lookupTable[256];
	for (int i = 0; i < 256; i++) {
		lookupTable[i] = (uint8_t)MIN(255, MAX(0, i + sin(PI*i / 255.0f)*brightness));
	}

	int nr = img.rows;
	int nc = img.cols;

	for (int i = 0; i < nr; i++) {
		uint8_t* data = img.ptr<uint8_t>(i);
		for (int j = 0; j < nc; j++) {
			uint8_t B = data[0];
			uint8_t G = data[1];
			uint8_t R = data[2];

			data[0] = lookupTable[B];
			data[1] = lookupTable[G];
			data[2] = lookupTable[R];
			
			data += img.channels();
		}
	}
}