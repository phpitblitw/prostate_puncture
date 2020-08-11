#pragma once
#include "GeometryNode.h"

class AFX_EXT_CLASS TexGeometryNode :public GeometryNode
{
public:
	virtual int getType(void){return TEX_GEOMETRY_NODE;}
	virtual void FinalRelease(void);

	virtual ~TexGeometryNode(void);
	TexGeometryNode(void);

protected:
	void	ComputeTexCoords(void);//根据用户的实际需要确定纹理坐标

	virtual void  RenderStart(void); //绘制之前的初始化

public:
	Texture2D * getTexture(void);
	void setTexture( Texture2D *texture);
	void setTexCoord(unsigned int index, float t0, float t1);
	void setTexGenPara(GLuint genPara = GL_OBJECT_LINEAR);

protected:
	BOOL		m_bTexCoordGen;   //是否使用自动纹理坐标标志，缺省为TRUE，设置纹理坐标时改为FALSE

	Texture2D   * m_pTexture;  //纹理指针
	float       * m_pTexCoord;	// 纹理坐标指针
	GLuint		m_TexGenPara; //自动纹理坐标生成方式
};
