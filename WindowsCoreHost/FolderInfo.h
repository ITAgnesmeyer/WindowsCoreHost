#pragma once
#include <string>
#include <vector>

class FolderInfo
{
public:
	FolderInfo(std::wstring folderName);
	~FolderInfo();
    std::vector<std::wstring> SubFolders;
    std::vector<std::wstring> Files;
    bool Exist();
private:
    std::wstring Folder;
    bool FolderExist;
    void GetInfo();
    
};

