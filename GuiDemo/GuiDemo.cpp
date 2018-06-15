#include "GuiDemo.h"
#include <QTimer>
#include <iostream>

GuiDemo::GuiDemo(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	if (initVideoCapture()) {
		rate = videoCapture.get(cv::CAP_PROP_FPS);
		rate = 15.0f;
		videoCapture >> mCVFrame;
		if (!mCVFrame.empty()) {
			mImage = mat2Image(mCVFrame);
			ui.imageLabel->setPixmap(QPixmap::fromImage(mImage));
			mTimer = new QTimer(this);
			mTimer->setInterval(1000 / rate);
			connect(mTimer, SIGNAL(timeout()), this, SLOT(nextFrame()));
			mTimer->start();
		}
	}
}

bool GuiDemo::initVideoCapture()
{
	videoCapture.open(0);
	
	bool ret = videoCapture.isOpened();

	return ret;
}

void GuiDemo::faceBeautyProcess()
{

}

QImage GuiDemo::mat2Image(cv::Mat& cvImg)
{
	QImage qImg;
	if (cvImg.channels() == 3)                             //3 channels color image
	{

		cv::cvtColor(cvImg, cvImg, CV_BGR2RGB);
		qImg = QImage((const unsigned char*)(cvImg.data),
			cvImg.cols, cvImg.rows,
			cvImg.cols*cvImg.channels(),
			QImage::Format_RGB888);
	}
	else if (cvImg.channels() == 1)                    //grayscale image
	{
		qImg = QImage((const unsigned char*)(cvImg.data),
			cvImg.cols, cvImg.rows,
			cvImg.cols*cvImg.channels(),
			QImage::Format_Indexed8);
	}
	else
	{
		qImg = QImage((const unsigned char*)(cvImg.data),
			cvImg.cols, cvImg.rows,
			cvImg.cols*cvImg.channels(),
			QImage::Format_RGB888);
	}

	return qImg;
}

void GuiDemo::outputLog(QString cnt)
{
	
}

void GuiDemo::nextFrame()
{
	std::cout << "nextFrame" << std::endl;

	videoCapture >> mCVFrame;

	if (!mCVFrame.empty()) {
		mImage = mat2Image(mCVFrame);
		ui.imageLabel->setPixmap(QPixmap::fromImage(mImage));
		update();
	}
}
