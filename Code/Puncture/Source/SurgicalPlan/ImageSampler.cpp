#include "stdafx.h"
#include "ImageSampler.h"
#include "CPubFunc.h"

using namespace SURGICALPLAN;

ImageSampler::ImageSampler()
{
	m_pDataPackage = NULL;

	//m_MoveDir.x = 0.2722;
	//m_MoveDir.y = 0.9526;
	//m_MoveDir.z = 0.1361; //初始值为手动计算的
	m_MoveDir.x = 0;
	m_MoveDir.y = 0;
	m_MoveDir.z = 1; //初始值为手动计算的

	m_UpDir.x = 0;
	m_UpDir.y = -1; 
	m_UpDir.z = 0; //模拟超声平面的法向量，实验阶段初始值设为y轴方向，实际需要改的

	m_ScanCenter.x = 56.25;
	m_ScanCenter.y = 132.5;
	m_ScanCenter.z = 42.35;

	m_fImageRes = 0.4;//mm
}

ImageSampler::~ImageSampler()
{
}

BOOL ImageSampler::SetDataPackage(SurgicalPlanPtr pPackage)
{
	if (pPackage != nullptr)
	{
		m_pDataPackage = pPackage;
		return TRUE;
	}
	else
		return FALSE;
}

void ImageSampler::GetMaskPlanData(MaskDataType *pImage, Point3D lefttop, Point3D righttop, Point3D leftbottom, Point3D rightbottom, int cx, int cy)
{
	double dx = 1.0, dy = 1.0, dz = 1.0;
	double ddx = 1.0, ddy = 1.0, ddz = 1.0;
	double Srcx, Srcy, Srcz;
	int maxx, maxy, maxz;
	int tx, ty, tz;
	MaskDataType Temp = 0;

	m_pDataPackage->GetMaskDataPtr()->GetMaskVolumeSize(maxx, maxy, maxz);

	dx = (righttop.x - lefttop.x) / (cx - 1);
	dy = (righttop.y - lefttop.y) / (cx - 1);
	dz = (righttop.z - lefttop.z) / (cx - 1);

	ddx = (leftbottom.x - lefttop.x) / (cy - 1);
	ddy = (leftbottom.y - lefttop.y) / (cy - 1);
	ddz = (leftbottom.z - lefttop.z) / (cy - 1);

	for (int y = 0; y < cy; y++)
	{
		Srcx = (double)(lefttop.x + y * ddx);  //起始点
		Srcy = (double)(lefttop.y + y * ddy);
		Srcz = (double)(lefttop.z + y * ddz);
		for (int x = 0; x < cx; x++)
		{
			tx = CPubFunc::Round(Srcx);
			ty = CPubFunc::Round(Srcy);
			tz = CPubFunc::Round(Srcz);
			Temp = 0;
			if ((tx >= 0) && (ty >= 0) && (tz >= 0) && (tx < maxx) && (ty < maxy) && (tz < maxz))  //在体积空间内
				Temp = m_pDataPackage->GetMaskDataPtr()->GetPointValue(Srcx, Srcy, Srcz);	//采用了最邻近插值
			else
				Temp = 0;
			*(pImage + x) = Temp;
			Srcx += dx;
			Srcy += dy;
			Srcz += dz;
		}
		pImage += cx;
	}

}
//设置B超平面的法向量（即相机的up方向）
BOOL ImageSampler::SetPlaneDir(double nx, double ny, double nz)
{
	m_UpDir.x = nx;
	m_UpDir.y = ny;
	m_UpDir.z = nz;
	CPubFunc::VectorNormalize(&m_UpDir);
	return TRUE;
}
void ImageSampler::GetPlaneDir(double &nx, double &ny, double &nz)
{
	nx = m_UpDir.x;
	ny = m_UpDir.y;
	nz = m_UpDir.z;
}
 //设置B超探头运动的方向（即前进at的方向）
BOOL ImageSampler::SetMoveDir(double dx, double dy, double dz)
{
	m_MoveDir.x = dx;
	m_MoveDir.y = dy;
	m_MoveDir.z = dz;
	CPubFunc::VectorNormalize(&m_UpDir);
	return TRUE;
}
void ImageSampler::GetMoveDir(double &dx, double &dy, double &dz)
{
	dx = m_MoveDir.x;
	dy = m_MoveDir.y;
	dz = m_MoveDir.z;
}
//设置B超探头在体积中的中心位置（即图像bottom边的中心点）
BOOL ImageSampler::SetCenterPt(double x, double y, double z)
{
	m_ScanCenter.x = x;
	m_ScanCenter.y = y;
	m_ScanCenter.z = z;
	return TRUE;
}

void ImageSampler::GetCenterPt(double &x, double &y, double &z)
{
	x = m_ScanCenter.x;
	y = m_ScanCenter.y;
	z = m_ScanCenter.z;
}


