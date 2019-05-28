// SampleHost.cpp : Definiert den Einstiegspunkt für die Anwendung.
//

#include "stdafx.h"
#include "WindowsCoreHost.h"
#include <cstdio>
#include <string>
#include <vector>
#include <Windows.h>
#include <algorithm>
#include <combaseapi.h>
#include "coreclrhost.h"
#include "StringFunctions.h"
#include "Parameter.h"
#include "ClrRuntimeHelper.h"
#include "FolderInfo.h"
#define FS_SEPARATOR _T("\\")
#define PATH_DELIMITER _T(";")
#define CORECLR_FILE_NAME _T("coreclr.dll")


// Function pointer types for the managed call and callback
typedef int (*report_callback_ptr)(int progress);
typedef char* (*doWork_ptr)(const char* jobName, int iterations, int dataSize, double* data,
                            report_callback_ptr callbackFunction);

int ReportProgressCallback(int progress);

void ShowErrorMessage(std::wstring message)
{
    MessageBox(nullptr, message.c_str(),_T("ERROR"),MB_OK | MB_ICONERROR);
}

void ShowErrorMessage(std::wstring message, int result)
{
    
    message.append(StringFunctions::IntToString(result));
    ShowErrorMessage(message);
}

void FillArgs(std::vector<std::wstring> params, char**& args)
{
    args = new char*[params.size()];
    for (int i = 0; i < params.size(); i++)
    {
        std::string str;
        str = StringFunctions::WCharToCChar(params[i]);
        args[i] = const_cast<char*>(str.c_str());
    }
}

void GetAppPathCleaned(std::wstring cmdLine, std::vector<std::wstring> parameter, std::wstring& cmdLineFirst)
{
    cmdLineFirst = cmdLine;
    if (!parameter.empty())
    {
        cmdLineFirst = parameter[0];
    }
    auto pos = cmdLineFirst.find_last_of(L'\"');
    if (pos > 0)
    {
        cmdLineFirst = cmdLineFirst.substr(1, pos - 1);
    }


    wchar_t runtimePath[MAX_PATH];
    GetFullPathName(cmdLineFirst.c_str(), MAX_PATH, runtimePath, nullptr);
     wchar_t* last_slash = wcsrchr(runtimePath, FS_SEPARATOR[0]);
    if (last_slash != nullptr)
        *last_slash = 0;
    cmdLineFirst = runtimePath;


}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow)
{

   
    Parameter params(hInstance);
    params.IdAssemblyName = IDS_MANAGED_ASSEMBLY;
    params.IdMethodName = IDS_MANAGED_LIB_FUNCTION;
    params.IdNamespaceName = IDS_MANAGED_LIB_NAMESPACE;
    params.IdVersion = IDS_MANAGED_LIB_VERSION;
    params.IdWithArgs = IDS_MANAGED_FUNC_ARGS;
    params.IdWithReturn = IDS_MANAGED_FUNC_RETURN;
    if(!params.FillData())
    {
        ShowErrorMessage(params.GetLastError());
        return -1;
    }
    
    std::wstring cmdLine = GetCommandLine();
    auto parameter = StringFunctions::Split(cmdLine, ' ');

    std::wstring clrBasePath;
    GetAppPathCleaned(cmdLine, parameter, clrBasePath);

    FillArgs(parameter,params.Arguments );
    params.AppPath = clrBasePath;

    /*const auto xstr = MAKEINTRESOURCEW(6);
    std::wstring resString = xstr;
    ShowErrorMessage(resString);*/

    FolderInfo fi(_T("C:\\Program Files\\dotnet\\shared\\Microsoft.NETCore.App"));

    if(fi.Exist())
    {
        for (size_t i = 0; i < fi.SubFolders.size(); i++)
        {
            wprintf( fi.SubFolders[i].c_str());
        }
    }

    clrBasePath = _T("C:\\Program Files\\dotnet\\shared\\Microsoft.NETCore.App\\3.0.0-preview-27122-01");
    ClrRuntimeHelper helper(clrBasePath,&params);
    if(!helper.InitClr())
    {
        ShowErrorMessage(helper.GetLastError());
        return -1;
    }

    int retVal;
   if(!helper.Run(retVal))
   {
       ShowErrorMessage(helper.GetLastError());
       return -1;
   }
    
    return retVal;

}



// </Snippet7>
int ReportProgressCallback(int progress)
{
    // Just print the progress parameter to the console and return -progress
    printf("Received status from managed code: %d\n", progress);
    return -progress;
}

