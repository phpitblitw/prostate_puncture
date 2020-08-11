//*****************************************************************
// 文件名 :						Target.h
// 版本	 :						1.0
// 目的及主要功能 :				靶点目标类
// 创建日期 :					2019.11.1
// 修改日期 :					
// 作者 :						Fiki
// 修改者 :						
// 联系方式 :					fiki@seu.edu.cn
//*****************************************************************/
#pragma once


/*****************************************************************
Project Files Included
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
						Routine Definitions
*****************************************************************/

namespace SURGICALPLAN
{
	//空间点类
	class SURGICALPLAN_API Target
	{
	public:
		Target();
		virtual ~Target();

	public:
		Point3D m_TargetPoint;		//目标点的空间坐标
		Point3D m_StartPoint;		//穿刺起始点的空间坐标
	};

}
