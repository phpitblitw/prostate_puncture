#include "PunctureWindow.h"
#include <string>
#include <qmessagebox.h>
#include "ErrorManager/ErrorCodeDefine.h"
#pragma execution_character_set("utf-8")  //设置默认编码格式 避免中文乱码

#define UPDATE_INTERVAL 20  //图像刷新间隔(ms)
#define MRI_MOVE_STEP 1  //键盘控制MRI移动  实际步长

PunctureWindow::PunctureWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	m_b2DAcquired = false;
	m_b3DAcquired = false;
	m_bRegistered = false;
	m_NDIOperatorPtr = nullptr;
	m_USBCapturerPtr = nullptr;
	m_SurgicalPlanPtr = nullptr;
	m_AnalyseProcessPtr = nullptr;
	m_FrameDataPtr = nullptr;
	InitWindow();
}

void PunctureWindow::InitWindow()
{
	//按钮状态初始化
	ui.BtnInitDevice->setEnabled(true);
	ui.BtnQuit->setEnabled(false);

	//绑定信号与槽
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(OnTimerTimeout()));  //定时刷新显示图像
	connect(ui.BtnInitDevice, SIGNAL(clicked()), this, SLOT(InitDevice()));  //初始化设备
	connect(ui.BtnRegister, SIGNAL(clicked()), this, SLOT(OnBtnRegisterClicked()));  //医生手动点击按钮 锁定坐标
	connect(ui.BtnQuit, SIGNAL(clicked()), this, SLOT(Quit()));  //点击退出程序 释放各个设备
	connect(ui.view2D1, SIGNAL(KeyLeftPressed()), this, SLOT(MoveMRILeft()));
	connect(ui.view2D1, SIGNAL(KeyRightPressed()), this, SLOT(MoveMRIRight()));
	connect(ui.view2D1, SIGNAL(KeyUpPressed()), this, SLOT(MoveMRIUp()));
	connect(ui.view2D1, SIGNAL(KeyDownPressed()), this, SLOT(MoveMRIDown()));
	connect(ui.view2D2, SIGNAL(KeyLeftPressed()), this, SLOT(MoveMRIForward()));
	connect(ui.view2D2, SIGNAL(KeyRightPressed()), this, SLOT(MoveMRIBackward()));
	connect(ui.view2D2, SIGNAL(KeyUpPressed()), this, SLOT(MoveMRIUp()));
	connect(ui.view2D2, SIGNAL(KeyDownPressed()), this, SLOT(MoveMRIDown()));

	//允许手动配准
	ui.BtnRegister->setEnabled(false);
}

void PunctureWindow::closeEvent(QCloseEvent * event)
{
	this->Quit();
}

int PunctureWindow::InitDevice()
{
	//灰化按钮
	ui.BtnInitDevice->setEnabled(false);

	//初始化
	char exePath[MAX_PATH];
	memset(exePath, 0, MAX_PATH);
	GetModuleFileNameA(NULL, exePath, MAX_PATH);  //找到当前exe文件名
	std::string strConfigRootPath = std::string(exePath);
	strConfigRootPath = strConfigRootPath.substr(0, strConfigRootPath.rfind('\\')) + "\\Config\\";  //生成config文件夹路径
	std::string strIniFileName;

	//初始化NDI设备
	strIniFileName = strConfigRootPath + "NDIDevice.ini";
	m_NDIOperatorPtr.reset(new NDIOperator());
	if (m_NDIOperatorPtr->InitNDIDevice(strIniFileName) != LIST_NO_ERROR)
	{
		QMessageBox::information(this, "错误", "启动NDI设备失败");
		return ER_InitNDIDeviceFailed;
	}

	//初始化超声设备
	strIniFileName = strConfigRootPath + "USBDevice.ini";
	m_USBCapturerPtr.reset(new USBCapturer());
	if (m_USBCapturerPtr->InitUSBDevice(strIniFileName) != LIST_NO_ERROR)
	{
		QMessageBox::information(this, "错误", "启动超声设备失败");
		return ER_InitUSBDeviceFailed;
	}

	//创建手术计划模块
	strIniFileName = strConfigRootPath + "SurgicalPlan.ini";
	m_SurgicalPlanPtr.reset(new SurgicalPlan());
	if (m_SurgicalPlanPtr->InPortAsFileSet(strIniFileName) != LIST_NO_ERROR)
	{
		QMessageBox::information(this, "错误", "导入手术计划失败");
		return ER_FileOpenFailed;
	}

	//创建分析处理模块
	strIniFileName = strConfigRootPath + "AnalyseProcess.ini";
	m_AnalyseProcessPtr.reset(new AnalyseProcess());
	m_AnalyseProcessPtr->BindUpdateFrameEvent(std::bind(&PunctureWindow::UpdateFrame, this, std::placeholders::_1));
	m_AnalyseProcessPtr->SetNDIDevicePtr(m_NDIOperatorPtr);
	m_AnalyseProcessPtr->SetUSBCapturerPtr(m_USBCapturerPtr);
	m_AnalyseProcessPtr->SetSurgicalPlanPtr(m_SurgicalPlanPtr);
	if (m_AnalyseProcessPtr->InitAnalyseProcess(strIniFileName)!=LIST_NO_ERROR)
	{
		QMessageBox::information(this, "错误", "启动分析模块失败");
		return ER_InitAnalyseProcessFailed;
	}
	
	//启动分析
	m_NDIOperatorPtr->StartTracking();
	m_USBCapturerPtr->StartGrab();
	m_AnalyseProcessPtr->StartAnalyse();

	//开启定时器
	m_timer.setInterval(UPDATE_INTERVAL);
	m_timer.start();

	//开启按钮
	ui.BtnRegister->setEnabled(true);
	ui.BtnQuit->setEnabled(true);

	return LIST_NO_ERROR;
}

