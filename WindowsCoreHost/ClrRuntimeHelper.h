#pragma once
#include <string>
#include "coreclrhost.h"
#include "Parameter.h"


class Parameter;

class ClrRuntimeHelper
{
    typedef int (*main_args_return_ptr)(char* argv[]);
    typedef void (*main_args_ptr)(char* argv[]);
    typedef void (*main_ptr)();
public:
    explicit ClrRuntimeHelper(std::wstring basePath,Parameter* parameter);
    
    
    
    ~ClrRuntimeHelper();

    std::wstring GetRuntimeBasePath() const;


    std::wstring GetCoreClrLibPath() const;
   
    bool InitClr();
    std::wstring GetLastError() const;
    
    bool Run(int& retVal);
private:
    Parameter* _Parameter;
    std::string HostName="SimpleHost";
    bool UseReturn{};
    bool UseArgs{};
    std::wstring RunTimeBasePath;
    std::wstring CoreClrLibPath;
    HMODULE CoreClrModuleHandle{};
    void* HostHandle{};
    unsigned int DomainId{};
    std::wstring LastError;
    coreclr_initialize_ptr InitializeCoreClr{};
    coreclr_create_delegate_ptr CreateManagedDelegate{};
    coreclr_shutdown_ptr ShutDownCoreClr{};
    main_args_return_ptr MainArgsReturn{};
    main_args_ptr MainArgs{};
    main_ptr Main{};
    const std::wstring FS_SEPARATOR = _T("\\");
    const std::wstring PATH_DELIMITER = _T(";");
    const std::wstring CORECLR_FILE_NAME = _T("coreclr.dll");
     bool InitClrFunctionPointers();
    bool InitializeClrHost();
    bool LoadClrLib();
    void InitPointer();
    void PrepareDelegate(void*** managedDelegate);
    bool CreateDelegate(void*** managedDelegate);
    bool Execute(int& returnValue);
    void BuildTpaList(const wchar_t* directory, const wchar_t* extension, std::wstring& tpaList);
};

