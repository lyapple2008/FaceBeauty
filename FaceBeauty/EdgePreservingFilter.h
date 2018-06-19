#ifndef EDGE_PRESERVING_FILTER_H
#define EDGE_PRESERVING_FILTER_H

#include <opencv2/core.hpp>

void filter_by_rbf(cv::Mat& inFrame, cv::Mat& outFrame, 
						     float sigma_spatial, float sigma_range, float *buffer = 0);

void filter_by_bilaterFiler_opencv(cv::Mat& inFrame, cv::Mat& outFrame,
							int d, double sigma_color, double sigma_space);

void filter_by_AnisotropicFilter(cv::Mat& inFrame, cv::Mat& outFrame,
							float lambda, float k, int niters);


#endif
