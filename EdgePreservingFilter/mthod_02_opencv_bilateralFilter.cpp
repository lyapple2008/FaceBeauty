#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;

//定义全局变量
const int g_ndMaxValue = 100;
const int g_nsigmaColorMaxValue = 200;
const int g_nsigmaSpaceMaxValue = 200;
int g_ndValue;
int g_nsigmaColorValue;
int g_nsigmaSpaceValue;

Mat g_srcImage;
Mat g_dstImage;

//定义回调函数
void on_bilateralFilterTrackbar(int, void*);

int main()
{
	g_srcImage = imread("../TestSets/face_003.jpg");

	//判断图像是否加载成功
	if (g_srcImage.empty())
	{
		cout << "图像加载失败!" << endl;
		return -1;
	}
	else
		cout << "图像加载成功!" << endl << endl;

	namedWindow("原图像", WINDOW_AUTOSIZE);
	imshow("原图像", g_srcImage);

	//定义输出图像窗口属性和轨迹条属性
	namedWindow("双边滤波图像", WINDOW_AUTOSIZE);
	g_ndValue = 10;
	g_nsigmaColorValue = 10;
	g_nsigmaSpaceValue = 10;

	char dName[20];
	sprintf(dName, "邻域直径 %d", g_ndMaxValue);

	char sigmaColorName[20];
	sprintf(sigmaColorName, "sigmaColor %d", g_nsigmaColorMaxValue);

	char sigmaSpaceName[20];
	sprintf(sigmaSpaceName, "sigmaSpace %d", g_nsigmaSpaceMaxValue);

	//创建轨迹条
	createTrackbar(dName, "双边滤波图像", &g_ndValue, g_ndMaxValue, on_bilateralFilterTrackbar);
	on_bilateralFilterTrackbar(g_ndValue, 0);

	createTrackbar(sigmaColorName, "双边滤波图像", &g_nsigmaColorValue,
		g_nsigmaColorMaxValue, on_bilateralFilterTrackbar);
	on_bilateralFilterTrackbar(g_nsigmaColorValue, 0);

	createTrackbar(sigmaSpaceName, "双边滤波图像", &g_nsigmaSpaceValue,
		g_nsigmaSpaceMaxValue, on_bilateralFilterTrackbar);
	on_bilateralFilterTrackbar(g_nsigmaSpaceValue, 0);

	waitKey(0);

	return 0;
}

void on_bilateralFilterTrackbar(int, void*)
{
	bilateralFilter(g_srcImage, g_dstImage, g_ndValue, g_nsigmaColorValue, g_nsigmaSpaceValue);
	imshow("双边滤波图像", g_dstImage);
}