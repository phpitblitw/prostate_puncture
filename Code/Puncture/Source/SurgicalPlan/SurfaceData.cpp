#include "stdafx.h"
#include "SurfaceData.h"

using namespace SURGICALPLAN;

SurfaceData::SurfaceData()
{
	m_BoundBox.x1 = m_BoundBox.y1 = m_BoundBox.z1 = 0;
	m_BoundBox.x2 = m_BoundBox.y2 = m_BoundBox.z2 = 200;

	m_pPlanPts[0].x = 0;
	m_pPlanPts[0].y = 100;
	m_pPlanPts[0].z = 0;

	m_pPlanPts[1].x = 200;
	m_pPlanPts[1].y = 100;
	m_pPlanPts[1].z = 0;

	m_pPlanPts[2].x = 200;
	m_pPlanPts[2].y = 100;
	m_pPlanPts[2].z = 200;

	m_pPlanPts[3].x = 0;
	m_pPlanPts[3].y = 100;
	m_pPlanPts[3].z = 200;

}


SurfaceData::~SurfaceData()
{
}

void SurfaceData::GetBoundBox(float &x1, float &x2, float &y1, float &y2, float &z1, float &z2)
{
	x1 = m_BoundBox.x1;
	x2 = m_BoundBox.x2;
	y1 = m_BoundBox.y1;
	y2 = m_BoundBox.y2;
	z1 = m_BoundBox.z1;
	z2 = m_BoundBox.z2;
}

void SurfaceData::SetBoundBox(float x1, float x2, float y1, float y2, float z1, float z2)
{
	m_BoundBox.x1 = x1;
	m_BoundBox.x2 = x2;
	m_BoundBox.y1 = y1;
	m_BoundBox.y2 = y2;
	m_BoundBox.z1 = z1;
	m_BoundBox.z2 = z2;
}

void SurfaceData::SetSamplePlan(Point3D lefttop, Point3D righttop, Point3D leftbottom, Point3D rightbottom)
{
	m_pPlanPts[0].x = rightbottom.x;
	m_pPlanPts[0].y = rightbottom.y;
	m_pPlanPts[0].z = rightbottom.z;

	m_pPlanPts[1].x = righttop.x;
	m_pPlanPts[1].y = righttop.y;
	m_pPlanPts[1].z = righttop.z;

	m_pPlanPts[2].x = lefttop.x;
	m_pPlanPts[2].y = lefttop.y;
	m_pPlanPts[2].z = lefttop.z;

	m_pPlanPts[3].x = leftbottom.x;
	m_pPlanPts[3].y = leftbottom.y;
	m_pPlanPts[3].z = leftbottom.z;
}

void SurfaceData::SetSamplePlan(Coordinate lefttop, Coordinate righttop, Coordinate leftbottom, Coordinate rightbottom)
{
	m_pPlanPts[0].x = rightbottom.x;
	m_pPlanPts[0].y = rightbottom.y;
	m_pPlanPts[0].z = rightbottom.z;

	m_pPlanPts[1].x = righttop.x;
	m_pPlanPts[1].y = righttop.y;
	m_pPlanPts[1].z = righttop.z;

	m_pPlanPts[2].x = lefttop.x;
	m_pPlanPts[2].y = lefttop.y;
	m_pPlanPts[2].z = lefttop.z;

	m_pPlanPts[3].x = leftbottom.x;
	m_pPlanPts[3].y = leftbottom.y;
	m_pPlanPts[3].z = leftbottom.z;
}

Point3D * SurfaceData::GetSamplePlan()
{
	return m_pPlanPts;
}

