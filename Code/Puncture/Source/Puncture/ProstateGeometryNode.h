#pragma once
#include "SceneGraph/SceneObjectNode.h"
#include "SurgicalPlan/SurfaceData.h"

class ProstateGeometryNode:public SceneObjectNode
{
public:
	ProstateGeometryNode();
	virtual ~ProstateGeometryNode();

	virtual void DrawObject();

	void SetSurfaceData(SURGICALPLAN::SurfaceDataPtr pSurfaceData);

protected:
	void DrawBoundBox();
	void DrawSamplePlan();

protected:
	SURGICALPLAN::SurfaceDataPtr m_pSurfaceData;

};

