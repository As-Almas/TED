// JSONER.cpp : Определяет точку входа для приложения.
//
#include "Resource.h"
#include "JSONER.h"
#include "Preview.h"
#include "RegistrateUser.h"
#include "Menu_Logic.h"

void ____STOP_DEBUG_OF_LOGICS_AS_ALMAS_MESSAGE____() {
    long long x = 1;
    EnableWindow(GetConsoleWindow(), FALSE);
    while (true)
    {
        OutputDebugStringA("\n\n\n_________________!!! DON'T ATTACH DEBUGER TO ME !!!_________________\n\n\n");
        OutputDebugStringA("\n\n\n_________________!!! ____STOP_DEBUG_OF_LOGICS_AS_ALMAS_MESSAGE____ was activated ME !!!_________________\n\n\n");
        std::cout << "\n\n\n_________________!!! DON'T ATTACH DEBUGER TO ME !!!_________________\n\n\n"<< "\n\n\n_________________!!! ____STOP_DEBUG_OF_LOGICS_AS_ALMAS_MESSAGE____ was activated ME !!!_________________\n\n\n";
        Sleep(x+=10);
        if (x >= 1000ll) {
            DebugBreak();
        }
        if (x >= 2000ll) {
            EnableWindow(GetConsoleWindow(), TRUE);
            exit(x * -1ll);
        }
    }
}


UI* name = NULL;
UI* password = NULL;
UI* code = NULL;
UI* submit = NULL;

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "trash/UI.lib")

Window MainWindow = Window::Window(GetModuleHandleW(NULL));
LPCWSTR PassText = L"Your password", LoginText = L"Your login", CodeText = L"Your unique registration code";


int WindowInstaller() {
    // uncomment for release
   /* if (IsDebuggerPresent()) {
        ____STOP_DEBUG_OF_LOGICS_AS_ALMAS_MESSAGE____();
        return 0;
    }*/

    MainWindow.DestroyAllItems();
    MainWindow.MenuItemDisable(ID_OPENHOMEWINDOW);
    MainWindow.SetMaxSize(500, 500);
    MainWindow.SetMinSize(400, 400);
    HMENU menu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_INSTALLER_MENU));
    SetMenu(MainWindow.GetWindow(), menu);
   

    
    SetWindowPos(MainWindow.GetWindow(), HWND_TOP, 0, 0, 400, 400, SWP_ASYNCWINDOWPOS);
        
    LoadData(L"TED_AS_ALMAS", MainWindow);
    
    name = new UI(MainWindow.GetWindow(), IDI_LOGIN);
    password = new UI(MainWindow.GetWindow(), IDI_PASSWORD);
    code = new UI(MainWindow.GetWindow(), IDI_CODE);
    submit = new UI(MainWindow.GetWindow(), IDI_SUBMIT);

    MainWindow.AddUI(submit);
    MainWindow.AddUI(name);
    MainWindow.AddUI(password);
    MainWindow.AddUI(code);

    submit->SetOnClick(OnSubmitClick);
    
    name->AddStyle(ES_CENTER | ES_AUTOHSCROLL | WS_BORDER | WS_TABSTOP);
    password->AddStyle(ES_CENTER | ES_AUTOHSCROLL | ES_PASSWORD | WS_BORDER | WS_TABSTOP);
    code->AddStyle(ES_CENTER | ES_AUTOHSCROLL | ES_PASSWORD | WS_BORDER | WS_TABSTOP);
    submit->AddStyle(BS_OWNERDRAW | WS_BORDER | WS_TABSTOP);
    submit->AddTextStyle(DT_CENTER | DT_VCENTER);
    submit->SetUserCursor(MAKEINTRESOURCE(IDC_HAND));
    submit->SetFont(L"UNISPACE", 20);
    submit->SetOnClick(OnSubmitClick);
    
    name->EnableUpdateText(false);
    password->EnableUpdateText(false);
    code->EnableUpdateText(false);

    submit->SetDefTextColor(0, 0, 0);
    submit->SetFocusTextColor(255, 255, 255);
    submit->SetDefBackground(0, 220, 0);
    submit->SetFocusBackground(0, 180, 0);

    name->Create(WC_EDIT, UI_PARAMS{ 0.05,0.05, 0.90, 0.075, L"" });
    password->Create(WC_EDIT, UI_PARAMS{ 0.05,0.15, 0.90, 0.075, L"" });
    code->Create(WC_EDIT, UI_PARAMS{ 0.05,0.25, 0.90, 0.075, L"" });
    submit->Create(WC_BUTTON, UI_PARAMS{ 0.05,0.35, 0.90, 0.075, L"sign in" });

    SendMessage(name->GetUIWindow(), EM_SETCUEBANNER, FALSE, (LPARAM)LoginText);
    SendMessage(password->GetUIWindow(), EM_SETCUEBANNER, FALSE, (LPARAM)PassText);
    SendMessage(code->GetUIWindow(), EM_SETCUEBANNER, FALSE, (LPARAM)CodeText);

    SendMessage(name->GetUIWindow(), EM_SETLIMITTEXT, 100, 0);
    SendMessage(password->GetUIWindow(), EM_SETLIMITTEXT, 100, 0);
    SendMessage(code->GetUIWindow(), EM_SETLIMITTEXT, 100, 0);
    
    SendMessage(name->GetUIWindow(), WM_SETFONT, (WPARAM)submit->GetUIFont(), TRUE);
    SendMessage(password->GetUIWindow(), WM_SETFONT, (WPARAM)submit->GetUIFont(), TRUE);
    SendMessage(code->GetUIWindow(), WM_SETFONT, (WPARAM)submit->GetUIFont(), TRUE);
    
    SendMessage(password->GetUIWindow(), EM_SETPASSWORDCHAR, L'*', 0);
    SendMessage(code->GetUIWindow(), EM_SETPASSWORDCHAR, L'*', 0);
    

    while (MainWindow.isOpen())
    {

    }
    MainWindow.DestroyAllItems();
    MainWindow = NULL;
    
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
        WindowInstaller();

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

