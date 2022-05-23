bool LoadResources();

bool CreateFolder(TCHAR path[], TCHAR name[]);
void DeleteFolder(TCHAR path[], TCHAR name[]);
bool GenerateFiles(TCHAR path[], TCHAR name[], int type);
bool SetProjactParams(TCHAR path[], TCHAR name[], const char *author, const char *description);

void TerminateResources();