#include "utf8_helper.h"


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

// 将ansi字符串转换成UNICODE
static LPWSTR MultiByteToUnicode(char* szText) {
    int nLength, nLen;
    wchar_t* pBuffer;

    nLen = (int)strlen(szText);
    nLength = MultiByteToWideChar(CP_ACP, 0, szText, nLen, NULL, 0);
    pBuffer = new wchar_t[nLength + 1];
    MultiByteToWideChar(CP_ACP, 0, szText, nLen, (LPWSTR)pBuffer, nLength);
    pBuffer[nLength] = 0;
    return pBuffer;
}

// 将UNICODE字符串转换成UTF8
static LPSTR UnicodeToUTF8(LPWSTR szText) {
    char* pBuffer;
    int nLength;
    int nLen;
    nLen = (int)wcslen(szText);
    // 获取转换后的字符串长度
    nLength = WideCharToMultiByte(CP_UTF8, 0, szText, nLen, NULL, 0, 0, 0);
    int rc = GetLastError();
    pBuffer = new char[nLength + 1];
    // 进行字符转换
    WideCharToMultiByte(CP_UTF8, 0, szText, nLen, pBuffer, nLength, 0, 0);
    pBuffer[nLength] = 0;
    return pBuffer;
}

void TransMultiByte2UTF8(const std::string& from, std::string& to) {
    // 打开一个数据库，如果改数据库不存在，则创建一个名字为databaseName的数据库文件
    char szFileName[MAX_PATH] = {0};
#if defined(WIN32) || defined(WINDOWS) || defined(_WIN32)
    strncpy_s(szFileName, from.c_str(), MAX_PATH);
#else
    strncpy(szFileName, from.c_str(), MAX_PATH);
#endif

    WCHAR* pWideFile = MultiByteToUnicode(szFileName);
    char* pUTF8File = UnicodeToUTF8(pWideFile);
    to = pUTF8File;
}
