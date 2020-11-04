#include "GraphicsView2D.h"
#include <vector>


GraphicsView2D::GraphicsView2D(QWidget *parent)
{
	this->setParent(parent);
	m_pScene = new QGraphicsScene(this);
	m_pPixmapItem = new QGraphicsPixmapItem(m_pixmap);
	m_pScene->addItem(m_pPixmapItem);
	this->setScene(m_pScene);

}

GraphicsView2D::~GraphicsView2D()
{
	delete m_pPixmapItem;
	delete m_pScene;
}

void GraphicsView2D::keyPressEvent(QKeyEvent * event)
{
	switch (event->key())
	{
	case Qt::Key_Left:
		emit(KeyLeftPressed());
		break;
	case Qt::Key_Right:
		emit(KeyRightPressed());
		break;
	case Qt::Key_Up:
		emit(KeyUpPressed());
		break;
	case Qt::Key_Down:
		emit(KeyDownPressed());
		break;
	default:
		break;
	}
}

//载入8UC3 BGR的cv::Mat格式的数据,转为QPixmap存储
void GraphicsView2D::LoadImg(cv::Mat img)
{	
	m_img = img;
}

void GraphicsView2D::LoadProstateMask(cv::Mat prostateMask)
{
	if (prostateMask.empty())
		return;
	m_prostateMask = Mask2Edge(prostateMask.clone());
	cv::Mat prostatePalette(m_prostateMask.rows, m_prostateMask.cols, CV_8UC3, cv::Scalar(0, 255, 0));
	prostatePalette.copyTo(m_img, m_prostateMask);
}

void GraphicsView2D::LoadLesionMask(cv::Mat lesionMask)
{
	if (lesionMask.empty())
		return;
	m_lesionMask = Mask2Edge(lesionMask.clone());
	cv::Mat lesionPalette(m_lesionMask.rows, m_lesionMask.cols, CV_8UC3, cv::Scalar(0, 0, 255));
	lesionPalette.copyTo(m_img, m_lesionMask);
}

void GraphicsView2D::LoadRectumMask(cv::Mat rectumMask)
{
	if (rectumMask.empty())
		return;
	m_rectumMask = Mask2Edge(rectumMask.clone());
	cv::Mat rectumPalette(m_rectumMask.rows, m_rectumMask.cols, CV_8UC3, cv::Scalar(255, 0, 0));
	rectumPalette.copyTo(m_img, m_rectumMask);
}

//将8UC3 BGR的cv::Mat格式的数据m_img,转为QPixmap存储并显示
void GraphicsView2D::ShowImg()
{
	cv::Mat convertedImg;
	cv::cvtColor(m_img, convertedImg, cv::COLOR_BGR2RGB);
	QImage tempImg((const unsigned char*)(convertedImg.data), convertedImg.cols, convertedImg.rows, convertedImg.step, QImage::Format_RGB888);
	m_pixmap = QPixmap::fromImage(tempImg);
	m_pPixmapItem->setPixmap(m_pixmap);
	this->setSceneRect(0, 0, m_pixmap.width(), m_pixmap.height());
	this->fitInView(m_pPixmapItem, Qt::KeepAspectRatio);
	this->centerOn(m_pPixmapItem);
}

cv::Mat GraphicsView2D::Mask2Edge(cv::Mat src)
{
	int ddepth = CV_16S;
	int scale = 1, delta = 0;
	cv::Mat res, grad_x, grad_y, grad;
	//TODO
	//cv::imwrite("D:\\other\\beforeSobel.bmp", src);
	//int temp1 = src.type();
	//int temp2 = grad_x.type();
	//int temp3 = grad_y.type();
	//int temp4 = grad.type();
	//int temp5 = res.type();
	//TODO
	//求水平 竖直方向梯度
	Sobel(src, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT);
	Sobel(src, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT);
	//梯度转为绝对值
	convertScaleAbs(grad_x, grad_x);
	convertScaleAbs(grad_y, grad_y);
	//合并梯度
	addWeighted(grad_x, 1, grad_y, 1, 0, grad);
	//边缘强度置为1
	threshold(grad, res, 1, 1, 0);

	//cv::imwrite("D:\\other\\afterSobel.bmp", src);  //TODO
	return res;
}
