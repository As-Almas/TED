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
    const int libCount = 7;
    const wchar_t* libArr[libCount] = {
        L"Filer.dll",
        L"iconv.dll",
        L"libxml2.dll",
        L"zlib1.dll",
        L"components/Editor.dll",
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

void OnClick(UI* el) {
    CHAR* buf = new CHAR[MAX_PATH];
    GetWindowTextA(el->GetUIWindow(), buf, MAX_PATH);

    ShowProjectWindow(buf);
    delete[] buf;
}

void CreateFromLink(UI* el) {
    SendMessage(GetParent(el->GetUIWindow()), WM_COMMAND, IDM_CREATE_PROJACT, 0);
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

UI* link = NULL;
void LoadUserProjects(LPCWSTR folderName, Window MainWindow) {
    WCHAR* temp = new WCHAR[MAX_PATH];
    GetTempPath(MAX_PATH, temp);
    wstring path = wstring(temp) + folderName;
    Logic::setPath(path + L"\\UserProjects.txt");
    HMODULE filer = LoadLibrary(L"Filer.dll");
    if (!filer)
    {
        MessageBox(MainWindow.GetWindow(), L"ERROR: 'filer.dll' is not found!", L"ERROR #4", MB_ICONERROR);
        MainWindow.Destroy();
        exit(-1);
    }
    FOLDER_EXISTS isExists = (FOLDER_EXISTS)GetProcAddress(filer, "FolderExists");
    FOLDER_EXISTS fileCreate = (FOLDER_EXISTS)GetProcAddress(filer, "CreateFileFromPath");
    FOLDER_EXISTS DirCreate = (FOLDER_EXISTS)GetProcAddress(filer, "CreateFolder");
    if (!*isExists) {
        MessageBox(MainWindow.GetWindow(), L"ERROR: 'filer.dll' was corrupted!", L"ERROR #4", MB_ICONERROR);
        MainWindow.Destroy();
        exit(-1);
    }
    if (!isExists(path.c_str())) {
        DirCreate(path.c_str());
        fileCreate(Logic::getPath());
    }

    wifstream file(Logic::getPath(), ios::in | ios::_Nocreate);
    wstring wide;
    if (!file.is_open()) {
        if (!fileCreate(Logic::getPath()))
            MessageBox(MainWindow.GetWindow(), L"WARN: cannot load last projects list!", L"WARN", MB_ICONWARNING);
    }
    else {
        wstring line;
        int index = 1001;
        float lastY = 0.21;

        while (getline(file, line))
        {
            if (line.empty() || index > 1010)
                continue;
            if (isExists(line.c_str())) {
                link = new UI(MainWindow.GetWindow(), index);

                link->AddStyle(BS_OWNERDRAW);
                link->DrawBottomBorder();
                link->DrawLeftBorder();
                link->SetDefBackground(0, 190, 190);
                link->SetFocusBackground(0, 190, 190);
                link->SetDefTextColor(0, 0, 190);
                link->SetFocusTextColor(0, 0, 150);
                link->AddTextStyle(DT_BOTTOM);
                link->SetOnClick(OnClick);
                link->SetMaxSize(0, 25);
                link->SetFont(L"Arial", 20);
                link->SetUserCursor(IDC_HAND);
                link->weight = 700;
                MainWindow.AddUI(link);

                wide = line;
                linkList.push_back(wide.c_str());
                link->SetParams(WC_BUTTON, UI_PARAMS{ 0.0f,lastY, 0.49f,0.1f, wide });
                links.push_back(link);
                lastY += 0.089f;
                index++;
            }
        }
        isExists = NULL;
    }
    if (links.empty()) {
        UI* text = new UI(MainWindow.GetWindow(), 1002);
        text->AddStyle(BS_OWNERDRAW);
        text->SetDefBackground(0, 190, 190);
        text->SetFocusBackground(0, 190, 190);
        text->SetDefTextColor(0, 0, 190);
        text->SetFocusTextColor(0, 0, 150);
        text->SetUserCursor(IDC_HAND);
        text->isUnderLine = TRUE;
        text->AddTextStyle(DT_CENTER);
        text->AddTextStyle(DT_VCENTER);
        text->SetOnClick(CreateFromLink);
        text->SetParams(WC_BUTTON, UI_PARAMS{ 0.0f, 0.5f, 0.49f, 0.1f, L"List is empty.\n Create project now!" });
        MainWindow.AddUI(text);
        links.push_back(text);
    }
    if (filer)
        FreeLibrary(filer);
}


void LoadIcons(Window MainWindow) {
    //HANDLE x  = LoadImage(GetModuleHandle(NULL), L"logo.png", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_LOADTRANSPARENT);
    //cout << GetLastError();
    
}

std::vector<UI*> LoadData(LPCWSTR folderName, Window MainWindow) {
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
        return vector<UI*>();
    }
    Preview.SetElementText(IDC_LOADER_TEXT, L"Init...");
    Preview.SetElementValue(IDC_LOADER_PROGRESS, 1);
    Sleep(500);
    
    Preview.SetElementText(IDC_LOADER_TEXT, L"Loading user latest projects...");
    Preview.SetElementValue(IDC_LOADER_PROGRESS, 5);
    LoadUserProjects(folderName, MainWindow);
    Sleep(500);
    
    Preview.SetElementText(IDC_LOADER_TEXT, L"Check app modules...");
    Preview.SetElementValue(IDC_LOADER_PROGRESS, 20);
    CheckModules();
    Sleep(500);

    Preview.SetElementText(IDC_LOADER_TEXT, L"Loading icons...");
    LoadIcons(MainWindow);
    Preview.SetElementValue(IDC_LOADER_PROGRESS, 30);
    Sleep(500);

    Preview.SetElementText(IDC_LOADER_TEXT, L"Finishing...");
    Sleep(500);
    Preview.SetElementValue(IDC_LOADER_PROGRESS, 100);
    Sleep(1000);

    Preview.Destroy();
    WaitForSingleObject(DialogThread, INFINITE);
    CloseHandle(DialogThread);
    EnableWindow(MainWindow.GetWindow(), TRUE);
    SetActiveWindow(MainWindow.GetWindow());
    MainWindow.Show();
    return links;
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

