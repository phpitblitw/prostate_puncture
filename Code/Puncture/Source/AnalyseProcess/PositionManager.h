//*****************************************************************
// 文件名 :						PositionManager.h
// 版本	 :						1.0
// 目的及主要功能 :				B超/MRI模拟采样之间 坐标转换
// 创建日期 :					2020.05.05
// 修改日期 :					
// 作者 :						SunYizhe
// 修改者 :						
// 联系方式 :					
//*****************************************************************/
#pragma once

/*****************************************************************
Project Files Included
*****************************************************************/
#include "ComUtility/Coordinate.h"
#include "ComUtility/Matrix.h"
#include "NDIOperator/Attitude.h"
#include "AnalyseConfig.h"
#include "ComUtility/Attitude.h"

//using namespace fsutility;

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
	//位置处理类
	class PositionManager;
	typedef std::shared_ptr <PositionManager> PositionManagerPtr;

	class ANALYSEPROCESS_API PositionManager
	{
	public:
		PositionManager();
		~PositionManager();

		BOOL SetBaseUSPosition(fsutility::Attitude BaseUSAttitude);	//设置base处，超声位置参数
		BOOL SetBaseMRIPosition(fsutility::Attitude BaseMRIAttitude);	//设置base处，MRI模拟采样位置参数
		BOOL CalculateTransformMatrix();	//根据base处超声和MRI模拟采样的未知参数，计算转换矩阵
		BOOL SetCurUSPosition(fsutility::Attitude CurUSAttitude);	//设置当前超声位置
		BOOL UpDate();	//更新当前MRI模拟采样位置参数


	public:
		//配准base处,超声位置参数
		//Coordinate m_BaseUSScanCenter;
		//Coordinate m_BaseUSRightDir;
		//Coordinate m_BaseUSUpDir;
		//Coordinate m_BaseUSMoveDir;
		fsutility::Attitude m_BaseUSAttitude;
		//配准base处,MRI模拟采样位置参数(wld坐标)
		//Coordinate m_BaseMRIScanCenter;
		//Coordinate m_BaseMRIRightDir;
		//Coordinate m_BaseMRIUpDir;
		//Coordinate m_BaseMRIMoveDir;
		fsutility::Attitude m_BaseMRIAttitude;
		//实时超声位置参数
		//Coordinate m_CurUSScanCenter;
		//Coordinate m_CurUSRightDir;
		//Coordinate m_CurUSUpDir;
		//Coordinate m_CurUSMoveDir;
		fsutility::Attitude m_CurUSAttitude;
		//实时MRI模拟采样位置参数(wld坐标)
		//Coordinate m_CurMRIScanCenter;
		//Coordinate m_CurMRIRightDir;
		//Coordinate m_CurMRIUpDir;
		//Coordinate m_CurMRIMoveDir;
		fsutility::Attitude m_CurMRIAttitude;
		//超声/MRI模拟采样，位置参数变换矩阵
		//TransformMatrix×[USScanCenter,USRightDir,USUpDir,USMoveDir]=[MRIScanCenter,MRIRightDir,MRIUpDir,MRIMoveDir]
		fsutility::Matrix m_TransformMatrix;
	private:
		//fsutility::Matrix ConstructAttitude2Matrix(NDIOPERATOR::Attitude attitude);	//根据attitude计算4*4变换矩阵
	};
}