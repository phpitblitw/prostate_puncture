#pragma once
#include "Opencv2/opencv.hpp"
#include "TypeDefineHeader.h"

namespace ImageRender
{
	class AFX_EXT_CLASS ImageRenderActor
	{
	public:
		ImageRenderActor();
		virtual ~ImageRenderActor(void);

	public:
		//绘制图像
		virtual void Render(Graphics* pGraphics, int x, int y, int cx, int cy);

		//virtual void LoadImageFile(CString strFilePath);
		virtual void LoadImageFile(cv::Mat &SrcData);

		virtual void CreateGrayImageRender(int cx ,int cy);
		virtual void CreateRGBImageRender(int cx ,int cy);
		
		virtual ThBitmapPtr GetSliceBitmap();
	public:
		
		//图像缩放和平移
		virtual float GetZoom();
		virtual BOOL  SetZoom(float fZoom);
		virtual void  Zoom(float fZoom);
		virtual void  OffsetImage(float dx, float dy); //相对偏移量

		virtual void  GetOffset(float &fx, float &fy);
		virtual BOOL  SetOffset(float fx, float fy);

		virtual void  ResetZoomAndOffset();
		virtual RectF GetImageRect();
		virtual void  SetImageRect(RectF imageRect);

		//屏幕坐标和图像坐标转换（主要图像缩放、平移，以及图像和窗口等比例处理）
		virtual void  ScreenToImage(double &x, double &y);
		virtual void  ImageToScreen(double &x, double &y);

		void  EnableHighQualityRender(BOOL bHighQuality);

	protected:
		//计算图像绘制位置
		virtual void  CalculateRenderPosition(int x, int y, int cx, int cy);
		static void Mat2Bitmap(cv::Mat &m, Gdiplus::Bitmap *pBitmap);
	protected:
		//Bitmap for render to window;
		ThBitmapPtr m_SliceBitmap;

		float m_fZoom;
		float m_fxoffset;
		float m_fyoffset;

		RectF m_ImageRect;
		BOOL   m_bHighQualityRender;
		bool   m_bModifyImage;
	};
	
}