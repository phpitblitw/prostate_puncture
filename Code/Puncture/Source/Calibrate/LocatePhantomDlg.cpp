#include "LocatePhantomDlg.h"
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <string>
#include <sstream>
#include <fstream>
//#pragma execution_character_set("utf-8")  //避免中文乱码 参考https://blog.csdn.net/liyuanbhu/article/details/72596952
const int TIME_WAIT = 5000;  //采集多个点的坐标前，等待的时长
const int TIME_INTERVAL = 200;  //采集多个点的坐标之间 间隔的市场
const int PT_NUM = 30;  //采集多个点的数量

LocatePhantomDlg::LocatePhantomDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.SensorNumber->setValue(-1);

	//信号与槽
	connect(ui.SensorNumber, SIGNAL(valueChanged(int)), this, SLOT(ChangeSensor(int)));
	connect(ui.BtnRecord, SIGNAL(clicked()), this, SLOT(OnBtnRecordClicked()));
	connect(ui.BtnRecordRepeat, SIGNAL(clicked()), this, SLOT(OnBtnRecordRepeatClicked()));
	connect(ui.BtnDelete, SIGNAL(clicked()), this, SLOT(OnBtnDeleteClicked()));
	connect(ui.BtnSave, SIGNAL(clicked()), this, SLOT(OnBtnSaveClicked()));
}

LocatePhantomDlg::~LocatePhantomDlg()
{
	m_NDIOperatorPtr->ResetSensorNumber();  //将NDI设备使用的sensor，重置为ini配置文件上所规定的
}

void LocatePhantomDlg::SetNDIOperator(NDIOperatorPtr pNDIOperator)
{
	m_NDIOperatorPtr = pNDIOperator;
	if (m_NDIOperatorPtr == nullptr)
	{
		QMessageBox::information(this, u8"错误", u8"NDI设备为空");
		return;
	}
	else if (!m_NDIOperatorPtr->IsTracking())
	{
		QMessageBox::information(this, u8"错误", u8"NDI设备未在运行");
	}
	ui.SensorNumber->setValue(m_NDIOperatorPtr->GetSensorNumber());
}

void LocatePhantomDlg::ChangeSensor(int n)
{
	m_NDIOperatorPtr->SetSensorNumber(n);
}

void LocatePhantomDlg::UpdatePreview()
{
	int i, j;
	string s;
	//显示点坐标
	for (i = 0; i < m_vctPoints.size(); i++)
	{
		s += to_string(i+1);  //已标注的序号
		s += '\t';
		for (j = 0; j < 3; j++)
		{
			stringstream sstream;  //将float浮点数转为string类型，参考https://blog.csdn.net/feidaji/article/details/85123002
			sstream << m_vctPoints[i][j];
			s += sstream.str() + '\t';
		}
		s += '\n';
	}
	ui.LocatedPts->setPlainText(QString::fromLocal8Bit(s.c_str()));  //更新显示
}

void LocatePhantomDlg::OnBtnRecordClicked()
{
	float fX, fY, fZ, fAlpha, fBeta, fGama;
	m_euler = m_NDIOperatorPtr->GetCurEulerAttitude();
	m_euler.GetPosition(fX, fY, fZ, fAlpha, fBeta, fGama);  //获取欧拉角参数信息
	m_vctPoints.push_back(vector<float>{fX, fY, fZ});  //存储点坐标
	this->UpdatePreview();
}

//采集多个点的坐标
void LocatePhantomDlg::OnBtnRecordRepeatClicked()
{
	int i;
	float fX, fY, fZ, fAlpha, fBeta, fGama;
	Sleep(TIME_WAIT);  //先等待一定的市场，从而允许操作者将探头放到对应位置
	for (i = 0; i < PT_NUM; i++)
	{
		m_euler = m_NDIOperatorPtr->GetCurEulerAttitude();
		m_euler.GetPosition(fX, fY, fZ, fAlpha, fBeta, fGama);  //获取欧拉角参数信息
		m_vctPoints.push_back(vector<float>{fX, fY, fZ});  //存储点坐标
		this->UpdatePreview();
		Sleep(TIME_INTERVAL);
	}
	QMessageBox::information(this, u8"完成", u8"多点坐标采集完成");
}

void LocatePhantomDlg::OnBtnDeleteClicked()
{
	if (!m_vctPoints.empty())
		m_vctPoints.pop_back();
	this->UpdatePreview();
}

void LocatePhantomDlg::OnBtnSaveClicked()
{
	int i, j;
	string s,info;
	stringstream sstream;  //将float浮点数转为string类型，参考https://blog.csdn.net/feidaji/article/details/85123002
	QString qstrTxtFilename = QFileDialog::getSaveFileName(this, tr(u8"存储点集"), "D:/", "*.txt");
	string strTxtFileName = (const char*)qstrTxtFilename.toLocal8Bit();  //转换为std::string 且避免中文字符出现乱码
	if (strTxtFileName.empty())
		return;
	
	ofstream outfile(strTxtFileName);
	for (i = 0; i < m_vctPoints.size(); i++)
	{
		for (j = 0; j < 3; j++)
		{
			outfile << m_vctPoints[i][j] << '\t';
		}
		outfile << '\n';
	}
	outfile.close();

	sstream.clear();
	sstream << "已存储了" << m_vctPoints.size() << "个点的位置";
	info = sstream.str();
	QMessageBox::information(this, u8"完成", QString::fromLocal8Bit(info.c_str()));
}
