#include "stdafx.h"
#include "ClrRuntimeHelper.h"
#include "coreclrhost.h"
#include "StringFunctions.h"
#include "Parameter.h"
#include "ErrorInfo.h"


void ClrRuntimeHelper::InitPointer()
{
    this->HostHandle = nullptr;
    this->DomainId = 0;
    this->CoreClrModuleHandle = nullptr;
    this->InitializeCoreClr = nullptr;
    this->CreateManagedDelegate = nullptr;
    this->ShutDownCoreClr = nullptr;
    this->MainArgsReturn = nullptr;
    this->MainArgs = nullptr;
    this->Main = nullptr;
}



ClrRuntimeHelper::ClrRuntimeHelper(std::wstring basePath,Parameter* parameter)
{
    InitPointer();
     this->RunTimeBasePath = basePath;
    this->CoreClrLibPath = this->RunTimeBasePath;
    this->CoreClrLibPath.append(this->FS_SEPARATOR);
    this->CoreClrLibPath.append(this->CORECLR_FILE_NAME);
    this->_Parameter = parameter;
}

ClrRuntimeHelper::~ClrRuntimeHelper()
{

    if (this->HostHandle != nullptr)
        this->ShutDownCoreClr(this->HostHandle, this->DomainId);


    if (this->CoreClrModuleHandle != nullptr)
        FreeLibrary(this->CoreClrModuleHandle);


    this->InitPointer();
}

std::wstring ClrRuntimeHelper::GetRuntimeBasePath() const
{
    return this->RunTimeBasePath;
}

std::wstring ClrRuntimeHelper::GetCoreClrLibPath() const
{
    return this->CoreClrLibPath;
}

bool ClrRuntimeHelper::InitClrFunctionPointers()
{
    this->InitializeCoreClr = reinterpret_cast<coreclr_initialize_ptr>(GetProcAddress(this->CoreClrModuleHandle, "coreclr_initialize"));
    this->CreateManagedDelegate = reinterpret_cast<coreclr_create_delegate_ptr>(GetProcAddress(this->CoreClrModuleHandle, "coreclr_create_delegate"));
    this->ShutDownCoreClr = reinterpret_cast<coreclr_shutdown_ptr>(GetProcAddress(this->CoreClrModuleHandle, "coreclr_shutdown"));
    if (this->InitializeCoreClr == nullptr)
    {
        this->LastError = _T("coreclr_initialize not found");
        return false;
    }

    if (this->CreateManagedDelegate == nullptr)
    {
        this->LastError = _T("coreclr_create_delegate not found");
        return false;
    }

    if (this->ShutDownCoreClr == nullptr)
    {
        this->LastError = _T("coreclr_shutdown not found");
        return false;
    }
    return true;
}

bool ClrRuntimeHelper::InitializeClrHost()
{
    std::wstring tpaList;
    
    BuildTpaList(this->RunTimeBasePath.c_str(), _T(".dll"), tpaList);
    if(this->_Parameter->AppPath != this->RunTimeBasePath)
        BuildTpaList(this->_Parameter->AppPath.c_str(), _T(".dll"), tpaList);

    const char* propertyKeys[] = {
        "TRUSTED_PLATFORM_ASSEMBLIES" // Trusted assemblies
    };

    std::string cTpaList;
    StringFunctions::WCharToCChar(tpaList, cTpaList);

    const char* propertyValues[] = {
        cTpaList.c_str()
    };

    std::string charRunTimePath;

    StringFunctions::WCharToCChar(this->RunTimeBasePath, charRunTimePath);

    int hr = this->InitializeCoreClr(charRunTimePath.c_str(),
                                     this->HostName.c_str(),
                                     sizeof(propertyKeys)/sizeof(char*),
                                     propertyKeys,
                                     propertyValues,
                                     &this->HostHandle,
                                     &this->DomainId);

    if(hr < 0)
    {
        std::wstring message = _T("coreclr_initialize failed - status:") + StringFunctions::IntToString(hr);
        message.append(ErrorInfo::GetErrorMessage(hr));
        this->LastError = message;
        return false;
    }
    return true;
}

bool ClrRuntimeHelper::LoadClrLib()
{
    this->CoreClrModuleHandle = LoadLibraryEx(this->CoreClrLibPath.c_str(), nullptr, 0);
    if (this->CoreClrModuleHandle == nullptr)
    {
        this->LastError = _T("ERROR: Failed to load CoreCLR from:\n");
        this->LastError.append(this->CoreClrLibPath);
        return false;
    }
    return true;
}

bool ClrRuntimeHelper::InitClr()
{
    if (!LoadClrLib()) 
        return false;

    if (!InitClrFunctionPointers()) 
        return false;

    if (!InitializeClrHost()) 
        return false;



    return true;
}

std::wstring ClrRuntimeHelper::GetLastError() const
{
    return this->LastError;
}

void ClrRuntimeHelper::PrepareDelegate(void*** managedDelegate)
{
    if(this->_Parameter->UseArgs && this->_Parameter->UseReturn)
    {
        *managedDelegate = reinterpret_cast<void**>(&this->MainArgsReturn);    
    }
    if(this->_Parameter->UseArgs && !this->_Parameter->UseReturn)
    {
        *managedDelegate = reinterpret_cast<void**>(&this->MainArgs);
    }

    if(!this->_Parameter->UseArgs && !this->_Parameter->UseReturn)
    {
        *managedDelegate = reinterpret_cast<void**>(&this->Main);
    }
}

