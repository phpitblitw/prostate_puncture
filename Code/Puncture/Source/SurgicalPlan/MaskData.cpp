#include "stdafx.h"
#include "MaskData.h"
#include <fstream>
#include "CPubFunc.h"
#include "ErrorManager//ErrorCodeDefine.h"


using namespace SURGICALPLAN;

MaskData::MaskData()
{
	InitData();
}


MaskData::~MaskData()
{
	DestroyData();
}

void MaskData::InitData()
{
	m_pMRIMaskData = NULL;

	m_nPixNumberX = 0;
	m_nPixNumberY = 0;
	m_nPixNumberZ = 0;

	m_MaskRes.x = 0.625;
	m_MaskRes.y = 0.625;
	m_MaskRes.z = 3.85;
}

void MaskData::DestroyData()
{
	if (m_pMRIMaskData != NULL)
		delete[]m_pMRIMaskData;
	m_pMRIMaskData = NULL;
}

BOOL MaskData::LoadRawMask(CString strFileName, int cx, int cy, int cz)
{
	std::ifstream infile;
	infile.open(strFileName.GetBuffer());
	if (!infile.bad())
	{
		m_nPixNumberX = cx;
		m_nPixNumberY = cy;
		m_nPixNumberZ = cz;
		if (m_pMRIMaskData != NULL)
			delete[]m_pMRIMaskData;
		m_pMRIMaskData = new MaskDataType[cx*cy*cz];
		infile.read((char *)m_pMRIMaskData, cx*cy*cz * sizeof(MaskDataType));
		infile.close();
		return LIST_NO_ERROR;

	}
	else
		return ER_InportMaskDataError;
}

void MaskData::SetMaskVolumeResolution(double ResX, double ResY, double ResZ)
{
	m_MaskRes.x = ResX;
	m_MaskRes.y = ResY;
	m_MaskRes.z = ResZ;

}

void MaskData::GetMaskVolumeResolution(double &ResX, double &ResY, double &ResZ)
{
	ResX = m_MaskRes.x;
	ResY = m_MaskRes.y;
	ResZ = m_MaskRes.z;
}

MaskDataType MaskData::GetPointValue(double x, double y, double z)
{
	int tx = CPubFunc::Round(x);
	int ty = CPubFunc::Round(y);
	int tz = CPubFunc::Round(z);

	MaskDataType data;
	if (tx >= 0 && tx < m_nPixNumberX - 1 && ty >= 0 && ty < m_nPixNumberY - 1 && tz >= 0 && tz < m_nPixNumberZ - 1)
		data = *(m_pMRIMaskData + tx + (ty + tz * m_nPixNumberY)*m_nPixNumberX);
	else
		data = 0;

	return data;
}

void MaskData::GetMaskVolumeSize(int &x, int &y, int &z)
{
	x = m_nPixNumberX;
	y = m_nPixNumberY;
	z = m_nPixNumberZ;
}