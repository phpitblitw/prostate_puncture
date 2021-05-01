#include "AnnotateWindow.h"
#include <stdio.h>
#include <io.h>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <qtextstream.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <opencv2/opencv.hpp>
//#pragma execution_character_set("utf-8")  //避免中文乱码 参考https://blog.csdn.net/liyuanbhu/article/details/72596952

//比较两个形如"baseUS_5.bmp"的字符串，谁的序号更靠前
bool cmpFileName(string s1, string s2)
{
	int l1, r1, l2, r2;
	l1 = s1.rfind("_") + 1;
	r1 = s1.rfind(".");
	l2 = s2.rfind("_") + 1;
	r2 = s2.rfind(".");
	s1 = s1.substr(l1, r1 - l1);
	s2 = s2.substr(l2, r2 - l2);
	return atoi(s1.data()) < atoi(s2.data());
}

AnnotateWindow::AnnotateWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_nImageIndex = -1;
	m_vctTriangleVertex = vector<QVector3D>(6, QVector3D(0, 0, 0));
	ui.BtnSave3D->setEnabled(false);
	ui.RadioBtnTransverse->setChecked(false);
	ui.RadioBtnSagittal->setChecked(false);
	ui.PixelSizeX->setText(u8"0");
	ui.PixelSizeY->setText(u8"0");
	m_nImageWidth = 0;
	m_nImageHeight = 0;

	//连接信号与槽
	connect(ui.BtnSetImageDir, SIGNAL(clicked()), this, SLOT(OnBtnBtnSetImageDirClicked()));
	connect(ui.BtnLoadPhantomTriangles, SIGNAL(clicked()), this, SLOT(OnBtnLoadPhantomTriangles()));
	connect(ui.BtnFormerImage, SIGNAL(clicked()), this, SLOT(OnBtnFormerImageClicked()));
	connect(ui.BtnNextImage, SIGNAL(clicked()), this, SLOT(OnBtnNextImageClicked()));
	connect(ui.BtnFinish, SIGNAL(clicked()), this, SLOT(OnBtnFinishClicked()));
	connect(ui.Annotate2D, SIGNAL(markFinishSignal()), this, SLOT(OnBtnFinishClicked()));
	connect(ui.BtnSave2D, SIGNAL(clicked()), this, SLOT(OnBtnSave2DClicked()));
	connect(ui.BtnSave3DInImage, SIGNAL(clicked()), this, SLOT(OnBtnSave3DInImageClicked()));
	connect(ui.BtnSave3D, SIGNAL(clicked()), this, SLOT(OnBtnSave3DInPhantomClicked()));
	connect(ui.Annotate2D, SIGNAL(mouseMoveSignal(float, float)), this, SLOT(UpdateMousePos(float, float)));
}

AnnotateWindow::~AnnotateWindow()
{
}

void AnnotateWindow::UpdateMousePos(float x, float y)
{
	ui.LabelPosX->setText(QString::number(x, 'f', 3));
	ui.LabelPosY->setText(QString::number(y, 'f', 3));
}

