#pragma once
#include "stdafx.h"

#ifdef _WIN32  
#include <direct.h>  
#include <io.h>  
#else  
#include <stdarg.h>  
#include <sys/stat.h>  
#endif  

#ifdef _WIN32  
#define ACCESS _access  
#define MKDIR(a) _mkdir((a))  
#else
#define ACCESS access  
#define MKDIR(a) mkdir((a),0755)  
#endif  

int CreatDir(char * pDir);

void GetDirAndCreate(string FullName);

bool isNum(string str);

void CCol(int back, int col);



