#pragma once

bool Create(LPWSTR path);
bool AddObjact(LPWSTR path, const char* name, const char* value);
bool AddToObjact(LPWSTR path, const char* arrName, const char* nameOfVal, const char* value);
