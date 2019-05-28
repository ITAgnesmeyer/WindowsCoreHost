#pragma once
#include <string>

class ErrorInfo
{
public:
	ErrorInfo();
	~ErrorInfo();
    static std::wstring GetErrorMessage(int errorCode);
};

