#pragma once
#include "sceneobjectnode.h"

class AFX_EXT_CLASS PathNode :public SceneObjectNode
{
public:
	PathNode(void);
	virtual ~PathNode(void);

	virtual void Draw();
	virtual	int getType(){return PATH_NODE;}

	void SetBeginPoint(VECTOR beginp);
	void SetBeginPoint(float beginx, float beginy, float beginz);
	void SetEndPoint(VECTOR endp);
	void SetEndPoint(float endx, float endy, float endz);

	void GetBeginPoint(VECTOR *beginp);
	void GetBeginPoint(float *beginx,float *beginy,float *beginz);
	void GetEndPoint(VECTOR *endp);
	void GetEndPoint(float *endx,float *endy,float *endz);

	void SetLineWidth(float width);
	void GetLineWidth(float *width);

	void SetLineColor(float red, float green, float blue);
	void GetLineColor(float *red, float *green,float *blue);

protected:
	void DrawPath();

protected:
	VECTOR m_vBeginPoint; //手术路径起始点
	VECTOR m_vEndPoint;  //手术路径中止点
	float  m_fPathWidth; //表示手术路径标志线的粗细

	VECTOR m_vColor; //手术路径标志线颜色

};
