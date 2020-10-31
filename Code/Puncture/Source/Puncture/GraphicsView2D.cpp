#include "GraphicsView2D.h"


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

//载入8UC3 BGR的cv::Mat格式的数据,转为QPixmap存储
void GraphicsView2D::LoadImg(cv::Mat img)
{
	m_img = img;
}

void GraphicsView2D::LoadProstateMask(cv::Mat prostateMask)
{
	if (!prostateMask.data)
		return;
	m_prostateMask = Mask2Edge(prostateMask);
	cv::Mat prostatePalette(prostateMask.rows, prostateMask.cols, CV_8UC3, cv::Scalar(0, 127, 0));
	prostatePalette.copyTo(m_img, m_prostateMask);
}

void GraphicsView2D::LoadLesionMask(cv::Mat lesionMask)
{
	if (!lesionMask.data)
		return;
	m_lesionMask = Mask2Edge(lesionMask);
	cv::imwrite("D:\\other\\leisonMask.bmp", m_lesionMask); //TODO
	cv::imwrite("D:\\other\\before.bmp",m_img);	//TODO
	cv::Mat lesionPalette(lesionMask.rows, lesionMask.cols, CV_8UC3, cv::Scalar(0, 0, 127));
	lesionPalette.copyTo(m_img, m_lesionMask);
	cv::imwrite("D:\\other\\after.bmp",m_img);  //TODO
}

void GraphicsView2D::LoadRectumMask(cv::Mat rectumMask)
{
	if (!rectumMask.data)
		return;
	m_rectumMask = Mask2Edge(rectumMask);
	cv::Mat rectumPalette(rectumMask.rows, rectumMask.cols, CV_8UC3, cv::Scalar(127, 0, 0));
	rectumPalette.copyTo(m_img, m_rectumMask);
}

//将8UC3 BGR的cv::Mat格式的数据m_img,转为QPixmap存储并显示
void GraphicsView2D::ShowImg()
{
	cv::cvtColor(m_img, m_img, cv::COLOR_BGR2RGB);
	QImage tempImg((const unsigned char*)(m_img.data), m_img.cols, m_img.rows, m_img.step, QImage::Format_RGB888);
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
	return res;
}
