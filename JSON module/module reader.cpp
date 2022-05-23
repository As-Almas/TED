#include "pch.h"
#include "module reader.h"
#include <fstream>

using JSON = nlohmann::json;

void ReadJsonrd(LPWSTR path, JSON& json) {
    ifstream reader(path);
    if (!reader.is_open())
        return;

    reader >> json;

    reader.close();

}


string GetValue(const char* block, LPWSTR pathToFile) {
	JSON json;
    ReadJsonrd(pathToFile, json);

    JSON::iterator it = json.find(block);
    if (it == json.end()) {
        MessageBox(NULL, L"ERROR json format!", L"JSON error", MB_ICONERROR);
        return ""s;
    }
    string s = it.value().dump(4);
    s = s.substr(1);
    s = s.substr(0, s.size() - 1);
    return s;

}

string GetValueFromData(const char* data, const char* block)
{
    JSON json = JSON::parse(data);
    
    
    JSON::iterator it = json.find(block);

    if (it == json.end()) {
        MessageBox(NULL, L"ERROR json format!", L"JSON error", MB_ICONERROR);
        return ""s;
    }
    
    string s = it.value().dump(4);
    s = s.substr(1);
    s = s.substr(0, s.size() - 1);
    return s;
}
