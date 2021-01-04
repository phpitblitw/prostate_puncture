#pragma once

#include <QDialog>
#include <vector>
#include "NDIOperator/NDIOperator.h"
#include "ui_LocatePhantomDlg.h"
using namespace NDIOPERATOR;
using namespace std;

class LocatePhantomDlg : public QDialog
{
	Q_OBJECT

public:
	LocatePhantomDlg(QWidget *parent = Q_NULLPTR);
	//LocatePhantomDlg(QWidget *parent = Q_NULLPTR, NDIOperatorPtr pNDIOperator = nullptr);
	~LocatePhantomDlg();

	void SetNDIOperator(NDIOperatorPtr pNDIOperator);
	void UpdateNDIData(NDIOPERATOR::Attitude euler) { m_euler = euler; }

private:
	Ui::LocatePhantomDlg ui;
	NDIOperatorPtr  m_NDIOperatorPtr;  //NDI设备操作指针--持有该指针，只是为了方便修改使用的sensor序号。NDI数据在CalibrateWindow中采集
	NDIOPERATOR::Attitude  m_euler;  //超声探头姿态(欧拉角形式)
	vector<vector<float>> m_vctPoints;  //n*3矩阵，记录每个点的位置
	void UpdatePreview();  //显示当前已标注的点

private slots:
	void ChangeSensor(int n);
	void OnBtnRecordClicked();  //记录当前点位置
	void OnBtnDeleteClicked();  //删除最近添加的点
	void OnBtnSaveClicked();  //存储至文件
};
