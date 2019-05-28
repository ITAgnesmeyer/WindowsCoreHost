#include "stdafx.h"
#include "ErrorInfo.h"


ErrorInfo::ErrorInfo()
= default;


ErrorInfo::~ErrorInfo()
= default;

std::wstring ErrorInfo::GetErrorMessage(int errorCode)
{
    


    //Returns the last Win32 error, in string format. Returns an empty string if there is no error.
    //Get the error message, if any.
    DWORD errorMessageId = errorCode;
    if(errorMessageId == 0)
        return std::wstring(); //No error message has been recorded

    LPWSTR messageBuffer = nullptr;
    size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageId, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&messageBuffer), 0, NULL);

    std::wstring message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;

}
