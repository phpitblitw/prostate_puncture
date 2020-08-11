//*****************************************************************
// 文件名 :						ImageSampler.cpp
// 版本	 :						1.0
// 目的及主要功能 :				求3D数据的2维截面
// 创建日期 :					
// 修改日期 :					2020.05.08
// 作者 :						Tanghui
// 修改者 :						SunYizhe
// 联系方式 :					
//*****************************************************************/

///////////////////////////////////////////////////////
////////////////////////include////////////////////////
///////////////////////////////////////////////////////
#include "pch.h"
#include "AnalyseProcess/ImageSampler.h"
#include "ErrorManager/ErrorCodeDefine.h"
#include "SurgicalPlan/CPubFunc.h"
//#include "ImageSampler.h"
//#include "CPubFunc.h"

using namespace ANALYSEPROCESS;

/*****************************************************************
Name:			ImageSampler
Inputs:
	none
Return Value:
	none
Description:	ImageSampler构造函数
*****************************************************************/
ANALYSEPROCESS::ImageSampler::ImageSampler()
{
	m_pDataPackage = NULL;
	m_dUSPixelSize = 0;
	m_nResolution.cx = 0;
	m_nResolution.cy = 0;
	m_dMRIPixelSizeX = 0;
	m_dMRIPixelSizeY = 0;
	m_dMRIPixelSizeZ = 0;
}


/*****************************************************************
Name:			~ImageSampler
Inputs:
	none
Return Value:
	none
Description:	ImageSampler析构函数
*****************************************************************/
ANALYSEPROCESS::ImageSampler::~ImageSampler()
{

}


/*****************************************************************
Name:			SetDataPackage
Inputs:
	SurgicalPlanPtr pPackage - 手术计划指针
Return Value:
	none
Description:	设置手术计划指针
*****************************************************************/
int ANALYSEPROCESS::ImageSampler::SetDataPackage(SurgicalPlanPtr pPackage)
{
	if (pPackage != nullptr)
	{
		m_pDataPackage = pPackage;
		return LIST_NO_ERROR;
	}
	else
		return ER_NoSurgicalPlan;
}


/*****************************************************************
Name:			SetPosition
Inputs:
	Coordinate xxx - 指示当前切片在三维空间姿态参数的一组齐次坐标(WLD)
Return Value:
	none
Description:	设置当前切片在三维空间的姿态参数
*****************************************************************/
void ANALYSEPROCESS::ImageSampler::SetPosition(Coordinate ScanCenter, Coordinate RightDir, Coordinate UpDir, Coordinate MoveDir)
{
	m_ScanCenter = ScanCenter;
	m_RightDir = RightDir;
	m_UpDir = UpDir;
	m_MoveDir = MoveDir;
	return;
}


/*****************************************************************
Name:			SetUSPixelSize
Inputs:
	double dPixelSize - B超图像像素大小(像素对应的物理尺寸)
Return Value:
	none
Description:	设置B超图像像素大小(像素对应的物理尺寸)
*****************************************************************/
void ANALYSEPROCESS::ImageSampler::SetUSPixelSize(double dPixelSize)
{
	m_dUSPixelSize = dPixelSize;
	return;
}


/*****************************************************************
Name:			SetImageSize
Inputs:
	int cx - B超图像宽度(像素数)
	int cy - B超图像高度(像素数)
Return Value:
	none
Description:	设置B超图像分辨率(像素数)
*****************************************************************/
void ANALYSEPROCESS::ImageSampler::SetImageSize(int cx, int cy)
{
	m_nResolution.cx = cx;
	m_nResolution.cy = cy;
	return;
}


