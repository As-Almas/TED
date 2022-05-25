#include "Preview.h"
#include "Resource.h"
#include "Dialog_Creator.h"
#include "Menu_Logic.h"

#include <fstream>
#include <Windows.h>
#include <ctime>
#include <CommCtrl.h>

using namespace std;

typedef bool (*FOLDER_EXISTS)(LPCWSTR path);
vector<wstring> linkList;
vector <UI*> links;
wchar_t* currDir;


Dialog_Creator Preview;
fstream file;
void ModuleError(wstring errText, wstring title) {
    MessageBox(NULL, errText.c_str(), title.c_str(), MB_ICONERROR);
    exit(-1);
}
bool CheckModules() {
    HMODULE lib;
    const int libCount = 6;
    const wchar_t* libArr[libCount] = {
        L"Filer.dll",
        L"iconv.dll",
        L"libxml2.dll",
        L"zlib1.dll",
        L"components/JSON.dll",
        L"comdlg32.dll"
    };
    for (int i = 0; i < libCount; i++) {
        lib = LoadLibrary(libArr[i]);
        if (!lib) {
            ModuleError(L"ERROR '"s + libArr[i] + L"' is not found"s, L"MODULE ERROR - "s + libArr[i]);
            return false;
        }
        FreeLibrary(lib);
    }
    return true;
}

void LoadCallBack(int id, int act) {
    if (id == -1 && act == INIT) {
        HWND wnd = Preview.GetElement(IDC_LOADER_PROGRESS);
        SendMessage(wnd, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 100));
        SendMessage(wnd, PBM_SETSTEP, (WPARAM)1, 0);
        SendMessage(wnd, PBM_SETBARCOLOR, 0, (LPARAM)RGB(0, 190, 0));
    }
}
// start of checkin - creating new thread
DWORD CALLBACK CreateDB(LPVOID data) {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    Preview = Dialog_Creator(GetConsoleWindow(), IDD_LOADBAR, LoadCallBack);
    Preview.SetElementText(IDC_LOADER_TEXT, L"Init...");
    Preview.SetElementValue(IDC_LOADER_PROGRESS, 1);
    ShowWindow(GetConsoleWindow(), SW_SHOW);
    return 0;
}

void LoadData(LPCWSTR folderName, Window MainWindow) {
    EnableWindow(MainWindow.GetWindow(), FALSE);
    HANDLE DialogThread = CreateThread(
        0,
        0,
        CreateDB,
        0,
        0,
        0
        );
    if (!DialogThread) {
        MessageBox(NULL, L"ERROR: cannot startup data loader!", L"ERROR #10", MB_ICONERROR);
        MainWindow.Show();
        return;
    }
    Preview.SetElementText(IDC_LOADER_TEXT, L"Init...");
    Preview.SetElementValue(IDC_LOADER_PROGRESS, 1);
    Sleep(500);
    
    Preview.SetElementText(IDC_LOADER_TEXT, L"Check app modules...");
    Preview.SetElementValue(IDC_LOADER_PROGRESS, 50);
    CheckModules();
    Sleep(500);

    Preview.SetElementText(IDC_LOADER_TEXT, L"Finishing...");
    Sleep(500);
    Preview.SetElementValue(IDC_LOADER_PROGRESS, 100);
    Sleep(1000);

    Preview.Destroy();
    WaitForSingleObject(DialogThread, INFINITE);
    CloseHandle(DialogThread);
    EnableWindow(MainWindow.GetWindow(), TRUE);
    MainWindow.Show();
    SetActiveWindow(MainWindow.GetWindow());
    return;
}

void AddProjectPath(LPCSTR pathTo)
{
    string strPath = pathTo;
    wstring path(strPath.begin(), strPath.end());
    for (wstring Lpath : linkList)
        if (Lpath == path)
            return;
    linkList.push_back(path.c_str());
}

void SaveData(LPCWSTR folderName)
{
    WCHAR* temp = new WCHAR[MAX_PATH];
    GetTempPath(MAX_PATH, temp);
    wstring path = wstring(temp) + folderName;

    wfstream out(path + L"\\UserProjects.txt");
    if (out.is_open()) {
        linkList.clear();
        for (wstring str : linkList) {
            out << str << "\n";
        }
        linkList.clear();
        out.close();
        out.clear();
    }
}

