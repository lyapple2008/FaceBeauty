// https://github.com/ufoym/RecursiveBF

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "rbf.hpp"
#include <stdio.h>
#include <time.h>
#include <stdint.h>


class Timer {
private:
	unsigned long begTime;
public:
	void start() { begTime = clock(); }
	float elapsedTime() { return float((unsigned long)clock() - begTime) / CLOCKS_PER_SEC; }
};

int main(int argc, char*argv[])
{
	const int n = 10;
	const char * filename_in = "../TestSets/face_001.jpg";
	const char * filename_out = "face_001.jpg";
	float sigma_spatial = 0.03;
	float sigma_range = 0.1;

	int width, height, channel;
	unsigned char * img = stbi_load(filename_in, &width, &height, &channel, 0);
	unsigned char * img_out = new unsigned char[width*height*channel];
	float *buffer = new float[(width * height* channel
								+ width * height
								+ width * channel
								+ width) * 2];
	Timer timer;

	if (!img) {
		printf("Fail to open file %s", filename_in);
		return -1;
	}

	timer.start();
	for (int i = 0; i < n; i++) {
		//recursive_bf(img, img_out, sigma_spatial, sigma_range, width, height, channel, buffer);
		_recursive_bf(img, sigma_spatial, sigma_range, width, height, channel, buffer);
	}
	printf("Internal Buffer: %2.5fms\n", timer.elapsedTime() / n * 1000);

	// Í¼ÏñÈÚºÏ
	//float coef = 0.3;
	//uint8_t *raw_img = img;
	//uint8_t *filtered_img = img_out;
	//for (int i = 0; i < height; i++) {
	//	for (int j = 0; j < width; j++) {
	//		int r = (raw_img[0] - filtered_img[0]) * coef + filtered_img[0];
	//		int g = (raw_img[1] - filtered_img[1]) * coef + filtered_img[1];
	//		int b = (raw_img[2] - filtered_img[2]) * coef + filtered_img[2];
	//		if (r < 0) {
	//			r = 0;
	//		}
	//		else if (r > 255) {
	//			r = 255;
	//		}

	//		if (g < 0) {
	//			g = 0;
	//		}
	//		else if (g > 255) {
	//			g = 255;
	//		}

	//		if (b < 0) {
	//			b = 0;
	//		}
	//		else if (b > 255) {
	//			b = 255;
	//		}

	//		filtered_img[0] = r;
	//		filtered_img[1] = g;
	//		filtered_img[2] = b;

	//		raw_img += 3;
	//		filtered_img += 3;
	//	}
	//}

	//stbi_write_bmp(filename_out, width, height, channel, img_out);
	stbi_write_bmp(filename_out, width, height, channel, img);
	delete[] img;
	delete[] img_out;
	delete[] buffer;

	return 0;
}
