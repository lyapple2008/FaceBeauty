#include "EdgePreservingFilter.h"
#include "rbf.hpp"
#include <opencv2/imgproc.hpp>

#define MIN2(a, b) ((a) < (b) ? (a) : (b))  
#define MAX2(a, b) ((a) > (b) ? (a) : (b))  
#define CLIP3(x, a, b) MIN2(MAX2(a,x), b)

void filter_by_rbf(cv::Mat& inFrame, cv::Mat& outFrame, 
							 float sigma_spatial, float sigma_range, float *buffer)
{
	uint8_t *img_out = outFrame.ptr(0);
	int width = inFrame.cols;
	int height = inFrame.rows;
	int channels = inFrame.channels();

	//outFrame = cv::Mat::zeros(inFrame.size(), inFrame.type());

	recursive_bf(inFrame.ptr(0), img_out, 
				sigma_spatial, sigma_range,
				width, height, channels, buffer);

	uint8_t *pImg = img_out;
	for (int i = 0; i < height; i++) {
		uint8_t *data = outFrame.ptr<uint8_t>(i);
		int rowLen = width * channels;
		memcpy(data, pImg, rowLen);
		pImg += rowLen;
	}

	//delete[] img_out;
}

void filter_by_bilaterFiler_opencv(cv::Mat& inFrame, cv::Mat& outFrame,
							int d, double sigma_color, double sigma_space)
{
	cv::bilateralFilter(inFrame, outFrame, d, sigma_color, sigma_space);
}

void filter_by_AnisotropicFilter(cv::Mat& inFrame, cv::Mat& outFrame, 
						float lambda, float k, int niters)
{
	outFrame = cv::Mat::zeros(inFrame.size(), inFrame.type());
	int nr = inFrame.rows;
	int nc = inFrame.cols;
	int nChannels = inFrame.channels();
	int stride = nc * nChannels;

	if (nChannels != 3) {
		return;
	}

	float expMap[512] = { 0.0 };
	float kk = 1.0f / (k * k);
	for (int i = -255; i <= 255; i++) {
		expMap[i + 255] = exp(-i*i*kk) * lambda;
	}

	int NI_B, SI_B, EI_B, WI_B;
	float CN_B, CS_B, CE_B, CW_B;
	int NI_G, SI_G, EI_G, WI_G;
	float CN_G, CS_G, CE_G, CW_G;
	int NI_R, SI_R, EI_R, WI_R;
	float CN_R, CS_R, CE_R, CW_R;
	uint8_t cur_B, cur_G, cur_R;
	for (int n = 0; n < niters; n++) {
		for (int i = 1; i < nr - 1; i++) {
			uint8_t* curData = inFrame.ptr<uint8_t>(i - 1);
			uint8_t* upData = inFrame.ptr<uint8_t>(i);
			uint8_t* downData = inFrame.ptr<uint8_t>(i + 1);
			uint8_t* outData = outFrame.ptr<uint8_t>(i);
			for (int j = nChannels; j < stride - nChannels; j += nChannels) {
				// B
				cur_B = curData[j];
				NI_B = upData[j] - cur_B;
				SI_B = downData[j] - cur_B;
				EI_B = curData[j + nChannels] - cur_B;
				WI_B = curData[j - nChannels] - cur_B;
				CN_B = expMap[NI_B + 255];
				CS_B = expMap[SI_B + 255];
				CE_B = expMap[EI_B + 255];
				CW_B = expMap[WI_B + 255];
				outData[j] = CLIP3(cur_B + lambda*(CN_B*NI_B + CS_B*SI_B + CE_B*EI_B + CW_B*WI_B), 0, 255);

				// G
				cur_G = curData[j + 1];
				NI_G = upData[j + 1] - cur_G;
				SI_G = downData[j + 1] - cur_G;
				EI_G = curData[j + nChannels + 1] - cur_G;
				WI_G = curData[j - nChannels + 1] - cur_G;
				CN_G = expMap[NI_G + 255];
				CS_G = expMap[SI_G + 255];
				CE_G = expMap[EI_G + 255];
				CW_G = expMap[WI_G + 255];
				outData[j + 1] = CLIP3(cur_G + lambda*(CN_G*NI_G + CS_G*SI_G + CE_G*EI_G + CW_G*WI_G), 0, 255);

				// R
				cur_R = curData[j + 2];
				NI_R = upData[j + 2] - cur_R;
				SI_R = downData[j + 2] - cur_R;
				EI_R = curData[j + nChannels + 2] - cur_R;
				WI_R = curData[j - nChannels + 2] - cur_R;
				CN_R = expMap[NI_R + 255];
				CS_R = expMap[SI_R + 255];
				CE_R = expMap[EI_R + 255];
				CW_R = expMap[WI_R + 255];
				outData[j + 2] = CLIP3(cur_R + lambda*(CN_R*NI_R + CS_R*SI_R + CE_R*EI_R + CW_R*WI_R), 0, 255);

			}
		}
	}
}
