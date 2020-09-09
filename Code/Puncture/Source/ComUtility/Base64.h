#pragma once
#include <string>
class AFX_EXT_CLASS Base64
{
public:
	Base64(void);
	~Base64(void);
	
	static std::string Encode(const unsigned char* Data,int DataByte);
    /*解码
    DataByte
        [in]输入的数据长度,以字节为单位
    OutByte
        [out]输出的数据长度,以字节为单位,请不要通过返回值计算
        输出数据的长度
    */
	 static std::string Decode(const char* Data,int DataByte,int& OutByte);
};

