#pragma once

namespace fsutility
{
    class AFX_EXT_CLASS FSStringHelper
    {
    public:
        FSStringHelper(void);
        ~FSStringHelper(void);

		static CString ToCString(int val)
		{
			CString str;
			str.Format(_T("%d"), val);
			return str;
		}

		static CString ToCString(unsigned int val)
		{
			CString str;
			str.Format(_T("%u"), val);
			return str;
		}

		static CString ToCString(float val)
		{
			CString str;
			str.Format(_T("%f"), val);
			return str;
		}

		static CString ToCString(double val)
		{
			CString str;
			str.Format(_T("%g"), val);
			return str;
		}

		static BOOL IsNumberStr(const CString& str)
		{
			for (int i=0; i<str.GetLength(); i++)
			{
				if (str[i] >= _T('0') && str[i] <= _T('9'))
					continue;
				return FALSE;
			}

			return str.IsEmpty() ? FALSE : TRUE;
		}

		static BOOL IsLetterStr(const CString& str)
		{
			for (int i=0; i<str.GetLength(); i++)
			{
				if ((str[i] >= _T('a') && str[i] <= _T('z')) ||
					(str[i] >= _T('A') && str[i] <= _T('Z')))
					continue;
				return FALSE;
			}

			return str.IsEmpty() ? FALSE : TRUE;
		}

		static BOOL IsLetterOrNumStr(const CString& str)
		{
			for (int i=0; i<str.GetLength(); i++)
			{
				if ((str[i] >= _T('a') && str[i] <= _T('z')) ||
					(str[i] >= _T('A') && str[i] <= _T('Z')) ||
					(str[i] >= _T('0') && str[i] <= _T('9')))
					continue;
				return FALSE;
			}

			return str.IsEmpty() ? FALSE : TRUE;
		}
    };

}
