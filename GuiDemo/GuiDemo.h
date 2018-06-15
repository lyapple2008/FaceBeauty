#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_GuiDemo.h"
#include <opencv2/opencv.hpp>

class GuiDemo : public QMainWindow
{
	Q_OBJECT

public:
	GuiDemo(QWidget *parent = Q_NULLPTR);

private:
	bool initVideoCapture();
	void faceBeautyProcess();
	QImage mat2Image(cv::Mat& cvImg);

	void outputLog(QString cnt);
private slots:
	void nextFrame();
private:
	Ui::GuiDemoClass ui;

	QImage mImage;
	cv::Mat mCVFrame;
	cv::Mat mBeautyFrame;
	cv::VideoCapture videoCapture;

	QTimer *mTimer;
	double rate;
};
