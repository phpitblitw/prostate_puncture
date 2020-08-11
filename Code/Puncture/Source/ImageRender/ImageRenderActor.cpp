#include "StdAfx.h"
#include "ImageRenderActor.h"
#include "PaletterManager.h"
#include <vector>
#include <cmath>
#include <omp.h>
#include "opencv2/opencv.hpp"
#define WIDTHBYTES(bits) ((DWORD)(((bits)+31) & (~31)) / 8)

using namespace std;
using namespace ImageRender;

ImageRenderActor::ImageRenderActor()
{
	m_bHighQualityRender = TRUE;
	m_bModifyImage = true;
	ResetZoomAndOffset();
}

ImageRenderActor::~ImageRenderActor(void)
{
}


//void ImageRenderActor::LoadImageFile(CString strFilePath)
//{
//	m_SliceBitmap.reset(new Bitmap(strFilePath));
//	return;
//}

void ImageRenderActor::Mat2Bitmap(cv::Mat &m, Gdiplus::Bitmap *pBitmap)
{

	int width = m.cols;
	int height = m.rows;

	// Bitmap *pBitmap = new Bitmap(width, height, PixelFormat8bppIndexed); 
	if (!pBitmap)
	{
		return;
	}

	Gdiplus::BitmapData bmpData;
	Gdiplus::Rect rect(0, 0, width, height);
	pBitmap->LockBits(&rect, 0x0002, PixelFormat24bppRGB, &bmpData);
	BYTE *pByte = (BYTE*)bmpData.Scan0;

	int widNew = WIDTHBYTES(width * 3 * 8);
	for (int i = 0; i < height; i++)
	{
		memcpy(pByte + i * widNew, m.data + i * width * 3, width * 3);
	}

	pBitmap->UnlockBits(&bmpData);
	return;
}

void ImageRenderActor::LoadImageFile(cv::Mat &SrcData)
{
	Gdiplus::Bitmap *t_bitmap = new Gdiplus::Bitmap(SrcData.cols, SrcData.rows, PixelFormat24bppRGB);

	//测试开始
	//cv::Mat gryImg;
	//if (SrcData.channels() > 1)
	//	cv::cvtColor(SrcData, gryImg, cv::COLOR_BGR2GRAY);
	//Gdiplus::Bitmap *t_bitmap1 = new Gdiplus::Bitmap(gryImg.cols, gryImg.rows, PixelFormat8bppIndexed);

	//CString s1 = _T("I:\study\prostate\projects\zzw_0120\data_2117839_水膜_完整数据\B超示例.bmp");
	//CString s2 = _T("I:\\study\\prostate\\projects\\zzw_0120\\data_2117839_水膜_完整数据\\B超示例.bmp");

	//Bitmap *temp;
	//temp = new Bitmap(s1);

	//Bitmap *temp2;
	//temp = new Bitmap(s2);
	//测试截止


	Mat2Bitmap(SrcData, t_bitmap);
	m_SliceBitmap.reset(t_bitmap);
	return;
}

void ImageRenderActor::CreateGrayImageRender(int nWidth ,int nHeight)
{
	Rect rect(0,0,nWidth, nHeight);
	std::shared_ptr<BitmapData> pBitmapData(new BitmapData);
	m_SliceBitmap.reset(new Bitmap(nWidth, nHeight, PixelFormat8bppIndexed));
	ColorPalette *Palette = PaletterManager::Instance().GetGrayColorPalette();
	m_SliceBitmap->SetPalette(Palette);		
	m_SliceBitmap->LockBits(&rect, ImageLockModeWrite, PixelFormat8bppIndexed, &(*pBitmapData));
	BYTE *pByte = (BYTE *) pBitmapData->Scan0;
	int nStride = pBitmapData->Stride;
	//for( int iRow=0; iRow<nHeight; iRow++ )
	//{
	//	for( int iCol=0; iCol<nWidth; iCol++ )
	//	{
	//		pByte[iRow*nStride+iCol] = 0;
	//	}
	//}
	memset(pByte,0,nHeight*nStride);
	m_SliceBitmap->UnlockBits( &(*pBitmapData) );

}

