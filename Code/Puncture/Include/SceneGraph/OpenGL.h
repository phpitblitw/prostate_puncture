// OpenGL.h: interface for the COpenGL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENGL_H__5AC1525A_2B06_4EB9_9A29_283A9AF2C826__INCLUDED_)
#define AFX_OPENGL_H__5AC1525A_2B06_4EB9_9A29_283A9AF2C826__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <gl/gl.h>
#include <gl/glu.h>
//#include <gl/glaux.h>

class AFX_EXT_CLASS COpenGL  
{
public:
	COpenGL();
	virtual ~COpenGL();

	HWND	m_hWnd;
	HDC		m_hDC;
	HGLRC	m_hRC;

	void    DestroyOpenGL();

	virtual BOOL initOpenGL(HWND hWnd);
	virtual	void OpenGLSize(int cx, int cy);
	virtual void Draw()=0;

private:
	BOOL    SetWindowPixelFormat();
    BOOL    CreateViewGLContext();
};

#endif // !defined(AFX_OPENGL_H__5AC1525A_2B06_4EB9_9A29_283A9AF2C826__INCLUDED_)