//获取所有图片名 参考https://blog.csdn.net/how0723/article/details/78105308
void AnnotateWindow::OnBtnBtnSetImageDirClicked()
{
	m_vctImagePath.clear();
	m_nImageIndex = 0;
	//从用户处 获取文件路径
	QString qstrRoot = QFileDialog::getExistingDirectory(this, "图片目录", "D:/", QFileDialog::ShowDirsOnly);
	m_strRootDir = qstrRoot.toLocal8Bit();
	if (m_strRootDir.empty())
		return;
	//构造文件名模板
	string inPath = m_strRootDir + "/*.bmp";
	//用于查找的句柄
	intptr_t handle;
	struct _finddata_t fileinfo;
	handle = _findfirst(inPath.data(), &fileinfo);
	//第一个文件
	if (handle == -1)
		return;
	else
		m_vctImagePath.push_back(fileinfo.name);
	//后续文件
	while (!_findnext(handle, &fileinfo))
	{
		m_vctImagePath.push_back(fileinfo.name);
	}
	_findclose(handle);
	sort(m_vctImagePath.begin(), m_vctImagePath.end(), cmpFileName);  //按序号大小排序
	//初始化各个图片数据的容器
	m_vctPtPos2D = vector<vector<QPointF>>(m_vctImagePath.size(), vector<QPointF>(POINT_NUM, QPointF(0, 0)));  //初始化为每一个点都是(0,0)
	m_vctPtPos3DInPhantom = vector <vector<QVector3D>>(m_vctImagePath.size(), vector<QVector3D>(POINT_NUM / 3, QVector3D(0, 0, 0)));  //初始化每个体模坐标系下的点都是(0,0,0)
	m_vctPtPos3DInImage = vector <vector<QVector3D>>(m_vctImagePath.size(), vector<QVector3D>(POINT_NUM / 3, QVector3D(0, 0, 0)));  //初始化每个3D图像坐标系下的点都是(0,0,0)
	//载入第0张图片
	m_nImageIndex = 0;
	ui.Annotate2D->LoadImg(cv::imread(m_strRootDir + '/' + m_vctImagePath[m_nImageIndex]));
	ui.LabelFilename->setText(QString::fromLocal8Bit(m_vctImagePath[m_nImageIndex].c_str()));
	ui.Annotate2D->ShowImg();
	//读取图片长宽
	cv::Mat img = cv::imread(m_strRootDir + '/' + m_vctImagePath[m_nImageIndex]);
	m_nImageWidth = img.cols;
	m_nImageHeight = img.rows;
}

void AnnotateWindow::Cal3DPosInImage(bool bTransverse, float fPixelSizeX, float fPixelSizeY)
{
	int i, j;
	QPointF ptPos2D;
	float x2D, y2D;  //2D图片的坐标
	float fX, fY, fZ;  //3D图片坐标系下的坐标
	QVector3D ptPos3D;
	vector<QVector3D> ptsPos3D;
	//m_vctPtPos3DInImage.clear();
	m_vctPtPos3DInImage = vector <vector<QVector3D>>(m_vctImagePath.size(), vector<QVector3D>(POINT_NUM / 3, QVector3D(0, 0, 0)));  //初始化每个3D图像坐标系下的点都是(0,0,0)
	for (i = 0; i < m_vctPtPos2D.size(); i++)  //处理每一张图片
	{
		ptsPos3D.clear();
		if (m_vctPtPos2D[i][0] == QPointF(0, 0))  //说明这张图片未曾被标注
			continue;
		for (j = 0; j < POINT_NUM/3; j++)  //各图片仅处理中间一列的点
		{
			ptPos2D = m_vctPtPos2D[i][j * 3 + 1];  //获取这一行中间的点的2D图像坐标系坐标(像素坐标)
			//将原点 由图片左上角 移动至图片底边中点
			ptPos2D.setX(ptPos2D.x() - m_nImageWidth / 2.0);
			ptPos2D.setY(m_nImageHeight - ptPos2D.y());
			//对于2D图像坐标系坐标，将单位由像素(pixel)变为毫米(mm)
			ptPos2D.setX(ptPos2D.x()*fPixelSizeX);
			ptPos2D.setY(ptPos2D.y()*fPixelSizeY);
			//将2D图像坐标系坐标，转为3D图像坐标系坐标
			if (bTransverse)  //横断面，则z为0
				ptPos3D = QVector3D(ptPos2D.x(), ptPos2D.y(), 0);
			else  //矢状面 则x为0
				ptPos3D = QVector3D(0, ptPos2D.y(), ptPos2D.x());
			ptsPos3D.push_back(ptPos3D);
		}
		m_vctPtPos3DInImage[i]=ptsPos3D;
	}
}

