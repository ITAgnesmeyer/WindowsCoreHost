#pragma once
#include <string>
#include <vector>

class StringFunctions
{
public:
	StringFunctions();
	~StringFunctions();
    static void WCharToCChar(std::wstring wideString, std::string& resultShortString);
    static std::string WCharToCChar(std::wstring wideString);
    static void CCharToWChar(std::string shortString, std::wstring& resultWideString);
    static std::wstring CCharToWChar(std::string shortString) ;
    static std::wstring StringToUpper(std::wstring strToConvert) ;
    static std::vector<std::wstring> Split(const std::wstring& s, wchar_t separator);
    static std::wstring IntToString(int result);
    static bool StringContains(std::wstring source, std::wstring testString);
    /*static std::wstring GetErrorMessage(int errorCode);*/

};

