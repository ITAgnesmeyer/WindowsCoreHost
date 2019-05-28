#include "stdafx.h"
#include "FolderInfo.h"
#include <vector>





FolderInfo::FolderInfo(std::wstring folderName)
{
    this->Folder = folderName;
    this->FolderExist = false;
    this->Files.clear();
    this->SubFolders.clear();
    GetInfo();
}

FolderInfo::~FolderInfo()
{
    this->Files.clear();
    this->SubFolders.clear();
}

bool FolderInfo::Exist()
{
    return this->FolderExist;
}

void FolderInfo::GetInfo()
{
    WIN32_FIND_DATA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    std::wstring baseFolder = this->Folder;
    baseFolder.append(_T("\\*"));
    this->FolderExist = true;
    hFind = ::FindFirstFile(baseFolder.c_str(),&ffd);
    if(INVALID_HANDLE_VALUE == hFind)
    {
        this->FolderExist = false;
        return;
    }
    do
    {
       if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
       {
           std::wstring folder = ffd.cFileName;
           this->SubFolders.push_back(folder);
       }
       else
       {
           std::wstring file = ffd.cFileName;
           this->Files.push_back(file);
       }
    }
    while (FindNextFile(hFind, &ffd) != 0);
    ::FindClose(hFind);

}
