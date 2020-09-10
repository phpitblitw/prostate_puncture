#include "pch.h"
#include "CUSImgWnd.h"

using namespace ImageRender;

CUSImgWnd::CUSImgWnd()
{
	m_pParentWnd = NULL;
	m_SrcImageRender = nullptr;
}

CUSImgWnd::~CUSImgWnd()
{}

BEGIN_MESSAGE_MAP(CUSImgWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CUSImgWnd::CreateWnd(CWnd *pParentWnd, RECT rectWnd, DWORD Style,
	DWORD ExStyle, LPCTSTR pWndName, UINT ID)
{
	m_pParentWnd = pParentWnd;
	m_ID = ID;

	LPCTSTR ptt = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
		0, (HBRUSH)GetStockObject(BLACK_BRUSH));
	CreateEx(ExStyle, ptt,
		pWndName,
		Style,
		rectWnd,
		pParentWnd, ID);
	ShowWindow(SW_SHOWNORMAL);
}

void CUSImgWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
	if (m_SrcImageRender != nullptr)
	{
		Graphics g(dc.GetSafeHdc());
		CRect rect;
		GetClientRect(rect);
		m_SrcImageRender->Render(&g, 0, 0, rect.Width(), rect.Height());
	}
	return;
}

void CUSImgWnd::Draw(CDC *pDC)
{
}

void CUSImgWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

}

void CUSImgWnd::GetImageSize(int &cx, int &cy)
{
	if (m_SrcImageRender != nullptr)
	{
		cx = m_SrcImageRender->GetSliceBitmap()->GetWidth();
		cy = m_SrcImageRender->GetSliceBitmap()->GetHeight();
		while (cx % 4 != 0)
		{
			cx++;
		}
	}
	else
	{
		cx = 0;
		cy = 0;
	}
}

BOOL CUSImgWnd::UpdateSrcImage(cv::Mat &SrcData)
{
	m_SrcImageRender.reset(new ImageRenderActor);
	m_SrcImageRender->LoadImageFile(SrcData);
	//m_SrcImageRender->LoadImageFile(strFilePath);
	return TRUE;
}

