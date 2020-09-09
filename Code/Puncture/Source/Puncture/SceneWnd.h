#pragma once
#include "ImageRender/ImageRenderActor.h"
#include "SceneGraph/Scene3DMessageWnd.h"
#include "SceneGraph/SphereNode.h"
#include "ProstateGeometryNode.h"
// SceneDemoWnd
using namespace ImageRender;

class SceneWnd : public Scene3DMessageWnd
{
	DECLARE_DYNAMIC(SceneWnd)

public:
	SceneWnd();
	virtual ~SceneWnd();

	BOOL CreateWnd(CWnd *pParentWnd, RECT rectWnd,
		DWORD Style = WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_BORDER | WS_OVERLAPPED,
		DWORD ExStyle = NULL, LPCTSTR pWndName = NULL, UINT ID = 0);//创建窗口,在主窗口中调用

	void SetBoundBox(double x1, double x2, double y1, double y2, double z1, double z2);
	void AttachSurfaceData(SURGICALPLAN::SurfaceDataPtr pSurfaceData);
	void UpdateScanCenter(double x, double y, double z);

protected:
	void InitScene();
	void ChangeView();

public:
	SceneGroup * section_group;     //所有节点都放在这个下面
	ProstateGeometryNode * m_pProstateSurfaceNode;
	SphereNode *m_pScanCenterNode;

	//坐标范围
	double        m_rangex, m_rangey, m_rangez; 
	double		  m_centerx, m_centery, m_centerz;
	BOOL  m_bCurProjMode; //TRUE--透视投影；FALSE--正交投影
	float m_fAspect; //窗口的宽长比

protected:
	CWnd * m_pParentWnd;
	int	 m_ID;
	SURGICALPLAN::SurfaceDataPtr m_pSurfaceData;

	BOOL m_bLButtonDown;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

typedef std::shared_ptr <SceneWnd> SceneWndPtr;

