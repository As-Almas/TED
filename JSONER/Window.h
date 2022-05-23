#pragma once
#include "framework.h"
#include "Headers.h"
#include "../UI/UI_includes.h"
#include <vector>

#define MAX_LOADSTRING 100
class UI;
class Window
{
private:
	static MSG msg;
	static HACCEL hAccelTable;
	static HWND window;
	static WNDCLASSEXW wcex;
	static HINSTANCE hInst;
	static int minW, minH;
	static void(*InterfaceCAllBack)(int id, int action);
	static std::vector<UI*> WindowUI;
	static WCHAR szTitle[MAX_LOADSTRING];                  
	static WCHAR szWindowClass[MAX_LOADSTRING];         
	static bool isInited;
public:
	Window(HINSTANCE hInstance);
	static bool Init(void(*InterfaceCB)(int id,int action));
	static bool isOpen();

	static void AddUI(UI* el);

	static void DestroyAllItems();
	static void DestroyItem(int id);
	static bool Destroy();

	static void DebugModeOut();
	static void MenuItemDisable(int id);
	static void MenuItemEnable(int id);
	static void DebugModeTurn();

	static void SetMenuIcon(UINT menuID, LPCWSTR path);
	static void SetMenuIcon(UINT menuID, UINT imageID);

	static void Hide();
	static void Show();

	static bool GetInitStatus();
	static HWND GetWindow();
	static HMENU GetWindowMenu();

	friend LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

