#include "pch.h"
#include "Filer.h"
#include "reader.h"
#include "ProjactCreate_Handler.hpp"
#include <iostream>
#include <fstream>
#include <commdlg.h>
#include <CommCtrl.h>

#pragma comment(lib, "comctl32.lib")

using namespace std;
string filePath;

typedef bool(*INIT_EDITOR)(HWND main, LPCWSTR fileName);

const WCHAR* Filters = L"JSON File (*.json)\0*.json\0XML Files (*.xml)\0*.xml;*.xhtml\0Script file (*.ls)\0*.ls;*sl\0All files(*.*)\0*";

bool Create(HWND main)
{
    OPENFILENAME OpenFileName = { 0 };
    WCHAR FilePath[MAX_PATH] = L"example.json";
    ZeroMemory(&OpenFileName, sizeof(OpenFileName));
    OpenFileName.lStructSize = sizeof(OpenFileName);
    OpenFileName.lpstrFilter = Filters;
    OpenFileName.lpstrFile = FilePath;
    OpenFileName.nMaxFile = MAX_PATH; 
    OpenFileName.Flags = OFN_EXPLORER | OFN_CREATEPROMPT | OFN_NOCHANGEDIR;
    OpenFileName.hwndOwner = main;

    if (GetSaveFileName(&OpenFileName)) {
        ifstream check(OpenFileName.lpstrFile, ios_base::in | ios::_Nocreate);
        if (check.is_open()) {
            check.close();
            int res = MessageBox(NULL, L"File already was created!\nIf you click ok, file contant maybe deleted!", L"INFO", MB_ICONINFORMATION | MB_OKCANCEL);
            if (res != IDOK) {
                return true;
            }
        }
        else {
            HANDLE hFile = CreateFile(OpenFileName.lpstrFile,                // name of the write
                GENERIC_WRITE,          // open for writing
                0,                      // do not share
                NULL,                   // default security
                CREATE_ALWAYS,          // create new file only
                FILE_ATTRIBUTE_NORMAL,  // normal file
                NULL);                  // no attr. template

            if (hFile == INVALID_HANDLE_VALUE)
            {
                MessageBox(main, L"ERROR: cannot create file!", L"ERROR #4", MB_ICONERROR);
                return false;
            }
            CloseHandle(hFile);
        }
        wstring x = OpenFileName.lpstrFile;
        filePath = string(x.begin(), x.end());
        x.clear();
        return true;
    }
    
    return true;
}

const char* Open(HWND main)
{
    OPENFILENAME OpenFileName = { 0 };
    WCHAR FilePath[MAX_PATH] = L"";
    ZeroMemory(&OpenFileName, sizeof(OpenFileName));
    OpenFileName.lStructSize = sizeof(OpenFileName);
    OpenFileName.lpstrFilter = Filters;
    OpenFileName.lpstrFile = FilePath;
    OpenFileName.nMaxFile = MAX_PATH;
    OpenFileName.Flags = OFN_EXPLORER | OFN_FORCESHOWHIDDEN;
    OpenFileName.hwndOwner = main;

    if (GetSaveFileName(&OpenFileName)) {
        
        ifstream check(OpenFileName.lpstrFile, ios_base::in);
        if (!check.is_open()) {
            MessageBox(NULL, L"File not found!", L"ERROR #4", MB_ICONERROR | MB_OK);
            return nullptr;
        }
        check.close();
        wstring x = OpenFileName.lpstrFile;
        filePath = string(x.begin(), x.end());
        x.clear();
        return (const char*)OpenFileName.lpstrFile;
    }
    return nullptr;
}

bool OpenProjact(HWND main)
{
    TCHAR path[MAX_PATH];

    const char* path_param = "C:/";

    BROWSEINFO bi = { 0 };
    bi.hwndOwner = main;
    bi.lpszTitle = L"Browse for folder...";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_NONEWFOLDERBUTTON;
    bi.lParam = (LPARAM)path_param;

    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

    if (pidl != 0)
    {
        //get the name of the folder and put it in path
        SHGetPathFromIDList(pidl, path);

        //free memory used
        IMalloc* imalloc = 0;
        if (SUCCEEDED(SHGetMalloc(&imalloc)))
        {
            imalloc->Free(pidl);
            imalloc->Release();
        }
        wstring str = path;
        str += L"\\";
        str += L"config.json";

        string test = GetJsonValue("main", (LPWSTR)str.c_str());
        str.clear();
        folderPath.clear();
        folderPath = ConvertToString(path);
        if (test.empty()) {
            MessageBox(NULL, L"Cannot read property json file!", L"ERROR #7", MB_ICONERROR);
            return false;
        }

        return true;
    }
    return false;
}

void GetFolderPath(char*arr) {
    copy(folderPath.begin(), folderPath.end(), arr);
    arr[folderPath.size()] = '\0';
}

void GetFilePath(char* arr)
{
    copy(filePath.begin(), filePath.end(), arr);
    arr[filePath.size()] = '\0';
}

bool CreateProjact(HWND main)
{
    InitCommonControls();
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    HICON icon = (HICON)LoadImage(GetModuleHandleW(NULL), L"components/icons/new projact.ico", IMAGE_ICON, 200, 100, LR_LOADFROMFILE);
    
    if(!icon)
        LogErrorMessage(GetLastError());

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetModuleHandleW(NULL);
    wcex.hIcon =  icon;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"ProjactCreater";
    wcex.hIconSm = icon;

    if (!RegisterClassExW(&wcex)) {
        int errorMessageID = GetLastError();
        LogErrorMessage(errorMessageID);
        MessageBox(main, L"Error: cannot init projact creator!", L"ERROR #5", MB_ICONERROR);
        return false;
    }
    EnableWindow(main, FALSE);

    HWND ProjactCreateWindow = CreateWindowW(wcex.lpszClassName,
        L"Logics - create projact window",
        WS_OVERLAPPEDWINDOW &~ WS_THICKFRAME & ~WS_MAXIMIZE & ~WS_MINIMIZE &~WS_MAXIMIZEBOX,
        CW_USEDEFAULT,0,
        600,400,
        main,
        NULL,NULL,0);
    if (!ProjactCreateWindow) {
        LogErrorMessage(GetLastError());
        MessageBox(main, L"Error: cannot init projact creator!", L"ERROR #5", MB_ICONERROR);
        return false;
    }
    ShowWindow(ProjactCreateWindow, SW_SHOW);
    
    Sender(ProjactCreateWindow, main);

    UnregisterClass(wcex.lpszClassName, GetModuleHandleW(NULL));
    EnableWindow(main, TRUE);
    SetActiveWindow(main);
    return isCreated;
}

bool CreateFileFromPath(LPCWSTR path)
{
    HANDLE hFile = CreateFile(path, GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_HIDDEN, NULL);
    if(hFile == INVALID_HANDLE_VALUE || !hFile)
        return false;
    CloseHandle(hFile);
    return true;
}

void LogErrorMessage(int errorID)
{
    LPWSTR messageBuffer = nullptr;
    size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);


    MessageBox(NULL, messageBuffer, L"ERROR #5", MB_ICONWARNING);
    LocalFree(messageBuffer);
}
