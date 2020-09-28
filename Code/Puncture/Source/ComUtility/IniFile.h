#pragma once
#include <string>

namespace fsutility
{
    class AFX_EXT_CLASS CIniFile
    {
    public:
        enum OpenMode
        {
            modeRead = 0x00000001,
            modeWrite= 0x00000002,
            modeReadWrite = 0x00000003
        };

    public:
        CIniFile();
        CIniFile(const CString& strFilePath);
        BOOL Open(const CString& strFilePath, OpenMode mode = modeReadWrite);
        virtual ~CIniFile(void);

        void Move2Section(const CString& strSecName);

        BOOL WriteKey(const CString& strKeyName, const CString& strValue) const;
        BOOL WriteKey(const CString& strKeyName, int nVal) const;
        BOOL WriteKey(const CString& strKeyName, float fVal) const;
        BOOL WriteKey(const CString& strKeyName, unsigned int nVal) const;
        BOOL WriteKey(const CString& strKeyName, LPVOID lpStruct, UINT uSizeStruct) const ;

        /**
         * @Description: read key from ini file.
         * @param strValInOut may represents the default value, also the value out.
         */
        void ReadKey(const CString& strKeyName, CString& strValInOut) const;

		void ReadKey(const CString& strKeyName, std::string& strValInOut) const;

        void ReadKey(const CString& strKeyName, int& nValInOut) const;

        void ReadKey(const CString& strKeyName, unsigned int& nValInOut) const;

		void ReadKey(const CString& strKeyName, DWORD& nValInOut) const;

        void ReadKey(const CString& strKeyName, float& fValInOut) const;

		void ReadKey(const CString& strKeyName, double& dValInOut) const;
        
        BOOL ReadKey(const CString& strKeyName, LPVOID lpStructOut,
            UINT uSizeStruct) const;
        /**
         * @Description: Replace current section.
         */
        BOOL WriteSection(const CString& strKey_Value) const;

        BOOL CanRead() const;
        BOOL CanWrite() const;
        BOOL CanReadWrite() const;

    private:
        CString m_pStrSecName;
        CString m_pStrFileName;

        unsigned int m_nOpenFlags;
    };
}