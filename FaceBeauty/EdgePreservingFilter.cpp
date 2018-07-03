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

	//uint8_t *pImg = img_out;
	//for (int i = 0; i < height; i++) {
	//	uint8_t *data = outFrame.ptr<uint8_t>(i);
	//	int rowLen = width * channels;
	//	memcpy(data, pImg, rowLen);
	//	pImg += rowLen;
	//}

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
	outFrame = cv::Mat::zeros(inFrame.size(), CV_32SC3);
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

static void computeIntegralFrame(cv::Mat& inFrame, cv::Mat& integralFrame)
{
	int cols = inFrame.cols;
	int rows = inFrame.rows;
	int channels = inFrame.channels();

	if (cols == 0 || rows == 0 || channels != 3) {
		return;
	}

	integralFrame = cv::Mat::zeros(inFrame.size(), CV_32SC3);

	uint8_t* inData = inFrame.ptr<uint8_t>(0);
	int32_t* sumData = integralFrame.ptr<int32_t>(0);
	int sumB = 0, sumG = 0, sumR = 0;
	for (int c = 0; c < cols; c++) {
		sumB += inData[0];
		sumG += inData[1];
		sumR += inData[2];
		sumData[0] = sumB;
		sumData[1] = sumG;
		sumData[2] = sumR;

		inData += 3;
		sumData += 3;
	}

	for (int r = 1; r < rows; r++) {
		uint8_t* curData = inFrame.ptr<uint8_t>(r);
		int32_t* preSumData = integralFrame.ptr<int32_t>(r-1);
		int32_t* curSumData = integralFrame.ptr<int32_t>(r);
		sumB = 0; sumG = 0; sumR = 0;
		for (int c = 0; c < cols; c++) {
			sumB += curData[0];
			sumG += curData[1];
			sumR += curData[2];

			curSumData[0] = sumB + preSumData[0];
			curSumData[1] = sumG + preSumData[1];
			curSumData[2] = sumR + preSumData[2];

			curData += 3;
			preSumData += 3;
			curSumData += 3;
		}
	}
}

static void computePowIntegralFrame(cv::Mat& inFrame, cv::Mat& integralFrame)
{
	int cols = inFrame.cols;
	int rows = inFrame.rows;
	int channels = inFrame.channels();

	if (cols == 0 || rows == 0 || channels != 3) {
		return;
	}

	integralFrame = cv::Mat::zeros(inFrame.size(), CV_64FC3);

	uint8_t* inData = inFrame.ptr<uint8_t>(0);
	double* sumData = integralFrame.ptr<double>(0);
	double sumB = 0, sumG = 0, sumR = 0;
	for (int c = 0; c < cols; c++) {
		sumB += inData[0] * inData[0];
		sumG += inData[1] * inData[1];
		sumR += inData[2] * inData[2];
		sumData[0] = sumB;
		sumData[1] = sumG;
		sumData[2] = sumR;

		inData += 3;
		sumData += 3;
	}

	for (int r = 1; r < rows; r++) {
		uint8_t* curData = inFrame.ptr<uint8_t>(r);
		double* preSumData = integralFrame.ptr<double>(r-1);
		double* curSumData = integralFrame.ptr<double>(r);
		sumB = 0; sumG = 0; sumR = 0;
		for (int c = 0; c < cols; c++) {
			sumB += curData[0] * curData[0];
			sumG += curData[1] * curData[1];
			sumR += curData[2] * curData[2];

			curSumData[0] = sumB + preSumData[0];
			curSumData[1] = sumG + preSumData[1];
			curSumData[2] = sumR + preSumData[2];

			curData += 3;
			preSumData += 3;
			curSumData += 3;
		}
	}
}

