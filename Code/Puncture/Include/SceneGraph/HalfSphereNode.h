#pragma once
#include "sceneobjectnode.h"

class AFX_EXT_CLASS HalfSphereNode :	public SceneObjectNode
{
public:
	HalfSphereNode(void);
	virtual ~HalfSphereNode(void);

	// 设置球体参数，半径，等分数
	void SetSphere(float radius, int slices=32); 

protected:
	virtual  void   DrawObject(void); //半圆的具体绘制函数
	void drawHalfSphere(float radius, int numMajor, int numMinor);

protected:
	float	m_fRadius;		// 半径
	int		m_iSlices;		// 经纬线等份数

};