void AnnotateWindow::Cal3DPosInPhantom()
{
	int indexImg, markedNum, row;
	float ratio;  //线段长度的比值，[left,mid]/[left,right]，用于计算相似三角形的大小比例
	QPointF ptLeft, ptMid, ptRight;  //左侧顶点，中间点，右侧顶点(2D图片坐标)
	QVector3D vertexLeft, mid, vertexRight;  //左侧顶点，中间点，右侧顶点(3D体模坐标系坐标)
	//m_vctPtPos3D.clear();
	m_vctPtPos3DInPhantom = vector <vector<QVector3D>>(m_vctImagePath.size(), vector<QVector3D>(POINT_NUM / 3, QVector3D(0, 0, 0)));  //初始化每个体模坐标系下的点都是(0,0,0)
	/********计算每张图片每行的中间点，在体模坐标系下的坐标************/
	for (indexImg = 0, markedNum = 0; indexImg < m_vctPtPos2D.size(); indexImg++)  //每张图片的数据存为一行
	{
		vector<QVector3D> ptsPos3D;  //当前这张图片中，各行中点在体模坐标系下的坐标
		if (m_vctPtPos2D[indexImg][0] == QPointF(0, 0))  //说明这张图片未曾被标注
			continue;
		for (row = 0; row < POINT_NUM / 3; row++)  //分别处理每一行(图片中每行标注了3个点)
		{
			ptLeft = m_vctPtPos2D[indexImg][row * 3];
			ptMid = m_vctPtPos2D[indexImg][row * 3 +1];
			ptRight = m_vctPtPos2D[indexImg][row * 3 + 2];
			vertexLeft = m_vctTriangleVertex[row * 2];
			vertexRight = m_vctTriangleVertex[row * 2 + 1];
			ratio = sqrt(QPointF::dotProduct((ptMid - ptLeft), (ptMid - ptLeft))) / sqrt(QPointF::dotProduct((ptRight - ptLeft), (ptRight - ptLeft)));  //计算线段长度比值,即相似三角形长度比
			mid = vertexLeft + ratio * (vertexRight - vertexLeft);  //中间点在体模坐标系下的坐标
			ptsPos3D.push_back(mid);
		}
		m_vctPtPos3DInPhantom[indexImg]=ptsPos3D;
	}
}

void AnnotateWindow::OnBtnFormerImageClicked()
{
	if (m_nImageIndex <= 0)
		return;
	m_nImageIndex--;
	ui.LabelFilename->setText(QString::fromLocal8Bit(m_vctImagePath[m_nImageIndex].c_str()));
	ui.Annotate2D->LoadImg(cv::imread(m_strRootDir + '/' + m_vctImagePath[m_nImageIndex]));
	ui.Annotate2D->ShowImg();
}

void AnnotateWindow::OnBtnNextImageClicked()
{
	if (m_nImageIndex >= m_vctImagePath.size() - 1)
		return;
	m_nImageIndex++;
	ui.LabelFilename->setText(QString::fromLocal8Bit(m_vctImagePath[m_nImageIndex].c_str()));
	ui.Annotate2D->LoadImg(cv::imread(m_strRootDir + '/' + m_vctImagePath[m_nImageIndex]));
	ui.Annotate2D->ShowImg();
}

void AnnotateWindow::OnBtnFinishClicked()
{
	vector<QPointF> pointPos = ui.Annotate2D->GetPointPos();
	if (pointPos.size() != POINT_NUM)
	{
		QMessageBox::information(this, u8"注意", u8"当前已标注的点的数量不正确。 请重新标注当前帧");
		ui.Annotate2D->ShowImg();  //重置当前帧
	}
	else
	{
		m_vctPtPos2D[m_nImageIndex]=pointPos;
		if (m_nImageIndex >= m_vctImagePath.size() - 1)
		{
			QMessageBox::information(this, u8"注意", u8"已到达最后一帧");
			return;
		}
		m_nImageIndex++;
		ui.LabelFilename->setText(QString::fromLocal8Bit(m_vctImagePath[m_nImageIndex].c_str()));
		ui.Annotate2D->LoadImg(cv::imread(m_strRootDir + '/' + m_vctImagePath[m_nImageIndex]));
		ui.Annotate2D->ShowImg();
	}
}

