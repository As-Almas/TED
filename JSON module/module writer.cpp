#include "pch.h"
#include "module writer.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
using JSON = nlohmann::json;

void ReadJson(LPWSTR path, JSON&json){
    ifstream reader(path);
    if (!reader.is_open())
        return;

    reader >> json;

    reader.close();

}
void WriteJson(LPWSTR path, JSON& json) {
    ofstream writer(path);
    if (!writer.is_open())
        return;
    
    writer << json.dump(4);

    writer.close();
}

bool Create(LPWSTR path)
{
    bool result = true;
    ofstream file(path);
    result = file.is_open();
    if (result) {
        file << "{}";
        file.close();
    }
    return result;
}


bool AddObjact(LPWSTR path, const char* name, const char* value)
{
    try {

        JSON json;
        ReadJson(path, json);

        json[name] = value;
        WriteJson(path, json);
        json.clear();
        return true;
    }
    catch(...){
        MessageBox(NULL, L"JSON error", L"ERROR INFO", MB_ICONERROR);
        return false;
    }
}

bool AddToObjact(LPWSTR path, const char* arrName, const char* nameOfVal, const char* value)
{
    try
    {
        JSON json;
        ReadJson(path, json);

        json[arrName][nameOfVal] = value;

        WriteJson(path, json);
        json.clear();
        return true;
    }
    catch (...)
    {
        MessageBox(NULL, L"JSON error", L"ERROR INFO", MB_ICONERROR);
        return false;
    }
}
