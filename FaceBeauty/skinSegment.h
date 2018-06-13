#ifndef SKIN_SEGMENT_H
#define SKIN_SEGMENT_H

#include <opencv2/core.hpp>

void skinSegment_rgb(cv::Mat& inFrame, cv::Mat& binaryFrame);
void skinSegment_ycbcr_cbcr(cv::Mat& inFrame, cv::Mat& binaryFrame);
void skinSegment_hsv(cv::Mat& inFrame, cv::Mat& binaryFrame);
void skinSegment_ycbcr_cb(cv::Mat& inFrame, cv::Mat& binaryFrame);

#endif