bool ClrRuntimeHelper::CreateDelegate(void*** managedDelegate)
{
    auto entryPointAssemblyName = StringFunctions::WCharToCChar(this->_Parameter->AssemblyNameBase + _T(", Version=") + this->_Parameter->Version);
    auto entryPointTypeName = StringFunctions::WCharToCChar(this->_Parameter->NamespaceName);
    if(!StringFunctions::StringContains(this->_Parameter->NamespaceName,_T(".")))
    {
        entryPointTypeName = StringFunctions::WCharToCChar(this->_Parameter->AssemblyNameBase + _T(".") + this->_Parameter->NamespaceName);
    }
    auto entryPointMethodName = StringFunctions::WCharToCChar(this->_Parameter->MethodName);
    int hr = this->CreateManagedDelegate(this->HostHandle,
                                         this->DomainId, entryPointAssemblyName.c_str(), entryPointTypeName.c_str(),
                                         entryPointMethodName.c_str(), *managedDelegate);

    if(hr < 0)
    {
        std::wstring msgMessage = _T("coreclr_create_delegate failed\n");
        msgMessage.append(_T("AssemblyName:") + StringFunctions::CCharToWChar(entryPointAssemblyName) + _T("\n"));
        msgMessage.append(_T("Entrypoint TypeName:") + StringFunctions::CCharToWChar(entryPointTypeName) + _T("\n"));
        msgMessage.append(_T("Entrypoint MethodName:") + StringFunctions::CCharToWChar(entryPointMethodName) + _T("\n"));
        msgMessage.append(_T("Status:"));
        msgMessage.append(StringFunctions::IntToString(hr));
        msgMessage.append(_T("\n"));
        msgMessage.append(ErrorInfo::GetErrorMessage(hr));
        this->LastError = msgMessage;
        return false;
    }
    return true;
}

bool ClrRuntimeHelper::Execute(int& returnValue)
{
    if(this->_Parameter->UseArgs && this->_Parameter->UseReturn)
    {
        try
        {
            returnValue = this->MainArgsReturn(this->_Parameter->Arguments);
        }
        catch (const std::exception&)
        {
            this->LastError = _T("Error Executing ") + this->_Parameter->NamespaceName + this->_Parameter->MethodName;
            return false;
        }
    }

    if(this->_Parameter->UseArgs && !this->_Parameter->UseReturn)
    {
        try
        {
            this->MainArgs(this->_Parameter->Arguments);
        }
        catch (const std::exception&)
        {
            this->LastError = _T("Error Executing ") + this->_Parameter->NamespaceName + this->_Parameter->MethodName;
            return false;
        }
    }

    if(!this->_Parameter->UseArgs && !this->_Parameter->UseReturn)
    {
        try
        {
            this->Main();
        }
        catch (const std::exception&)
        {
            this->LastError = _T("Error Executing ") + this->_Parameter->NamespaceName + this->_Parameter->MethodName;
            return false;
        }
    }

    return true;
}

bool ClrRuntimeHelper::Run(int& retVal)
{
    void**managedDelegate = nullptr;

    int returnValue = 0;
    PrepareDelegate(&managedDelegate);

    if (!CreateDelegate(&managedDelegate)) 
        return false;
    if (!Execute(returnValue)) 
        return false;
    retVal = returnValue;
    return true;
}


void ClrRuntimeHelper::BuildTpaList(const wchar_t* directory, const wchar_t* extension, std::wstring& tpaList)
{
    // This will add all files with a .dll extension to the TPA list.
       // This will include unmanaged assemblies (coreclr.dll, for example) that don't
       // belong on the TPA list. In a real host, only managed assemblies that the host
       // expects to load should be included. Having extra unmanaged assemblies doesn't
       // cause anything to fail, though, so this function just enumerates all dll's in
       // order to keep this sample concise.
    std::wstring searchPath(directory);
    searchPath.append(FS_SEPARATOR);
    searchPath.append(_T("*"));
    searchPath.append(extension);

    WIN32_FIND_DATAW findData;
    HANDLE fileHandle = FindFirstFile(searchPath.c_str(), &findData);

    if (fileHandle != INVALID_HANDLE_VALUE)
    {
        do
        {
            // Append the assembly to the list
            tpaList.append(directory);
            tpaList.append(FS_SEPARATOR);
            tpaList.append(findData.cFileName);
            tpaList.append(PATH_DELIMITER);

            // Note that the CLR does not guarantee which assembly will be loaded if an assembly
            // is in the TPA list multiple times (perhaps from different paths or perhaps with different NI/NI.dll
            // extensions. Therefore, a real host should probably add items to the list in priority order and only
            // add a file if it's not already present on the list.
            //
            // For this simple sample, though, and because we're only loading TPA assemblies from a single path,
            // and have no native images, we can ignore that complication.
        } while (FindNextFile(fileHandle, &findData));
        FindClose(fileHandle);
    }
}


