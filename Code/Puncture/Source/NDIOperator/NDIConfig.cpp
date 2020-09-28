//*****************************************************************
// 文件名 :						NDIConfig.h
// 版本	 :						1.0
// 目的及主要功能 :				NID设备设置
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
#include "NDIConfig.h"
#include "ComUtility//IniFile.h"
#include "ErrorManager//ErrorCodeDefine.h"


using namespace NDIOPERATOR;

/*****************************************************************
Name:			NDIConfig(待完成)
Inputs:
	none
Return Value:
	none
Description:	NDIConfig构造函数
*****************************************************************/
NDIConfig::NDIConfig()
{
}//NDIConfig


/*****************************************************************
Name:			~NDIConfig(待完成)
Inputs:
	none
Return Value:
	none
Description:	NDIConfig析构函数
*****************************************************************/
NDIConfig::~NDIConfig()
{
}//~NDIConfig


/*****************************************************************
Name:			LoadConfigFile(待完成)
Inputs:
	CString t_strFilePathName - Config文件所在路径及文件名
Return Value:
	none
Description:	读取Config文件相关信息
*****************************************************************/
int NDIConfig::LoadConfigFile(CString t_strFilePathName)
{
	//判断文件是否存在
	if (!PathFileExists(t_strFilePathName))
	{
		return ER_WrongNDIConfigFilePathName;
	}

	fsutility::CIniFile t_ScanConfig;	//config文件

	if (!t_ScanConfig.Open(t_strFilePathName))
	{
		return ER_OpenNDIConfigFileFailed;
	}
	
	//t_ScanConfig.Move2Section(_T("DataBase"));
	//t_ScanConfig.ReadKey(_T("DataBaseServer"), m_strDataBaseServer);

	//ndi探头连接NDI设备的端口号
	t_ScanConfig.Move2Section(_T("Hostname"));
	t_ScanConfig.ReadKey(_T("hostname"), m_strHostname);

	//B超探头初始UpDir
	t_ScanConfig.Move2Section(_T("Initial MoveDir"));
	t_ScanConfig.ReadKey(_T("x"), m_MoveDir.x);
	t_ScanConfig.ReadKey(_T("y"), m_MoveDir.y);
	t_ScanConfig.ReadKey(_T("z"), m_MoveDir.z);
	m_MoveDir.w = 0;		//MoveDir为向量
	m_MoveDir.Normalize();	//向量单位化

	//B超探头初始UpDir
	t_ScanConfig.Move2Section(_T("Initial UpDir"));
	t_ScanConfig.ReadKey(_T("x"), m_UpDir.x);
	t_ScanConfig.ReadKey(_T("y"), m_UpDir.y);
	t_ScanConfig.ReadKey(_T("z"), m_UpDir.z);
	m_UpDir.w = 0;			//UpDir为向量
	m_UpDir.Normalize();	//向量单位化

	//标定矩阵
	t_ScanConfig.Move2Section(_T("Calibration Matrix"));
	t_ScanConfig.ReadKey(_T("T[0][0]"), m_CalibrationMatrixElem[0]);
	t_ScanConfig.ReadKey(_T("T[0][1]"), m_CalibrationMatrixElem[1]);
	t_ScanConfig.ReadKey(_T("T[0][2]"), m_CalibrationMatrixElem[2]);
	t_ScanConfig.ReadKey(_T("T[0][3]"), m_CalibrationMatrixElem[3]);
	t_ScanConfig.ReadKey(_T("T[1][0]"), m_CalibrationMatrixElem[4]);
	t_ScanConfig.ReadKey(_T("T[1][1]"), m_CalibrationMatrixElem[5]);
	t_ScanConfig.ReadKey(_T("T[1][2]"), m_CalibrationMatrixElem[6]);
	t_ScanConfig.ReadKey(_T("T[1][3]"), m_CalibrationMatrixElem[7]);
	t_ScanConfig.ReadKey(_T("T[2][0]"), m_CalibrationMatrixElem[8]);
	t_ScanConfig.ReadKey(_T("T[2][1]"), m_CalibrationMatrixElem[9]);
	t_ScanConfig.ReadKey(_T("T[2][2]"), m_CalibrationMatrixElem[10]);
	t_ScanConfig.ReadKey(_T("T[2][3]"), m_CalibrationMatrixElem[11]);
	t_ScanConfig.ReadKey(_T("T[3][0]"), m_CalibrationMatrixElem[12]);
	t_ScanConfig.ReadKey(_T("T[3][1]"), m_CalibrationMatrixElem[13]);
	t_ScanConfig.ReadKey(_T("T[3][2]"), m_CalibrationMatrixElem[14]);
	t_ScanConfig.ReadKey(_T("T[3][3]"), m_CalibrationMatrixElem[15]);
	m_CalibrationMatrix.SetValue(m_CalibrationMatrixElem);

	return LIST_NO_ERROR;
}//LoadConfigFile
