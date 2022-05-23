#pragma once
#include "framework.h"
#include "Headers.h"



class Dialog_Creator
{
private:
	static void(*InerfaceCallBack)(int id, int action);
	static int result;
	static HWND parent;
	static HWND dialogWindow;
	static bool isOpen;
	static void Click(int id);
public:
    friend INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	Dialog_Creator();
	Dialog_Creator(HWND parent, int resourceId, void(*InerfaceCB)(int id, int action));
	static void SetElementText(int id, LPCWSTR text);
	static void SetElementValue(int id, int value);
	static HWND GetElement(int id);
	static int GetResult();
	static void Destroy();
};