/*****************************************************************
Name:			SetMRIPixelSize
Inputs:
	double dPixelSizeX - MRI体素大小(x方向)
	double dPixelSizeY - MRI体素大小(y方向)
	double dPixelSizeZ - MRI体素大小(z方向)
Return Value:
	none
Description:	设置MRI数据体素大小
*****************************************************************/
void ANALYSEPROCESS::ImageSampler::SetMRIPixelSize(double dPixelSizeX, double dPixelSizeY, double dPixelSizeZ)
{
	m_dMRIPixelSizeX = dPixelSizeX;
	m_dMRIPixelSizeY = dPixelSizeY;
	m_dMRIPixelSizeZ = dPixelSizeZ;
	return;
}


/*****************************************************************
Name:			GetSampleMaskPlan
Inputs:
	MaskDataType* pImage - 结果mask数据指针
	int nScanType - B超扫描类型，0-冠状面，1-矢状面
	int nMaskType - MRI模拟采样mask类型，0-前列腺，1-病灶
Return Value:
	int - error info
Description:	设置MRI数据体素大小
*****************************************************************/
int ANALYSEPROCESS::ImageSampler::GetSampleMaskPlan(MaskDataType *pImage, int nScanType, int nMaskType)
{
	//计算B超切面的四个角点，对应的3D MRI数据的WLD坐标
	if (nScanType==0)
		CalculateCoronalMaskPlan();//计算冠状面四个角点位置
	else if (nScanType == 1)
		CalculateSagittalMaskPlan();//计算矢状面四个角点位置
	else
		return ER_SampleParameterError;

	//将四个角点的WLD坐标转为IJK坐标
	m_LeftTop = WLDToIJK(m_LeftTop);
	m_RightTop = WLDToIJK(m_RightTop);
	m_LeftBottom = WLDToIJK(m_LeftBottom);
	m_RightBottom = WLDToIJK(m_RightBottom);

	//使用四个角点的IJK坐标，指导切割二维截面
	if (nMaskType != 0 && nMaskType != 1)
		return ER_SampleParameterError;
	GetMaskPlanData(m_pDataPackage->GetMaskDataPtr(nMaskType), pImage);
	//if (nMaskType == 0)
	//	//暂时设置为SurgicalPlan的MaskDataptr，应具体指定为ProstateMaskPtr TODO
	//	GetMaskPlanData(m_pDataPackage->GetMaskDataPtr(), pImage);
	//else if (nMaskType == 1)
	//	//应指定为lesionMaskPtr TODO
	//	GetMaskPlanData(m_pDataPackage->GetMaskDataPtr(), pImage);
	//else
	//	return ER_SampleParameterError;
	return LIST_NO_ERROR;
}

/*****************************************************************
Name:			IJKToWLD
Inputs:
	Coordinate pt - IJK坐标
Return Value:
	none
Description:	将IJK坐标转换至WLD坐标
*****************************************************************/
Coordinate ANALYSEPROCESS::ImageSampler::IJKToWLD(Coordinate pt)
{
	Coordinate ptRes;
	ptRes.x = pt.x*m_dMRIPixelSizeX;
	ptRes.y = pt.y*m_dMRIPixelSizeY;
	ptRes.z = pt.z*m_dMRIPixelSizeZ;
	ptRes.w = pt.w;
	return ptRes;
}


/*****************************************************************
Name:			WLDToIJK
Inputs:
	Coordinate pt - WLD坐标
Return Value:
	none
Description:	将WLD坐标转换至IJK坐标
*****************************************************************/
Coordinate ANALYSEPROCESS::ImageSampler::WLDToIJK(Coordinate pt)
{
	Coordinate ptRes;
	ptRes.x = pt.x/m_dMRIPixelSizeX;
	ptRes.y = pt.y/m_dMRIPixelSizeY;
	ptRes.z = pt.z/m_dMRIPixelSizeZ;
	ptRes.w = pt.w;
	return ptRes;
}


