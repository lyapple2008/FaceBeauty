#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
	std::string picPath = "../TestSets/face_009.jpg";
	cv::Mat inFrame = cv::imread(picPath);
	if (inFrame.empty()) {
		std::cout << "Fail to open " << picPath << std::endl;
		return -1;
	}

	std::vector<cv::Mat> imageRGB;
	cv::split(inFrame, imageRGB);

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

	cv::Mat outFrame;
	cv::merge(imageRGB, outFrame);

	cv::imshow("before", inFrame);
	cv::imshow("after", outFrame);
	cv::waitKey(0);

	return 0;
}