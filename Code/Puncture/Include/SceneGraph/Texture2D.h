// Texture2D.h: interface for the Texture2D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTURE2D_H__EB9C8543_4FB1_48F9_B2AB_448BFF397D9C__INCLUDED_)
#define AFX_TEXTURE2D_H__EB9C8543_4FB1_48F9_B2AB_448BFF397D9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Container.h"

#include <gl/gl.h>
#include <gl/glu.h>
//#include <gl/glaux.h>

class AFX_EXT_CLASS Texture2D : public Container  
{
public:
	Texture2D(void);
	virtual ~Texture2D(void);

	// 已知纹理数据，设置纹理图像
	void setImage(int components, int width, int height, void * texImage, int level, GLenum datatype=GL_UNSIGNED_BYTE);
	// 已知纹理数据，纹理数据为灰度图象，设置纹理图像
	void setGrayImage(int width , int height , void * texImage ,int level , GLenum datatype=GL_UNSIGNED_BYTE);

	// 执行纹理映射
	virtual void Draw(void);

	// iMode : GL_CLAMP or GL_REPEAT
	void setWrapS(int iMode);
	int  getWrapS(void) { return m_iWrapS;}

	// iMode : GL_CLAMP or GL_REPEAT
	void setWrapT(int iMode);
	int  getWrapT(void) { return m_iWrapT;}
	
	// GL_NEAREST or GL_LINEAR
	void setMinFilter(int iMode);
	int  getMinFilter(void) { return m_iMinFilter; }

	// GL_NEAREST or GL_LINEAR
	void setMagFilter(int iMode);
	int  getMagFilter(void) { return m_iMagFilter; }

	//dataType: default GL_UNSIGNED_BYTE
	GLenum getDataType(void) { return m_DataType;}

	// iMode : GL_DECAL or GL_MODULATE
	void setEnvMode(int iMode);
	int  getEnvMode(void){ return m_iEnvMode;}

protected:
    int		m_iWrapS;		// GL_CLAMP   or GL_REPEAT
    int		m_iWrapT;		// GL_CLAMP	  or GL_REPEAT
    int		m_iMinFilter;	// GL_NEAREST or GL_LINEAR
    int		m_iMagFilter;	// GL_NEAREST or GL_LINEAR
	int		m_iEnvMode;		// GL_DECAL or GL_MODULATE

	BOOL	m_bStateChanged;// 数据改变的标志
	GLuint	m_uDisplayList;	// 显示列表
	GLuint	m_uBindIndex;	// for glGenTextures 
	GLenum  m_DataType;     //纹理数据类型

	/*---------- Texture2D image infomation of "iLevel 0" ----------*/
	
	int		m_iComponents;	// Must be 3, or 4. 
	int		m_iWidth; 		// Must be 2^n for some integer n. 
	int		m_iHeight;		// Must be 2^n for some integer n. 
};

#endif // !defined(AFX_TEXTURE2D_H__EB9C8543_4FB1_48F9_B2AB_448BFF397D9C__INCLUDED_)