void filter_by_localMeanSquareFilter(cv::Mat& inFrame, cv::Mat& outFrame, float delta)
{
	int rows = inFrame.rows;
	int cols = inFrame.cols;
	int channels = inFrame.channels();
	int winHalfW = MAX2(rows, cols) * 0.02;
	int winHalfH = winHalfW;

	if (rows < (2*winHalfH + 1) || cols < (2*winHalfW + 1) || channels != 3) {
		return ;
	}

	cv::Mat integralFrame;
	computeIntegralFrame(inFrame, integralFrame);

	cv::Mat powIntegralFrame;
	computePowIntegralFrame(inFrame, powIntegralFrame);

	outFrame = cv::Mat::zeros(inFrame.size(), inFrame.type());
	float invWinSize = 1.0 / ((2*winHalfH + 1) * (2*winHalfW + 1));
	for (int r = winHalfH; r < rows - winHalfH; r++) {
		int32_t* topIntData = integralFrame.ptr<int32_t>(r - winHalfH);
		int32_t* intData = integralFrame.ptr<int32_t>(r);
		int32_t* bottomIntData = integralFrame.ptr<int32_t>(r + winHalfH);
		double* topPowIntData = powIntegralFrame.ptr<double>(r - winHalfH);
		double* powIntData = powIntegralFrame.ptr<double>(r);
		double* bottomPowIntData = powIntegralFrame.ptr<double>(r + winHalfH);
		uint8_t* curOutData = outFrame.ptr<uint8_t>(r);
		uint8_t* curInData = inFrame.ptr<uint8_t>(r);

		float meanB = 0.0, meanG = 0.0, meanR = 0.0;
		float varB = 0.0, varG = 0.0, varR = 0.0;
		float kB = 0.0, kG = 0.0, kR = 0.0;
		for (int c = winHalfW; c < cols - winHalfW; c++) {
			meanB = bottomIntData[3 * (c + winHalfW)] - bottomIntData[3 * (c - winHalfW)] 
					- topIntData[3 * (c + winHalfW)] + topIntData[3 * (c - winHalfW)];
			meanG = bottomIntData[3 * (c + winHalfW) + 1] - bottomIntData[3 * (c - winHalfW) + 1] 
					- topIntData[3 * (c + winHalfW) + 1] + topIntData[3 * (c - winHalfW) + 1];
			meanR = bottomIntData[3 * (c + winHalfW) + 2] - bottomIntData[3 * (c - winHalfW) + 2] 
					- topIntData[3 * (c + winHalfW) + 2] + topIntData[3 * (c - winHalfW) + 2];

			varB = bottomPowIntData[3 * (c + winHalfW)] - bottomPowIntData[3 * (c - winHalfW)] 
					- topPowIntData[3 * (c + winHalfW)] + topPowIntData[3 * (c - winHalfW)];
			varG = bottomPowIntData[3 * (c + winHalfW) + 1] - bottomPowIntData[3 * (c - winHalfW) + 1] 
					- topPowIntData[3 * (c + winHalfW) + 1] + topPowIntData[3 * (c - winHalfW) + 1];
			varR = bottomPowIntData[3 * (c + winHalfW) + 2] - bottomPowIntData[3 * (c - winHalfW) + 2] 
					- topPowIntData[3 * (c + winHalfW) + 2] + topPowIntData[3 * (c - winHalfW) + 2];

			varB = invWinSize * (varB - invWinSize * meanB * meanB);
			varG = invWinSize * (varG - invWinSize * meanG * meanG);
			varR = invWinSize * (varR - invWinSize * meanR * meanR);

			meanB = invWinSize * meanB;
			meanG = invWinSize * meanG;
			meanR = invWinSize * meanR;

			kB = varB / (varB + delta);
			kG = varG / (varG + delta);
			kR = varR / (varR + delta);

			curOutData[3 * c] = CLIP3(curInData[3 * c] * kB + (1 - kB)*meanB, 0, 255);
			curOutData[3 * c + 1] = CLIP3(curInData[3 * c + 1] * kG + (1 - kG)*meanG, 0, 255);
			curOutData[3 * c + 2] = CLIP3(curInData[3 * c + 2] * kR + (1 - kR)*meanR, 0, 255);
		}
	}

	for (int r = 0; r < winHalfH; r++) {
		uint8_t* inData = inFrame.ptr<uint8_t>(r);
		uint8_t* outData = outFrame.ptr<uint8_t>(r);
		memcpy(outData, inData, cols * channels);
	}

	for (int r = rows - winHalfH; r < rows; r++) {
		uint8_t* inData = inFrame.ptr<uint8_t>(r);
		uint8_t* outData = outFrame.ptr<uint8_t>(r);
		memcpy(outData, inData, cols * channels);
	}

	for (int r = winHalfH; r < rows - winHalfH; r++) {
		uint8_t* inData = inFrame.ptr<uint8_t>(r);
		uint8_t* outData = outFrame.ptr<uint8_t>(r);
		memcpy(outData, inData, winHalfW * channels);
		inData += (cols - winHalfW) * channels;
		outData += (cols - winHalfW) * channels;
		memcpy(outData, inData, winHalfW * channels);
	}
}

