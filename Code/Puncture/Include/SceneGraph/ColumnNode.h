#pragma once
#include "sceneobjectnode.h"

class AFX_EXT_CLASS ColumnNode :	public SceneObjectNode
{
public:
	ColumnNode(void);
	virtual ~ColumnNode(void);

	//设置圆柱体参数
	void SetColumn(float radius,float height);

protected:
	virtual  void   DrawObject(void); //具体绘制圆柱体的函数
	void DrawSolidCylinder(float radius, float height); //绘制圆柱体，含有上下底面

protected:
	float	m_fRadius;		// 底面半径
	float	m_fHeight;		// 高度（Y方向）
};