void ImageRenderActor::CreateRGBImageRender(int nWidth ,int nHeight)
{
	Rect rect(0,0,nWidth, nHeight);
	std::shared_ptr<BitmapData> pBitmapData(new BitmapData);
	m_SliceBitmap.reset(new Bitmap(nWidth, nHeight, PixelFormat24bppRGB));
}


ThBitmapPtr ImageRenderActor::GetSliceBitmap()
{
	return m_SliceBitmap;
}

template<class T1, class T2>
void  DataNormalizeToImageBuffer(int cx, int cy,  int nStride, T1* pData, T2* pDstData,  int w, int l)
{
	int index1=0;
	int indexbmp=0;
	//#pragma omp parallel for
	for( int iRow=0; iRow<cy; iRow++ )
	{
		for( int iCol=0; iCol<cx; iCol++ )
		{
			int index1 = iRow*cx+iCol;
			int index2 = iRow*nStride+iCol;
			T1 data = pData[index1];
			if(data<(l-w/2))
				pDstData[index2] = 0;
			else if(data>(l+w/2))
				pDstData[index2] = 255;
			else 
				pDstData[index2] = 255*(data-l+w/2)/w;
		}
	}
}

void  DataNormalizeToImageBuffer(int cx, int cy,  int nStride, BYTE* pData, BYTE* pDstData)
{
	int index1=0;
	int indexbmp=0;
    //#pragma omp parallel for
	for( int iRow=0; iRow<cy; iRow++ )
	{
		for( int iCol=0; iCol<cx; iCol++ )
		{
			pDstData[iRow*nStride+iCol] = pData[iRow*cx+iCol];
		}
	}
}


////对窗宽范围作了限制，避免
//BOOL SliceImageRenderActor::UpdateRenderImageWL(int w, int l)
//{
//	if(w==0)
//	{
//		w=256;
//		l=128;
//	}
//
//	//如果窗宽窗位与当前一致，则不需要更新
//	if(m_SliceImageData != NULL && (w>=1) && (w!=m_nCurrentW || l!=m_nCurrentL))
//	{
//		//不能调整窗宽窗位
//		//20150922 暂时不做限制，避免某些cpr图像也不能调整窗宽窗位
//		//if(m_SliceImageData->IsExportImage() && (m_SliceBitmap!=NULL))
//		//{
//		//	return FALSE;
//		//}
//
//		vtkImageData *pImageData = m_SliceImageData->GetImageData();
//		int nWidth = m_SliceImageData->GetWidth();
//		int nHeight = m_SliceImageData->GetHeight();
//
//		//Create Bitmap
//		m_SliceBitmap.reset(new Bitmap(nWidth, nHeight, PixelFormat8bppIndexed));
//		std::shared_ptr<BitmapData> pBitmapData(new BitmapData);
//
//		//copy normalize data to bitmap buffer
//		Rect rect(0,0,m_SliceBitmap->GetWidth(),m_SliceBitmap->GetHeight());
//		m_SliceBitmap->LockBits(&rect, ImageLockModeWrite, PixelFormat8bppIndexed, &(*pBitmapData) );
//
//		//if(w<1)
//		//{
//		//	w = m_nCurrentW;// pImageData->GetScalarTypeMax()-pImageData->GetScalarTypeMin();
//		//	l = m_nCurrentL;// (pImageData->GetScalarTypeMax()+pImageData->GetScalarTypeMin())/2;
//		//}
//		m_nCurrentW = w;
//		m_nCurrentL = l;
//
//		//normalize
//		BYTE *pByte = (BYTE *) pBitmapData->Scan0;
//		void *pData = pImageData->GetScalarPointer();
//		int nScalarSize = pImageData->GetScalarSize();
//		int nStride = pBitmapData->Stride;
//
//		if(nScalarSize==1)//8
//		{
//			DataNormalizeToImageBuffer(nWidth, nHeight, nStride, (BYTE *)pData, pByte , m_nCurrentW, m_nCurrentL);
//		}
//		else if(nScalarSize == 2)
//		{
//			DataNormalizeToImageBuffer(nWidth, nHeight,  nStride, (short *)pData, pByte , m_nCurrentW, m_nCurrentL);
//		}
//		else if(nScalarSize == 4)
//		{
//			DataNormalizeToImageBuffer(nWidth, nHeight,  nStride, (int *)pData,  pByte , m_nCurrentW, m_nCurrentL);
//		}
//
//		m_SliceBitmap->UnlockBits( &(*pBitmapData) );
//
//		CString strValue;
//		strValue.Format(_T("%d"), m_nCurrentW);
//		m_SliceImageData->AddTagValue(WindowWidth, strValue);
//
//		strValue.Format(_T("%d"), m_nCurrentL);
//		m_SliceImageData->AddTagValue(WindowCenter, strValue);
//
//		RetransferBitmap();
//	
//		UpatePalette();
//
//		m_bModifyImage = true;
//
//		return TRUE;
//	}
//	return FALSE;
//}


