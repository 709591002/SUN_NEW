
#define   LINKITSELF             1   /*a node linking with itself*/
#define   REDUNDANTLINK    2   /* more than one links exist in the same node pairs*/
#define   BEYONDLIMIT         3   /*beyond limit*/
#define   UNCONNECTED       4   /*network is unconnected*/
#define   WRONGCLUSTER    5   /*wrong cluster*/
#define   UNKNOWNMODEL  6   /*unknown network model*/
#define   NOLINK                   7   /*link doesn't exist in the network*/
#define   WRONGPARAMETER   8 /*uncorrected network modeling paramter*/

#define   NOCLUSTER             10  
#define   NOCLUSTEDNODE             11 

#pragma once
#include "stdafx.h"

#ifdef _WIN32  
#include <direct.h>  
#include <io.h>  
#else  
#include <stdarg.h>  
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#endif  

#ifdef _WIN32  
#define ACCESS _access  
#define MKDIR(a) _mkdir((a))  
#else
#define _strdup strdup
#define ACCESS access  
#define MKDIR(a) mkdir((a),0755)  
#endif  

int CreatDir(char * pDir);

void GetDirAndCreate(string FullName);

bool isNum(string str);

void CCol(int back, int col);



