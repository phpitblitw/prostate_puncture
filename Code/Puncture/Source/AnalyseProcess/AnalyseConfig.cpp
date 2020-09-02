//*****************************************************************
// 文件名 :						AnalyseConfig.cpp
// 版本	 :						1.0
// 目的及主要功能 :				AnalyseProcess参数设置
// 创建日期 :					2020.4.28
// 修改日期 :					
// 作者 :						SunYizhe
// 修改者 :						
// 联系方式 :					
//*****************************************************************/

///////////////////////////////////////////////////////
////////////////////////include////////////////////////
///////////////////////////////////////////////////////
#include "pch.h"
#include "AnalyseProcess/AnalyseConfig.h"
#include "ComUtility/IniFile.h"
#include "ErrorManager/ErrorCodeDefine.h"

using namespace ANALYSEPROCESS;

/*****************************************************************
Name:			AnalyseConfig
Inputs:
	none
Return Value:
	none
Description:	AnalyseConfig构造函数
*****************************************************************/
AnalyseConfig::AnalyseConfig()
{
	m_ResX = 0;
	m_ResY = 0;
	m_ResZ = 0;
	m_SizeX = 0;
	m_SizeY = 0;
	m_SizeZ = 0;
}

/*****************************************************************
Name:			AnalyseConfig
Inputs:
	none
Return Value:
	none
Description:	AnalyseConfig析构函数
*****************************************************************/
AnalyseConfig::~AnalyseConfig()
{
}

/*****************************************************************
Name:			LoadConfigFile
Inputs:
	CString t_strFilePathName - Config文件所在路径及文件名
Return Value:
	none
Description:	读取Config文件相关信息
*****************************************************************/
int AnalyseConfig::LoadConfigFile(CString t_strFilePathName)
{
	//判断文件是否存在
	if (!PathFileExists(t_strFilePathName))
	{
		return ER_WrongAnalyseConfigFilePathName;
	}

	fsutility::CIniFile t_ScanConfig;	//config文件

	if (!t_ScanConfig.Open(t_strFilePathName))
	{
		return ER_OpenAnalyseConfigFileFailed;
	}

	//MRI分辨率
	t_ScanConfig.Move2Section(_T("ImageSize"));
	t_ScanConfig.ReadKey(_T("x"), m_ResX);
	t_ScanConfig.ReadKey(_T("y"), m_ResY);
	t_ScanConfig.ReadKey(_T("z"), m_ResZ);
	//MRI体素大小
	t_ScanConfig.Move2Section(_T("VoxelSize"));
	t_ScanConfig.ReadKey(_T("x"), m_SizeX);
	t_ScanConfig.ReadKey(_T("y"), m_SizeY);
	t_ScanConfig.ReadKey(_T("z"), m_SizeZ);

	//切割前列腺最大截面
	//ScanCenter点
	t_ScanConfig.Move2Section(_T("ScanCenter"));
	t_ScanConfig.ReadKey(_T("x"), m_ScanCenter.x);
	t_ScanConfig.ReadKey(_T("y"), m_ScanCenter.y);
	t_ScanConfig.ReadKey(_T("z"), m_ScanCenter.z);
	m_ScanCenter.w = 1;	//点的w置为1
	m_ScanCenter.Normalize();
	//RightDir向量
	t_ScanConfig.Move2Section(_T("RightDir"));
	t_ScanConfig.ReadKey(_T("x"), m_RightDir.x);
	t_ScanConfig.ReadKey(_T("y"), m_RightDir.y);
	t_ScanConfig.ReadKey(_T("z"), m_RightDir.z);
	m_RightDir.w = 0;
	m_RightDir.Normalize();		//向量单位化
	//UpDir向量
	t_ScanConfig.Move2Section(_T("UpDir"));
	t_ScanConfig.ReadKey(_T("x"), m_UpDir.x);
	t_ScanConfig.ReadKey(_T("y"), m_UpDir.y);
	t_ScanConfig.ReadKey(_T("z"), m_UpDir.z);
	m_UpDir.w = 0;
	m_UpDir.Normalize();		//向量单位化
	//MoveDir向量
	t_ScanConfig.Move2Section(_T("MoveDir"));
	t_ScanConfig.ReadKey(_T("x"), m_MoveDir.x);
	t_ScanConfig.ReadKey(_T("y"), m_MoveDir.y);
	t_ScanConfig.ReadKey(_T("z"), m_MoveDir.z);
	m_MoveDir.w = 0;
	m_MoveDir.Normalize();		//向量单位化
	//return LIST_NO_ERROR;
	return 0;	//待删除
}