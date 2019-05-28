#include "stdafx.h"
#include "StringFunctions.h"
#include <algorithm>


StringFunctions::StringFunctions()
= default;


StringFunctions::~StringFunctions()
= default;


void StringFunctions::WCharToCChar(std::wstring wideString, std::string& resultShortString)
{
    resultShortString.clear();
    resultShortString.assign(wideString.begin(), wideString.end());
}

std::string StringFunctions::WCharToCChar(std::wstring wideString)
{
    std::string retString;
    WCharToCChar(wideString, retString);
    return retString;
}

void StringFunctions::CCharToWChar(std::string shortString, std::wstring& resultWideString)
{
    resultWideString.clear();
    resultWideString.assign(shortString.begin(), shortString.end());
}

std::wstring StringFunctions::CCharToWChar(std::string shortString)
{
    std::wstring retString;
    CCharToWChar(shortString, retString);
    return retString;
}

std::wstring StringFunctions::StringToUpper(std::wstring strToConvert)
{
    std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::toupper);
    return strToConvert;
}

std::vector<std::wstring> StringFunctions::Split(const std::wstring& s, wchar_t separator)
{
    std::vector<std::wstring> output;
    std::wstring::size_type prev_pos = 0, pos = 0;
    while ((pos = s.find(separator, pos)) != std::string::npos)
    {
        std::wstring substring(s.substr(prev_pos, pos - prev_pos));
        output.push_back(substring);
        prev_pos = ++pos;
    }
    output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word
    return output;
}

std::wstring StringFunctions::IntToString(int result)
{
    wchar_t buf[20];
    _itow_s(result, buf, 10);
    std::wstring retValue(buf);
    return retValue;
}

bool StringFunctions::StringContains(std::wstring source, std::wstring testString)
{
    //s1.find(s2) != std::string::npos
    return source.find(testString) != std::wstring::npos;
}

