#include "stdafx.h"
#include "ResourceLoader.h"


ResourceLoader::ResourceLoader()
= default;


ResourceLoader::~ResourceLoader()
= default;

std::wstring ResourceLoader::LoadStringToWString(HINSTANCE hInstance, UINT uID)
{
     std::wstring sDest;
    LoadString(hInstance, uID, sDest);
    return sDest;
}

int ResourceLoader::LoadStringW(HINSTANCE hInstance, UINT uID, std::wstring& wsDest)
{
     PWCHAR wsBuffer;
    wsDest.clear();
    if (const size_t len = ::LoadString(hInstance, uID, reinterpret_cast<PWCHAR>(&wsBuffer), 0))
    {
        wsDest.assign(wsBuffer, len);
    }
    return static_cast<int>(wsDest.length());
}
