#pragma once

namespace fsutility
{
class AFX_EXT_CLASS CDiskManager
{
public:
	CDiskManager(void);
	~CDiskManager(void);

	//获取磁盘的空闲大小，单位是 M
	static float GetDiscSpaceFree(CString path);
};

}
