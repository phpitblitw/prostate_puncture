//*****************************************************************
// 文件名 :						ImageSampler.h
// 版本	 :						1.0
// 目的及主要功能 :				求3D数据的2维截面
// 创建日期 :					
// 修改日期 :					2020.05.08
// 作者 :						Tanghui
// 修改者 :						SunYizhe
// 联系方式 :					
//*****************************************************************/
#pragma once

/*****************************************************************
Library Files Included
*****************************************************************/
//#include "SurgicalPlan.h"
#include "ComUtility/Coordinate.h"
#include "SurgicalPlan/SurgicalPlan.h"

using namespace SURGICALPLAN;
using namespace fsutility;

/*****************************************************************
Defines
*****************************************************************/
#ifdef ANALYSEPROCESS_EXPORT
#define ANALYSEPROCESS_API __declspec(dllexport)
#else
#define ANALYSEPROCESS_API __declspec(dllimport)
#endif

/*****************************************************************
						Routine Definitions
定义截取平面ImageSampler类
	i.	根据MRI模拟采样的位置参数，裁剪MRI原始数据的2维截面(冠状面/矢状面)
	ii.	根据MRI模拟采样的位置参数，裁剪前列腺mask/病灶mask的2维截面(冠状面/矢状面)
*****************************************************************/

namespace ANALYSEPROCESS
{
	//按照点和方向对MRI/Mask体数据进行模拟采样，得到采样的面和对应的轮廓线
//********输入采样坐标均为WLD坐标系下，考虑了分辨率，单位为mm，内部转换为IJK坐标系***
	class ImageSampler;
	typedef std::shared_ptr<ImageSampler> ImageSamplerPtr;
	class ANALYSEPROCESS_API ImageSampler
	{
	public:
		ImageSampler();
		virtual ~ImageSampler();

		int SetDataPackage(SurgicalPlanPtr pPackage);
		void SetPosition(Coordinate ScanCenter, Coordinate RightDir, Coordinate UpDir, Coordinate MoveDir);		//设置当前切片位置参数 WLD坐标
		void SetUSPixelSize(double dPixelSize);		//设置B超图像像素大小(像素对应的物理尺寸)
		void SetImageSize(int cx, int cy);			//设置B超图像分辨率(像素数)
		void SetMRIPixelSize(double dPixelSizeX, double dPixelSizeY, double dPixelSizeZ);	//设置MRI数据体素大小
		//void GetSampleOriginPlan(); //TODO
		int GetSampleMaskPlan(MaskDataType *pImage, int nScanType, int nMaskType);	//截取MASK截面，nScanType对应扫描类型，0-冠状面，1-矢状面。nMaskType对应mask类型，0-前列腺，1-病灶
		void GetPlaneCorners(Coordinate& leftTop, Coordinate& rightTop, Coordinate& leftBottom, Coordinate& rightBottom);	//获取本切片四个角点 在MRI模拟采样3D空间的坐标 (wld)

		Coordinate IJKToWLD(Coordinate pt);
		Coordinate WLDToIJK(Coordinate pt);
		
	private:
		void CalculateCoronalMaskPlan();	//计算B超冠状面图像四个角点的WLD坐标
		void CalculateSagittalMaskPlan();	//计算B超矢状面图像四个角点的WLD坐标
		void GetMRIPlaneData(MRIDataType tMRIDataPtr, MaskDataType *pImage);			//计算原始MRI切片数据
		void GetMaskPlanData(MaskDataPtr tMaskDataPtr, MaskDataType *pImage);		//计算MASK切片数据

	private:
		SurgicalPlanPtr m_pDataPackage; //指向数据的指针

		//本切片在MRI模拟采样3D空间中的姿态参数(wld)
		Coordinate m_ScanCenter;		//B超探头位置坐标
		Coordinate m_RightDir;			//2维B超图像的x轴正方向对应的三维空间向量
		Coordinate m_UpDir;				//2维B超图像的y轴负方向对应的三维空间向量
		Coordinate m_MoveDir;			//B超探头前进的方向，即从直肠入口到最大截面位置的向量

		//本切片四个角点 在MRI模拟采样3D空间的坐标 (wld)
		Coordinate m_LeftTop;
		Coordinate m_RightTop;
		Coordinate m_LeftBottom;
		Coordinate m_RightBottom;

		//本切片四个角点 在MRI模拟采样3D空间的坐标 (ijk)
		Coordinate m_LeftTopIJK;
		Coordinate m_RightTopIJK;
		Coordinate m_LeftBottomIJK;
		Coordinate m_RightBottomIJK;

		double m_dUSPixelSize;			//B超图像中，1个像素对应的实际物理尺寸(mm)
		ImageSize m_nResolution;		//B超图像尺寸(像素数)

		//MRI数据中 x,y,z方向上的体素大小(mm)
		double m_dMRIPixelSizeX;
		double m_dMRIPixelSizeY;
		double m_dMRIPixelSizeZ;

	};

	typedef std::shared_ptr<ImageSampler> ImageSamplerPtr;

}
