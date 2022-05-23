// JSONER.cpp : Определяет точку входа для приложения.
//
#include "Resource.h"
#include "JSONER.h"
#include "Preview.h"
#include "Menu_Logic.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "trash/UI.lib")

Window MainWindow = Window::Window(GetModuleHandleW(NULL));


int WindowIniter() {
    // uncomment for release
    /*if (IsDebuggerPresent()) {
        throw std::exception("Please don't attach a debugger to me!");
        return 0;
    }*/
    MainWindow.DestroyAllItems();
    MainWindow.MenuItemDisable(IDM_CREATE_FILE);
    MainWindow.MenuItemDisable(IDM_FILE_OPENFILE);
    MainWindow.MenuItemDisable(ID_OPENHOMEWINDOW);

    UI* text = new UI(MainWindow.GetWindow(), 0);
    UI* decoration = new UI(MainWindow.GetWindow(), 1);
    UI* NewProject = new UI(MainWindow.GetWindow(), IDM_CREATE_PROJACT);
    UI* OpenProject = new UI(MainWindow.GetWindow(), IDM_OPEN_PROJACT);

    text->AddStyle(SS_OWNERDRAW);
    decoration->AddStyle(SS_OWNERDRAW);
    decoration->AddStyle(WS_BORDER);
    NewProject->AddStyle(BS_OWNERDRAW);
    OpenProject->AddStyle(BS_OWNERDRAW);

    NewProject->SetDefBackground(0, 200, 0);
    NewProject->AddTextStyle(DT_CENTER | DT_VCENTER);
    NewProject->SetFocusBackground(0, 170, 0);
    NewProject->SetFocusTextColor(255, 255, 255);
    NewProject->SetUserCursor(IDC_HAND);


    OpenProject->AddTextStyle(DT_CENTER | DT_VCENTER);
    OpenProject->SetDefBackground(0, 200, 0);
    OpenProject->SetFocusBackground(0, 170, 0);
    OpenProject->DrawBottomBorder();
    OpenProject->SetFocusTextColor(255, 255, 255);
    OpenProject->SetUserCursor(IDC_HAND);

    text->SetDefTextColor(0, 150, 150);
    text->isUnderLine = TRUE;
    text->AddTextStyle(DT_BOTTOM);
    decoration->SetDefBackground(0, 190, 190);


    MainWindow.AddUI(text);


    MainWindow.AddUI(decoration);
    MainWindow.AddUI(NewProject);
    MainWindow.AddUI(OpenProject);

    std::vector<UI*> latest = LoadData(L"LOGICS_As_Almas", MainWindow);

    // uncomment for release
    //MainWindow.DebugModeOut();

    text->Create(WC_STATIC, UI_PARAMS{ 0.0f,0.15f,0.2f,0.05f, L"Open last project:" });
    decoration->Create(WC_STATIC, UI_PARAMS{ 0.0f,0.2f,0.5f,0.85f, L"" });
    OpenProject->Create(WC_BUTTON, UI_PARAMS{ 0.7,0.6,0.3,0.19999,L"Open project" });
    NewProject->Create(WC_BUTTON, UI_PARAMS{ 0.7,0.8,0.3,0.2,L"New project" });
    for (UI* link : latest) {
        link->Create();
    }
    
    

    while (MainWindow.isOpen())
    {

    }
    MainWindow.DestroyAllItems();
    MainWindow = NULL;
    SaveData(L"LOGICS_As_Almas");
    return 0;
}

int main()
{
    try
    {
        MainWindow = Window::Window(GetModuleHandleW(NULL));
        Logic::Init(MainWindow);
        if (!MainWindow.Init(Logic::Menu))
            return -1;
        WindowIniter();

        return 1;
    } catch (const std::exception&err)
    {
        MessageBoxA(MainWindow.GetWindow(), err.what(), "ERROR #0", MB_ICONERROR);
    }catch (...)
    {
        MessageBox(NULL, L"Unknow program error", L"ERROR #???", MB_ICONERROR);
    } 
    return 0;
}

