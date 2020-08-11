#pragma once
#include "opengl.h"
#include "camera.h"
#include "Trackball.h"
#include "scenegroup.h"
#include "resource.h"
#include "Enum.h"
#include "MouseSelect.h"

class AFX_EXT_CLASS COpenGL3D :	public COpenGL
{
public:
	COpenGL3D(void);
	~COpenGL3D(void);

public:
	//虚函数
	virtual void Draw();
	virtual void DrawBoundBox(); //绘制包围盒
	virtual void OpenGLSize(int cx, int cy);
	virtual BOOL initOpenGL(HWND hWnd);
	virtual void DestroyOpenGL(void);

	void setSceneGraph(SceneGroup * root);
	BOOL IsSceneSeted(){return m_SceneSeted;}
	// 设置、获取背景颜色
	void setClearColor(float red,float green,float blue,float alpha=1.0f);
	void getClearColor(float *red, float *green, float *blue, float *alpha);

	//消息响应函数，跟trackball有关
	afx_msg void OnButtonDown(CPoint point);
	afx_msg void OnButtonUp(CPoint point);
	afx_msg void OnMouseMove(CPoint point);
	afx_msg void OnMouseWheel(short zDelta, CPoint pt);

	// 用鼠标操作
	afx_msg void OnMouseSelect(void);			// 拾取图形（叶节点）
	afx_msg void OnMouseRotate(void);			// 绕原点任意方向旋转
	afx_msg void OnMouseTranslate(void);        //平移（选中节点）
	afx_msg void OnUpdateMouseSelect(CCmdUI* pCmdUI);	
	afx_msg void OnUpdateMouseRotate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMouseTranslate(CCmdUI *pCmdUI);

	// 设置、获取鼠标交互的方式
	void setMouseInteraction(int  type);
	int  getMouseInteraction(void);

	void setMouseTranslateSteplength(float length) {m_fMouseTranslateSteplength = length;}

	// 拾取当前场景的某个节点
	void selectNode(SceneNode * node);
	void selectNode(char *name);
	// 取消已拾取的节点
	void UnselectNode(void);

	// 设置双面光照标志
	void setTwoSideLight(BOOL twoside);

	SceneGroup * getRoot() {return m_pSceneRoot;}

protected:
	void initializeGL(void);

public:
	CPoint  m_Point;   //鼠标刚才所在的位置
	SceneGraphMatrix  m_matRotation; //旋转矩阵
	float	m_fClearColor[4];	// 背景颜色

	int		m_iMouseInteraction;	// 鼠标交互的方式
	int		m_iMouseX, m_iMouseY;	// 鼠标的坐标
	BOOL	m_bButtonDown;		// 鼠标按下
	float     m_fMouseTranslateSteplength; //鼠标平移步长，缺省为10

	float   m_fScale; //缩放倍数
	float   m_fScaleDelta; //缩放的步长

	BOOL	m_bNodeSelected;	// 是否某一节点已被拾取
	SceneNode *m_pSelectedNode;	// 已被拾取的节点
	char	m_strSelectedNodeName[NODE_NAME_LENGTH]; // 已被拾取的节点名字

	Camera m_Camera;
	Camera *getCamera(){return &m_Camera;}

	int m_iViewportHeight;
	int m_iViewportWidth;
	BOOL m_SceneSeted;

	BOOL m_bTwoSideLightUsed; //是否使用双面光照标志

protected:
	SceneGroup  * m_pSceneRoot;
};
