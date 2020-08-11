#pragma once
#include <string>

class AFX_EXT_CLASS FsStringConvert
{
public:
	FsStringConvert(void);
	virtual ~FsStringConvert(void);

	//UTF-8
	static std::wstring MyUstringToWString(std::string &source);
	static std::wstring MyUstringToWString(const char* source);

	static std::wstring MyAstringToWString(std::string &source);
	static std::wstring MyAstringToWString(const char* source);
	static std::string  MyWstringToAString(std::wstring &source);
	static std::string  MyWstringToAString(LPCWSTR source);
	static std::string  GBKToUTF8(const std::string& strGBK) ;
	static std::string  UTF8ToGBK(const std::string& strUTF8) ;
};
