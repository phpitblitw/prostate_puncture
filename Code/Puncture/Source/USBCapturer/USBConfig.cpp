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

	//双平面的左侧长条区域
	t_ScanConfig.Move2Section(_T("Dual Left Bar"));
	t_ScanConfig.ReadKey(_T("x"), m_DualLeftBarRect.x);
	t_ScanConfig.ReadKey(_T("y"), m_DualLeftBarRect.y);
	t_ScanConfig.ReadKey(_T("width"), m_DualLeftBarRect.width);
	t_ScanConfig.ReadKey(_T("height"), m_DualLeftBarRect.height);

	//指示横断面(T)/矢状面(S)的字符区域
	t_ScanConfig.Move2Section(_T("Char Sensor Type"));
	t_ScanConfig.ReadKey(_T("x"), m_CharSensorTypeRect.x);
	t_ScanConfig.ReadKey(_T("y"), m_CharSensorTypeRect.y);
	t_ScanConfig.ReadKey(_T("width"), m_CharSensorTypeRect.width);
	t_ScanConfig.ReadKey(_T("height"), m_CharSensorTypeRect.height);

	//单平面 右侧坐标轴区域
	t_ScanConfig.Move2Section(_T("One Plane Right Axis"));
	t_ScanConfig.ReadKey(_T("x"), m_OnePlaneRightAxisRect.x);
	t_ScanConfig.ReadKey(_T("y"), m_OnePlaneRightAxisRect.y);
	t_ScanConfig.ReadKey(_T("width"), m_OnePlaneRightAxisRect.width);
	t_ScanConfig.ReadKey(_T("height"), m_OnePlaneRightAxisRect.height);

	//双平面模式 上方(横断面)右侧坐标轴区域
	t_ScanConfig.Move2Section(_T("Dual Plane Up Right Axis"));
	t_ScanConfig.ReadKey(_T("x"), m_DualPlaneUpRightAxisRect.x);
	t_ScanConfig.ReadKey(_T("y"), m_DualPlaneUpRightAxisRect.y);
	t_ScanConfig.ReadKey(_T("width"), m_DualPlaneUpRightAxisRect.width);
	t_ScanConfig.ReadKey(_T("height"), m_DualPlaneUpRightAxisRect.height);

	//双平面模式 下方(矢状面)右侧坐标轴区域
	t_ScanConfig.Move2Section(_T("Dual Plane Down Right Axis"));
	t_ScanConfig.ReadKey(_T("x"), m_DualPlaneDownRightAxisRect.x);
	t_ScanConfig.ReadKey(_T("y"), m_DualPlaneDownRightAxisRect.y);
	t_ScanConfig.ReadKey(_T("width"), m_DualPlaneDownRightAxisRect.width);
	t_ScanConfig.ReadKey(_T("height"), m_DualPlaneDownRightAxisRect.height);

	//单平面模式下 图像区域
	t_ScanConfig.Move2Section(_T("One Plane ROI"));
	t_ScanConfig.ReadKey(_T("x"), m_OnePlaneROI.x);
	t_ScanConfig.ReadKey(_T("y"), m_OnePlaneROI.y);
	t_ScanConfig.ReadKey(_T("width"), m_OnePlaneROI.width);
	t_ScanConfig.ReadKey(_T("height"), m_OnePlaneROI.height);

	//双平面模式下 上方(横断面)图像区域
	t_ScanConfig.Move2Section(_T("Dual Up ROI"));
	t_ScanConfig.ReadKey(_T("x"), m_DualUpROI.x);
	t_ScanConfig.ReadKey(_T("y"), m_DualUpROI.y);
	t_ScanConfig.ReadKey(_T("width"), m_DualUpROI.width);
	t_ScanConfig.ReadKey(_T("height"), m_DualUpROI.height);

	//双平面模式下 下方(矢状面)图像区域
	t_ScanConfig.Move2Section(_T("Dual Down ROI"));
	t_ScanConfig.ReadKey(_T("x"), m_DualDownROI.x);
	t_ScanConfig.ReadKey(_T("y"), m_DualDownROI.y);
	t_ScanConfig.ReadKey(_T("width"), m_DualDownROI.width);
	t_ScanConfig.ReadKey(_T("height"), m_DualDownROI.height);

	return LIST_NO_ERROR;
}//LoadConfigFile
