#pragma once
//用于标注的图片显示widget的扩展
#include <qgraphicsview.h>
#include <qgraphicsitem.h>
#include <Qt3DInput/qkeyevent.h>
#include <vector>
#include "opencv2/opencv.hpp"
#define POINT_NUM 15  //需要标注的顶点的数量
using namespace std;

class GraphicsAnnotator :
	public QGraphicsView
{
	Q_OBJECT
public:
	GraphicsAnnotator(QWidget *parent = Q_NULLPTR);
	~GraphicsAnnotator();

	void keyPressEvent(QKeyEvent * event);

	void LoadImg(cv::Mat img);
	void ShowImg();  //更新图元 显示图像
	vector<QPointF> GetPointPos() { return m_pointPos; }
	cv::Mat Mask2Edge(cv::Mat src);

signals:
	void markFinishSignal();  //标记单帧图片已经标注完毕
//	void KeyLeftPressed();
//	void KeyRightPressed();
//	void KeyUpPressed();
//	void KeyDownPressed();

protected:
	void mouseReleaseEvent(QMouseEvent *event) override;  //鼠标点击事件


private:
	void createGamaLut(uchar lutGama[256], float fC, float fGama);  //构造gama变换查找表
	cv::Mat transformImg(cv::Mat& srcImg);  //使用查找表进行变换
	void DeleteLastPoint();  //删除最后一个选定的点

private:
	cv::Mat m_img;  //图像数据总是存储在此
	uchar m_lut[256];  //超声图像gama变换查找表
	QGraphicsScene* m_pScene;
	QPixmap m_pixmap;
	QGraphicsPixmapItem * m_pPixmapItem;
	vector<QPointF> m_pointPos;  //POINT_NUM个点的x,y坐标(图元坐标系)
	vector<QGraphicsEllipseItem*> m_pointItem;  //POINT_NUM个点，用图元进行标注
signals:
	void mouseMoveSignal(float x,float y);
};

