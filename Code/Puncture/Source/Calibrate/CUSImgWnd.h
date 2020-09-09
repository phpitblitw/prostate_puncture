#pragma once

#include <afxwin.h>
#include <opencv2/opencv.hpp>
#include "ImageRender/ImageRenderActor.h"
using namespace ImageRender;

class CUSImgWnd :	public CWnd
{
	//TODO 需要DECLARE_DYNAMIC宏吗？
public:
	CUSImgWnd();
	virtual ~CUSImgWnd();

public:
	void CreateWnd(CWnd *pParentWnd, RECT rectWnd,
		DWORD Style = WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_BORDER | WS_OVERLAPPED,
		DWORD ExStyle = NULL, LPCTSTR pWndName = NULL, UINT ID = 0);//创建窗口,在主窗口中调用

	virtual void Draw(CDC *pDC);

	void GetImageSize(int &cx, int &cy); //获取图像的大小

public:
	virtual BOOL UpdateSrcImage(cv::Mat &SrcData);

protected:
	CWnd * m_pParentWnd;
	int	 m_ID;

	ImageRenderActorPtr m_SrcImageRender;

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

typedef std::shared_ptr<CUSImgWnd> CUSImgWndPtr;

