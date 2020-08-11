// CUSImageWnd.cpp: 实现文件
//

#include "stdafx.h"
#include "Puncture.h"
#include "CUSImageWnd.h"
#include "ImageRender/PaletterManager.h"

using namespace ImageRender;
// CUSImageWnd

IMPLEMENT_DYNAMIC(CUSImageWnd, CWnd)

CUSImageWnd::CUSImageWnd()
{
	m_pParentWnd = NULL;
	m_SrcImageRender = nullptr;

}

CUSImageWnd::~CUSImageWnd()
{
}


BEGIN_MESSAGE_MAP(CUSImageWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CUSImageWnd 消息处理程序

void CUSImageWnd::CreateWnd(CWnd *pParentWnd, RECT rectWnd, DWORD Style,
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


//原始OnPaint()函数
//void CUSImageWnd::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//					   // TODO: 在此处添加消息处理程序代码
//	if (m_SrcImageRender != nullptr)
//	{
//		Graphics g(dc.GetSafeHdc());
//		CRect rect;
//		GetClientRect(rect);
//		m_SrcImageRender->Render(&g, 0, 0, rect.Width(), rect.Height());
//
//		if (m_MaskImageRender != nullptr)
//		{
//			m_MaskImageRender->Render(&g, 0, 0, rect.Width(), rect.Height());
//		}
//	}
//	return;
//}

void CUSImageWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码

	if (m_SrcImageRender != nullptr)
	{
		Graphics g(dc.GetSafeHdc());
		CRect rect;
		GetClientRect(rect);

		//双缓冲OnPaint()解决屏闪问题
		Bitmap clBmp(rect.Width(), rect.Height());	//内存bitmap
		Graphics *grPtr= Graphics::FromImage(&clBmp);	//内存Graphics

		m_SrcImageRender->Render(grPtr, 0, 0, rect.Width(), rect.Height());

		if (m_MaskImageRender != nullptr)
		{
			m_MaskImageRender->Render(grPtr, 0, 0, rect.Width(), rect.Height());
		}
		//g.DrawImage(&clBmp, rect);
		g.DrawImage(&clBmp, 0, 0);
	}
	return;
}




void CUSImageWnd::Draw(CDC *pDC)
{

}


void CUSImageWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	
}


//BOOL CUSImageWnd::LoadSrcImage(CString strFilePath)
//{
//	m_SrcImageRender.reset(new ImageRenderActor);
//	m_SrcImageRender->LoadImageFile(strFilePath);
//
//	if (m_SrcImageRender->GetSliceBitmap() != nullptr)
//	{
//		//初始化mask图像
//		m_MaskImageRender.reset(new ImageRenderActor);
//		m_MaskImageRender->CreateGrayImageRender(m_SrcImageRender->GetSliceBitmap()->GetWidth(),
//			m_SrcImageRender->GetSliceBitmap()->GetHeight());
//		m_MaskImageRender->GetSliceBitmap()->SetPalette(PaletterManager::Instance().GetOverlayGrayColorPalette());
//
//		return TRUE;
//	}
//	return FALSE;
//}

void CUSImageWnd::GetImageSize(int &cx, int &cy)
{
	if (m_SrcImageRender != nullptr)
	{
		cx = m_SrcImageRender->GetSliceBitmap()->GetWidth();
		cy = m_SrcImageRender->GetSliceBitmap()->GetHeight();
		while (cx%4!=0)
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

//BOOL CUSImageWnd::LoadMaskImage(CString strFilePath)
//{
//	if (m_SrcImageRender != nullptr)
//	{
//		m_MaskImageRender.reset(new ImageRenderActor());
//		m_MaskImageRender->LoadImageFile(strFilePath);
//		m_MaskImageRender->GetSliceBitmap()->SetPalette(PaletterManager::Instance().GetOverlayGrayColorPalette());
//		return TRUE;
//	}
//
//	return FALSE;
//}

BOOL CUSImageWnd::UpdateMaskImage(BYTE *pMaskData)
{
	if (m_MaskImageRender != nullptr)
	{
		ThBitmapPtr img = m_MaskImageRender->GetSliceBitmap();
		Gdiplus::Rect rect(0, 0, img->GetWidth(), img->GetHeight());
		std::shared_ptr<BitmapData> DstBitmapData(new BitmapData);
		img->LockBits(&rect, ImageLockModeWrite, img->GetPixelFormat(), &(*DstBitmapData));
		BYTE *pByte = (BYTE *)DstBitmapData->Scan0;
		memcpy(pByte, pMaskData, img->GetWidth()*img->GetHeight());
		img->UnlockBits(&(*DstBitmapData));

		RedrawWindow(NULL);

		return TRUE;
	}

	return FALSE;
}

BOOL CUSImageWnd::UpdateSrcImage(cv::Mat &SrcData)
{
	m_SrcImageRender.reset(new ImageRenderActor);
	m_SrcImageRender->LoadImageFile(SrcData);
	//m_SrcImageRender->LoadImageFile(strFilePath);

	if (m_SrcImageRender->GetSliceBitmap() != nullptr)
	{
		//初始化mask图像
		m_MaskImageRender.reset(new ImageRenderActor);
		m_MaskImageRender->CreateGrayImageRender(m_SrcImageRender->GetSliceBitmap()->GetWidth(),
			m_SrcImageRender->GetSliceBitmap()->GetHeight());
		m_MaskImageRender->GetSliceBitmap()->SetPalette(PaletterManager::Instance().GetOverlayGrayColorPalette());

		return TRUE;
	}
	return FALSE;
}


BOOL CUSImageWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}