void ImageSampler::CalculateCoronalMaskPlan()
{
	Point3D xDir = CPubFunc::VectorCross(m_MoveDir, m_UpDir);
	Point3D yDir = CPubFunc::VectorCross(xDir, m_MoveDir);
	CPubFunc::VectorNormalize(&xDir);
	CPubFunc::VectorNormalize(&yDir);
	double dTrueHalfCx = m_iSize.cx*m_fImageRes / 2;
	double dTrueCy = m_iSize.cy*m_fImageRes;

	m_LeftBottom.x = m_ScanCenter.x - dTrueHalfCx * xDir.x;
	m_LeftBottom.y = m_ScanCenter.y - dTrueHalfCx * xDir.y;
	m_LeftBottom.z = m_ScanCenter.z - dTrueHalfCx * xDir.z;
	m_RightBottom.x = m_ScanCenter.x + dTrueHalfCx * xDir.x;
	m_RightBottom.y = m_ScanCenter.y + dTrueHalfCx * xDir.y;
	m_RightBottom.z = m_ScanCenter.z + dTrueHalfCx * xDir.z;
	m_LeftTop.x = m_LeftBottom.x + dTrueCy * yDir.x;
	m_LeftTop.y = m_LeftBottom.y + dTrueCy * yDir.y;
	m_LeftTop.z = m_LeftBottom.z + dTrueCy * yDir.z;
	m_RightTop.x = m_RightBottom.x + dTrueCy * yDir.x;
	m_RightTop.y = m_RightBottom.y + dTrueCy * yDir.y;
	m_RightTop.z = m_RightBottom.z + dTrueCy * yDir.z;

	//设置对应三维空间的平面
	m_pDataPackage->GetSurfaceDataPtr()->SetSamplePlan(m_LeftTop, m_RightTop, m_LeftBottom, m_RightBottom);
}

void ImageSampler::CalculateSagittalMaskPlan()
{
	double TrueHalfcx = m_iSize.cx*m_fImageRes / 2.0;
	m_LeftBottom.x = m_ScanCenter.x - TrueHalfcx * m_MoveDir.x;
	m_LeftBottom.y = m_ScanCenter.y - TrueHalfcx * m_MoveDir.y;
	m_LeftBottom.z = m_ScanCenter.z - TrueHalfcx * m_MoveDir.z;

	m_RightBottom.x = m_ScanCenter.x + TrueHalfcx * m_MoveDir.x;
	m_RightBottom.y = m_ScanCenter.y + TrueHalfcx * m_MoveDir.y;
	m_RightBottom.z = m_ScanCenter.z + TrueHalfcx * m_MoveDir.z;

	Point3D yDir = CPubFunc::VectorCross(m_MoveDir, m_UpDir);
	CPubFunc::VectorNormalize(&yDir);

	double fTrueSizeY = m_iSize.cy * m_fImageRes;
	m_LeftTop.x = m_LeftBottom.x + fTrueSizeY * yDir.x;
	m_LeftTop.y = m_LeftBottom.y + fTrueSizeY * yDir.y;
	m_LeftTop.z = m_LeftBottom.z + fTrueSizeY * yDir.z;

	m_RightTop.x = m_RightBottom.x + fTrueSizeY * yDir.x;
	m_RightTop.y = m_RightBottom.y + fTrueSizeY * yDir.y;
	m_RightTop.z = m_RightBottom.z + fTrueSizeY * yDir.z;

	//设置对应三维空间的平面
	m_pDataPackage->GetSurfaceDataPtr()->SetSamplePlan(m_LeftTop, m_RightTop, m_LeftBottom, m_RightBottom);
}


void ImageSampler::GetSampleMaskPlan(MaskDataType *pImage, int cx, int cy, bool type)
{
	m_iSize.cx = cx;
	m_iSize.cy = cy;

	//CalculateSampleMaskPlan();
	if (type)
		CalculateCoronalMaskPlan();//计算冠状面角点坐标
	else
		CalculateSagittalMaskPlan();//计算矢状面角点坐标
	Point3D lefttop, righttop, leftbottom, rightbottom;
	Point3D Res;
	m_pDataPackage->GetMaskDataPtr()->GetMaskVolumeResolution(Res.x, Res.y, Res.z);
	lefttop = WLDToIJK(m_LeftTop, Res);
	leftbottom = WLDToIJK(m_LeftBottom, Res);
	righttop = WLDToIJK(m_RightTop, Res);
	rightbottom = WLDToIJK(m_RightBottom, Res);

	GetMaskPlanData(pImage, lefttop, righttop, leftbottom, rightbottom, cx, cy);
}

Point3D ImageSampler::WLDToIJK(Point3D pt, Point3D Res)
{
	Point3D newpt;
	newpt.x = pt.x / Res.x;
	newpt.y = pt.y / Res.y;
	newpt.z = pt.z / Res.z;
	return newpt;
}

Point3D ImageSampler::IJKToWLD(Point3D pt, Point3D Res)
{
	Point3D newpt;
	newpt.x = pt.x * Res.x;
	newpt.y = pt.y * Res.y;
	newpt.z = pt.z * Res.z;
	return newpt;
}