void AnnotateWindow::OnBtnSave2DClicked()
{
	int indexImg, indexPt, markedNum;
	std::string info;
	QString qstrTxtFilename = QFileDialog::getSaveFileName(this, tr(u8"存储点集在图像坐标系下的2D坐标(图片数*每张图9个点*xy两个分量)"), "D:/", "*.txt");
	string strTxtFileName = (const char*)qstrTxtFilename.toLocal8Bit();  //转换为std::string 且避免中文字符出现乱码
	if (strTxtFileName.empty())
		return;
	ofstream outfile(strTxtFileName);
	for (indexImg = 0,markedNum=0; indexImg < m_vctPtPos2D.size(); indexImg++)  //每张图片的数据存为一行
	{
		if (m_vctPtPos2D[indexImg][0] == QPointF(0, 0))  //检测该点是否被赋值过
			continue;
		markedNum++;
		outfile << std::to_string(indexImg) << '\t';  //存储点的下标
		//存储本图片所有点的x坐标
		for (indexPt = 0; indexPt < POINT_NUM; indexPt++)
			outfile << m_vctPtPos2D[indexImg][indexPt].x() << '\t';
		//存储本图片所有点的y坐标
		for (indexPt = 0; indexPt < POINT_NUM; indexPt++)
			outfile << m_vctPtPos2D[indexImg][indexPt].y() << '\t';
		outfile << '\n';
	}
	outfile.close();
	stringstream sstream;
	ostringstream ostream;
	sstream << "总共" << m_vctPtPos2D.size() << "张图片，存储了其中已标注的" << markedNum << "张。";
	info = sstream.str();
	QMessageBox::information(this, u8"完成", QString::fromLocal8Bit(info.c_str()));
}

void AnnotateWindow::OnBtnSave3DInImageClicked()
{
	//正确性检查
	bool bTransverse;
	float fPixelSizeX,fPixelSizeY;
	QString qStr;
	if ((!ui.RadioBtnTransverse->isChecked()) && !(ui.RadioBtnSagittal->isChecked()))
	{
		QMessageBox::information(this, u8"错误", u8"请选择扫描平面类型");
		return;
	}
	else
		bTransverse = ui.RadioBtnTransverse->isChecked();
	qStr = ui.PixelSizeX->text();
	fPixelSizeX = qStr.toFloat();
	qStr = ui.PixelSizeY->text();
	fPixelSizeY = qStr.toFloat();
	if (fPixelSizeX <= 0 || fPixelSizeY<=0)
	{
		QMessageBox::information(this, u8"错误", u8"像素物理尺寸应大于0");
		return;
	}
	if (m_nImageWidth <= 0 || m_nImageHeight <= 0)
	{
		QMessageBox::information(this, u8"错误", u8"读取图片尺寸错误");
		return;
	}
	//计算每张图片每行的中间点，在3D图片坐标系下的坐标
	this->Cal3DPosInImage(bTransverse, fPixelSizeX,fPixelSizeY);
	//将结果存储至txt文件
	/********将结果存储至txt文件************/
	std::string info;  //提示信息
	QString qstrTxtFilename = QFileDialog::getSaveFileName(this, tr(u8"存储点集在3D图像坐标系下的物理坐标(图片数*每张图3个点*xyz3个分量)"), "D:/", "*.txt");
	string strTxtFileName = (const char*)qstrTxtFilename.toLocal8Bit();  //转换为std::string 且避免中文字符出现乱码
	if (strTxtFileName.empty())
		return;
	ofstream outfile(strTxtFileName);
	int indexImg = 0, markedNum;
	for (indexImg = 0, markedNum = 0; indexImg < m_vctPtPos3DInImage.size(); indexImg++)  //每张图片的数据存为一行
	{
		if (m_vctPtPos3DInImage[indexImg][0]==QVector3D(0,0,0))  //检测该点是否被赋值过
			continue;
		markedNum++;
		outfile << std::to_string(indexImg) << '\t';  //存储点的下标
		vector<QVector3D> ptPos3D;  //当前这张图片中，各行中点在体模坐标系下的坐标
		for (int row = 0; row < POINT_NUM / 3; row++)  //仅存储每一列中间的点
		{
			outfile << m_vctPtPos3DInImage[indexImg][row].x() << '\t'
				<< m_vctPtPos3DInImage[indexImg][row].y() << '\t'
				<< m_vctPtPos3DInImage[indexImg][row].z() << '\t';
		}
		outfile << '\n';
	}
	outfile.close();
	/********输出提示信息************/
	stringstream sstream;
	sstream << "已存储了" << markedNum << "张图片";
	info = sstream.str();
	QMessageBox::information(this, u8"完成", QString::fromLocal8Bit(info.c_str()));
}

