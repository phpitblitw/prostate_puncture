#include "GraphicsAnnotator.h"
#include <vector>
#pragma execution_character_set("utf-8")  //避免中文乱码 参考https://blog.csdn.net/liyuanbhu/article/details/72596952
//#include <Windows.h>


GraphicsAnnotator::GraphicsAnnotator(QWidget *parent)
{
	this->setParent(parent);
	m_pScene = new QGraphicsScene(this);
	m_pPixmapItem = new QGraphicsPixmapItem(m_pixmap);  //图片图元
	m_pScene->addItem(m_pPixmapItem);
	this->setScene(m_pScene);

	createGamaLut(m_lut, 1.0, 0.5);  //初始化gama校正参数
}

GraphicsAnnotator::~GraphicsAnnotator()
{
	delete m_pPixmapItem;
	delete m_pScene;
}

void GraphicsAnnotator::keyPressEvent(QKeyEvent * event)
{
	switch (event->key())
	{
	//case Qt::Key_Left:
	//	emit(KeyLeftPressed());
	//	break;
	//case Qt::Key_Right:
	//	emit(KeyRightPressed());
	//	break;
	//case Qt::Key_Up:
	//	emit(KeyUpPressed());
	//	break;
	//case Qt::Key_Down:
	//	emit(KeyDownPressed());
	//	break;
	case Qt::Key_Delete:
		this->DeleteLastPoint();
		break;
	case Qt::Key_Backspace:
		this->DeleteLastPoint();
		break;
	default:
		break;
	}
}

//载入8UC3 BGR的cv::Mat格式的数据
void GraphicsAnnotator::LoadImg(cv::Mat img)
{
	img.copyTo(m_img);

	//图片处理
	cv::Mat m_imgGray;  //灰度图 中间变量
	cv::cvtColor(m_img, m_imgGray, cv::COLOR_BGR2GRAY);
	cv::threshold(m_imgGray, m_imgGray, 1, 255, cv::THRESH_BINARY_INV);  //筛选出灰度<=1的部分,设置为255
	m_img.setTo(0, m_imgGray);  //将灰度<=1的部分 置为0
	this->transformImg(m_img);  //使用查找表变换图片 使图像更加易读
}

//将8UC3 BGR的cv::Mat格式的数据m_img,转为QPixmap存储并显示
void GraphicsAnnotator::ShowImg()
{
	cv::Mat convertedImg;
	cv::cvtColor(m_img, convertedImg, cv::COLOR_BGR2RGB);
	QImage tempImg((const unsigned char*)(convertedImg.data), convertedImg.cols, convertedImg.rows, convertedImg.step, QImage::Format_RGB888);
	m_pixmap = QPixmap::fromImage(tempImg);
	m_pPixmapItem->setPixmap(m_pixmap);
	this->setSceneRect(0, 0, m_pixmap.width(), m_pixmap.height());
	this->fitInView(m_pPixmapItem, Qt::KeepAspectRatio);
	this->centerOn(m_pPixmapItem);
	//重置标注数据
	m_pointPos.clear();  //标注坐标清空
	for (int i = 0; i < m_pointItem.size(); i++)  //清空标注图元
	{
		m_pScene->removeItem(m_pointItem[i]);
		delete m_pointItem[i];
	}
	m_pointItem.clear();
}

cv::Mat GraphicsAnnotator::Mask2Edge(cv::Mat src)
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

//gama变换 s=c*r^gama  其中 r为原始灰度值 c为变换后灰度值
void GraphicsAnnotator::createGamaLut(uchar lutGama[256], float fC, float fGama)
{
	memset(lutGama, 0, sizeof(uchar) * 2);  //灰度值过低的点 直接置为0
	for (int i = 2; i < 256; i++)
	{
		lutGama[i] = fC * uchar(pow(float(i) / 255, fGama) * 255) < 255 ? fC * uchar(pow(float(i) / 255, fGama) * 255) : 255;
	}
	return;
}

//使用查找表 对cv::Mat赋值
//Mat遍历方式 参考http://www.opencv.org.cn/opencvdoc/2.3.2/html/doc/tutorials/core/how_to_scan_images/how_to_scan_images.html
//传参方式 参考https://blog.csdn.net/u012814856/article/details/84099328
cv::Mat GraphicsAnnotator::transformImg(cv::Mat& srcImg)
{
	//确保输入的图片深度与uchar相同
	CV_Assert(srcImg.depth() != sizeof(uchar));

	int channels = srcImg.channels();
	int nRows = srcImg.rows;
	int nCols = srcImg.cols*channels;
	int x, y;
	uchar* pCur;

	if (srcImg.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	for (y = 0; y < nRows; y++)
	{
		pCur = srcImg.ptr<uchar>(y);  //第y行首元素
		for (x = 0; x < nCols; x++)
		{
			pCur[x] = m_lut[pCur[x]];
		}
	}
	return srcImg;
}

void GraphicsAnnotator::DeleteLastPoint()
{
	int index = m_pointPos.size() - 1;
	if (index < 0)
		return;
	//删除最后一个点坐标
	m_pointPos.pop_back();
	//删除最后一个点的示意图元
	m_pScene->removeItem(m_pointItem[index]);
	delete m_pointItem[index];
	m_pointItem.pop_back();
}

void GraphicsAnnotator::mouseReleaseEvent(QMouseEvent *event)
{
	//emit mouseMoveSignal(event->x(), event->y());
	QPoint curPos = event->pos();  //视图坐标系下的坐标
	QPointF scenePos = this->mapToScene(curPos);  //场景坐标系下的坐标
	QPointF itemPos = m_pPixmapItem->mapFromParent(scenePos);  //图元坐标系下的
	emit mouseMoveSignal(itemPos.x(), itemPos.y());

	m_pointPos.push_back(itemPos);  //记录当前点坐标
	//加入一个示意图元
	QGraphicsEllipseItem *item = new QGraphicsEllipseItem(QRectF(0, 0, 12, 12));
	item->setBrush(QColor(255, 255, 0));
	m_pScene->addItem(item);
	item->setPos(scenePos.x()-6,scenePos.y()-6);  //设置椭圆位置（图元坐标系下）
	m_pointItem.push_back(item);

	QGraphicsView::mouseReleaseEvent(event);
}