#include "pch.h"
#include "../UI/UI_includes.h"
#include <iostream>
#include <vector>

#pragma comment(lib, "trash/UI.lib")

using namespace std;

const  int idOfEditor = 1;
COLORREF back = RGB(255, 255, 255),
txt = RGB(0, 0, 0),
commands = RGB(0, 0, 0),
functions = RGB(0, 0, 0);

bool LoadEditorResources(HWND mainWnd) {
	return false;
}


bool InitEditor(HWND mainWnd) {
	
	return true;
}