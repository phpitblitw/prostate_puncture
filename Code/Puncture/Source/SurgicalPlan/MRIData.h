//*****************************************************************
// 文件名 :						MRIData.h
// 版本	 :						1.0
// 目的及主要功能 :				磁共振数据管理模块
// 创建日期 :					2019.11.1
// 修改日期 :					
// 作者 :						Fiki
// 修改者 :						
// 联系方式 :					
//*****************************************************************/
#pragma once

/*****************************************************************
Library Files Included
*****************************************************************/
#include "StructDef.h"

/*****************************************************************
Defines
*****************************************************************/
#ifdef SURGICALPLAN_EXPORT
#define SURGICALPLAN_API __declspec(dllexport)
#else
#define SURGICALPLAN_API __declspec(dllimport)
#endif

/*****************************************************************
Defines
*****************************************************************/


/*****************************************************************
						Routine Definitions
*****************************************************************/
namespace SURGICALPLAN
{
	class SURGICALPLAN_API MRIData
	{
	public:
		MRIData();
		virtual ~MRIData();

	public:
		int LoadRawMRI(CString t_strFileName, int cx, int cy, int cz);		//导入数据
		void SetMRIVolumeResolution(double ResX, double ResY, double ResZ);
		void GetMRIVolumeResolution(double &ResX, double &ResY, double &ResZ);

		void GetMRIVolumeSize(int &x, int &y, int &z);
		MRIDataType GetPointValue(double x, double y, double z); //获取某一点的MRI数据，采用最邻近插值
		
	protected:
		void InitData();
		void DestroyData();

	private:
		MRIDataType * m_pMRIData;			//磁共振裸数据
		Point3D m_MRIRes;					//MRI图像的分辨率

		int m_nPixNumberX;					//数据区三坐标尺寸
		int m_nPixNumberY;
		int m_nPixNumberZ;
	};

	typedef std::shared_ptr<MRIData> MRIDataPtr;

}
