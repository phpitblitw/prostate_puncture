// GLFont.h: interface for the CGLFont class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLFONT_H__1F38C981_2654_11D3_955D_8422F5C00000__INCLUDED_)
#define AFX_GLFONT_H__1F38C981_2654_11D3_955D_8422F5C00000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Enum.h"

class AFX_EXT_CLASS CGLFont  
{
public:
	CGLFont();
	virtual ~CGLFont();

	void   SetFontType(GLFontType FontType){m_FontType = FontType;};
	void   GetFontType(GLFontType* FontType){*FontType = m_FontType;};

    void   SetFontSize(double dThick){m_dThick = dThick;};
    void   GetFontSize(double* dThick){*dThick = m_dThick;};

	BOOL CreateFont(CDC* pDrawDC, char* fontname);
	UINT GetTextListID();

//Parameters
private:
	GLFontType   m_FontType;     //font type
    double       m_dThick;       //the letters thick 

	UINT		 m_TextListID;     //the ID of the displaylist 

};

#endif // !defined(AFX_GLFONT_H__1F38C981_2654_11D3_955D_8422F5C00000__INCLUDED_)
