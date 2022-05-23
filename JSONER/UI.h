#pragma once
#include <Windows.h>
#include <CommCtrl.h>

#include "UI structs.h"

#define ERROR_UI 13893

class UI
{
private:
	struct DrawBorderType {
		bool top = false;
		bool bottom = false;
		bool left = false;
		bool right = false;
	} border;
	int id;
	HCURSOR cursor;
	int maxHeight = 0, maxWidth = 0;
	UI_PARAMS parametrs;
	DWORD anyStyles = 0;
	DWORD textStyles = 0;
	HWND parentWnd;
	HBRUSH back;
	COLORREF defBack=RGB(255,255,255), FocusBack = RGB(255,255,255), MouseDownBack = RGB(255,255,255);
	COLORREF defText = RGB(0,0,0), FocusText = RGB(0,0,0), MouseDownText = RGB(0,0,0);
	//font territory
	LPCWSTR fontName = L"Times New Roman";
	int fontSize = 20;
	HFONT font = NULL;
	// end of font territory
	LPCWSTR type;
	HWND wnd;
	void(*onClick)(UI* element);
	void(*onFocus)(UI* element);
	void(*onFocusOut)(UI* element);

	bool isFocused = false;
	

	void DrawUIText(DRAWITEMSTRUCT* dc, COLORREF color, int fSize);
	void DrawDef(DRAWITEMSTRUCT* dc);
	void DrawFocus(DRAWITEMSTRUCT* dc);
	void DrawBorder(DRAWITEMSTRUCT* dc);
public:
	int weight = 700;
	BOOL isItalic = FALSE;
	BOOL isUnderLine = FALSE;
	BOOL isStrikeOut = FALSE;

	UI(HWND parent, int id);

	bool Create(LPCWSTR elType, UI_PARAMS params);
	bool Create();


	void Resize();
	void Resize(UI_PARAMS params);
	void Click();
	void Focus();
	void UnFocus();
	void Draw(DRAWITEMSTRUCT*dc);

	void SetText(LPCWSTR text);
	void SetParams(LPCWSTR elType, UI_PARAMS params);
	void SetMaxSize(int width, int height);

	void SetFont(LPCWSTR fontName, int size);
	void AddTextStyle(DWORD userStyle);
	bool LoadFont();
	void UnLoadFont();

	void AddStyle(DWORD userStyle);
	void SetUserCursor(LPWSTR id);
	

	void DrawTopBorder();
	void DrawLeftBorder();
	void DrawRightBorder();
	void DrawBottomBorder();

	void SetDefBackground(int r, int g, int b);
	void SetFocusBackground(int r, int g, int b);

	void SetDefTextColor(int r, int g, int b);
	void SetFocusTextColor(int r, int g, int b);

	void SetOnClick(void(*func)(UI*));
	void SetOnFocus(void(*func)(UI*));
	void SetOnFocusOut(void(*func)(UI*));

	bool isFocus();

	void Destroy();

	HWND GetUIWindow();

	~UI();


	bool operator ==(const int otherId);
	bool operator !=(const int otherId);

	bool operator ==(const UI otherUI);
	bool operator !=(const UI otherUI);

	bool operator ==(const HWND otherWindow);
	bool operator !=(const HWND otherWindow);

	void operator =(const UI otherUI);
};

