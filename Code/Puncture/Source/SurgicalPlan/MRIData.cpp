//*****************************************************************
// 文件名 :						MRIData.cpp
// 版本	 :						1.0
// 目的及主要功能 :				磁共振数据管理模块
// 创建日期 :					2019.11.1
// 修改日期 :					2020.05.14
// 作者 :						Fiki
// 修改者 :						孙易辙
// 联系方式 :					
//*****************************************************************/
#include "stdafx.h"
#include "MRIData.h"
#include "ErrorManager//ErrorCodeDefine.h"
#include <fstream>
#include "CPubFunc.h"

using namespace SURGICALPLAN;


/*****************************************************************
Name:			MRIData
Inputs:
	none
Return Value:
	none
Description:	MRIData构造函数
*****************************************************************/
MRIData::MRIData()
{
	InitData();
}//MRIData


/*****************************************************************
Name:			~MRIData
Inputs:
	none
Return Value:
	none
Description:	MRIData析构函数
*****************************************************************/
MRIData::~MRIData()
{
	DestroyData();
}//~MRIData

void MRIData::InitData()
{
	m_pMRIData = NULL;

	m_nPixNumberX = 0;
	m_nPixNumberY = 0;
	m_nPixNumberZ = 0;

	//TODO 暂时使用预设的默认值
	m_MRIRes.x = 0.625;
	m_MRIRes.y = 0.625;
	m_MRIRes.z = 3.85;
}

void MRIData::DestroyData()
{
	if (m_pMRIData != NULL)
		delete[]m_pMRIData;
	m_pMRIData = NULL;
}



/*****************************************************************
Name:			LoadRawMRI
Inputs:
	none
Return Value:
	none
Description:	导入数据
*****************************************************************/
BOOL MRIData::LoadRawMRI(CString t_strFileName, int cx, int cy, int cz)
{
	std::ifstream infile;
	infile.open(t_strFileName.GetBuffer());
	if (!infile.bad())
	{
		m_nPixNumberX = cx;
		m_nPixNumberY = cy;
		m_nPixNumberZ = cz;
		if (m_pMRIData != NULL)
			delete[]m_pMRIData;
		m_pMRIData = new MRIDataType[cx*cy*cz];
		infile.read((char *)m_pMRIData, cx*cy*cz * sizeof(MRIDataType));
		infile.close();
		return LIST_NO_ERROR;
	}
	else
		return ER_InportMRIDataError;
}

void MRIData::SetMRIVolumeResolution(double ResX, double ResY, double ResZ)
{
	m_MRIRes.x = ResX;
	m_MRIRes.y = ResY;
	m_MRIRes.z = ResZ;
}

void MRIData::GetMRIVolumeResolution(double &ResX, double &ResY, double &ResZ)
{
	ResX = m_MRIRes.x;
	ResY = m_MRIRes.y;
	ResZ = m_MRIRes.z;
}

MRIDataType MRIData::GetPointValue(double x, double y, double z)
{
	int tx = CPubFunc::Round(x);
	int ty = CPubFunc::Round(y);
	int tz = CPubFunc::Round(z);

	MRIDataType data;
	if (tx >= 0 && tx < m_nPixNumberX - 1 && ty >= 0 && ty < m_nPixNumberY - 1 && tz >= 0 && tz < m_nPixNumberZ - 1)
		data = *(m_pMRIData + tx + (ty + tz * m_nPixNumberY)*m_nPixNumberX);
	else
		data = 0;

	return data;
}

void MRIData::GetMRIVolumeSize(int &x, int &y, int &z)
{
	x = m_nPixNumberX;
	y = m_nPixNumberY;
	z = m_nPixNumberZ;
	return;
}