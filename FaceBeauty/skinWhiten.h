#ifndef SKIN_WHITEN_H
#define SKIN_WHITEN_H

#include <opencv2/core.hpp>

// level: 0~1.0
void skinWhiten_brightness(cv::Mat& img, float level);

// logarithmic Curve
void skinWhiten_logCurve(cv::Mat& img, uint8_t logTable);
#endif
