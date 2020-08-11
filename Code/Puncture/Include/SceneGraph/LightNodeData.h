#if !defined (LIGHT_NODE_DATA_H)
#define       LIGHT_NODE_DATA_H

#include "Enum.h"

class AFX_EXT_CLASS LightNodeData
{
public:
	LightNodeData(void);
   ~LightNodeData(void);

	void	TurnOn(void){ m_bTurnOn=TRUE; m_bStateChanged=TRUE; }
	void	TurnOff(void){ m_bTurnOn=FALSE; m_bStateChanged=TRUE; }

    void	SetColor(float r, float g, float b);
    void	GetColor(float *r, float *g, float *b);

    void	SetDiffuseIntensity(float intensity);
    float	GetDiffuseIntensity(void);

    void	SetAmbientIntensity(float intensity);
    float	GetAmbientIntensity(void);

    void	SetSpecularIntensity(float intensity);
    float	GetSpecularIntensity(void);

    void	SetDirection(float nx, float ny, float nz);
	void	GetDirection(float *nx, float *ny, float *nz);

	void	SetCutoffAngle(float angle);
	float	GetCutoffAngle(void);

    void	SetAttenuation(float a0, float a1, float a2);
    void	GetAttenuation(float *a0, float *a1, float *a2);

public:
	int		m_iIndex;		// 光源的索引号
	BOOL	m_bStateChanged;// 数据改变的标志
	BOOL	m_bTurnOn;
	float	m_fColor[3];
	float	m_fDiffuseIntensity;
	float	m_fAmbientIntensity;
	float	m_fSpecularIntensity;	// white color intensity
	float	m_fDirection[3];
	float	m_fCutoffAngle;
	float	m_fAttenuation[3];

};

#endif

