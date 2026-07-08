#pragma once
#include <iostream>

namespace why
{
    /**
    * @brief 将UTF-8编码的字符称转变为本地编码的字符
    * @param strUTF8 UTF-8编码的字符
    * @return 返回本地编码的字符
    */
    std::string UTF8ToLocal(const std::string& strUTF8);

    /**
    * @brief 将本地编码的字符转变为UTF-8编码的字符
    * @param strLocal 本地编码的字符
    * @return 返回UTF-8编码的字符
    */
    std::string LocalToUTF8(const std::string& strLocal);


    /**
    * @brief 将宽字符串转换成UTF-8的格式
    * @param strInput 等待转换的字符串
    * @return 返回转码后得结果
    */
    std::string UTF8Encode(const std::wstring& strInput);

    /**
    * @brief 将UTF-8的格式的字符串转换成当前格式
    * @param strInput 等待转换的字符串
    * @return 返回转码后得结果
    */
    std::wstring UTF8Decode(const std::string& strInput);

    /**
    * @brief 将当前字符串转换成unicode-16的格式
    * @param strInput 等待转换的字符串
    * @return 转换后得结果
    */
    std::wstring Ascii2Unicode(const std::string& strInput);

    /**
    * @brief 将当前字符串转换成Ascii的格式
    * @param strInput 等待转换的字符串
    * @return 转换后得结果
    */
    std::string Unicode2Ascii(const std::wstring& strInput);
}