void PunctureWindow::OnTimerTimeout()
{
	m_showMutex.lock();
	m_showMutex.unlock();
	//检测数据是否导入
	if (m_FrameDataPtr == nullptr)
		return;

	m_showMutex.lock();
	//2D显示
	if(m_b2DAcquired)
	{
		if (!m_FrameDataPtr->m_USImgT.empty())  //横断面
		{
			ui.view2D1->LoadImg(m_FrameDataPtr->m_USImgT);
			ui.view2D1->LoadProstateMask(m_FrameDataPtr->m_prostateMaskT);
			ui.view2D1->LoadLesionMask(m_FrameDataPtr->m_lesionMaskT);
			ui.view2D1->LoadRectumMask(m_FrameDataPtr->m_rectumMaskT);
			ui.view2D1->ShowImg();
		}
		
		if (!m_FrameDataPtr->m_USImgS.empty())  //矢状面
		{
			ui.view2D2->LoadImg(m_FrameDataPtr->m_USImgS);
			ui.view2D2->LoadProstateMask(m_FrameDataPtr->m_prostateMaskS);
			ui.view2D2->LoadLesionMask(m_FrameDataPtr->m_lesionMaskS);
			ui.view2D2->LoadRectumMask(m_FrameDataPtr->m_rectumMaskS);
			ui.view2D2->ShowImg();
		}
	}
	//3D显示
	if(m_b3DAcquired)
	{
		//TODO
	}
	m_showMutex.unlock();
}

void PunctureWindow::OnBtnRegisterClicked()
{
	m_AnalyseProcessPtr->Register();
	m_bRegistered = true;
}

void PunctureWindow::Quit()
{
	//停止各个模块
	if (m_AnalyseProcessPtr != nullptr)
		m_AnalyseProcessPtr->StopAnalyse();
	if (m_NDIOperatorPtr != nullptr)
		m_NDIOperatorPtr->StopTracking();
	if (m_USBCapturerPtr != nullptr)
		m_USBCapturerPtr->StopGrab();
	//关闭窗口
	this->close();
}

void PunctureWindow::MoveMRILeft()
{
	m_AnalyseProcessPtr->MoveMRIRight(-MRI_MOVE_STEP);
}

void PunctureWindow::MoveMRIRight()
{
	m_AnalyseProcessPtr->MoveMRIRight(MRI_MOVE_STEP);
}

void PunctureWindow::MoveMRIUp()
{
	m_AnalyseProcessPtr->MoveMRIUp(MRI_MOVE_STEP);
}

void PunctureWindow::MoveMRIDown()
{
	m_AnalyseProcessPtr->MoveMRIUp(-MRI_MOVE_STEP);
}

void PunctureWindow::MoveMRIForward()
{
	m_AnalyseProcessPtr->MoveMRIForward(MRI_MOVE_STEP);
}

void PunctureWindow::MoveMRIBackward()
{
	m_AnalyseProcessPtr->MoveMRIForward(-MRI_MOVE_STEP);
}

void PunctureWindow::UpdateFrame(FrameDataPtr t_FrameDataPtr)
{
	m_showMutex.lock();
	m_FrameDataPtr = t_FrameDataPtr;
	m_b2DAcquired = true;
	m_b3DAcquired = true;
	m_showMutex.unlock();
}