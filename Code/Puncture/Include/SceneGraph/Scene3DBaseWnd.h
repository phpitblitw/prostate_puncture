#pragma once
#include "OpenGl3D.h"
#include "SceneGroup.h"
#include "GLFont.h"
#include "Text2DNode.h"
#include <vector>
// Scene3DBaseWnd

class AFX_EXT_CLASS Scene3DBaseWnd : public CWnd
{
	DECLARE_DYNAMIC(Scene3DBaseWnd)

public:
	Scene3DBaseWnd();
	virtual ~Scene3DBaseWnd();

	virtual void ReDraw(void);
	virtual void ReDrawBoundbox(void);

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void   SetSceneRoot(SceneGroup * root);

	void   SetWndProj(float FovY, float Aspect, float NearZ, float FarZ);
	void   SetWndProj(float left,float right,float bottom,float top,float nearZ,float farZ);
	void   SetWndView(VECTOR vEyePt, VECTOR vLookatPt, VECTOR vUpVec);
	void   SetWndView(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);

	void   ScaleObjectInWnd(float scalex,float scaley,float scalez);
	void   RotateObjectInWnd(float angle, float nx, float ny, float nz);
	void   TranslateObjectInWnd(float tx, float ty, float tz);

	void   ResetTransform();
	void   SetTwoSideLight(BOOL twoside); //新增，双面光照接口，1－14，唐慧

	void   SetShowText(BOOL bShow);
	size_t   AddText2D(UINT id, int x, int y, float scale, float red, float green, float blue,char *pos);
	bool   ChangeText2DText(int id, char *p);
	bool   ChangeText2DPos(int id, int x, int y);
	bool   ChangeText2DScale(int id, float scale);
	bool   ChangeText2DColor(int id, float red, float green, float blue);
	bool   DeleteText2D(int id);

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void	  BuildFont(void);

protected:

	BOOL		m_bShowText; //是否显示文字
	typedef  std::vector<CText2DNode> TextArray;
	TextArray m_Text2DArray;

	int       m_WndWidth;
	int       m_WndHeight;
	CGLFont   m_Font;

	void *m_pCurrentScreenImage;

public:
	COpenGL3D m_OpenGL3D;

};


