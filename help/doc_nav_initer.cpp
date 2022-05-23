#include "doc_nav_initer.h"
#include <fstream>
#include <vector>
#include <string>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#pragma comment(lib, "libxml2.lib")
#pragma comment(lib, "comctl32.lib")
using namespace std;

static string EnAlphabet = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz1234567890";
vector<const char*> paths;
HWND hwndTree;
HWND textWindow;
xmlDoc* doc;

HTREEITEM doc_nav_initer::InsTreeItem( HTREEITEM hParent, LPSTR szText, HTREEITEM hAfter, int id)
{
	if (!hwndTree)
		return NULL;
	TV_INSERTSTRUCT tvins;
	HTREEITEM hItem;

	memset(&tvins, 0, sizeof(tvins));
	tvins.item.mask = TVIF_TEXT | TVIF_PARAM;
	tvins.item.pszText = szText;
	tvins.item.cchTextMax = lstrlen(szText);
	tvins.hInsertAfter = hAfter;
	tvins.hParent = hParent;
	tvins.item.lParam = id;
	hItem = TreeView_InsertItem(hwndTree, &tvins);
	return hItem;
}

bool doc_nav_initer::ParseXmlData()
{
	// getting root element of navigation file and if root is NULL return false
	xmlNode* root = xmlDocGetRootElement(doc);
	if (!root)
		return false;
	// id of clickable element
	int i = 1;
	// tree parent item
	HTREEITEM parentItem = NULL;
	// parse xml 
	for (xmlNode* header = root->children; header != NULL; header = header->next) {
		// return false if tree window is not created
		if (!hwndTree)
			return false;
		// check on 'header' element
		if (header->type == XML_ELEMENT_NODE) {
			// parse 'header' element of navigation xml
			for (xmlNode* text = header->children; text != NULL; text = text->next) {
				// code in this if func create a new parent item of tree
				if (text->type == XML_TEXT_NODE) {
					const char* buf = (const char*) xmlNodeGetContent(text);
					bool isHaveSymbol = false;
					// check string on symbol
					for (auto symbol : string(buf)) {
						if (isHaveSymbol)
							break;
						for (auto AlphaSymbol : EnAlphabet)
						{
							if (symbol == AlphaSymbol)
								isHaveSymbol = true;
						}
					}
					//if string have a symbol this code create new parent element of tree
					if (isHaveSymbol)
						parentItem = InsTreeItem(NULL, (char*)buf, TVI_LAST);
				}
				// this code check xml on 'el' element
				// if 'el' element have path and text this code created new navigation item
				if (text->type == XML_ELEMENT_NODE) {
					xmlChar* path = xmlGetProp(text, BAD_CAST"path");
					xmlChar* txt = xmlGetProp(text, BAD_CAST"text");
					if (string((const char*)path).empty())
						continue;
					if (string((const char*)txt).empty())
						continue;
					// check parentItem and create tree item
					if (parentItem != NULL) {
						if (InsTreeItem(parentItem, (char*)txt, TVI_LAST, i) != NULL) {
							paths.push_back((const char*)path);
							i++;
						}
					}
				}
			}
		}
	}
	return true;
}

doc_nav_initer::doc_nav_initer(LPCSTR pathToNavFile)
{
	doc = xmlReadFile(pathToNavFile, "UTF-8", NULL);
	if (!doc) {
		InsTreeItem(NULL, (char*)"ERROR: not found", TVI_LAST);
		return;
	}
}


bool doc_nav_initer::Init(HWND Tree, HWND txtWnd)
{
	hwndTree = Tree;
	textWindow = txtWnd;
	return ParseXmlData();
}

void doc_nav_initer::Change(LPNMTREEVIEW item)
{
	int id = item->itemNew.lParam;
	if (id == 0)
		return;
	if (paths.size() < id)
		return;
	
	ifstream fileReader(paths[id - 1]);
	if (!fileReader.is_open()) {
		SetWindowText(textWindow, string("Not found: \""s + paths[id-1] + "\" file"s).c_str());
		return;
	}
	string docTxt;
	string line;
	while (getline(fileReader, line))
	{
		docTxt += line + "\r\n";
	}

	SetWindowText(textWindow, docTxt.c_str());
	fileReader.close();
}


doc_nav_initer::~doc_nav_initer()
{
	hwndTree = NULL;
	if(doc != NULL)
		xmlFreeDoc(doc);
	doc = NULL;
	paths.clear();
}

