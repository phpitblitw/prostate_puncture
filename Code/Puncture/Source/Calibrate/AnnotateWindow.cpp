#include "AnnotateWindow.h"
#include <stdio.h>
#include <io.h>
#include <algorithm>
#include <fstream>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <string>
#include <opencv2/opencv.hpp>
#pragma execution_character_set("utf-8")  //避免中文乱码 参考https://blog.csdn.net/liyuanbhu/article/details/72596952

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

	//连接信号与槽
	connect(ui.BtnSetImageDir, SIGNAL(clicked()), this, SLOT(OnBtnBtnSetImageDirClicked()));
	connect(ui.BtnFormerImage, SIGNAL(clicked()), this, SLOT(OnBtnFormerImageClicked()));
	connect(ui.BtnNextImage, SIGNAL(clicked()), this, SLOT(OnBtnNextImageClicked()));
	connect(ui.BtnFinish, SIGNAL(clicked()), this, SLOT(OnBtnFinishClicked()));
	connect(ui.BtnSave, SIGNAL(clicked()), this, SLOT(OnBtnSaveClicked()));
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
	//初始化标注信息
	m_vctImageData = vector<vector<QPointF>>(m_vctImagePath.size(), vector<QPointF>(POINT_NUM, QPointF(0, 0)));  //初始化为每一个点都是[0,0]
	//载入第0张图片
	m_nImageIndex = 0;
	ui.Annotate2D->LoadImg(cv::imread(m_strRootDir + '/' + m_vctImagePath[m_nImageIndex]));
	ui.LabelFilename->setText(QString::fromLocal8Bit(m_vctImagePath[m_nImageIndex].c_str()));
	ui.Annotate2D->ShowImg();
}

void AnnotateWindow::OnBtnFormerImageClicked()
{
	if (m_nImageIndex <= 0)
		return;
	m_nImageIndex--;

	ui.Annotate2D->LoadImg(cv::imread(m_strRootDir + '/' + m_vctImagePath[m_nImageIndex]));
	ui.Annotate2D->ShowImg();
}

void AnnotateWindow::OnBtnNextImageClicked()
{
	if (m_nImageIndex >= m_vctImagePath.size() - 1)
		return;
	m_nImageIndex++;
	ui.Annotate2D->LoadImg(cv::imread(m_strRootDir + '/' + m_vctImagePath[m_nImageIndex]));
	ui.Annotate2D->ShowImg();
}

void AnnotateWindow::OnBtnFinishClicked()
{
	vector<QPointF> pointPos = ui.Annotate2D->GetPointPos();
	if (pointPos.size() != POINT_NUM)
	{
		QMessageBox::information(this, "注意", "当前已标注的点的数量不正确。 请重新标注当前帧");
		ui.Annotate2D->ShowImg();  //重置当前帧
	}
	else
	{
		m_vctImageData[m_nImageIndex]=pointPos;
		if (m_nImageIndex >= m_vctImagePath.size() - 1)
		{
			QMessageBox::information(this, "注意", "已到达最后一帧");
			return;
		}
		m_nImageIndex++;
		ui.Annotate2D->LoadImg(cv::imread(m_strRootDir + '/' + m_vctImagePath[m_nImageIndex]));
		ui.Annotate2D->ShowImg();
	}
}

void AnnotateWindow::OnBtnSaveClicked()
{
	int indexImg, indexPt, markedNum;
	std::string info;
	QString qstrTxtFilename = QFileDialog::getSaveFileName(this, tr("存储点集"), "D:/", "*.txt");
	string strTxtFileName = (const char*)qstrTxtFilename.toLocal8Bit();  //转换为std::string 且避免中文字符出现乱码
	if (strTxtFileName.empty())
		return;
	ofstream outfile(strTxtFileName);
	for (indexImg = 0,markedNum=0; indexImg < m_vctImageData.size(); indexImg++)  //每张图片的数据存为一行
	{
		if (m_vctImageData[indexImg][0] == QPointF(0, 0))  //检测该点是否被赋值过
			continue;
		markedNum++;
		outfile << std::to_string(indexImg) << '\t';  //存储点的下标
		//存储本图片所有点的x坐标
		for (indexPt = 0; indexPt < POINT_NUM; indexPt++)
			outfile << m_vctImageData[indexImg][indexPt].x() << '\t';
		//存储本图片所有点的y坐标
		for (indexPt = 0; indexPt < POINT_NUM; indexPt++)
			outfile << m_vctImageData[indexImg][indexPt].y() << '\t';
		outfile << '\n';
	}
	outfile.close();
	stringstream sstream;
	sstream << "总共" << markedNum << "张图片，存储了其中已标注的" << m_vctImageData.size() << "张。";
	info = sstream.str();
	QMessageBox::information(this, "完成", QString::fromLocal8Bit(info.c_str()));
}


