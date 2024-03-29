#include "stdafx.h"
#include "Attitude.h"

using namespace fsutility;

Attitude::Attitude()
{}

Attitude::~Attitude()
{}

void Attitude::SetValue(Coordinate ScanCenter, Coordinate RightDir, Coordinate UpDir, Coordinate MoveDir)
{
	m_ScanCenter = ScanCenter;
	m_RightDir = RightDir;
	m_UpDir = UpDir;
	m_MoveDir = MoveDir;
}

//变换矩阵mtx作用于本姿态，即TransformMatrix×[SrcScanCenter,SrcRightDir,SrcUpDir,SrcMoveDir]=[ResScanCenter,ResRightDir,ResUpDir,ResMoveDir]
Attitude Attitude::Transform(Matrix mtx)
{
	Attitude result;
	result.m_ScanCenter = mtx.GetMultiply(this->m_ScanCenter);
	result.m_RightDir = mtx.GetMultiply(this->m_RightDir);
	result.m_UpDir = mtx.GetMultiply(this->m_UpDir);
	result.m_MoveDir = mtx.GetMultiply(this->m_MoveDir);
	return result;
}