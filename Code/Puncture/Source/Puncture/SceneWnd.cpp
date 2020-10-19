// SceneDemoWnd.cpp: 实现文件
//

#include "stdafx.h"
#include "Puncture.h"
#include "SceneWnd.h"


// SceneDemoWnd

IMPLEMENT_DYNAMIC(SceneWnd, Scene3DMessageWnd)

SceneWnd::SceneWnd()
{
	m_pParentWnd = NULL;
	section_group = NULL;
	m_pScanCenterNode = NULL;
	m_bCurProjMode = FALSE;
	m_fAspect = 1.0;

	m_rangex = m_rangey = m_rangez = 200.0;
	m_centerx = m_centery = m_centerz = 100.0;

	m_bLButtonDown = FALSE;
}

SceneWnd::~SceneWnd()
{
}


BEGIN_MESSAGE_MAP(SceneWnd, Scene3DMessageWnd)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// SceneDemoWnd 消息处理程序


BOOL SceneWnd::CreateWnd(CWnd *pParentWnd, RECT rectWnd, DWORD Style,
	DWORD ExStyle, LPCTSTR pWndName, UINT ID)
{
	BOOL ret = Scene3DMessageWnd::Create(NULL, _T("SceneDemo3DWnd"), WS_CLIPCHILDREN | WS_CHILD | WS_VISIBLE, rectWnd, pParentWnd, ID);	//原始代码
	//测试
	//BOOL ret = 1;
	//LPCTSTR ptt = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
	//	0, (HBRUSH)GetStockObject(BLACK_BRUSH));
	//
	//CreateEx(ExStyle, ptt,
	//	pWndName,
	//	Style,
	//	rectWnd,
	//	a, ID);
	//测试

	m_pParentWnd = pParentWnd;
	m_ID = ID;
	
	InitScene();
	return ret;

}

void SceneWnd::UpdateScanCenter(double x, double y, double z)
{
	m_pScanCenterNode->setPosition((float)x, (float)y, (float)z);
	ReDraw();
}

void SceneWnd::InitScene()
{
	SceneGroup * root = new SceneGroup;
	section_group = new SceneGroup;
	section_group->setName("root");

	m_pProstateSurfaceNode = new ProstateGeometryNode;
	section_group->AddChild(m_pProstateSurfaceNode);

	m_pScanCenterNode = new SphereNode;
	m_pScanCenterNode->SetSphere(2.0f);
	m_pScanCenterNode->SetColor(1.0, 0.0, 0.0);
	section_group->AddChild(m_pScanCenterNode);

	root->AddChild(section_group);

	this->SetSceneRoot(root);
	this->m_OpenGL3D.selectNode(section_group);

	//背景颜色
	this->m_OpenGL3D.setClearColor(0.3f, 0.5f, 0.8f, 1.0f);
	this->m_OpenGL3D.setMouseInteraction(MOUSE_ROTATE);

}

void SceneWnd::SetBoundBox(double x1, double x2, double y1, double y2, double z1, double z2)
{
	m_rangex = x2 - x1;
	m_rangey = y2 - y1;
	m_rangez = z2 - z1;

	m_centerx = x1 + m_rangex / 2.0;
	m_centery = y1 + m_rangey / 2.0;
	m_centerz = z1 + m_rangez / 2.0;

	m_pSurfaceData->SetBoundBox(x1, x2, y1, y2, z1, z2);

	ChangeView();

	//把场景的中心平移到原点
	section_group->Translate(-m_centerx, -m_centery, -m_centerz);
}

void SceneWnd::ChangeView()
{
	if (m_bCurProjMode)
	{
		this->SetWndProj(70.0f, m_fAspect, 1.0f, 5000.0f); //原始大小
		this->SetWndView(0.0f, 40.0, 500.0f, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

	}
	else
	{
		float WndCX = (float)fmax(m_rangex, m_rangey);
		float max = (float)fmax(WndCX, (float)m_rangez);
		float X = 0.8f*WndCX;
		this->SetWndProj(-X * m_fAspect, X*m_fAspect, -X, X, -20.0f*max, 20.0f*max); //原始大小
		this->SetWndView(0.2f*max, 0.2f*max, 0.6f*max, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

	}
}

void SceneWnd::AttachSurfaceData(SURGICALPLAN::SurfaceDataPtr pSurfaceData)
{
	m_pSurfaceData = pSurfaceData;
	m_pProstateSurfaceNode->SetSurfaceData(pSurfaceData);
	ReDraw();
}

void SceneWnd::OnSize(UINT nType, int cx, int cy)
{
	Scene3DMessageWnd::OnSize(nType, cx, cy);

	if (cx > 0 && cy > 0)
	{
		m_fAspect = (float)cx / (float)cy;

		ChangeView();
	}

}

void SceneWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_bLButtonDown = TRUE;
	this->SendMessage(WM_IM_NROTATE_BEGIN, (WPARAM)&point);
}


void SceneWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	this->SendMessage(WM_IM_NROTATE_END, (WPARAM)&point);
	m_bLButtonDown = FALSE;
}


void SceneWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (m_bLButtonDown)
		this->SendMessage(WM_IM_NROTATE_PROGRESS, (WPARAM)&point);
}
