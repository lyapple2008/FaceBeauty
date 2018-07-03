#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include "../FaceBeauty/skinWhiten.h"
#include "../FaceBeauty/skinSegment.h"
#include "../FaceBeauty/preprocess.h"

int main(int argc, char* argv[])
{
	std::string picPath = "../TestSets/face_010.jpg";
	cv::Mat inFrame = cv::imread(picPath);
	if (inFrame.empty()) {
		std::cout << "Fail to open " << picPath << std::endl;
		return -1;
	}

	cv::Mat resultFrame = inFrame.clone();

	//float level = 0.5;
	//skinWhiten_brightness(resultFrame, level);

	cv::Mat skinMask;
	//skinSegment_ycbcr_cbcr(inFrame, skinMask);
	skinSegment_rgb(inFrame, skinMask);
	cv::blur(skinMask, skinMask, cv::Size(45, 45));
	cv::imshow("skinMask", skinMask);

	int beta = 6;
	uint8_t table[256];
	skinWhiten_genLogCurve(table, beta);
	skinWhiten_mapTable(resultFrame, inFrame, table, table, table);

	blendImageWithMask(resultFrame, inFrame, skinMask);

	cv::imshow("before", inFrame);
	cv::imshow("after", resultFrame);
	cv::waitKey(0);

	return 0;
}