// https://github.com/ShiqiYu/libfacedetection

#include <opencv2/opencv.hpp>
#include <facedetect-dll.h>

#define DETECT_BUFFER_SIZE	0x20000

int main(int argc, char* argv[])
{
	std::string picPath = "../TestSets/img_490.jpg";
	cv::Mat inFrame = cv::imread(picPath);

	if (inFrame.empty()) {
		std::cout << "Fail to open " << picPath << std::endl;
		return -1;
	}

	cv::Mat grayFrame;
	cv::cvtColor(inFrame, grayFrame, CV_BGR2GRAY);
	
	int *pResults = NULL;
	unsigned char* pBuffer = new unsigned char[DETECT_BUFFER_SIZE];
	if (!pBuffer) {
		std::cout << "Fail to malloc." << std::endl;
		return -1;
	}

	int doLandmark = 1;

	// face detection
	//pResults = facedetect_frontal(pBuffer, grayFrame.ptr(0), grayFrame.cols,
	//	grayFrame.rows, grayFrame.step, 1.2f, 2, 48, doLandmark);

	//pResults = facedetect_frontal_surveillance(pBuffer, grayFrame.ptr(0), grayFrame.cols,
	//	grayFrame.rows, grayFrame.step, 1.2f, 2, 48, doLandmark);

	//pResults = facedetect_multiview(pBuffer, grayFrame.ptr(0), grayFrame.cols,
	//	grayFrame.rows, grayFrame.step, 1.2f, 2, 48, doLandmark);

	pResults = facedetect_multiview_reinforce(pBuffer, grayFrame.ptr(0), grayFrame.cols,
		grayFrame.rows, grayFrame.step, 1.2f, 2, 48, 0, doLandmark);

	// draw result
	for (int i = 0; i < (pResults ? *pResults : 0); i++) {
		short * p = ((short*)(pResults + 1)) + 142 * i;
		int x = p[0];
		int y = p[1];
		int w = p[2];
		int h = p[3];
		int neighbors = p[4];
		int angle = p[5];

		printf("face_rect=[%d, %d, %d, %d], neighbors=%d, angle=%d\n", x, y, w, h, neighbors, angle);
		cv::rectangle(inFrame, cv::Rect(x, y, w, h), cv::Scalar(0, 255, 0), 2);
		if (doLandmark) {
			for (int j = 0; j < 68; j++) {
				cv::circle(inFrame, cv::Point((int)p[6 + 2 * j],
					(int)p[6 + 2 * j + 1]), 
					1, cv::Scalar(0, 255, 0));
			}
		}
	}

	cv::imshow("Results_frontal", inFrame);

	cv::waitKey(0);

	return 0;
}