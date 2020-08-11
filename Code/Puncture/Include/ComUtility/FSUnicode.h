#pragma once

namespace fsutility
{
class AFX_EXT_CLASS CFSUnicode
{
public:
	CFSUnicode(void);
	~CFSUnicode(void);

	void SetString(LPCSTR psASting);
	void SetString(LPCWSTR psWString);

	LPCSTR GetStringA(void);
	LPCWSTR GetStringW(void);

protected:
	wchar_t* c2w(wchar_t *pwstr,const char *pcstr);
	char* w2c(char *pcstr,const wchar_t *pwstr, size_t len);

protected:
	//LPCWSTR m_pWString;
	//LPCSTR m_pAString;
	CStringW m_strStringW;
	CStringA m_strStringA;
};

}
