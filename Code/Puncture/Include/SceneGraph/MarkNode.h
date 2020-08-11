#pragma once
#include "SceneGroup.h"
#include "TextNode.h"
#include "SphereNode.h"

class AFX_EXT_CLASS MarkNode :public SceneGroup
{
public:
	MarkNode(void);
	MarkNode(UINT TextListID);
	virtual ~MarkNode(void);

	virtual	int getType(){return MARK_NODE;}

	virtual void Draw();
	void SetText(const char *str);
	//该Postion是相对于Sphere的位置
	void    SetTextPostion(float px,float py,float pz);
	void	SetTextColor(float red, float green, float blue);
	void	SetTextSize(float sx, float sy, float sz); 

	// 设置球体参数，半径，等分数
	void SetSphere(float radius, int slices=32); 
	void SetSphereColor(float red,float green,float blue);//设置球体颜色

protected:
	SphereNode * m_sphere;
	TextNode * m_text;
};
