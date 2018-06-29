#include <opencv2/opencv.hpp>

void skinSegment_01(cv::Mat& inFrame, cv::Mat& binaryFrame);
void skinSegment_02(cv::Mat& inFrame, cv::Mat& binaryFrame);
void skinSegment_03(cv::Mat& inFrame, cv::Mat& binaryFrame);
void skinSegment_04(cv::Mat& inFrame, cv::Mat& binaryFrame);

int main(int argc, char* argv[])
{
	std::string picPath = "../TestSets/face_003.jpg";
	cv::Mat inFrame = cv::imread(picPath);

	if (inFrame.empty()) {
		std::cout << "Fail to open " << picPath << std::endl;
		return -1;
	}

	cv::Mat maskFrame = cv::Mat(inFrame.rows, inFrame.cols, CV_8UC1, cv::Scalar(0));

	//skinSegment_01(inFrame, maskFrame);
	skinSegment_02(inFrame, maskFrame);
	//skinSegment_03(inFrame, maskFrame);
	//skinSegment_04(inFrame, maskFrame);

	cv::imshow("inFrame", inFrame);
	cv::imshow("maskFrame", maskFrame);

	cv::waitKey(0);

	return 0;
}

static uint8_t local_max(uint8_t a, uint8_t b, uint8_t c)
{
	uint8_t max_data = a;
	if (b > max_data) {
		max_data = b;
	}
	if (c > max_data) {
		max_data = c;
	}
	return max_data;
}

static uint8_t local_min(uint8_t a, uint8_t b, uint8_t c)
{
	uint8_t min_data = a;
	if (b < min_data) {
		min_data = b;
	}
	if (c < min_data) {
		min_data = c;
	}
	return min_data;
}


void skinSegment_01(cv::Mat& inFrame, cv::Mat& binaryFrame)
{
	int nr = inFrame.rows;
	int nc = inFrame.cols;

	for (int i = 0; i < nr; i++) {
		uint8_t* data = inFrame.ptr<uint8_t>(i);
		uint8_t* binaryData = binaryFrame.ptr<uint8_t>(i);
		for (int j = 0; j < nc; j++) {
			uint8_t B = data[0];
			uint8_t G = data[1];
			uint8_t R = data[2];

			bool isSkin = false;
			if (R > 95 && G > 40 && B > 20 && R > G && R > B
				&& local_max(R, G, B) - local_min(R, G, B) > 15
				&& std::abs(R - G) > 15) {
				binaryData[j] = 255;
			}
			data += inFrame.channels();
		}
	}
}

void skinSegment_02(cv::Mat& inFrame, cv::Mat& binaryFrame)
{
	cv::Mat ycbcrFrame;
	cv::cvtColor(inFrame, ycbcrFrame, CV_BGR2YCrCb);

	int nr = inFrame.rows;
	int nc = inFrame.cols;

	for (int i = 0; i < nr; i++) {
		uint8_t* data = ycbcrFrame.ptr<uint8_t>(i);
		uint8_t* binaryData = binaryFrame.ptr<uint8_t>(i);
		for (int j = 0; j < nc; j++) {
			uint8_t cr = data[1];
			uint8_t cb = data[2];
			if (cr >= 133 && cr <= 173 &&
				cb >= 77 && cb <= 127) {
				binaryData[j] = 255;
			}
			data += inFrame.channels();
		}
	}
}

void skinSegment_03(cv::Mat& inFrame, cv::Mat& binaryFrame)
{
	cv::Mat ycbcrFrame;
	cv::cvtColor(inFrame, ycbcrFrame, CV_BGR2HSV);

	int nr = inFrame.rows;
	int nc = inFrame.cols;

	for (int i = 0; i < nr; i++) {
		uint8_t* data = ycbcrFrame.ptr<uint8_t>(i);
		uint8_t* binaryData = binaryFrame.ptr<uint8_t>(i);
		for (int j = 0; j < nc; j++) {
			uint8_t h = data[0];
			uint8_t s = data[1];
			uint8_t v = data[2];
			if (h >= 0 && h <= 20 && s >= 48 && v >= 50) {
				binaryData[j] = 255;
			}
			data += inFrame.channels();
		}
	}

}

void skinSegment_04(cv::Mat& inFrame, cv::Mat& binaryFrame)
{
	cv::Mat ycrcb_image;
	cv::cvtColor(inFrame, ycrcb_image, CV_BGR2YCrCb); //首先转换成到YCrCb空间
	cv::Mat detect;
	std::vector<cv::Mat> channels;
	split(ycrcb_image, channels);
	binaryFrame = channels[1];
	channels[1].copyTo(binaryFrame);
	cv::threshold(binaryFrame, binaryFrame, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
}