#include "skinWhiten.h"

#define MIN2(a, b) ((a) < (b) ? (a) : (b))  
#define MAX2(a, b) ((a) > (b) ? (a) : (b))  
#define CLIP3(x, a, b) MIN2(MAX2(a,x), b)

void skinWhiten_brightness(cv::Mat& img, float level)
{
	int brightness = (int)(level * 100);
	uint8_t lookupTable[256];
	for (int i = 0; i < 256; i++) {
		lookupTable[i] = (uint8_t)MIN(255, MAX(0, i + sin(M_PI*i / 255.0f)*brightness));
	}

	int nr = img.rows;
	int nc = img.cols;

	for (int i = 0; i < nr; i++) {
		uint8_t* data = img.ptr<uint8_t>(i);
		for (int j = 0; j < nc; j++) {
			uint8_t B = data[0];
			uint8_t G = data[1];
			uint8_t R = data[2];

			data[0] = lookupTable[B];
			data[1] = lookupTable[G];
			data[2] = lookupTable[R];

			data += img.channels();
		}
	}
}

void skinWhiten_genLogCurve(uint8_t table[256], int beta)
{
	float ivLogBeta = 255.0 / log(beta);
	for (int i = 0; i < 256; i++) {
		table[i] = CLIP3(log(i * (beta - 1) / 255.0 + 1) * ivLogBeta, 0, 255);
	}
}

void skinWhiten_mapTable(cv::Mat & outFrame, cv::Mat & inFrame,
	uint8_t rTable[256], uint8_t gTable[256], uint8_t bTable[256])
{
	int rows = inFrame.rows;
	int cols = inFrame.cols;
	int channels = inFrame.channels();

	if (rows != outFrame.rows || cols != outFrame.cols || 
		channels != outFrame.channels() || channels != 3) {
		return;
	}

	for (int r = 0; r < rows; r++) {
		uint8_t* inData = inFrame.ptr<uint8_t>(r);
		uint8_t* outData = outFrame.ptr<uint8_t>(r);
		for (int c = 0; c < cols; c++) {
			outData[0] = bTable[inData[0]];
			outData[1] = gTable[inData[1]];
			outData[2] = rTable[inData[2]];

			inData += 3;
			outData += 3;
		}
	}
}

