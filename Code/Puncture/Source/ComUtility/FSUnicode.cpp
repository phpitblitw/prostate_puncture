#include "StdAfx.h"
#include "FSUnicode.h"

using namespace fsutility;

CFSUnicode::CFSUnicode(void)
{
	m_strStringW = L"";
	m_strStringA = "";
}

CFSUnicode::~CFSUnicode(void)
{
}

void CFSUnicode::SetString(LPCSTR psASting)
{
	m_strStringA = psASting;
	wchar_t wchar[1024]={0};
	c2w(wchar, psASting);
	m_strStringW =wchar;
}

void CFSUnicode::SetString(LPCWSTR psWString)
{
	m_strStringW = psWString;
	char ch[1024]={0};
	w2c(ch, m_strStringW, 1000);
	m_strStringA = ch;
}

LPCSTR CFSUnicode::GetStringA(void)
{
	return m_strStringA;
}
LPCWSTR CFSUnicode::GetStringW(void)
{
	return m_strStringW;
}

char* CFSUnicode::w2c(char *pcstr,const wchar_t *pwstr, size_t len)
{
	int nlength= (int)( wcslen(pwstr) );

	unsigned int nbytes = WideCharToMultiByte( 0, 0, 	pwstr, nlength, nullptr,0,nullptr, nullptr ); 

	if( nbytes > (int)len)
	{
		nbytes = (int)len;
	}

	WideCharToMultiByte( 0, 0,pwstr, nlength+1, pcstr,nbytes +1, nullptr, nullptr );

	return pcstr ;

}

wchar_t* CFSUnicode::c2w(wchar_t *pwstr,const char *pcstr)
{
	MultiByteToWideChar( CP_ACP, 0, pcstr,
		(int)strlen(pcstr) + 1, pwstr,   
		(int)strlen(pcstr) + 1 );

	return pwstr;
}