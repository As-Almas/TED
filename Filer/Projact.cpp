#include "pch.h"
#include "Projact.h"
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

typedef bool(*CrF)(LPWSTR path),
(*AddObj)(LPWSTR path, const char* name, const char* value),
(*AddToArr)(LPWSTR path, const char* arrName, const char* nameOfVal, const char* value);

HMODULE lib;
HMODULE xmlLib;
AddObj AddObjact;
AddToArr AddToObjact;

bool LoadResources()
{

    lib = LoadLibrary(L"components/JSON.dll");
    if (lib) {
        AddObjact = (AddObj)GetProcAddress(lib, "AddObjact");
        AddToObjact = (AddToArr)GetProcAddress(lib, "AddToObjact");
    }
    return lib != NULL;
}
bool CreateFolder(TCHAR path[], TCHAR name[])
{
    wstring str = path;
    str += L"\\";
    str += name;

    DWORD ftyp = GetFileAttributes(str.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES) {
        return CreateDirectory(str.c_str(), NULL);
    }
        

    if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
        int res = MessageBox(NULL, L"It folder has already created! All files can be change or remove!\n You sure", L"INFO", MB_OKCANCEL | MB_ICONINFORMATION);
        if (res == IDOK) {
            DeleteFolder(path, name);
            return CreateDirectory(str.c_str(), NULL);
        }
        else
            return false;
    }

    return false;
}

void DeleteFolder(TCHAR path[], TCHAR name[])
{
    wstring str = path;
    str += L"\\";
    str += name;

    DWORD ftyp = GetFileAttributes(str.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return;

    if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
        RemoveDirectory(str.c_str());
    }
}



bool GenerateFiles(TCHAR path[], TCHAR name[], int type)
{
    wstring str = path;
    str += L"\\";
    str += name;
    str += L"\\";
    str += L"config.json";
    
    if (!lib)
        return false;
    CrF fileCreate = (CrF)GetProcAddress(lib, "Create");
    

    wstring x = name;
    string strName(x.begin(), x.end());
    x.clear();

    bool res = fileCreate((LPWSTR)str.c_str()) && 
    AddObjact((LPWSTR)str.c_str(), "main", "Start.ls") &&
    AddObjact((LPWSTR)str.c_str(), "name", strName.c_str()) &&
    AddToObjact((LPWSTR)str.c_str(), "params", "TurnConsole", "false") &&
    AddToObjact((LPWSTR)str.c_str(), "params", "Mode", "Debug");

    wstring lsFilePath = path + L"\\"s + name + L"\\Start.ls"s;

    wfstream lsFile(lsFilePath, ios::trunc | ios::out);
    if (!lsFile.is_open())
        return false;
    switch (type)
    {
    case 0: {
        lsFile << "print(\"Hello world\");\n";
        lsFile << 
        "// print = function to print information (int, float, string and etc...)\n"
        "// documentation on the logic script programming language is in the JSONER documentation (article: \"logic script\")\n"
        "// ls and sl - logic script file extensions";
    }break;
    case 1: {
        lsFile << "import \"Graphics\"\n";
        lsFile <<
            L"var window = CreateWindow(\""s + name + L"\", 700, 400);\n"
            L"window.show()\n"
            L"var btn = window.AddButton(\"Btn text\")\n"
            L"window.loop()\n"
            L"// import - preproccess command for import module or lib file\n"
            L"// window - object of window class, which was create by CreateWindow\n"
            L"// window.show - showed window for user\n"
            L"// window.AddButtom -  adding button to user window \n"
            L"// window.loop - window handler loop\n";
    }break;
    }
    lsFile.close();
    return res;
}

bool SetProjactParams(TCHAR path[], TCHAR name[], const char* author, const char* description)
{
    wstring pathToJson = path;
    pathToJson += L"\\";
    pathToJson += name;
    pathToJson += L"\\";
    pathToJson += L"config.json";

    return AddObjact((LPWSTR)pathToJson.c_str(), "Author", author) &&
    AddObjact((LPWSTR)pathToJson.c_str(), "Description", description);
}

void TerminateResources()
{
    if(lib)
        FreeLibrary(lib);
    AddObjact = NULL;
    AddToObjact = NULL;
    lib = NULL;
    xmlLib = NULL;
}