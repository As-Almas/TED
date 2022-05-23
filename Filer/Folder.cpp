#include "pch.h"
#include "Folder.h"


bool FolderExists(LPWSTR path)
{
    DWORD dwAttrib = GetFileAttributes(path);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
        (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool CreateFolder(LPCWSTR path)
{
    return CreateDirectory(
        path,
        NULL
    );
}
