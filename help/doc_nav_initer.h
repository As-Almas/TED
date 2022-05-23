#pragma once
#include "framework.h"


class doc_nav_initer
{
public:
	doc_nav_initer(LPCSTR pathToNavFile);
	static bool Init(HWND Tree, HWND txtWnd);
	static void Change(LPNMTREEVIEW item);
	~doc_nav_initer();
private:
	static bool ParseXmlData();
	static HTREEITEM InsTreeItem(HTREEITEM hParent, LPSTR szText, HTREEITEM hAfter, int id = 0);
};