void filter_by_localMeanSquareFilter_grayscale(cv::Mat& inFrame, cv::Mat& outFrame, int level)
{
	int rows = inFrame.rows;
	int cols = inFrame.cols;
	int channels = inFrame.channels();
	int winHalfW = MAX2(rows, cols) * 0.02;
	int winHalfH = winHalfW;

	if (rows < (2 * winHalfH + 1) || cols < (2 * winHalfW + 1) || channels != 1) {
		return;
	}
	
	float delta = 10 + level * level * 5;

	// compute sum and square sum frame
	cv::Mat sumFrame = cv::Mat::zeros(inFrame.size(), CV_32SC1);
	cv::Mat sqsumFrame = cv::Mat::zeros(inFrame.size(), CV_64FC1);
	uint8_t* inData = inFrame.ptr<uint8_t>(0);
	int32_t* sumData = sumFrame.ptr<int32_t>(0);
	double* sqsumData = sqsumFrame.ptr<double>(0);
	int sum = 0;
	double sqsum = 0.0;
	for (int c = 0; c < cols; c++) {
		sum += *inData;
		sqsum = (*inData) * (*inData);
		*sumData = sum;
		*sqsumData = sqsum;

		sumData++;
		sqsumData++;
	}

	for (int r = 1; r < rows; r++) {
		uint8_t* curData = inFrame.ptr<uint8_t>(r);
		int32_t* preSumData = sumFrame.ptr<int32_t>(r - 1);
		int32_t* curSumData = sumFrame.ptr<int32_t>(r);
		double* preSqsumData = sqsumFrame.ptr<double>(r - 1);
		double* curSqsumData = sqsumFrame.ptr<double>(r);
		sum = 0;
		sqsum = 0.0;
		for (int c = 0; c < cols; c++) {
			sum += curData[c];
			sqsum += curData[c] * curData[c];

			curSumData[c] = sum + preSumData[c];
			curSqsumData[c] = sqsum + preSqsumData[c];
		}
	}

	outFrame = cv::Mat::zeros(inFrame.size(), inFrame.type());
	float invWinSize = 1.0 / ((2 * winHalfH + 1) * (2 * winHalfW + 1));
	for (int r = winHalfH; r < rows - winHalfH; r++) {
		int32_t* topSumData = sumFrame.ptr<int32_t>(r - winHalfH);
		int32_t* bottomSumData = sumFrame.ptr<int32_t>(r + winHalfH);
		double* topSqsumData = sqsumFrame.ptr<double>(r - winHalfH);
		double* bottomSqsumData = sqsumFrame.ptr<double>(r + winHalfH);
		uint8_t* inData = inFrame.ptr<uint8_t>(r);
		uint8_t* outData = outFrame.ptr<uint8_t>(r);
		float mean = 0.0, var = 0.0, k = 0.0;

		for (int c = winHalfW; c < cols - winHalfW; c++) {
			mean = bottomSumData[c + winHalfW] - bottomSumData[c - winHalfW]
			       - topSumData[c + winHalfW] + topSumData[c - winHalfW];
			var = bottomSqsumData[c + winHalfW] - bottomSqsumData[c - winHalfH]
				- topSumData[c + winHalfW] + topSumData[c - winHalfW];

			var = invWinSize * (var - invWinSize * mean * mean);
			mean = invWinSize * mean;
			k = var / (var + delta);

			outData[c] = CLIP3(inData[c] * k + (1 - k) * mean, 0, 255);
		}
	}

	for (int r = 0; r < winHalfH; r++) {
		uint8_t* inData = inFrame.ptr<uint8_t>(r);
		uint8_t* outData = outFrame.ptr<uint8_t>(r);
		memcpy(outData, inData, cols);
	}

	for (int r = rows - winHalfH; r < rows; r++) {
		uint8_t* inData = inFrame.ptr<uint8_t>(r);
		uint8_t* outData = outFrame.ptr<uint8_t>(r);
		memcpy(outData, inData, cols);
	}

	for (int r = winHalfH; r < rows - winHalfH; r++) {
		uint8_t* inData = inFrame.ptr<uint8_t>(r);
		uint8_t* outData = outFrame.ptr<uint8_t>(r);
		memcpy(outData, inData, winHalfW);
		inData += (cols - winHalfW);
		outData += (cols - winHalfW);
		memcpy(outData, inData, winHalfW);
	}
}