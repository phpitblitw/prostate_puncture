#pragma once
#include <qgraphicsview.h>
#include <qgraphicsitem.h>
#include <Qt3DInput/qkeyevent.h>
#include "opencv2/opencv.hpp"

class GraphicsView2D :
	public QGraphicsView
{
	Q_OBJECT
public:
	GraphicsView2D(QWidget *parent = Q_NULLPTR);
	~GraphicsView2D();

	void keyPressEvent(QKeyEvent * event);

	void LoadImg(cv::Mat img);
	void LoadProstateMask(cv::Mat prostateMask);  //��ȡǰ����mask������
	void LoadLesionMask(cv::Mat lesionMask);  //��ȡ����mask������
	void LoadRectumMask(cv::Mat rectumMask);  //��ȡֱ��mask������
	void ShowImg();  //����ͼԪ ��ʾͼ��
	cv::Mat Mask2Edge(cv::Mat src);

signals:
	void KeyLeftPressed();
	void KeyRightPressed();
	void KeyUpPressed();
	void KeyDownPressed();

private:
	void createGamaLut(uchar lutGama[256], float fC, float fGama);  //����gama�任���ұ�
	cv::Mat transformImg(cv::Mat& srcImg);  //ʹ�ò��ұ����б任

private:
	cv::Mat m_img;  //ͼ���������Ǵ洢�ڴ�
	cv::Mat m_prostateMask;  //ǰ����mask ǰ������Ϊ1(��ͬ)
	cv::Mat m_lesionMask;
	cv::Mat m_rectumMask;
	uchar m_lut[256];  //����ͼ����ұ�
	QGraphicsScene* m_pScene;
	QPixmap m_pixmap;
	QGraphicsPixmapItem * m_pPixmapItem;
};
