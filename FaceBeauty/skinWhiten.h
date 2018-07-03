#ifndef SKIN_WHITEN_H
#define SKIN_WHITEN_H

#include <opencv2/core.hpp>
#include <math.h>

// level: 0~1.0
void skinWhiten_brightness(cv::Mat& img, float level);

// logarithmic Curve
void skinWhiten_genLogCurve(uint8_t table[256], int beta);
void skinWhiten_mapTable(cv::Mat& outFrame, cv::Mat& inFrame,
	uint8_t rTable[256], uint8_t gTable[256], uint8_t bTable[256]);


#endif
