// GeometryNode.h: interface for the GeometryNode class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "SceneObjectNode.h"

class AFX_EXT_CLASS GeometryNode : public SceneObjectNode  
{
public:
	virtual int getType(void){return GEOMETRY_NODE;}

	virtual ~GeometryNode(void);
	GeometryNode(void);

protected:
	void		ComputeNormals(void);//根据给出的顶点和三角面索引计算法向量

	virtual void ComputeBoundBox(void);//根据顶点数据计算包围盒
	virtual void RenderStart(void); //绘制之前的初始化，比基类添加了ComputeNormals，绘制之前计算法向量
	virtual void DrawObject(void); //形体的具体绘制函数

	virtual void DrawBoundBox(); //绘制包围盒

public:
	void setPVertices(VECTOR *pVertices,unsigned int iVerNum);
	void setPIndices(VECTOR3ui *pIndices,unsigned int iTriNum);
	void setPNormals(VECTOR *pNormals, unsigned int iVerNum);
	void setIndexed(BOOL  indexed);

protected:
	BOOL		m_bVerticesSeted; //顶点指针是否已经设置标志
	BOOL		m_bIndexed;       //是否使用索引的标志
	BOOL		m_bNormalSeted;   //法向量是否设定标志，缺省为FALSE，由程序计算，设定法向量时改为TRUE


	VECTOR3ui	* m_pIndices;  //索引数组指针
	VECTOR		* m_pNormals;  //法向量指针
	VECTOR		* m_pVertices; //顶点数组指针
	unsigned int  m_iVerNum;     //顶点数目
	unsigned int  m_iTriNum;     //三角面数目
};

