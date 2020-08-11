#pragma once

#include <gl/gl.h>
#include <gl/glu.h>

#include "Vector.h"
#include "SceneNode.h"
#include "Enum.h"
#include "Material.h"
#include "MaterialFront.h"
#include "MaterialBack.h"
#include "Texture2D.h"
#include "Trackball.h"
#include "MouseSelect.h"

class AFX_EXT_CLASS SceneObjectNode :  public SceneNode
{
public:
	SceneObjectNode(void);
	virtual ~SceneObjectNode(void);

	virtual int getType(void){return SCENEOBJECT_NODE;}
	virtual void Draw();
	// 为 Mouse Select 操作绘制当前节点
	virtual void DrawForMouseSelect(void);

	virtual void FinalRelease(void);

	virtual void ComputeBoundBox(void) {};
protected:
	virtual  void   DrawObject(void); //形体的具体绘制函数，在该类中为空，在继承类中具体实现
	virtual  void   RenderStart(void); //绘制之前的初始化
	virtual  void   RenderEnd(void); //绘制之后的工作

public:
	Material * getMaterial(void);
	void setMaterial(Material *mat);

	MaterialFront *getFrontMaterial(void);
	void setFrontMaterial(MaterialFront *mat);

	MaterialBack *getBackMaterial(void);
	void setBackMaterial(MaterialBack *mat);

	void setPolygonMode(GLenum Mode = GL_FILL);

	void setTransparent(BOOL transparent); //设置物体透明属性
	void setBlendFactor(GLenum glSRCFactor, GLenum glDSTFactor); //设置融合因子

	void getDrawMartrix(double m[16]);

	BOOL IsEnableAlphaTest() {return m_bAlphaTest;}
	void setAlphaFunc(GLenum func, GLclampf ref);
	void setAlphaTest(BOOL istest);

protected:
	GLenum      m_eBlendSRCFactor;  //透明参数中源融合因子
	GLenum      m_eBlendDSTFactor;  //透明参数中目标融合因子

	Material	  * m_pMaterial; //材质属性指针
	MaterialFront * m_pFrontMaterial; //表面材质属性指针
	MaterialBack  * m_pBackMaterial;  //背面材质属性指针

	double        m_DrawMartirx[16];

	BOOL	   m_bAlphaTest; //是否使用alpha测试，04－8－23，add
	GLenum	   m_eAlphaFunc; //alpha测试函数，选项GL_NEVER、GL_LESS、GL_EQUAL、GL_LEQUAL
	//GL_GREATER、GL_NOTEQUAL、GL_GEQUAL、GL_ALWAYS，详见glAlphaFunc
	//SceneGraph中缺省使用GL_GEQUAL
    GLclampf   m_fAlphaRef; //alpha测试相对值，0－1之间
	//SceneGraph中缺省使用0，即缺省情况下alpha测试不起作用

	GLenum      m_ePolygonMode;//表面的绘制方式
};
