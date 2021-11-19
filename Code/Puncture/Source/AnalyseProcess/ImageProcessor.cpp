//*****************************************************************
// 文件名 :						ImageProcessor.h
// 版本	 :						1.0
// 目的及主要功能 :				处理2D图像工具类
// 创建日期 :					2021.11.12
// 修改日期 :					2021.11.12
// 作者 :						SunYizhe
// 修改者 :						
// 联系方式 :					fiki@seu.edu.cn
//*****************************************************************/
#include "pch.h"
#include <vector>
#include "opencv2/imgproc/types_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "ImageProcessor.h"


using namespace ANALYSEPROCESS;
using namespace cv;
using namespace std;

const int SPACE_X = 50;  //图像左右留出的边界区域(单位 pixel) 不作为ROI
const int SPACE_Y = 50;  //图像左右留出的边界区域(单位 pixel) 不作为ROI

ImageProcessor::ImageProcessor()
{
}


ImageProcessor::~ImageProcessor()
{
}

cv::Mat ImageProcessor::SegmentByThreshold(cv::Mat & srcImg)
{
	int maxIdx = 0, max = 0;
	cv::Mat gryImg;
	cv::Mat binaryImg;
	cv::Mat dstImg;
	vector<cv::Point> targetContour;  //目标 即最大的轮廓
	vector<vector<cv::Point>> contours;  //所有轮廓

	//转灰度
	cv::cvtColor(srcImg, gryImg, CV_BGR2GRAY);
	//取ROI
	int width = gryImg.cols;
	int height = gryImg.rows;
	cv::Rect roi(SPACE_X, SPACE_Y, width - SPACE_X * 2, height - SPACE_Y * 2);
	cv::Mat roiImg = gryImg(roi).clone();
	gryImg.setTo(0);
	roiImg.copyTo(gryImg(roi));
	//取阈值
	cv::threshold(gryImg, binaryImg, -1, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);  //OTSU方法自动取阈值，前景置为255
	//闭运算
	Mat element;  //闭运算卷积核
	element = cv::getStructuringElement(MORPH_RECT, cv::Size(3, 3));
	cv::morphologyEx(binaryImg, binaryImg, MORPH_CLOSE, element);
	//取轮廓
	cv::findContours(binaryImg, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);  //检索所有轮廓 不建立层次关系；存储所有轮廓点
	//找最大轮廓
	for (int i = 0; i < contours.size(); i++) {
		if (contours[i].size() > max) {
			maxIdx = i;
			max = contours[i].size();
		}
	}
	//仅保留最大轮廓
	targetContour = contours[maxIdx];
	dstImg = gryImg.clone();
	dstImg.setTo(0);
	cv::drawContours(dstImg, vector<vector<cv::Point>>{targetContour}, -1, Scalar(255), CV_FILLED);

	return dstImg;
}
