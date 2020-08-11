#pragma once
#include "StructDef.h"

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
	class SURGICALPLAN_API MaskData
	{
	public:
		MaskData();
		~MaskData();

		int LoadRawMask(CString strFileName, int cx, int cy, int cz); //导入前列腺在MRI中的Mask

		void SetMaskVolumeResolution(double ResX, double ResY, double ResZ);
		void GetMaskVolumeResolution(double &ResX, double &ResY, double &ResZ);

		void GetMaskVolumeSize(int &x, int &y, int &z);
		MaskDataType GetPointValue(double x, double y, double z); //获取某一点的掩膜数据，采用最邻近插值

	protected:
		void InitData();
		void DestroyData();

	protected:
		MaskDataType * m_pMRIMaskData;		//磁共振分割掩膜裸数据
		Point3D m_MaskRes;					//Mask图像的分辨率

		int m_nPixNumberX;					//数据区三坐标尺寸
		int m_nPixNumberY;
		int m_nPixNumberZ;

	};

	typedef std::shared_ptr<MaskData> MaskDataPtr;

}