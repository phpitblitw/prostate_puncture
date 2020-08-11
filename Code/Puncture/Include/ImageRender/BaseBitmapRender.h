#pragma once
#include "TypeDefineHeader.h"
//内存bitmap，render 首先把数据绘制到该bitmap中,然后再把Bitmap绘制DIB对象

using namespace Gdiplus;
namespace ImageRender
{
	class AFX_EXT_CLASS BaseBitmapRender
	{
	public:
		BaseBitmapRender(void);
		BaseBitmapRender(int cx, int cy);
		virtual ~BaseBitmapRender(void);

		virtual BOOL  CreateBitmapRender(int cx, int cy);

		//只有调用了该函数，才能使用RenderToWnd，否则绘制出来的空白图像
		virtual BOOL  FlushBitmapToDib();

		//Call next in Wnd's OnPaint() function
		virtual BOOL  RenderToWnd(CDC *pDC, int x=0, int y=0);
		virtual BOOL  RenderToWnd(CDC *pDC, CRect dstRect);

		virtual BOOL  DrawTransparent(CDC * pDC, int x=0, int y=0);
		virtual BOOL  DrawTransparent(CDC *pDC, CRect dstRect);

		//save render result to file
		virtual BOOL  SaveToBmpFile(CString strFileName);
		virtual BOOL  SaveTojppFile(CString strFileName);

		//使用背景色更新Bitmap对象
		virtual void RendBackgroundImage();

	public:
		Graphics* GetRenderGraphic();
		Bitmap*   GetBitmap(); 


	protected:
		//创建于Bitmap对象大小相同的DIB对象，提高在OnPaint调用是的绘制速度
		virtual BOOL CreateDib(int w, int h, OUT HBITMAP& hBmpSection, OUT BYTE** bmpData);

		//把Bitmap对象数据更新到DIB对象中，窗口的OnPaint对象直接使用DIB对象进行绘制
		virtual void FlushToDib(IN Bitmap* pMemBitmap, IN OUT BYTE** bmpData);

		virtual void MaskColor(COLORREF color);

		virtual void ClearDIBResource();

	protected:
		CRect m_FullRegion;
		int   m_nCX;
		int   m_nCY;

		ThBitmapPtr m_MemBitmap;
		ThGraphicsPtr m_MemGraphics;

		//DIB 加速图像绘制
		HBITMAP    m_hbitmap;       // bitmap handle
		BITMAPINFO m_bmi;           // bitmap header
		VOID       *m_pvBits;       // pointer to DIB section
		CImage	   m_Image;

		COLORREF   m_MaskColor;
		BOOL       m_bEnableMaskColor;
	};

}

