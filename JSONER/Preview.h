#pragma once


#include "Window.h"
#include "Dialog_Creator.h"
#include "../UI/UI_includes.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include <exception>


std::vector<UI*> LoadData(LPCWSTR folderName, Window MainWindow);
void AddProjectPath(LPCSTR path);
void SaveData(LPCWSTR folderName);

