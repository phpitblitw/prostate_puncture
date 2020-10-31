#pragma once
#include <qgraphicsview.h>
#include <qgraphicsitem.h>
#include "opencv2/opencv.hpp"

class GraphicsView2D :
	public QGraphicsView
{
	Q_OBJECT
public:
	GraphicsView2D(QWidget *parent = Q_NULLPTR);
	~GraphicsView2D();

	void LoadImg(cv::Mat img);
	void LoadProstateMask(cv::Mat prostateMask);  //读取前列腺mask并叠加
	void LoadLesionMask(cv::Mat lesionMask);  //读取肿瘤mask并叠加
	void LoadRectumMask(cv::Mat rectumMask);  //读取直肠mask并叠加
	void ShowImg();  //更新图元 显示图像
	cv::Mat Mask2Edge(cv::Mat src);

private:
	cv::Mat m_img;  //图像数据总是存储在此
	cv::Mat m_prostateMask;  //前列腺mask 前景区域为1(下同)
	cv::Mat m_lesionMask;
	cv::Mat m_rectumMask;
	QGraphicsScene* m_pScene;
	QPixmap m_pixmap;
	QGraphicsPixmapItem * m_pPixmapItem;
};

