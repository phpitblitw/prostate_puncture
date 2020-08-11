#if !defined (DIR_LIGHT_NODE_H)
#define       DIR_LIGHT_NODE_H

#include "gl\gl.h"
#include "SceneNode.h"
#include "LightNodeData.h"

class AFX_EXT_CLASS DirLightNode : public SceneNode
//class DirLightNode : public SceneNode
{
public:
	DirLightNode(void);
   ~DirLightNode(void);

	// 获取当前节点的类型
	virtual int  getType(void){ return DIR_LIGHT_NODE; }

   	// 执行光照明
 	virtual void Draw(void);

public:

	void	TurnOn(void){ m_data.TurnOn(); }
	void	TurnOff(void){ m_data.TurnOff(); }

    void	setColor(float r, float g, float b)
			{ m_data.SetColor(r,g,b); }

    void	getColor(float *r, float *g, float *b)
			{ m_data.GetColor(r,g,b); }

    void	setDiffuseIntensity(float intensity)
			{ m_data.SetDiffuseIntensity(intensity); }

    float	getDiffuseIntensity(void)
			{ return m_data.GetDiffuseIntensity();}

    void	setAmbientIntensity(float intensity)
			{ m_data.SetAmbientIntensity(intensity);}

    float	getAmbientIntensity(void)
			{ return m_data.GetAmbientIntensity();}

    void	setSpecularIntensity(float intensity)
			{ m_data.SetSpecularIntensity(intensity);}

    float	getSpecularIntensity(void)
			{ return m_data.GetSpecularIntensity();}

protected: 

	virtual void CreateLight(void);
protected: 
	LightNodeData m_data;
	GLuint	m_uDisplayList;	// 显示列表

};

#endif






