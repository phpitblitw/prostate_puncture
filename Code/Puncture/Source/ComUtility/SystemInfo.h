#pragma once

namespace fsutility
{
	class AFX_EXT_CLASS SystemInfo
	{
	public:
		SystemInfo(void);
		virtual ~SystemInfo(void);

		int GetCPUProcessorNum();
	};

}