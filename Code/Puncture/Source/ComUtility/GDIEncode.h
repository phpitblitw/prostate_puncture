#pragma once

namespace fsutility
{
	class AFX_EXT_CLASS GDIEncode
	{
	public:
		GDIEncode(void);
		virtual ~GDIEncode(void);

		static bool GetEncoderClsid(const WCHAR* format, CLSID* pClsid);


	};


}