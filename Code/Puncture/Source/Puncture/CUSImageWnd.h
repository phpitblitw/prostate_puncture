#pragma once

// CUSImageWnd
#include "Opencv2/opencv.hpp"
#include "ImageRender/ImageRenderActor.h"
using namespace ImageRender;

class CUSImageWnd : public CWnd
{
	DECLARE_DYNAMIC(CUSImageWnd)

public:
	CUSImageWnd();
	virtual ~CUSImageWnd();

public:
	void CreateWnd(CWnd *pParentWnd, RECT rectWnd,
		DWORD Style = WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_BORDER | WS_OVERLAPPED,
		DWORD ExStyle = NULL, LPCTSTR pWndName = NULL, UINT ID = 0);//创建窗口,在主窗口中调用

	virtual void Draw(CDC *pDC);

	void GetImageSize(int &cx, int &cy); //获取图像的大小


public:
	//virtual BOOL LoadSrcImage(CString strFilePath);
	//virtual BOOL LoadMaskImage(CString strFilePath);
	virtual BOOL UpdateSrcImage(cv::Mat &SrcData);
	virtual BOOL UpdateMaskImage(BYTE *pMaskData);

protected:
	CWnd * m_pParentWnd;
	int	 m_ID;

protected:
	ImageRenderActorPtr m_SrcImageRender;
	ImageRenderActorPtr m_MaskImageRender;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

typedef std::shared_ptr<CUSImageWnd> CUSImageWndPtr;


