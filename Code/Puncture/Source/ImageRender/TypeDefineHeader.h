#pragma  once

#include <memory>

namespace ImageRender
{
	class ImageRenderActor;
	typedef std::shared_ptr<ImageRenderActor> ImageRenderActorPtr;

	class BaseBitmapRender;
	typedef std::shared_ptr<BaseBitmapRender> BaseBitmapRenderPtr;
	
	typedef std::shared_ptr<Gdiplus::Pen> ThPenPtr;
	typedef std::shared_ptr<Gdiplus::Font> ThFontPtr;
	typedef std::shared_ptr<Gdiplus::SolidBrush> ThSolidBrushPtr;
	typedef std::shared_ptr<Gdiplus::Bitmap>     ThBitmapPtr;
	typedef std::shared_ptr<Gdiplus::Graphics>   ThGraphicsPtr;
}