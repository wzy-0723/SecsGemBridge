#include "TextUtil.h"
#ifdef WIN32
#include <windows.h>
#elif __linux__
#include <string>
#include <locale>
#include <codecvt>
#include <cstring>   // C++ 推荐
#endif
namespace why
{
    std::string UTF8ToLocal(const std::string& strUTF8)
    {
    #ifdef WIN32
        return Unicode2Ascii(UTF8Decode(strUTF8));
    #else
        return strUTF8;
    #endif
    }

    std::string LocalToUTF8(const std::string& strLocal)
    {
    #ifdef WIN32
        return UTF8Encode(Ascii2Unicode(strLocal));
    #else
        return strLocal;
    #endif
    }

    std::string UTF8Encode(const std::wstring& strInput)
    {
    #if defined(WIN32) || defined(WIN64)
        std::string strOutput;

        // LPSTR 是 Windows API 中一个非常常见的类型别名（typedef），
        // 全称是 Long Pointer to String（长指针指向字符串），
        // 它本质上是一个指向以 NULL 结尾的 ANSI 字符数组（即 C 风格字符串）的指针。
        LPSTR pstrRes = nullptr;


        //windows下标准的转宽字符为utf-8做法，先获取长度再转化
        int nLen = ::WideCharToMultiByte(CP_UTF8, 0, strInput.c_str(), strInput.size(), 0, 0, nullptr, nullptr);

        if (nLen > 0)
        {
            // pstrRes 分配内存时写 nLen + 1 而不是 nLen，最根本的原因是为了容纳字符串的结束符 \0（NULL 终止符）。
            // C/C++ 中处理以 null 结尾的字符串（null-terminated string）时最重要、最容易犯错的规则之一。
            //
            // 无论字符串本身有多长，只要你想把它当成 C 风格字符串（能被 strlen、strcpy、printf 等函数安全使用），
            // 就必须额外留出 1 个字节存放 \0。
            pstrRes = new CHAR[nLen + 1];
            memset(pstrRes, 0, nLen + 1);
        }
        else
        {
            return "";
        }

        ::WideCharToMultiByte(CP_UTF8, 0, strInput.c_str(), strInput.size(), pstrRes, nLen, nullptr, nullptr);
        pstrRes[nLen] = 0;
        strOutput = pstrRes;
        delete[]pstrRes;

        return strOutput;
    #else
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        std::string										 strOutput = conv.to_bytes(strInput).c_str();

        return strOutput;
    #endif
    }

    std::wstring UTF8Decode(const std::string& strInput)
    {
    #if defined(WIN32) || defined(WIN64)
        std::wstring strOutput;
        wchar_t* pstrRes = nullptr;

        int nLen = ::MultiByteToWideChar(CP_UTF8, 0, strInput.c_str(), strInput.size(), nullptr, 0);
        if (nLen > 0)
        {
            pstrRes = new wchar_t[nLen + 1];
            memset(pstrRes, 0, nLen + 1);
        }
        else
        {
            return L"";
        }

        ::MultiByteToWideChar(CP_UTF8, 0, strInput.c_str(), strInput.size(), pstrRes, nLen);
        pstrRes[nLen] = 0;
        strOutput = pstrRes;
        delete[]pstrRes;

        return strOutput;
    #else
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        std::wstring    strOutput = L"";

        try {
            strOutput = conv.from_bytes(strInput);
        }
        catch (const std::range_error&) {
            strOutput = conv.from_bytes(strInput.substr(0, conv.converted()));
        }

        return strOutput;
    #endif
    }


    std::wstring Ascii2Unicode(const std::string& strInput)
    {

    #if defined(WIN32) || defined(WIN64)
        int nLength = ::MultiByteToWideChar(CP_ACP, 0, strInput.c_str(), -1, nullptr, 0);
        if (nLength <= 0) return std::wstring(L"");
        wchar_t* szbuffer = new wchar_t[nLength + 2];
        ::MultiByteToWideChar(CP_ACP, 0, strInput.c_str(), -1, szbuffer, nLength);
        std::wstring strnew = szbuffer;
        delete[] szbuffer;
        return strnew;
    #else
        setlocale(LC_CTYPE, "");
        std::size_t iWLen = mbstowcs(0, strInput.c_str(), 0);
        wchar_t* wStr = new wchar_t[iWLen + 1];
        memset(wStr, 0, sizeof(wchar_t) * (iWLen + 1));
        mbstowcs(wStr, strInput.c_str(), iWLen + 1);
        std::wstring ret(wStr);
        delete[] wStr;
        return ret;
    #endif

    }


    std::string Unicode2Ascii(const std::wstring& strInput)
    {
    #if defined(WIN32) || defined(WIN64)
        int nLength = ::WideCharToMultiByte(CP_OEMCP, 0, strInput.c_str(), -1, nullptr, 0, nullptr, nullptr);
        if (nLength <= 0) return std::string("");
        char* szbuffer = new char[nLength + 2];
        memset(szbuffer, 0, nLength + 2);
        int len = ::WideCharToMultiByte(CP_OEMCP, 0, strInput.c_str(), -1, szbuffer, nLength, nullptr, nullptr);
        if (len > 0) {
            szbuffer[len] = 0;
        }
        std::string strnew = szbuffer;
        delete[] szbuffer;
        return strnew;
    #else
        setlocale(LC_CTYPE, "");
        std::size_t iLen = wcstombs(nullptr, strInput.c_str(), 0);
        char* str = new char[iLen + 1];
        memset(str, 0, sizeof(char) * (iLen + 1));
        wcstombs(str, strInput.c_str(), iLen + 1);
        std::string ret(str);
        delete[] str;
        return ret;
    #endif
    }
}