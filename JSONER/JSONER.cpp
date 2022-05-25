// JSONER.cpp : Определяет точку входа для приложения.
//
#include "Resource.h"
#include "JSONER.h"
#include "Preview.h"
#include "Menu_Logic.h"

void ____STOP_DEBUG_OF_LOGICS_AS_ALMAS_MESSAGE____() {
    long long x = 1;
    EnableWindow(GetConsoleWindow(), FALSE);
    while (true)
    {
        OutputDebugStringA("\n\n\n_________________!!! DON'T ATTACH DEBUGER TO ME !!!_________________\n\n\n");
        std::cout << "\n\n\n_________________!!! DON'T ATTACH DEBUGER TO ME !!!_________________\n\n\n";
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

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "trash/UI.lib")

Window MainWindow = Window::Window(GetModuleHandleW(NULL));


int WindowIniter() {
    // uncomment for release
    //if (IsDebuggerPresent()) {
    //    ____STOP_DEBUG_OF_LOGICS_AS_ALMAS_MESSAGE____();
    //    return 0;
    //}

    MainWindow.DestroyAllItems();
    MainWindow.MenuItemDisable(ID_OPENHOMEWINDOW);

    LoadData(L"TED_AS_ALMAS", MainWindow);
    

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

