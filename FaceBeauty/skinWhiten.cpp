#include "skinWhiten.h"

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

