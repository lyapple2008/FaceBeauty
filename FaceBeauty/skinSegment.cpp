#include "skinSegment.h"
#include <opencv2/imgproc.hpp>

static uint8_t local_max(uint8_t a, uint8_t b, uint8_t c)
{
	uint8_t max_data = a;
	if (b > max_data) {
		max_data = b;
	}
	if (c > max_data) {
		max_data = c;
	}
	return max_data;
}

static uint8_t local_min(uint8_t a, uint8_t b, uint8_t c)
{
	uint8_t min_data = a;
	if (b < min_data) {
		min_data = b;
	}
	if (c < min_data) {
		min_data = c;
	}
	return min_data;
}


void skinSegment_rgb(cv::Mat& inFrame, cv::Mat& binaryFrame)
{
	binaryFrame = cv::Mat(inFrame.size(), CV_8UC1, cv::Scalar(0));
	int nr = inFrame.rows;
	int nc = inFrame.cols;

	for (int i = 0; i < nr; i++) {
		uint8_t* data = inFrame.ptr<uint8_t>(i);
		uint8_t* binaryData = binaryFrame.ptr<uint8_t>(i);
		for (int j = 0; j < nc; j++) {
			uint8_t B = data[0];
			uint8_t G = data[1];
			uint8_t R = data[2];

			bool isSkin = false;
			if (R > 95 && G > 40 && B > 20 && R > G && R > B
				&& local_max(R, G, B) - local_min(R, G, B) > 15
				&& std::abs(R - G) > 15) {
				binaryData[j] = 255;
			}
			data += inFrame.channels();
		}
	}
}

void skinSegment_ycbcr_cbcr(cv::Mat& inFrame, cv::Mat& binaryFrame)
{
	uint8_t RANGE_CB_MIN = 77;
	uint8_t RANGE_CB_MAX = 127;
	uint8_t RANGE_CR_MIN = 133;
	uint8_t RANGE_CR_MAX = 173;
	binaryFrame = cv::Mat(inFrame.size(), CV_8UC1, cv::Scalar(0));
	cv::Mat ycbcrFrame;
	cv::cvtColor(inFrame, ycbcrFrame, CV_BGR2YCrCb);

	int nr = inFrame.rows;
	int nc = inFrame.cols;

	for (int i = 0; i < nr; i++) {
		uint8_t* data = ycbcrFrame.ptr<uint8_t>(i);
		uint8_t* binaryData = binaryFrame.ptr<uint8_t>(i);
		for (int j = 0; j < nc; j++) {
			uint8_t cr = data[1];
			uint8_t cb = data[2];
			if (cr >= RANGE_CR_MIN && cr <= RANGE_CR_MAX &&
				cb >= RANGE_CB_MIN && cb <= RANGE_CB_MAX) {
				binaryData[j] = 255;
			}
			data += inFrame.channels();
		}
	}
}

void skinSegment_hsv(cv::Mat& inFrame, cv::Mat& binaryFrame)
{
	binaryFrame = cv::Mat(inFrame.size(), CV_8UC1, cv::Scalar(0));
	cv::Mat ycbcrFrame;
	cv::cvtColor(inFrame, ycbcrFrame, CV_BGR2HSV);

	int nr = inFrame.rows;
	int nc = inFrame.cols;

	for (int i = 0; i < nr; i++) {
		uint8_t* data = ycbcrFrame.ptr<uint8_t>(i);
		uint8_t* binaryData = binaryFrame.ptr<uint8_t>(i);
		for (int j = 0; j < nc; j++) {
			uint8_t h = data[0];
			uint8_t s = data[1];
			uint8_t v = data[2];
			if (h >= 0 && h <= 20 && s >= 48 && v >= 50) {
				binaryData[j] = 255;
			}
			data += inFrame.channels();
		}
	}

}

void skinSegment_ycbcr_cb(cv::Mat& inFrame, cv::Mat& binaryFrame)
{
	binaryFrame = cv::Mat(inFrame.size(), CV_8UC1, cv::Scalar(0));
	cv::Mat ycrcb_image;
	cv::cvtColor(inFrame, ycrcb_image, CV_BGR2YCrCb); 
	cv::Mat detect;
	std::vector<cv::Mat> channels;
	split(ycrcb_image, channels);
	binaryFrame = channels[1];
	channels[1].copyTo(binaryFrame);
	cv::threshold(binaryFrame, binaryFrame, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
}
