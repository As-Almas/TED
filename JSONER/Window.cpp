#include "Window.h"
#include "Resource.h"
#include "../UI/UI_includes.h"
#include <CommCtrl.h>
#include <iostream>

HACCEL Window::hAccelTable;
HWND Window::window;
MSG Window::msg;
std::vector<UI*> Window::WindowUI;
WNDCLASSEXW Window::wcex;
int Window::minW, Window::minH;
HINSTANCE Window::hInst;
void(*Window::InterfaceCAllBack)(int id, int action);
WCHAR Window::szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR Window::szWindowClass[MAX_LOADSTRING];
bool Window::isInited = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
           int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            if (*Window::InterfaceCAllBack)
                Window::InterfaceCAllBack(wmId, EL_CLICK);
        for (UI* x : Window::WindowUI)
        {
            if (x->operator==(wmId)) {
                x->Click();
                break;
            }
        }
        return DefWindowProc(hWnd, message, wParam, lParam);
        
    }
    break;
    case WM_DRAWITEM: {
        int id = wParam;
        DRAWITEMSTRUCT* str = (DRAWITEMSTRUCT*)lParam;
        for (UI* x : Window::WindowUI) {
            if (x->operator==(id)) {
                x->Draw(str);
                break;
            }
        }
    }break;
    case WM_GETMINMAXINFO: {
        MINMAXINFO* pInfo = (MINMAXINFO*)lParam;
        POINT min = { Window::minW, Window::minH };
        pInfo->ptMinTrackSize = min;
        return 0;
    }break;
    case WM_SIZE: {
        RECT wndRect;
        GetWindowRect(Window::window, &wndRect);
        InvalidateRect(Window::window, &wndRect, TRUE);
        for (UI* x : Window::WindowUI) {
            x->Resize();
        }
    }break;
    case WM_SETCURSOR: {
        HWND wnd = (HWND)wParam;
        UINT ev = HIWORD(lParam);
        if (ev == WM_MOUSEMOVE) {
            LRESULT retVal = -1;
            for (UI* ui : Window::WindowUI) {
                if(ui->isFocus())
                    ui->UnFocus();
                if (wnd != Window::window) {
                    if (ui->operator==(wnd)) {
                        ui->Focus();
                        retVal = 1;
                    }
                }
                else continue;
            }
            if (retVal < 0)
                return DefWindowProc(hWnd, message, wParam, lParam);
            return retVal;
        }
    }break;
    case WM_CLOSE: {

        if (Window::Destroy())
            PostQuitMessage(0);
    }break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

Window::Window(HINSTANCE hInstance)
{
    InitCommonControls();
    hInst = hInstance;
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_JSONER, szWindowClass, MAX_LOADSTRING);

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0; 
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_JSONER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_JSONER);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));

    hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_JSONER));

    SetWindowText(GetConsoleWindow(), L"Logics - debug console");
}

bool Window::Init(void(*InterfaceCB)(int id, int action))
{
    Window::InterfaceCAllBack = InterfaceCB;
    RegisterClassExW(&wcex);

    window = CreateWindowW(
        szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | DS_CENTER,
        CW_USEDEFAULT, 0, 700, 400, nullptr, nullptr, hInst, nullptr
    );
    minW = 700;
    minH = 400;
    if (!window) {
        MessageBox(NULL, L"ERROR: cannot create window!", L"ERROR #2", MB_ICONERROR | MB_OK);
        return false;
    }
    ShowWindow(window, SW_SHOW);
    SetActiveWindow(window);

    UpdateWindow(window);
    isInited = true;
    return true;
}

bool Window::isOpen()
{
    if (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return true;
    }
    return false;
}

void Window::AddUI(UI* el)
{
    WindowUI.push_back(el);
}

void Window::DestroyAllItems()
{
    for (UI* x : WindowUI) {
        delete x;
    }
    WindowUI.clear();
    UpdateWindow(window);
}

void Window::DestroyItem(int id)
{
    int i = 0;
    for (UI* x : WindowUI) {
        if (x->operator==(id)) {
            delete x;
            WindowUI.erase(WindowUI.begin() + i);
            i--;
        }
        i++;
    }
}

bool Window::Destroy()
{
    int res = MessageBox(Window::window, L"Do you really want to leave?:(", L"Last message in my live :(", MB_OKCANCEL);
    if (res == IDOK) {
        DestroyAllItems();
        DestroyWindow(window);
        UnregisterClass(wcex.lpszClassName, wcex.hInstance);
        DestroyAcceleratorTable(hAccelTable);
        DestroyIcon(wcex.hIcon);
        isInited = false;
        return true;
    }
    return false;
}

void Window::DebugModeOut()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);
}

void Window::MenuItemDisable(int id)
{
    EnableMenuItem(GetMenu(window), id, MF_DISABLED);
}

void Window::MenuItemEnable(int id)
{
    EnableMenuItem(GetMenu(window), id, MF_ENABLED);
}

void Window::DebugModeTurn()
{
    ShowWindow(GetConsoleWindow(), SW_SHOW);

}

void Window::Hide()
{
    ShowWindow(window, SW_HIDE);
    EnableWindow(window, FALSE);
}

void Window::SetMenuIcon(UINT menuID, LPCWSTR path)
{
    HMENU menu = GetMenu(window);
    
    HBITMAP bitMap = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, GetSystemMetrics(SM_CXMENUCHECK), GetSystemMetrics(SM_CYMENUCHECK), LR_LOADFROMFILE);
    if (!bitMap) {
        DWORD err = GetLastError();
        LPVOID lpMsgBuf;
        std::cout<<std::endl << err <<std::endl;
        
        if (err != 0) {
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                err,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)&lpMsgBuf,
                0, NULL);
            MessageBox(window, (LPTSTR)lpMsgBuf, L"WARN", MB_ICONWARNING);
        }
        else {
            MessageBox(window, L"Unknow fatal error!", L"Fatal error", MB_ICONERROR);
            exit(-1);
        }
        return;
    }
    ::SetMenuItemBitmaps(menu, menuID, MF_BYCOMMAND, bitMap, bitMap);

    DeleteObject(bitMap);
}

void Window::SetMenuIcon(UINT menuID, UINT imageID)
{
    HMENU menu = GetMenu(window);

    HBITMAP bitMap = LoadBitmap(NULL, MAKEINTRESOURCE(imageID));
    if (!bitMap) {
        DWORD err = GetLastError();
        LPVOID lpMsgBuf;
        std::cout << std::endl << err << std::endl;

        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            err,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0, NULL);
        MessageBox(NULL, (LPTSTR)lpMsgBuf, L"WARN", MB_ICONWARNING);
        return;
    }
    ::SetMenuItemBitmaps(menu, menuID, MF_BYCOMMAND, bitMap, bitMap);

    DeleteObject(bitMap);
}

void Window::Show()
{
    EnableWindow(window, TRUE);
    ShowWindow(window, SW_SHOW);
}

HWND Window::GetWindow()
{
    return window;
}

bool Window::GetInitStatus()
{
    return isInited;
}

HMENU Window::GetWindowMenu()
{
    return GetMenu(window);
}



