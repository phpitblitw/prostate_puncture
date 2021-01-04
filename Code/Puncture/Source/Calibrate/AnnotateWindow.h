//仅用于标注的GUI界面，包括导入、存储等按钮
#pragma once
#include <QMainWindow>
#include <vector>
#include <string>
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

private slots:
	void OnBtnBtnSetImageDirClicked();  //设置图像组所在的路径(暂时仅导入第一帧)
	void OnBtnFormerImageClicked();  //转到前一帧
	void OnBtnNextImageClicked();  //转到下一帧
	void OnBtnFinishClicked();  //存储当前帧
	void OnBtnSaveClicked();  //存储已标注的所有数据

private:
	Ui::AnnotateWindow ui;
	string m_strRootDir;
	int m_nImageIndex;  //当前正在处理的图片序号
	vector<string> m_vctImagePath;  //图片文件名
	vector<vector<QPointF>> m_vctImageData;  //各个图片中各个点的位置
};
