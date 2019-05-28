#pragma once
#include <string>

class ResourceLoader
{
public:
	ResourceLoader();
	~ResourceLoader();
    
    static std::wstring LoadStringToWString(HINSTANCE hInstance, UINT uID);
private:
    static int LoadStringW(HINSTANCE hInstance, UINT uID, std::wstring& wsDest);
};