/*****************************************************************
Name:			CalculateCoronalMaskPlan
Inputs:
	none
Return Value:
	none
Description:	计算B超冠状面图像四个角点的WLD坐标
*****************************************************************/
void ANALYSEPROCESS::ImageSampler::CalculateCoronalMaskPlan()
{
	double dTrueHalfCx = m_nResolution.cx*m_dUSPixelSize / 2.0;	//超声图像宽度的一半(实际物理尺寸/mm)
	double dTrueCy = m_nResolution.cy*m_dUSPixelSize;			//超声图像高度(实际物理尺寸/mm)

	m_LeftBottom = m_ScanCenter - m_RightDir * dTrueHalfCx;
	m_RightBottom = m_ScanCenter + m_RightDir * dTrueHalfCx;
	m_LeftTop = m_LeftBottom + m_UpDir * dTrueCy;
	m_RightTop = m_RightBottom + m_UpDir * dTrueCy;
	return;
}


/*****************************************************************
Name:			CalculateCoronalMaskPlan
Inputs:
	none
Return Value:
	none
Description:	计算B超矢状面图像四个角点的WLD坐标
*****************************************************************/
void ANALYSEPROCESS::ImageSampler::CalculateSagittalMaskPlan()
{
	double dTrueHalfCx = m_nResolution.cx*m_dUSPixelSize / 2.0;	//超声图像宽度的一半(实际物理尺寸)
	double dTrueCy = m_nResolution.cy*m_dUSPixelSize;			//超声图像高度(实际物理尺寸)

	m_LeftBottom = m_ScanCenter - m_MoveDir * dTrueHalfCx;
	m_RightBottom = m_ScanCenter + m_MoveDir * dTrueHalfCx;
	m_LeftTop = m_LeftBottom + m_UpDir * dTrueCy;
	m_RightTop = m_RightBottom + m_UpDir * dTrueCy;
	return;
}


/*****************************************************************
Name:			GetMaskPlanData
Inputs:
	none
Return Value:
	none
Description:	计算MASK切片数据,由pImage指针带出
*****************************************************************/
void ANALYSEPROCESS::ImageSampler::GetMaskPlanData(MaskDataPtr tMaskDataPtr, MaskDataType *pImage)
{
	int x, y;	//B超图像像素坐标
	int tx, ty, tz;	//3D MRI模拟采样ITK坐标
	MaskDataType nCur;		//B超待处理的当前点 灰度值
	Coordinate dxUS, dyUS;	//2维B超图像，x或y方向上一个像素，对应到3维MRI模拟采样的IJK向量
	Coordinate pCur;		//B超待处理的当前点 对应到3维MRI模拟采样的IJK坐标
	int nMaskResX, nMaskResY, nMaskResZ;	//3维B超MASK，x,y,z方向上的分辨率(像素数)

	tMaskDataPtr->GetMaskVolumeSize(nMaskResX, nMaskResY, nMaskResZ);
	dxUS = (m_RightBottom - m_LeftBottom)*(1.0/(m_nResolution.cx - 1));
	dyUS = (m_LeftBottom - m_LeftTop)*(1.0/(m_nResolution.cy - 1));
	dxUS.w = 0;	//向量
	dyUS.w = 0;	//向量
	for (y = 0; y < m_nResolution.cy; y++)
	{
		pCur = m_LeftTop + dyUS * y;	//设定每一行的起始点(二维图像，其坐标点(0,0)处对应左上角))
		for (x = 0; x < m_nResolution.cx; x++)
		{
			tx = CPubFunc::Round(pCur.x);
			ty = CPubFunc::Round(pCur.y);
			tz = CPubFunc::Round(pCur.z);
			if ((tx >= 0) && (ty >= 0) && (tz >= 0) && (tx < nMaskResX) && (ty < nMaskResY) && (tz < nMaskResZ))	//判断是否在体积空间内
				nCur = tMaskDataPtr->GetPointValue(tx, ty, tz);	//使用最近邻插值
			else
				nCur = 0;
			*(pImage + x) = nCur;
			pCur = pCur + dxUS;
		}
		pImage += m_nResolution.cx;
	}
	return;
}