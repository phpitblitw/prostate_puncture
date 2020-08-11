#pragma once

class  CText2DNode
{
public:
	CText2DNode(void);
	CText2DNode(UINT ID, int x, int y, char *p);
	CText2DNode(const CText2DNode &node);
	~CText2DNode(void);
public:
	void MoveToPos(int x, int y);
	void ScaleText(float fScale);
	void SetText2DColor(float red, float green, float blue);
	void SetText2DText(char *p);
	CText2DNode& operator = (const CText2DNode &node);

	void DrawText(UINT TextListID);
	UINT GetID();

private:
	UINT      m_ID;
	int		  m_iStrX;   //文字显示的初始位置
	int		  m_iStrY;
	float     m_TextColorRed,m_TextColorGreen,m_TextColorBlue;
	float     m_fScale;
	CString   m_strText;      //the text for display
};
