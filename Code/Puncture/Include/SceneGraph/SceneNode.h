// SceneNode.h: interface for the SceneNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENENODE_H__7FDF7FF2_8043_48FA_AEE5_482FABE00E36__INCLUDED_)
#define AFX_SCENENODE_H__7FDF7FF2_8043_48FA_AEE5_482FABE00E36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Container.h"
#include "Enum.h"
#include "Vector.h"
#include "Rotation.h"
#include "Trackball.h"
#include "Matrix.h"

class AFX_EXT_CLASS SceneNode : public Container  
{
public:
 	SceneNode();
	virtual ~SceneNode();

public:
	virtual	int getType(){return SCENE_NODE_BASE;}

	// 搜索名为 name 的节点
	virtual SceneNode *Search(char *name);
	virtual void Draw(void)=0;
	// 为 Mouse Select 操作绘制当前节点
	virtual void DrawForMouseSelect(void){};

	void RotateAround(float angle, float nx, float ny, float nz,float refx, float refy, float refz);
	void RotateAround(float angle, VECTOR axis, VECTOR ref);

	void RotateInCameraAround(float angle, float nx, float ny, float nz,float refx, float refy, float refz);
	void RotateInCameraAround(float angle, VECTOR axis, VECTOR ref);

	void Rotate(float angle, float nx, float ny, float nz);
	void Rotate(float angle, VECTOR axis);

	// 在相机坐标系中的旋转变换
	void RotateInCamera(float angle, VECTOR axis);
	void RotateInCamera(float angle, float nx, float ny, float nz);	

	void Translate(VECTOR trans);
	void Translate(float dx, float dy, float dz);

	// 在相机坐标系中的平移变换
	void TranslateInCamera(VECTOR trans);
	void TranslateInCamera(float dx, float dy, float dz); 

	float getScaleStep(void);
	float getTranslateStep(void);
	float getRotateStep(void);
	void getScale(float *sx, float *sy, float *sz);
	void getRotation(float *angle, float *nx, float *ny, float *nz);
	void GetInverseRotation(float *angle,float *nx,float *ny,float *nz); //得到从根结点到该节点的旋转分量之和
	void getPosition(float *x,float *y,float *z);
	SceneNode *getParent(void){ return m_pParent; }
	void getName(char *name);
	BOOL getTransparent(); //获得是否透明属性，只对GeometryNode起作用

	void setScaleStep(float scalestep);
	void setTranslateStep(float transStep);
	void setRotateStep(float rotateStep);
	void setScale(float  sx, float  sy, float  sz);
	void setRotation(float  angle, float  nx, float  ny, float  nz);
	void setPosition(float x,float y,float z);
	void setParent(SceneNode *node){m_pParent=node;}
	void setName(const char *name);

	void Showed(void) {m_bShowed = TRUE;}  //当前节点显示
	void UnShowed(void) {m_bShowed = FALSE;} //当前节点隐藏
	BOOL IsShowed(void) { return m_bShowed; } //表示当前节点是否显示

	void Scale(float sx, float sy, float sz);
	void ResetTransform(void);
	void ResetScale(void) { m_scale = m_oldScale;}
	void ResetRotation(void) { m_rotation = m_oldRotation;}
	void ResetPosition(void) { m_position = m_oldPosition;}
	void Selected(void){m_bSelected = TRUE;	}
	void UnSelected(void){m_bSelected = FALSE; }
	BOOL IsSelected(void) {return m_bSelected; } //表示当前节点是否被拾取

	// 用鼠标跟踪旋转，在观察坐标系中，2004－3－2
	//跟踪球中心为坐标原点
    void MouseTrackStart(int iViewportWidth, int iViewportHeight, int mx, int my);
	void MouseTracking(int dx, int dy); 

	// 根节点定义在 World 坐标系中
	// FWTO : From World To Object
	// FOTW : From Object To World 
	// FCTO : From Camera To Object
	// FOTC : From Object To Camera 

	void LoadMatrix(void);	// 执行当前节点的 Translate, Rotate, Scale
	void LoadInverseMatrix(void);

	void LoadMatrix_FWTO(void);
	void LoadMatrix_FOTW(void);

