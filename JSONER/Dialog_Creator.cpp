#include "Dialog_Creator.h"
#include <CommCtrl.h>


void(*Dialog_Creator::InerfaceCallBack)(int id, int action);
int Dialog_Creator::result;
HWND Dialog_Creator::parent;
HWND Dialog_Creator::dialogWindow = NULL;
bool Dialog_Creator::isOpen = false;

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{   
    Dialog_Creator::dialogWindow = hWnd;
    switch (message)
    {
    case WM_INITDIALOG:
        if (*Dialog_Creator::InerfaceCallBack)
            Dialog_Creator::InerfaceCallBack(-1, INIT);
        return TRUE;
    case WM_DESTROY:
        EndDialog(hWnd, 1);
        Dialog_Creator::isOpen = true;
        SetActiveWindow(Dialog_Creator::parent);
        if (*Dialog_Creator::InerfaceCallBack)
            Dialog_Creator::InerfaceCallBack(-1, DESTROY);
        break;
    case WM_COMMAND:
        Dialog_Creator::Click(LOWORD(wParam));
        break;
    }
    if (Dialog_Creator::isOpen == false)
        SendMessage(hWnd, WM_DESTROY, NULL, NULL);
    return DefWindowProc(hWnd, message, wParam, lParam);
}

Dialog_Creator::Dialog_Creator() {
}

Dialog_Creator::Dialog_Creator(HWND parent, int resourceId, void(*InerfaceCB)(int id, int action)) 
{
    Dialog_Creator::parent = parent;
    InerfaceCallBack = InerfaceCB;
    Dialog_Creator::isOpen = true;
    result = DialogBox(GetModuleHandleW(NULL), MAKEINTRESOURCE(resourceId), parent, DialogProc);
}

void Dialog_Creator::Click(int id)
{
    if (*InerfaceCallBack)
        InerfaceCallBack(id, EL_CLICK);
}

void Dialog_Creator::SetElementText(int id, LPCWSTR text) {
    HWND wnd = GetDlgItem(Dialog_Creator::dialogWindow, id);
    SetWindowText(wnd, text);
}
void Dialog_Creator::SetElementValue(int id, int value) {
    HWND wnd = GetDlgItem(Dialog_Creator::dialogWindow, id);
    SendMessage(wnd, PBM_SETPOS, 0, 0);
    for (int i = 0; i < value; i++)
        SendMessage(wnd, PBM_STEPIT, 0, 0);
}
HWND Dialog_Creator::GetElement(int id) {
    return GetDlgItem(Dialog_Creator::dialogWindow, id);
}
void Dialog_Creator::Destroy()
{
    isOpen = false;
    EndDialog(Dialog_Creator::dialogWindow, 1);
}