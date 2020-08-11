#include "StdAfx.h"
#include "GDIEncode.h"
#include <GdiPlus.h>
#pragma comment(lib,"gdiplus.lib")

using namespace fsutility;

GDIEncode::GDIEncode(void)
{
}

GDIEncode::~GDIEncode(void)
{
}


bool GDIEncode::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num, size;
	Gdiplus::GetImageEncodersSize(&num, &size);
	Gdiplus::ImageCodecInfo* pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);
	bool found = false;
	for (UINT ix = 0; !found && ix < num; ++ix) {
		if (0 == _wcsicmp(pImageCodecInfo[ix].MimeType, format) == 0) {
			*pClsid = pImageCodecInfo[ix].Clsid;
			found = true;
		}
	}
	free(pImageCodecInfo);
	return found;
}