	void GetMatrix(float M[16]);		// 快速，精确
	void GetInverseMatrix(float M[16]);	// 快速，精确
	void GetMatrix_FWTO(float M[16]);	// 快速，精确
	void GetMatrix_FOTW(float M[16]);	// 快速，精确
	void GetMatrix_FCTO(float M[16]);	// 快速，精确
	void GetMatrix_FOTC(float M[16]);	// 求逆运算，较慢，不精确

	// 设 Object 坐标系的点 Po(xo, yo, zo, wo)
	// 设 World  坐标系的点 Pw(xw, yw, zw, ww)
	// 设 World  坐标系到 Object 坐标系的矩阵为 M_FWTO[16]
	// 设 Object 坐标系到 World  坐标系的矩阵为 M_FOTW[16]
	// “坐标系矩阵 M_FWTO” 与 “坐标系矩阵 M_FOTW” 互逆
	// “坐标系矩阵” 与 “点的变换矩阵” 互逆
	// 所以 VertexTransform_FWTO 表示  Po = Pw * M_FOTW
	// 所以 VertexTransform_FOTW 表示  Pw = Po * M_FWTO

	void VertexTransform_FWTO(float *x, float *y, float *z);
	void VertexTransform_FOTW(float *x, float *y, float *z);
	void VertexTransform_FCTO(float *x, float *y, float *z); // 求逆运算，较慢，不精确
	void VertexTransform_FOTC(float *x, float *y, float *z); 
	//要注意窗口y坐标中windows和opengl的转换，这个在应用程序中完成，唐慧，04－9－29
	void VertexTransform_WindowToObject(float *x, float *y, float *z); // 较慢
	void VertexTransform_ObjectToWindow(float *x, float *y, float *z); // 较慢

	// 设 V = P(x,y,z) - O(0,0,0)
	// 则 V'= P' - O'
	// 用于计算旋转轴等
	void VectorTransform_FWTO(VECTOR *V);
	void VectorTransform_FOTW(VECTOR *V);
	void VectorTransform_FCTO(VECTOR *V);	// 求逆运算，较慢，不精确
	void VectorTransform_FOTC(VECTOR *V); 

	void setBoundBox(float centerx,float centery,float centerz,float xrange,float yrange,float zrange); //设置包围盒的大小
	void setShowBoundBox(BOOL bShow);
	void setBoundColor(float red, float green, float blue);

	virtual  void   DrawBoundBox(); //绘制包围盒，在继承类中可以根据需要修改
	virtual  void   ComputeBoundBox(void) {};//根据顶点数据计算包围盒，在该类中BoundBox值全为0

protected:
	char m_strNodeName[NODE_NAME_LENGTH];// 当前节点的名字

	SceneNode *m_pParent;

	VECTOR      m_position;			// 当前节点在父节点中的位置参数
	ROTATION    m_rotation;			// 当前节点的旋转变换参数
	VECTOR      m_scale;			// 当前节点的比例变换参数

	BOOL        m_bTransparent;   //该节点是否透明标志，缺省为FALSE

	// 用于 ResetTransform()
	VECTOR      m_oldPosition;		
	ROTATION    m_oldRotation;		
	VECTOR      m_oldScale;			

	float		m_fRotateStep;		// 当前节点旋转变换的步长因子
	float		m_fTranslateStep;	// 当前节点平移变换的步长因子
	float		m_fScaleStep;		// 当前节点比例变换的步长因子

	Trackball   m_trackball;		//当前节点的Trackball，为拾取后对该节点操作使用
	BOOL        m_bSelected;		// 当前节点被拾取的标志
	BOOL        m_bShowed;			//当前节点是否在屏幕中显示标志

	BOOL		m_bShowBoundBox;	//是否显示包围盒标志，缺省为FALSE，选中该物体的时候改为TRUE
	BOUND_BOX   m_struBoundBox;		//包围盒
	VECTOR      m_struBoundColor;	//包围盒颜色，缺省为蓝色

};

#endif // !defined(AFX_SCENENODE_H__7FDF7FF2_8043_48FA_AEE5_482FABE00E36__INCLUDED_)
