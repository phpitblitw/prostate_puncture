#pragma once
#include "SceneObjectNode.h"

class AFX_EXT_CLASS SphereNode :public SceneObjectNode
{
public:
	SphereNode(void);
	virtual ~SphereNode(void);
	virtual	int getType(){return SPHERE_NODE;}

	// 设置球体参数，半径，等分数
	void SetSphere(float radius, int slices=32); 
	void SetColor(float red,float green,float blue);//设置球体颜色

	void GetSphereRadius(float *radius);

protected:

	virtual  void   DrawObject(void); //形体的具体绘制函数，在该类中为空，在继承类中具体实现
	void	drawSphere(float radius, int numMajor=16, int numMinor=16);

protected:
	float	m_fRadius;		// 半径
	int		m_iSlices;		// 经纬线等份数

	float   m_color[3];     //球体的颜色
};