void AnnotateWindow::OnBtnSave3DInPhantomClicked()
{
	//TODO
	int indexImg, markedNum, row, col;
	float ratio;  //线段长度的比值，[left,mid]/[left,right]，用于计算相似三角形的大小比例
	QPointF ptLeft, ptMid, ptRight;  //左侧顶点，中间点，右侧顶点(2D图片坐标)
	QVector3D vertexLeft, mid, vertexRight;  //左侧顶点，中间点，右侧顶点(3D体模坐标系坐标)
	/********计算每张图片每行的中间点，在体模坐标系下的坐标************/
	this->Cal3DPosInPhantom();
	/********将结果存储至txt文件************/
	std::string info;  //提示信息
	QString qstrTxtFilename = QFileDialog::getSaveFileName(this, tr(u8"存储点集在体模坐标系下的3D坐标(图片数*每张图3个点*xyz3个分量)"), "D:/", "*.txt");
	string strTxtFileName = (const char*)qstrTxtFilename.toLocal8Bit();  //转换为std::string 且避免中文字符出现乱码
	if (strTxtFileName.empty())
		return;
	ofstream outfile(strTxtFileName);
	for (indexImg = 0, markedNum = 0; indexImg < m_vctPtPos3DInPhantom.size(); indexImg++)  //每张图片的数据存为一行
	{
		//if (m_vctPtPos2D[indexImg][0] == QPointF(0, 0))  //检测该点是否被赋值过
		if(m_vctPtPos3DInPhantom[indexImg][0]==QVector3D(0,0,0))  //检测该点是否被赋值过
			continue;
		markedNum++;
		outfile << std::to_string(indexImg) << '\t';  //存储点的下标
		vector<QVector3D> ptPos3D;  //当前这张图片中，各行中点在体模坐标系下的坐标
		for (row = 0; row < POINT_NUM / 3; row++)  //分别处理每一行(图片中每行标注了3个点)
		{
			outfile << m_vctPtPos3DInPhantom[indexImg][row].x() << '\t'
				<< m_vctPtPos3DInPhantom[indexImg][row].y() << '\t'
				<< m_vctPtPos3DInPhantom[indexImg][row].z() << '\t';
		}
		outfile << '\n';
	}
	outfile.close();
	/********输出提示信息************/
	stringstream sstream;
	sstream << "总共" << m_vctPtPos2D.size() << "张图片，存储了其中已标注的" << markedNum << "张。";
	info = sstream.str();
	QMessageBox::information(this, u8"完成", QString::fromLocal8Bit(info.c_str()));
}

void AnnotateWindow::OnBtnLoadPhantomTriangles()
{
	int vertexNum;
	float x, y, z;
	m_vctTriangleVertex.clear();
	vertexNum = POINT_NUM / 3 * 2;  //共POINT_NUM/3*2个，因为图片中每行有3个点，其中2个为相似三角形的顶点
	string info;  //提示信息
	
	QString qstrTxtFilename = QFileDialog::getOpenFileName(this, tr(u8"载入体模三角形角点坐标(体模坐标系下 3D坐标)"), "D:/", "*.txt");
	string strTxtFileName = (const char*)qstrTxtFilename.toLocal8Bit();  //转换为std::string 且避免中文字符出现乱码
	if (strTxtFileName.empty())
		return;
	ifstream infile(strTxtFileName);
	//读取若干个个角点
	while (infile >> x && vertexNum>0)
	{
		infile >> y;
		infile >> z;
		m_vctTriangleVertex.push_back(QVector3D(x, y, z));
		vertexNum--;
	}
	infile.close();
	//输出提示信息
	stringstream sstream;
	if (vertexNum > 0)
	{
		sstream << "应读入" << POINT_NUM / 3 * 2 << "个顶点\t缺失" << vertexNum << "个顶点";
		info = sstream.str();
		QMessageBox::information(this, "错误", QString::fromLocal8Bit(info.c_str()));
	}
	else
	{
		sstream << "成功载入体模的三角形顶点数据";
		info = sstream.str();
		ui.BtnSave3D->setEnabled(true);
		QMessageBox::information(this, u8"完成", QString::fromLocal8Bit(info.c_str()));
	}
}


