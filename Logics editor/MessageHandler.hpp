#pragma once
#include "pch.h"

HWND editBox;
LRESULT CALLBACK Handler(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam) {
    
	switch (message)
	{
	case WM_CREATE:
		//editBox = CreateWindow(WC_EDIT, L"// Write your code in this!", WS_CHILD | WS_VISIBLE | WS_MAXIMIZE , 0, 0, 400, 500, wnd, (HMENU)12, NULL, NULL);

		break;
	default:
		break;
	}
    return DefWindowProc(wnd, message, wParam, lParam);
}
