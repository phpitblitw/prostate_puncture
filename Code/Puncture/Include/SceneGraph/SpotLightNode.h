#if !defined (SPOT_LIGHT_NODE_H)
#define       SPOT_LIGHT_NODE_H

#include "gl\gl.h"
#include "SceneNode.h"
#include "PointLightNode.h"

class AFX_EXT_CLASS SpotLightNode : public SceneNode
//class SpotLightNode : public SceneNode
{
public:
	SpotLightNode(void);
   ~SpotLightNode(void);

	// 获取当前节点的类型
	virtual int  getType(void){ return SPOT_LIGHT_NODE; }

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

    void	setAttenuation(float a0, float a1, float a2)
			{ m_data.SetAttenuation(a0, a1, a2);}

    void	getAttenuation(float *a0, float *a1, float *a2)
			{ m_data.GetAttenuation(a0, a1, a2);}

    void	setDirection(float nx, float ny, float nz)
			{ m_data.SetDirection(nx, ny, nz);}

    void	getDirection(float *nx, float *ny, float *nz)
			{ m_data.GetDirection(nx, ny, nz);}

	void	setCutoffAngle(float angle)
			{ m_data.SetCutoffAngle(angle);}

	float	getCutoffAngle(void)
			{ return m_data.GetCutoffAngle();}

protected: 

	virtual void CreateLight(void);
protected: 
	LightNodeData m_data;
	GLuint	m_uDisplayList;	// 显示列表
};

#endif