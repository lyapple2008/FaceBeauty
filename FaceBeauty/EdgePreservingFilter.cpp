#include "EdgePreservingFilter.h"
#include "rbf.hpp"
#include <opencv2/imgproc.hpp>

void filter_by_rbf(cv::Mat& inFrame, cv::Mat& outFrame, 
							 float sigma_spatial, float sigma_range)
{
	uint8_t *img_out = NULL;
	int width = inFrame.cols;
	int height = inFrame.rows;
	int channels = inFrame.channels();

	outFrame = cv::Mat::zeros(inFrame.size(), inFrame.type());

	recursive_bf(inFrame.ptr(0), img_out, 
				sigma_spatial, sigma_range,
				width, height, channels);

	uint8_t *pImg = img_out;
	for (int i = 0; i < height; i++) {
		uint8_t *data = outFrame.ptr<uint8_t>(i);
		int rowLen = width * channels;
		memcpy(data, pImg, rowLen);
		pImg += rowLen;
	}

	delete[] img_out;
}

void filter_by_bilaterFiler_opencv(cv::Mat& inFrame, cv::Mat& outFrame,
							int d, double sigma_color, double sigma_space)
{
	cv::bilateralFilter(inFrame, outFrame, d, sigma_color, sigma_space);
}