//*****************************************************************
// 文件名 :						AnalyseConfig.h
// 版本	 :						1.0
// 目的及主要功能 :				AnalyseProcess参数设置
// 创建日期 :					2020.4.28
// 修改日期 :					
// 作者 :						SunYizhe
// 修改者 :						
// 联系方式 :					
//*****************************************************************/
#pragma once

/*****************************************************************
Project Files Included
*****************************************************************/
#include "ComUtility/singleton.h"
#include "Coordinate.h"
#include "Matrix.h"

/*****************************************************************
Defines
*****************************************************************/
#ifdef ANALYSEPROCESS_EXPORT
#define ANALYSEPROCESS_API _declspec(dllexport)
#else
#define ANALYSEPROCESS_API _declspec(dllimport)
#endif

/*****************************************************************
						Routine Definitions
*****************************************************************/
namespace ANALYSEPROCESS
{
	class ANALYSEPROCESS_API AnalyseConfig : public fsutility::Singleton<AnalyseConfig>
	{
		friend class fsutility::Singleton<AnalyseConfig>;
	private:
		AnalyseConfig();
		virtual ~AnalyseConfig();
	public:
		int LoadConfigFile(CString t_strFilePathName);

		Coordinate m_ScanCenter;	//最大截面位置的坐标
		Coordinate m_RightDir;		//最大截面位置，2维B超图像的x轴正方向对应的三维空间向量
		Coordinate m_UpDir;			//最大截面位置，2维B超图像的y轴负方向对应的三维空间向量
		Coordinate m_MoveDir;		//B超探头前进的方向，即从直肠入口到最大截面位置的向量
		int m_ResX;		//x方向像素数
		int m_ResY;		//y方向像素数
		int m_ResZ;		//z方向像素数
		float m_SizeX;	//x方向体素大小
		float m_SizeY;	//y方向体素大小
		float m_SizeZ;	//z方向体素大小
	};
}
