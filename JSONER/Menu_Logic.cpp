#include "Menu_Logic.h"

#include "resource.h"
#include "Headers.h"
#include "Dialog_Creator.h"
#include "framework.h"
#include "Preview.h"


#include <string>
std::wstring UsersProjectsFilePath;

typedef bool(*Filer)(HWND), (*FILEADDFUNC)(LPCWSTR filePath, LPCSTR line);
typedef void(*GETFOLDER)(char*arr);
static Window MainWindow = NULL;
char* folderPath = new char[MAX_PATH + 1];
char* lastFilePath = new char[MAX_PATH + 1];


void GoToMainMenu() {
    if (!MainWindow.Destroy()) 
        return;
    wchar_t szFileName[MAX_PATH] = L"";
    GetModuleFileName(NULL, szFileName, MAX_PATH);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    BOOL res = CreateProcess(
        (LPWSTR)szFileName,
        NULL,
        NULL,
        NULL,
        TRUE,
        CREATE_NEW_CONSOLE,
        NULL,
        NULL,
        &si,
        &pi
    );
    if (!res) {
        MessageBox(MainWindow.GetWindow(), L"ERROR: cannot load home window!", L"restart error!", MB_ICONERROR);
        return;
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    exit(1);
}

void Logic::Init(Window window)
{
    MainWindow = window;
}


bool MessageOfError(LPCWSTR message) {
    int res = MessageBox(NULL, message, L"ERROR #4", MB_ICONERROR | MB_RETRYCANCEL);
    if (res == IDRETRY) {
        return true;
    }
    return false;
}

void Logic::Menu(int id, int action)
{
	if (action == EL_CLICK) {
        switch (id)
        {
            case IDM_EXIT: {
                MainWindow.Destroy();
            }break;
            case IDM_ABOUT: {
                Dialog_Creator(MainWindow.GetWindow(), IDD_ABOUTBOX, [](int id, int action) {
                    if (action == EL_CLICK)
                        if (id == IDM_CANCEL)
                            Dialog_Creator::Destroy();
                    });
            }break;
            case IDM_CREATE_FILE: {
                HMODULE lib = LoadLibrary(L"Filer.dll");
                if (!lib) {
                    if (MessageOfError(L"Error: cannot init file creator!"))
                        Logic::Menu(id, action);
                    return;
                }
                Filer create = (Filer)GetProcAddress(lib, "Create");
                if (!*create) {
                    if (MessageOfError(L"Error: cannot file creator was corupted!"))
                        Logic::Menu(id, action);
                    FreeLibrary(lib);
                    return;
                }
                if (!create(MainWindow.GetWindow())) {
                    MessageBox(NULL, L"Err: cannot create file!", L"ERROR #4", MB_ICONERROR);
                }
                GETFOLDER getPath = (GETFOLDER)GetProcAddress(lib, "GetFilePath");
                getPath(lastFilePath);
                FreeLibrary(lib);
            }break;
            case IDM_FILE_OPENFILE: {
                HMODULE lib = LoadLibrary(L"Filer.dll");
                if (!lib) {
                    if (MessageOfError(L"Error: cannot init file creator!"))
                        Logic::Menu(id, action);
                    return;
                }
                Filer create = (Filer)GetProcAddress(lib, "Open");
                if (!*create) {
                    if (MessageOfError(L"Error: cannot open file! Filer module was corupted!"))
                        Logic::Menu(id, action);
                    FreeLibrary(lib);
                    return;
                }
                if (!create(MainWindow.GetWindow())) {
                    MessageBox(NULL, L"Err: cannot open file!", L"ERROR #4", MB_ICONERROR);
                }
                GETFOLDER getPath = (GETFOLDER)GetProcAddress(lib, "GetFilePath");
                getPath(lastFilePath);
                FreeLibrary(lib);
            }break;
            
            case ID_OPENHOMEWINDOW: {
                GoToMainMenu();
            }break;
            case ID_HELP: {
                char pathToExe[MAX_PATH] = {'\0'};
                GetModuleFileNameA(NULL, pathToExe, MAX_PATH);
                std::string path = std::string(pathToExe).substr(0, std::string(pathToExe).find_last_of("\\"));
                path += "\\Logics help.exe"; // name of help module
                MainWindow.Hide();

                STARTUPINFOA si;
                PROCESS_INFORMATION pi;

                ZeroMemory(&si, sizeof(si));
                si.cb = sizeof(si);
                ZeroMemory(&pi, sizeof(pi));

                BOOL res = CreateProcessA(
                    path.c_str(),
                    NULL,
                    NULL,
                    NULL,
                    TRUE,
                    CREATE_NEW_CONSOLE,
                    NULL,
                    NULL,
                    &si,
                    &pi
                );
                if (!res) {
                    MessageBox(MainWindow.GetWindow(), L"ERROR: help module not found!", L"ERROR #5", MB_ICONERROR);
                    MainWindow.Show();
                    return;
                }
                WaitForSingleObject(pi.hProcess, INFINITE);

                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);

                MainWindow.Show();
            }break;
        }
	}
}

void Logic::UnInit()
{
    MainWindow = NULL;
}

void Logic::setPath(std::wstring path)
{
    UsersProjectsFilePath = path;
}

LPCWSTR Logic::getPath()
{
    return UsersProjectsFilePath.c_str();
}
