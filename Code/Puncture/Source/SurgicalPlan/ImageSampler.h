#pragma once
#include "SurgicalPlan.h"

/*****************************************************************
Defines
*****************************************************************/
#ifdef SURGICALPLAN_EXPORT
#define SURGICALPLAN_API __declspec(dllexport)
#else
#define SURGICALPLAN_API __declspec(dllimport)
#endif

namespace SURGICALPLAN
{
	//按照点和方向对MRI/Mask体数据进行模拟采样，得到采样的面和对应的轮廓线
//********输入采样坐标均为WLD坐标系下，考虑了分辨率，单位为mm，内部转换为IJK坐标系***
	class SURGICALPLAN_API ImageSampler
	{
	public:
		ImageSampler();
		virtual ~ImageSampler();

		BOOL SetDataPackage(SurgicalPlanPtr pPackage);
		BOOL SetPlaneDir(double nx, double ny, double nz);//设置B超平面的法向量（即相机的up方向）
		BOOL SetMoveDir(double dx, double dy, double dz); //设置B超探头运动的方向（即前进at的方向）
		BOOL SetCenterPt(double x, double y, double z); //设置B超探头在体积中的中心位置（即图像bottom边的中心点），单位为mm

		void GetPlaneDir(double &nx, double &ny, double &nz);
		void GetMoveDir(double &dx, double &dy, double &dz);
		void GetCenterPt(double &x, double &y, double &z);

		void GetSampleMaskPlan(MaskDataType *pImage, int cx, int cy, bool type);//获取截面图像 true-冠状面，false矢状面
		void SetImageResolution(double resolution) { m_fImageRes = resolution; }

	private:
		// 函数功能 ：计算任意切片的图像数据
		// 入口参数 ：pImage 存放数据指针，四个顶点在标准图像坐标系下的位置，切片图像的大小
		// 输入的四个点为图像坐标系(IJK)下的坐标
		void GetMaskPlanData(MaskDataType *pImage, Point3D lefttop, Point3D righttop, Point3D leftbottom, Point3D rightbottom, int cx, int cy);
		//根据输入的两个向量和一个中心点，计算裁剪面的四个角点坐标
		void CalculateCoronalMaskPlan();
		void CalculateSagittalMaskPlan();

		Point3D IJKToWLD(Point3D pt, Point3D Res);
		Point3D WLDToIJK(Point3D pt, Point3D Res);

	private:
		SurgicalPlanPtr m_pDataPackage; //指向数据的指针

		Point3D m_UpDir; //B超平面的法向量（即相机的up方向）
		Point3D m_MoveDir; //B超探头运动的方向（即前进at的方向）
		Point3D m_ScanCenter; //B超探头在体积中的中心位置

		Point3D   m_LeftTop, m_RightTop, m_LeftBottom, m_RightBottom;  //本切片数据在体积三维空间的位置

		ImageSize m_iSize; //截取的图像平面尺寸
		double  m_fImageRes; //截取图像的分辨率
	};

	typedef std::shared_ptr<ImageSampler> ImageSamplerPtr;

}
