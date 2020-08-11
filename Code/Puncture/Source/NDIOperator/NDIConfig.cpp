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

	return LIST_NO_ERROR;
}//LoadConfigFile
