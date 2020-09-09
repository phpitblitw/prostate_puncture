#include "StdAfx.h"
#include "BaseBitmapRender.h"
#include "GDIEncode.h"

//using namespace fsutility;
using namespace ImageRender;

BaseBitmapRender::BaseBitmapRender(void)
{
    m_nCX = 0;
	m_nCY = 0;
	m_hbitmap = NULL; 
	m_pvBits = NULL;

	m_MaskColor = RGB(0,0,0);
	m_bEnableMaskColor = FALSE;
}

BaseBitmapRender::BaseBitmapRender(int cx, int cy)
{
	m_nCX = 0;
	m_nCY = 0;
	m_hbitmap = NULL; 
	m_pvBits = NULL;
	CreateBitmapRender(cx, cy);
}


BaseBitmapRender::~BaseBitmapRender(void)
{
	ClearDIBResource();
}

BOOL BaseBitmapRender::CreateBitmapRender(int cx ,int cy)
{
	if(cx<=0 || cy<=0)
	{
		cx=4;
		cy=4;
	}
	ASSERT(cx>1 && cy>1);

	if(m_MemBitmap != NULL && m_nCX == cx && m_nCY == cy)
	{
		RendBackgroundImage();
		return TRUE;
	}

	m_nCX = cx;
    m_nCY = cy;

	//创建Bitmap对象，双缓冲绘制，GDI+
	m_MemBitmap.reset(new Bitmap(m_nCX, m_nCY, PixelFormat32bppPARGB ));
	m_MemGraphics.reset(Graphics::FromImage(&(*m_MemBitmap)));

	RendBackgroundImage();

	//创建DIB对象，大小与Bitmap对象保持一致
	if(m_hbitmap != NULL)
	{
		m_Image.Destroy();
		DeleteObject(m_hbitmap);
		m_hbitmap = NULL;
		m_pvBits  = NULL;
	}

	BOOL bSuccess = CreateDib(cx, cy, m_hbitmap, (BYTE**)(&m_pvBits));
	if(m_hbitmap != NULL)
	{
		m_Image.Attach(m_hbitmap);
		m_Image.SetHasAlphaChannel(true);
	}

	return bSuccess;
}

void BaseBitmapRender::ClearDIBResource()
{
	if(m_MemBitmap!=NULL)
	{
		m_MemBitmap.reset();
	}

	if(m_MemGraphics!=NULL)
	{
		m_MemGraphics.reset();
	}

	if(m_hbitmap != NULL)
	{
		m_Image.Destroy();
		DeleteObject(m_hbitmap);
		m_hbitmap = NULL;
		m_pvBits = NULL;
	}

	m_nCX = 0;
	m_nCY = 0;
}


Graphics* BaseBitmapRender::GetRenderGraphic()
{
	return m_MemGraphics.get();
}

Bitmap*   BaseBitmapRender::GetBitmap()
{
	return m_MemBitmap.get();
}

void BaseBitmapRender::RendBackgroundImage()
{
	//清空缓冲区
	if(m_MemGraphics != NULL)
	{
		//SolidBrush brush(Color::Black);
		//m_MemGraphics->FillRectangle(&brush, 0, 0, m_nCX, m_nCY);
		m_MemGraphics->Clear(Color::Black);
	}
	else
	{
		FS_ERROR(_T("Call BaseBitmapRender::RendBackgroundImage()  before create bitmap object"));
	}
}

//只有调用了该函数，才能使用RenderToWnd，否则绘制出来的空白图像
BOOL  BaseBitmapRender::FlushBitmapToDib()
{
	if(m_MemBitmap != NULL)
	{
		FlushToDib(m_MemBitmap.get(), (BYTE**)(&m_pvBits));
		//if(m_bEnableMaskColor)
		//{
		//	MaskColor(m_MaskColor);
		//}
		return TRUE;
	}
	return FALSE;
}


/*  
@ Function  : CreateDib  
@ brief     : 创建内存位图  
@ parameter : w  
@ parameter : h  
@ parameter : [out] hBmpSection OnPaint中会把这个位图直接往dc上贴  
@ parameter : [out] bmpData 位图的实际数据，gdiplus的会把东西都话到上面  
@ return    :   
@ remark    :   
@*/    
BOOL BaseBitmapRender::CreateDib(int w, int h, OUT HBITMAP& hBmpSection, OUT BYTE** bmpData)    
{    
	BITMAPINFO info = {0};    
	info.bmiHeader.biSize = sizeof(info.bmiHeader);    
	info.bmiHeader.biWidth = w;    
	info.bmiHeader.biHeight = -h;    
	info.bmiHeader.biPlanes = 1;    
	info.bmiHeader.biBitCount = 32;    
	info.bmiHeader.biCompression = BI_RGB;    
	info.bmiHeader.biSizeImage = w * h * 32 / 8;    
	HDC hdc = ::GetDC(NULL);    
	hBmpSection = ::CreateDIBSection(hdc, &info, DIB_RGB_COLORS, (void**)bmpData, NULL, 0);    
	::ReleaseDC(NULL, hdc);    
	return hBmpSection != NULL;    
}    


