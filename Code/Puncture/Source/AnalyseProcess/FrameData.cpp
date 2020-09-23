//*****************************************************************
// 文件名 :						FrameData.h
// 版本	 :						2.0
// 目的及主要功能 :				单帧信息类
// 创建日期 :					2019.11.1
// 修改日期 :					2020.05.07
// 作者 :						Fiki
// 修改者 :						SunYizhe
// 联系方式 :					fiki@seu.edu.cn
//*****************************************************************/

///////////////////////////////////////////////////////
////////////////////////include////////////////////////
///////////////////////////////////////////////////////
#include "pch.h"
#include "AnalyseProcess/FrameData.h"
#include "ErrorManager/ErrorCodeDefine.h"

using namespace ANALYSEPROCESS;

/*****************************************************************
Name:			FrameData
Inputs:
	none
Return Value:
	none
Description:	FrameData构造函数
*****************************************************************/
FrameData::FrameData()
{
	//m_pProstateMask = nullptr;
	//m_pLesionMask = nullptr;
	m_pFusionMask = nullptr;
	m_dImageRes = 0;
}//FrameData


/*****************************************************************
Name:			~FrameData
Inputs:
	none
Return Value:
	none
Description:	FrameData析构函数
*****************************************************************/
FrameData::~FrameData()
{
	//if (m_pProstateMask != nullptr)
	//{
	//	delete[] m_pProstateMask;
	//}
	//if (m_pLesionMask != nullptr)
	//{
	//	delete[] m_pLesionMask;
	//}
	if (m_pFusionMask != nullptr)
	{
		delete[] m_pFusionMask;
	}
}//~FrameData


/*****************************************************************
Name:			CreatMaskData
Inputs:
	none
Return Value:
	none
Description:	开辟2D掩模图像空间
*****************************************************************/
int	FrameData::CreatMaskData(int t_nShowWidth, int t_nShowHeight)
{
	//if (m_pProstateMask != nullptr)
	//{
	//	delete[] m_pProstateMask;
	//}
	//if (m_pLesionMask != nullptr)
	//{
	//	delete[] m_pLesionMask;
	//}
	if (m_pFusionMask != nullptr)
		delete[] m_pFusionMask;
	if (t_nShowWidth <= 100 || t_nShowWidth >= 4000
		|| t_nShowHeight <= 100 || t_nShowHeight >= 4000)
	{
		return ER_2DImageSizeError;
	}

	//m_pProstateMask = new BYTE[t_nShowWidth * t_nShowWidth];
	//m_pLesionMask = new BYTE[t_nShowWidth * t_nShowWidth];
	m_pFusionMask = new BYTE[t_nShowWidth * t_nShowWidth];
	return LIST_NO_ERROR;
}//CreatMaskData

/*****************************************************************
Name:			SetPosition
Inputs:
	Coordinate ScanCenter etc.	-	姿态参数(wld)
Return Value:
	none
Description:	设置当前切片MRI模拟采样姿态参数(wld)
*****************************************************************/
void FrameData::SetPosition(Coordinate ScanCenter, Coordinate RightDir, Coordinate UpDir, Coordinate MoveDir)
{
	m_ScanCenter = ScanCenter;
	m_RightDir = RightDir;
	m_UpDir = UpDir;
	m_MoveDir = MoveDir;
}//SetPosition