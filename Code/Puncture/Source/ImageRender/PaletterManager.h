#pragma once

//单实例，避免重复申请调色板
#include <vector>
#include "ComUtility/singleton.h"
#include "ColorTable.h"
using namespace std;
using namespace fsutility;

namespace ImageRender
{
	class AFX_EXT_CLASS PaletterManager: public Singleton<PaletterManager> 
	{
		friend class Singleton<PaletterManager>;
		PaletterManager(void);
		virtual ~PaletterManager(void);

	public:
		ColorPalette* GetGrayColorPalette();
		ColorPalette* GetOverlayGrayColorPalette();
		ColorPalette* GetNormalColorPalette(ColorTableName nColorName);
	protected:
		vector<unsigned char> m_GrayPaletteBuffer;
		vector<unsigned char> m_OverlayPaletteBuffer;
		vector<unsigned char> m_TotalPaletteBuffer;
		int  m_nOnePaletterSize;
	protected:
		void InitTotalPaletteBuffer();
	};
}
