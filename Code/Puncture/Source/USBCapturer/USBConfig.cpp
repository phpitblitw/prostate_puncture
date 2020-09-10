//*****************************************************************
// 文件名 :						USBConfig.h
// 版本	 :						1.0
// 目的及主要功能 :				超声图像采集设备设置
// 创建日期 :					2019.11.1
// 修改日期 :					
// 作者 :						Fiki
// 修改者 :						
// 联系方式 :					fiki@seu.edu.cn
//*****************************************************************/

///////////////////////////////////////////////////////
////////////////////////include////////////////////////
///////////////////////////////////////////////////////
#include "stdafx.h"
#include "USBConfig.h"
#include "ComUtility//IniFile.h"
#include "ErrorManager//ErrorCodeDefine.h"


using namespace USBCAPTURER;

/*****************************************************************
Name:			USBConfig(待完成)
Inputs:
	none
Return Value:
	none
Description:	USBConfig构造函数
*****************************************************************/
USBConfig::USBConfig()
{
	////B超坐标轴区域参数
	//m_Axis.x = 0;
	//m_Axis.y = 0;
	//m_Axis.width = 0;
	//m_Axis.height = 0;
	
	//右侧坐标轴坐标 用于判断分辨率
	m_RightAxisRect.x = 0;
	m_RightAxisRect.y = 0;
	m_RightAxisRect.width = 0;
	m_RightAxisRect.height = 0;

	//左侧坐标轴坐标 用于判断单平面/双平面
	m_UpAxisRect.x = 0;
	m_UpAxisRect.y = 0;
	m_UpAxisRect.width = 0;
	m_UpAxisRect.height = 0;
	m_DownAxisRect.x = 0;
	m_DownAxisRect.y = 0;
	m_DownAxisRect.width = 0;
	m_DownAxisRect.height = 0;

	//单屏幕下的参数
	m_FullRect.x = 0;
	m_FullRect.y = 0;
	m_FullRect.width = 0;
	m_FullRect.height = 0;

	//双屏幕下的参数
	m_UpRect.x = 0;
	m_UpRect.y = 0;
	m_UpRect.width = 0;
	m_UpRect.height = 0;

	m_DownRect.x = 0;
	m_DownRect.y = 0;
	m_DownRect.width = 0;
	m_DownRect.height = 0;

}//USBConfig


/*****************************************************************
Name:			~USBConfig(待完成)
Inputs:
	none
Return Value:
	none
Description:	USBConfig析构函数
*****************************************************************/
USBConfig::~USBConfig()
{
}//~USBConfig


/*****************************************************************
Name:			LoadConfigFile(待完成)
Inputs:
	CString t_strFilePathName - Config文件所在路径及文件名
Return Value:
	none
Description:	读取Config文件相关信息
*****************************************************************/
int USBConfig::LoadConfigFile(CString t_strFilePathName)
{
	//判断文件是否存在
	if (!PathFileExists(t_strFilePathName))
	{
		return ER_WrongUSBConfigFilePathName;
	}

	fsutility::CIniFile t_ScanConfig;	//config文件

	if (!t_ScanConfig.Open(t_strFilePathName))
	{
		return ER_OpenUSBConfigFileFailed;
	}

	////B超坐标轴区域坐标
	//t_ScanConfig.Move2Section(_T("Axis"));
	//t_ScanConfig.ReadKey(_T("StartX"), m_Axis.x);
	//t_ScanConfig.ReadKey(_T("StartY"), m_Axis.y);
	//t_ScanConfig.ReadKey(_T("Width"), m_Axis.width);
	//t_ScanConfig.ReadKey(_T("Height"), m_Axis.height);

	//右侧坐标轴坐标 用于判断分辨率
	t_ScanConfig.Move2Section(_T("Right Axis"));
	t_ScanConfig.ReadKey(_T("StartX"), m_RightAxisRect.x);
	t_ScanConfig.ReadKey(_T("StartY"), m_RightAxisRect.y);
	t_ScanConfig.ReadKey(_T("Width"), m_RightAxisRect.width);
	t_ScanConfig.ReadKey(_T("Height"), m_RightAxisRect.height);

	//左侧坐标轴坐标 用于判断单平面/双平面
	t_ScanConfig.Move2Section(_T("Left Axis"));
	t_ScanConfig.ReadKey(_T("StartX_Up"), m_UpAxisRect.x);
	t_ScanConfig.ReadKey(_T("StartY_Up"), m_UpAxisRect.y);
	t_ScanConfig.ReadKey(_T("Width_Up"), m_UpAxisRect.width);
	t_ScanConfig.ReadKey(_T("Height_Up"), m_UpAxisRect.height);

	t_ScanConfig.ReadKey(_T("StartX_Down"), m_DownAxisRect.x);
	t_ScanConfig.ReadKey(_T("StartY_Down"), m_DownAxisRect.y);
	t_ScanConfig.ReadKey(_T("Width_Down"), m_DownAxisRect.width);
	t_ScanConfig.ReadKey(_T("Height_Down"), m_DownAxisRect.height);

	//单平面/双平面ROI
	t_ScanConfig.Move2Section(_T("ROI"));
	t_ScanConfig.ReadKey(_T("StartX_Full"), m_FullRect.x);
	t_ScanConfig.ReadKey(_T("StartY_Full"), m_FullRect.y);
	t_ScanConfig.ReadKey(_T("Width_Full"), m_FullRect.width);
	t_ScanConfig.ReadKey(_T("Height_Full"), m_FullRect.height);

	t_ScanConfig.ReadKey(_T("StartX_Up"), m_UpRect.x);
	t_ScanConfig.ReadKey(_T("StartY_Up"), m_UpRect.y);
	t_ScanConfig.ReadKey(_T("Width_Up"), m_UpRect.width);
	t_ScanConfig.ReadKey(_T("Height_Up"), m_UpRect.height);

	t_ScanConfig.ReadKey(_T("StartX_Down"), m_DownRect.x);
	t_ScanConfig.ReadKey(_T("StartY_Down"), m_DownRect.y);
	t_ScanConfig.ReadKey(_T("Width_Down"), m_DownRect.width);
	t_ScanConfig.ReadKey(_T("Height_Down"), m_DownRect.height);

	return LIST_NO_ERROR;
}//LoadConfigFile