void  ImageRenderActor::EnableHighQualityRender(BOOL bHighQuality)
{
	m_bHighQualityRender = bHighQuality;
}

void ImageRenderActor::Render(Graphics* pGraphics, int x, int y, int cx, int cy)
{
	if((m_SliceBitmap != NULL) )
	{
		//初始化绘制质量参数
		if(m_bHighQualityRender)
		{
			pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
			pGraphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);
			pGraphics->SetTextRenderingHint(TextRenderingHintClearTypeGridFit);
		}
		else
		{
			pGraphics->SetSmoothingMode(SmoothingModeHighSpeed);
			pGraphics->SetInterpolationMode(InterpolationModeDefault);
			pGraphics->SetTextRenderingHint(TextRenderingHintSystemDefault);
		}

		//计算绘制矩形大小
		CalculateRenderPosition(x, y, cx, cy);

		pGraphics->DrawImage(&(*m_SliceBitmap), m_ImageRect);
	}
}


//======================================================================================
//计算图像绘制位置,输入坐标为屏幕显示图像区域大小
void ImageRenderActor::CalculateRenderPosition(int x, int y, int cx, int cy)
{
	int nWidth  = m_SliceBitmap->GetWidth();
	int nHeight = m_SliceBitmap->GetHeight();

	nWidth  = m_SliceBitmap->GetWidth();
	nHeight = m_SliceBitmap->GetHeight();

	int nMarginSize=5;
	float k1 = (float)(nWidth)/nHeight;
	float k2 = (float) (cx-2*nMarginSize)/(cy-2*nMarginSize);
	if(k1>=k2)
	{
		m_ImageRect.X      = (int)(x+nMarginSize);
		m_ImageRect.Width  = cx-2.0*nMarginSize;
		m_ImageRect.Height = (nHeight)*(cx-2.0*nMarginSize)/nWidth;
		m_ImageRect.Y      = (int)(y+cy/2-m_ImageRect.Height/2.0);
	}
	else
	{
		m_ImageRect.Y = y+nMarginSize;
		m_ImageRect.Height = cy-2*nMarginSize;
		m_ImageRect.Width = nWidth*(cy-2*nMarginSize)/nHeight;
		m_ImageRect.X = (int)(x+cx/2-m_ImageRect.Width/2);
	}

	RectF ZoomRect;
	//ZoomRect.X      = m_fxoffset*cx + m_ImageRect.X-m_ImageRect.Width*(m_fZoom-1)/2;
	//ZoomRect.Y      = m_fyoffset*cy + m_ImageRect.Y-m_ImageRect.Height*(m_fZoom-1)/2;

	ZoomRect.Width  = m_ImageRect.Width*m_fZoom;
	ZoomRect.Height = m_ImageRect.Height*m_fZoom;

	ZoomRect.X      = m_fxoffset*m_ImageRect.Width  + m_ImageRect.X-m_ImageRect.Width*(m_fZoom-1.0)/2.0;
	ZoomRect.Y      = m_fyoffset*m_ImageRect.Height + m_ImageRect.Y-m_ImageRect.Height*(m_fZoom-1.0)/2.0;

	m_ImageRect = ZoomRect;
}


