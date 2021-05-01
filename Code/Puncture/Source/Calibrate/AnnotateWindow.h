#pragma once
//仅用于标注的GUI界面，包括导入、存储等按钮
#include <QMainWindow>
#include <vector>
#include <string>
#include <qvector3d.h>
#include "ui_AnnotateWindow.h"
using namespace std;

class AnnotateWindow : public QMainWindow
{
	Q_OBJECT

public:
	AnnotateWindow(QWidget *parent = Q_NULLPTR);
	~AnnotateWindow();

public slots:
	void UpdateMousePos(float x, float y);

private:
	void Cal3DPosInImage(bool bTransverse, float fPixelSizeX, float fPixelSizeY); //根据已经标注好的图片数据，计算每张图片每行中心点的物理坐标(图像坐标系下)
	void Cal3DPosInPhantom();  //根据已经标注好的图片数据，计算每张图片每行中间点的坐标(体模坐标系下)

private slots:
	void OnBtnBtnSetImageDirClicked();  //设置图像组所在的路径(暂时仅导入第一帧),并且将各个装载图片数据的vector的size设置为图片数!!!
	void OnBtnFormerImageClicked();  //转到前一帧
	void OnBtnNextImageClicked();  //转到下一帧
	void OnBtnFinishClicked();  //存储当前帧
	void OnBtnSave2DClicked();  //存储已标注的所有数据(在2D图像坐标系中的坐标 图片数*每张图9个点)
	void OnBtnSave3DInImageClicked ();  //存储各个图片中，各行中间的点，在3D图片坐标系下的物理坐标(单位:mm)
	void OnBtnSave3DInPhantomClicked();  //计算并存储已标注的所有数据(在体模坐标系中的坐标 图片数*每张图3个点)
	void OnBtnLoadPhantomTriangles();  //载入体模参考点坐标(即每组N线的2个角点在体模坐标系下的坐标 对于3N线 即为x1y1z1 x2y2z2 x3y3z3 x4y4z4 x5y5z5 x6y6z6)

private:
	Ui::AnnotateWindow ui;
	string m_strRootDir;
	int m_nImageIndex;  //当前正在处理的图片序号
	int m_nImageWidth, m_nImageHeight;
	vector<string> m_vctImagePath;  //读入的所有图片文件名
	vector<vector<QPointF>> m_vctPtPos2D;  //各个图片中,各个点在2D图片坐标系下(原点在左上角)的像素坐标(单位：像素)
	vector<vector<QVector3D>> m_vctPtPos3DInImage;  //各个图片中，各行中间的点，在3D图片坐标系下(原点在底部中间)的物理坐标(单位:mm)
	vector <vector<QVector3D>> m_vctPtPos3DInPhantom;  //各个图片中，各行中间的点，在3D体模坐标系下的物理坐标(单位:mm)
	vector<QVector3D> m_vctTriangleVertex;  //体模坐标系下，相似三角形的顶点坐标(共POINT_NUM/3*2个，因为图片中每行有3个点，其中2个为相似三角形的顶点)
};
