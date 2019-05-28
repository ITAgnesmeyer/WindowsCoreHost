#pragma once
#include <string>

class Parameter
{
public:
    explicit Parameter(HINSTANCE hInstance);
    ~Parameter();
    UINT IdAssemblyName=0;
    UINT IdMethodName=0;
    UINT IdVersion=0;
    UINT IdNamespaceName=0;
    UINT IdWithReturn=0;
    UINT IdWithArgs=0;
    std::wstring AppPath;
    std::wstring AssemblyName;
    std::wstring AssemblyNameBase;
    std::wstring MethodName;
    std::wstring Version;
    std::wstring NamespaceName;
    bool UseArgs=false;
    bool UseReturn=false;
    bool FillData();
    std::wstring GetLastError() const;
    char** Arguments = nullptr;
private:
    const std::wstring TURE_TEST = _T("TRUE");
    HINSTANCE Instance;
    std::wstring WithReturn;
    std::wstring WithArgs;
    std::wstring LastError;
    bool TestResData();
    bool TestIds();

};

