#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <opencv2/core.hpp>

void whiteBalance(cv::Mat& in, cv::Mat& out);

// in maskImg region
// outImg = (inImg - outImg) * coef + outImg
// else region
// outImg = inImg;
void frame_enhance_with_mask(cv::Mat& outImg, cv::Mat& inImg, cv::Mat& maskImg, float coef);

void blendImageWithMask(cv::Mat& outImg, cv::Mat& inImg, cv::Mat& maskImg);

#endif
