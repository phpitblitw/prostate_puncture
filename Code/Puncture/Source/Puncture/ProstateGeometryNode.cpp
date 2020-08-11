#include "stdafx.h"
#include "ProstateGeometryNode.h"


ProstateGeometryNode::ProstateGeometryNode()
{
	m_pSurfaceData = nullptr;
}


ProstateGeometryNode::~ProstateGeometryNode()
{
}

void ProstateGeometryNode::SetSurfaceData(SURGICALPLAN::SurfaceDataPtr pSurfaceData)
{
	m_pSurfaceData = pSurfaceData;
}

void ProstateGeometryNode::DrawObject()
{
	DrawSamplePlan();
	DrawBoundBox();
}

void ProstateGeometryNode::DrawBoundBox()
{
	if (m_pSurfaceData != nullptr)
	{
		float left, right, top, bottom, nearZ, farZ;
		m_pSurfaceData->GetBoundBox(left, right, bottom, top, farZ, nearZ);

		glPushAttrib(GL_LIGHTING_BIT | GL_LINE_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glLineWidth(1.0);

		glColor3f(1.0, 0.0, 0.0);

		glBegin(GL_LINES);
		glVertex3f(left, top, nearZ);
		glVertex3f(left, bottom, nearZ);

		glVertex3f(right, top, nearZ);
		glVertex3f(right, bottom, nearZ);

		glVertex3f(left, top, farZ);
		glVertex3f(left, bottom, farZ);

		glVertex3f(right, top, farZ);
		glVertex3f(right, bottom, farZ);

		glVertex3f(left, top, nearZ);
		glVertex3f(left, top, farZ);

		glVertex3f(left, top, farZ);
		glVertex3f(right, top, farZ);

		glVertex3f(right, top, farZ);
		glVertex3f(right, top, nearZ);

		glVertex3f(right, top, nearZ);
		glVertex3f(left, top, nearZ);

		glVertex3f(left, bottom, nearZ);
		glVertex3f(left, bottom, farZ);

		glVertex3f(left, bottom, farZ);
		glVertex3f(right, bottom, farZ);

		glVertex3f(right, bottom, farZ);
		glVertex3f(right, bottom, nearZ);

		glVertex3f(right, bottom, nearZ);
		glVertex3f(left, bottom, nearZ);

		glEnd();
		glPopAttrib();

	}

}

void ProstateGeometryNode::DrawSamplePlan()
{
	if (m_pSurfaceData != nullptr)
	{
		SURGICALPLAN::Point3D *pPlanPoint = NULL;
		pPlanPoint = m_pSurfaceData->GetSamplePlan();

		glColor3f(0.5f, 0.5f, 0.5f);
		glBegin(GL_QUADS);
		for (int i = 0; i < 4; i++)
			glVertex3f((float)pPlanPoint[i].x, (float)pPlanPoint[i].y, (float)pPlanPoint[i].z);
		glEnd();
	}

}

