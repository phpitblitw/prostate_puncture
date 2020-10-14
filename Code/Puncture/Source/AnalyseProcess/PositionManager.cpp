//*****************************************************************
// 文件名 :						AnalyseConfig.cpp
// 版本	 :						1.0
// 目的及主要功能 :				AnalyseProcess参数设置
// 创建日期 :					2020.4.28
// 修改日期 :					
// 作者 :						SunYizhe
// 修改者 :						
// 联系方式 :					
//*****************************************************************/

///////////////////////////////////////////////////////
////////////////////////include////////////////////////
///////////////////////////////////////////////////////
#include "pch.h"
#include "PositionManager.h"
#include "ErrorManager/ErrorCodeDefine.h"

using namespace ANALYSEPROCESS;
using namespace fsutility;

/*****************************************************************
Name:			PositionManager
Inputs:
	none
Return Value:
	none
Description:	PositionManager构造函数
*****************************************************************/
PositionManager::PositionManager()
{
	//TODO
}

/*****************************************************************
Name:			PositionManager
Inputs:
	none
Return Value:
	none
Description:	PositionManager析构函数
*****************************************************************/
PositionManager::~PositionManager()
{
}

/*****************************************************************
Name:			SetBaseUSPosition
Inputs:
	fsutility Attitude BaseUSAttitude - 存储超声base处位置
Return Value:
	 int - Error Info
Description:	根据NDI设备传来的attitude，设置base处，超声探头位置参数
*****************************************************************/
BOOL PositionManager::SetBaseUSPosition(fsutility::Attitude BaseUSAttitude)
{
	m_BaseUSAttitude = BaseUSAttitude;
	return LIST_NO_ERROR;
}


/*****************************************************************
Name:			SetBaseMRIPosition
Inputs:	
	AnalyseConfig config - Config对象，存储配置文件信息
Return Value:
	int - Error Info
Description:	根据config文件，设置base处，MRI模拟采样位置参数
*****************************************************************/
BOOL PositionManager::SetBaseMRIPosition(fsutility::Attitude BaseMRIAttitude)
{
	m_BaseMRIAttitude = BaseMRIAttitude;
	return LIST_NO_ERROR;
}

/*****************************************************************
Name:			CalculateTransformMatrix
Inputs:
	None
Return Value:
	int - Error Info
Description:	
	US的一组位置参数，经过变换矩阵运算，即可得到MRI模拟采样的一组位置参数
	TransformMatrix×[USScanCenter,USRightDir,USUpDir,USMoveDir]=[MRIScanCenter,MRIRightDir,MRIUpDir,MRIMoveDir]
	根据base处，US和MRI模拟采样的一组位置参数，即可求得该变换矩阵
	若变换矩阵不存在，则返回错误信息
*****************************************************************/
BOOL PositionManager::CalculateTransformMatrix()
{
	//Matrix matrixMRIBasePosition(m_BaseMRIScanCenter, m_BaseMRIRightDir, m_BaseMRIUpDir, m_BaseMRIMoveDir);
	//Matrix matrixUSBasePosition(m_BaseUSScanCenter, m_BaseUSRightDir, m_BaseUSUpDir, m_BaseUSMoveDir);

	Matrix matrixMRIBasePosition(m_BaseMRIAttitude.m_ScanCenter, m_BaseMRIAttitude.m_RightDir, m_BaseMRIAttitude.m_UpDir, m_BaseMRIAttitude.m_MoveDir);
	Matrix matrixUSBasePosition(m_BaseUSAttitude.m_ScanCenter, m_BaseUSAttitude.m_RightDir, m_BaseUSAttitude.m_UpDir, m_BaseUSAttitude.m_MoveDir);
	//首先判断转换矩阵是否可逆
	if (matrixUSBasePosition.GetDeterminant() == 0)
	{
		return ER_CalculateTransformMatrix;
	}

	m_TransformMatrix = matrixMRIBasePosition.GetMultiply(matrixUSBasePosition.GetInverse());
	return LIST_NO_ERROR;
}

/*****************************************************************
Name:			SetCurUSPosition
Inputs:
	Attitude aBaseUS - 存储超声base处位置
Return Value:
	 int - Error Info
Description:	根据NDI设备传来的attitude，设置当前 超声探头姿态参数
*****************************************************************/
BOOL PositionManager::SetCurUSPosition(fsutility::Attitude CurUSAttitude)
{
	m_CurUSAttitude = CurUSAttitude;
	return LIST_NO_ERROR;
}

/*****************************************************************
Name:			UpDate
Inputs:
	None
Return Value:
	int - Error Info
Description:	根据当前的US未知参数，以及变换矩阵，求取当前MRI模拟采样位置参数
*****************************************************************/
BOOL PositionManager::UpDate()
{
	//m_CurMRIScanCenter = m_TransformMatrix.GetMultiply(m_CurUSScanCenter);
	//m_CurMRIRightDir = m_TransformMatrix.GetMultiply(m_CurUSRightDir);
	//m_CurMRIUpDir = m_TransformMatrix.GetMultiply(m_CurUSUpDir);
	//m_CurMRIMoveDir = m_TransformMatrix.GetMultiply(m_CurUSMoveDir);
	m_CurMRIAttitude=m_CurUSAttitude.Transform(m_TransformMatrix);
	return LIST_NO_ERROR;
}

/*****************************************************************
Name:			ConstructAttitude2Matrix
Inputs:
	Attitude attitude
Return Value:
	 Matrix - 变换矩阵
Description:	由Attitude构造变换矩阵
*****************************************************************/
//Matrix PositionManager::ConstructAttitude2Matrix(NDIOPERATOR::Attitude attitude)
//{
//	double m[16];
//	float x, y, z, roll, pitch, yaw;
//	attitude.GetPosition(x, y, z, roll, pitch, yaw);
//	m[0] = cos(yaw) * cos(pitch);
//	m[1] = cos(yaw) * sin(pitch) * sin(roll) - sin(yaw) * cos(roll);
//	m[2] = cos(yaw) * sin(pitch) * cos(roll) + sin(yaw) * sin(roll);
//	m[3] = x;
//	m[4] = sin(yaw) * cos(pitch);
//	m[5] = sin(yaw) * sin(pitch)*sin(roll) + cos(yaw) * cos(roll);
//	m[6] = sin(yaw) * sin(pitch)*cos(roll) - cos(yaw) * sin(roll);
//	m[7] = y;
//	m[8] = -sin(pitch);
//	m[9] = cos(pitch) * sin(roll);
//	m[10] = cos(pitch) * cos(roll);
//	m[11] = z;
//	m[12] = 0;
//	m[13] = 0;
//	m[14] = 0;
//	m[15] = 1;
//	return Matrix(m);
//}