//屏幕坐标系到显示图像Bitmap坐标系
//20150312新增加了最终图像支持任意角度旋转，所以先把屏幕坐标变换到Rotal后的图像，然后再变换到Rotal之前的图像
void ImageRenderActor::ScreenToImage(double &x, double &y)
{
	if(m_SliceBitmap!=NULL)
	{
		int nWidth  = m_SliceBitmap->GetWidth();
		int nHeight = m_SliceBitmap->GetHeight();
		x = nWidth*(x-m_ImageRect.GetLeft())/m_ImageRect.Width;
		y = nHeight*(y-m_ImageRect.GetTop())/m_ImageRect.Height;
	}
}


//显示图像Bitmap坐标系到屏幕坐标系
//20150312新增加了最终图像支持任意角度旋转，所以先把图像变换到Rotal后的图像，然后再从Rotal后图像变换到屏幕坐标
void ImageRenderActor::ImageToScreen(double &x, double &y)
{
	 if(m_SliceBitmap!=NULL)
	{
		int nWidth  = m_SliceBitmap->GetWidth();
		int nHeight = m_SliceBitmap->GetHeight();
		x = m_ImageRect.GetLeft() + x*m_ImageRect.Width/nWidth;
		y = m_ImageRect.GetTop()  + y*m_ImageRect.Height/nHeight;
	}
}


float ImageRenderActor::GetZoom()
{
	return m_fZoom;
}

BOOL ImageRenderActor::SetZoom(float fZoom)
{
	if(fabs(fZoom-m_fZoom)<0.000001)
	{
		return FALSE;
	}

	if(fZoom<0.2)
		m_fZoom = 0.2f;
	else if(fZoom>5)
		m_fZoom = 5.0f;
	else 
		m_fZoom = fZoom;

	return TRUE;
}


void ImageRenderActor::Zoom(float fZoom)
{
	float fNewZoom = fZoom*m_fZoom;

	SetZoom(fNewZoom);
}	

void ImageRenderActor::OffsetImage(float dx, float dy)
{
	m_fxoffset += dx;
	m_fyoffset += dy;

	//限制范围，否则会知道图像不可见【-1, 1】
	m_fxoffset = fmax(-1.0, fmin(m_fxoffset, 1.0));
	m_fyoffset = fmax(-1.0, fmin(m_fyoffset, 1.0));

}

void  ImageRenderActor::GetOffset(float &fx, float &fy)
{
	fx = m_fxoffset;
	fy = m_fyoffset;
}

BOOL ImageRenderActor::SetOffset(float fx, float fy)
{
	if(fabs(m_fxoffset-fx)<0.000001 && fabs(m_fyoffset-fy)<0.000001)
	{
		return FALSE;
	}

	m_fxoffset = fx;
	m_fyoffset = fy;

	m_fxoffset = fmax(-1.0, fmin(m_fxoffset, 1.0));
	m_fyoffset = fmax(-1.0, fmin(m_fyoffset, 1.0));

	return TRUE;
}

void ImageRenderActor::ResetZoomAndOffset()
{
	m_fxoffset = 0;
	m_fyoffset = 0;
	m_fZoom = 1.0f;

}


RectF ImageRenderActor::GetImageRect()
{
	return m_ImageRect;
}

void  ImageRenderActor::SetImageRect(RectF imageRect)
{
	m_ImageRect = imageRect;
}