/*  
@ Function  : FlushToDib  
@ brief     : 将Gdiplus的Bitamp转存到gdi的内存位图上  
@ parameter : [in] pMemBitmap  
@ parameter : [in, out] bmpData 就是上面的函数创建处理的内存位图的数据区  
@ return    :   
@ remark    :   
@*/    
void BaseBitmapRender::FlushToDib(IN Bitmap* pMemBitmap, IN OUT BYTE** bmpData)    
{    
	BitmapData data;    
	data.Height = pMemBitmap->GetHeight();    
	data.PixelFormat = pMemBitmap->GetPixelFormat();    
	data.Scan0 = *bmpData;    
	data.Stride = (pMemBitmap->GetWidth() * 32 / 8);    
	data.Width = pMemBitmap->GetHeight();    
	Rect rct(0, 0, pMemBitmap->GetWidth(), pMemBitmap->GetHeight());    
	pMemBitmap->LockBits(&rct, ImageLockModeRead | ImageLockModeUserInputBuf, pMemBitmap->GetPixelFormat(), &data);    
	pMemBitmap->UnlockBits(&data);    
}  


BOOL  BaseBitmapRender::RenderToWnd(CDC *pDC, int x, int y)
{
	//return DrawTransparent(pDC, x, y);

	if(m_hbitmap != NULL)
	{
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		HBITMAP hOldBmp = (HBITMAP)::SelectObject(memDC.GetSafeHdc(), m_hbitmap);    
		pDC->BitBlt(x, y, m_nCX, m_nCY, &memDC, 0, 0, SRCCOPY);

		//删除对象
		::SelectObject(memDC.GetSafeHdc(), hOldBmp);    
		memDC.DeleteDC();

		return TRUE;
	}
	return FALSE;
}

BOOL  BaseBitmapRender::RenderToWnd(CDC *pDC, CRect dstRect)
{
	//return DrawTransparent(pDC, dstRect);

	if(m_hbitmap != NULL && dstRect.Width()!=0 && dstRect.Height()!=0)
	{
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		HBITMAP hOldBmp = (HBITMAP)::SelectObject(memDC.GetSafeHdc(), m_hbitmap);    
		pDC->SetStretchBltMode(HALFTONE);
		pDC->StretchBlt(dstRect.left, dstRect.top, dstRect.Width(), dstRect.Height(), &memDC, 0, 0, m_nCX, m_nCY, SRCCOPY);

		::SelectObject(memDC.GetSafeHdc(), hOldBmp);    
		memDC.DeleteDC();

		return TRUE;
	}
	return FALSE;

}

BOOL  BaseBitmapRender::DrawTransparent(CDC *pDC, CRect dstRect)
{
	if(m_hbitmap != NULL)
	{
		if(dstRect.Width()==0 || dstRect.Height()==0)
		{
			return FALSE;
		}

		m_Image.Draw(pDC->GetSafeHdc(),dstRect);
		return TRUE;
	}
	return FALSE;
}


BOOL  BaseBitmapRender::DrawTransparent(CDC * pDC, int x, int y)
{
	if(m_hbitmap != NULL)
	{
		m_Image.Draw(pDC->GetSafeHdc(),x, y);
	}

	return TRUE;
}




BOOL  BaseBitmapRender::SaveToBmpFile(CString strFileName)
{
	// Save the altered image.
	CLSID pngClsid;
	if(m_MemBitmap!= NULL && GDIEncode::GetEncoderClsid(L"image/bmp", &pngClsid))
	{
		m_MemBitmap->Save(strFileName, &pngClsid, NULL);
		return TRUE;
	}
	return FALSE;
}

BOOL  BaseBitmapRender::SaveTojppFile(CString strFileName)
{
	CLSID pngClsid;

	if(m_MemBitmap!= NULL && GDIEncode::GetEncoderClsid(L"image/jpeg", &pngClsid))
	{
		//设置编码质量
		EncoderParameters encoderParameters;
		ULONG quality = 100;
		Status stat;
		encoderParameters.Count= 1;
		encoderParameters.Parameter[0].Guid= EncoderQuality;
		encoderParameters.Parameter[0].Type= EncoderParameterValueTypeLong;
		encoderParameters.Parameter[0].NumberOfValues= 1;
		encoderParameters.Parameter[0].Value= &quality;
		stat = m_MemBitmap->Save(strFileName, &pngClsid, &encoderParameters);

		return (stat == Ok);
	}
	else
	{
		FS_ERROR(_T("SaveTojppFile Failed"));
	}

	return FALSE;
}


void  BaseBitmapRender::MaskColor(COLORREF color)
{
	m_Image.SetTransparentColor(color);

	BYTE r = GetRValue(color);
	BYTE g = GetGValue(color);
	BYTE b = GetBValue(color);

	BYTE *pData = (BYTE*)m_pvBits;
	int nStride = m_nCX * 32 / 8;
	for(int i=0; i<m_nCY; i++)
	{
		for(int j=0; j<m_nCX; j++)
		{
			int offset = i*nStride+j*4;
			if(pData[offset] == r && pData[offset+1] == g && pData[offset+2]==b)
			{
				pData[offset+3] = 0;
			}
			else
			{
				pData[offset+3] = 255;
			}
		}
	}
}