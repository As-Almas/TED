
// help.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "help.h"
#include "doc_nav_initer.h"
#include <fstream>
#include <string>

#pragma comment(lib, "comctl32.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define MAX_LOADSTRING 100

HINSTANCE hInst;
doc_nav_initer init_doc("components/doc/nav.xml");
HWND txtWnd;

void InitDocText(HWND dHwnd) {
	txtWnd = CreateWindowEx(
		WS_EX_LEFTSCROLLBAR,
		WC_EDIT,
		"open the section in the documentation table of contents",
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY,
		0, 10,
		625, 325,
		dHwnd,
		(HMENU)0,
		hInst,
		NULL
	);
	
    EnableWindow(txtWnd, TRUE);
}
void InitDocTree(HWND dHwnd) {
	HWND dlgTree = GetDlgItem(dHwnd, IDC_DOC_TREE);

	init_doc.Init(dlgTree, txtWnd);
}

INT_PTR CALLBACK DialogProcess(HWND dHwnd, UINT dMessage, WPARAM dWparam, LPARAM dLparam) {
    switch (dMessage)
    {
    case WM_INITDIALOG: {
        InitDocText(dHwnd);
        InitDocTree(dHwnd);
        return TRUE;
    }break;
    case WM_COMMAND: {
        switch (dWparam)
        {
        case IDOK:
            EndDialog(dHwnd, 1);
            break;

        }
    }break;
	case WM_NOTIFY: {
	
		switch (((LPNMHDR)dLparam)->code)
		{
		case TVN_SELCHANGED: {
			LPNMTREEVIEW item = (LPNMTREEVIEW)dLparam;
			init_doc.Change(item);
		}break;
		}
	}break;
	case WM_DESTROY: {
		EndDialog(dHwnd, 1);
	}break;
	}
	return (INT_PTR)0;
}

int main() {
	hInst = GetModuleHandle(NULL);
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	DialogBox(hInst, MAKEINTRESOURCE(IDD_HELP_DIALOG), NULL, DialogProcess);
	 
}