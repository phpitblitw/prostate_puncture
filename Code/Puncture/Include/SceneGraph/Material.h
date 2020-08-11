// Material.h: interface for the Material class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATERIAL_H__A3C485AA_1F82_4FCD_9F15_ADB2B1EFBE9E__INCLUDED_)
#define AFX_MATERIAL_H__A3C485AA_1F82_4FCD_9F15_ADB2B1EFBE9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Container.h"

class AFX_EXT_CLASS Material : public Container  
{
public:
	Material();
	virtual ~Material();

	void operator=( Material mat);	// copy

	void  getEmissiveColor(float color[3]);
	void  getEmissiveColor(float *r,float *g,float *b);
	void  getSpecularColor(float color[3]);
	void  getSpecularColor(float *r,float *g,float *b);
	void  getDiffuseColor(float color[3]);
	void  getDiffuseColor(float *r, float *g, float *b);
	void  getAmbientColor(float color[3]);
	void  getAmbientColor(float *r,float *g,float *b);
	float getTransparency(void);
	float getShininess(void);

	void setTransparency(float transparency);
	void setShininess(float shininess);
	void setEmissiveColor(float color[3]);
	void setSpecularColor(float color[3]);
	void setDiffuseColor(float color[3]);
	void setAmbientColor(float color[3]);
	void setEmissiveColor(float r,float g,float b);
	void setSpecularColor(float r,float g,float b);
	void setDiffuseColor(float r,float g,float b);
	void setAmbientColor(float r,float g,float b);

   	// 将材质赋给图形对象
	void	Apply(void);

protected:
	float m_ambientColor[4];
	float m_diffuseColor[4];
	float m_specularColor[4];
	float m_emissiveColor[4];
	float m_shininess;
	float m_transparency;

};

#endif // !defined(AFX_MATERIAL_H__A3C485AA_1F82_4FCD_9F15_ADB2B1EFBE9E__INCLUDED_)
