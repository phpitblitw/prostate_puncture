#pragma once
//探头(mri模拟采样探头/实际超声探头)的空间姿态，即位置+方向
//ScanCenter	探头中心点
//RightDir		2维B超图像的x轴正方向对应的三维空间向量
//UpDir			2维B超图像的y轴负方向对应的三维空间向量
//MoveDir		B超探头指向的方向

#include "Coordinate.h"
#include "Matrix.h"

/*****************************************************************
Defines
*****************************************************************/

namespace fsutility
{
	class AFX_EXT_CLASS Attitude
	{
	public:
		Attitude();
		~Attitude();
		void SetValue(Coordinate ScanCenter, Coordinate RightDir, Coordinate UpDir, Coordinate MoveDir);
		Attitude Transform(Matrix mtx);		//表换矩阵mtx作用于本姿态，即TransformMatrix×[SrcScanCenter,SrcRightDir,SrcUpDir,SrcMoveDir]=[ResScanCenter,ResRightDir,ResUpDir,ResMoveDir]
	
	public:
		Coordinate m_ScanCenter;
		Coordinate m_RightDir;
		Coordinate m_UpDir;
		Coordinate m_MoveDir;
	};
}