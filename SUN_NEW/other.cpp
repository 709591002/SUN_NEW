#include "other.h"

//创建目录，支持linux
int CreatDir(char *pDir)
{
	int i = 0;
	int iRet;
	int iLen;
	char* pszDir;

	if (NULL == pDir)
	{
		return 0;
	}

	pszDir = strdup(pDir);
	iLen = strlen(pszDir);

	// 创建中间目录  
	for (i = 0; i < iLen; i++)
	{
		if (pszDir[i] == '\\' || pszDir[i] == '/')
		{
			pszDir[i] = '\0';

			//如果不存在,创建  
			iRet = ACCESS(pszDir, 0);
			if (iRet != 0)
			{
				iRet = MKDIR(pszDir);
				if (iRet != 0)
				{
					return -1;
				}
			}
			//支持linux,将所有\换成/  
			pszDir[i] = '/';
		}
	}

	iRet = MKDIR(pszDir);
	free(pszDir);
	return iRet;
}

//从文件名中获取路径名，并创建路径
void GetDirAndCreate(string FullName)
{
	//获取目录dir
	FullName = FullName.substr(0, FullName.find_last_of("/"));

	//转换读取目录的格式
	char* temp;
	const int len = FullName.length();
	temp = new char[len + 1];
	strcpy(temp, FullName.c_str());

	//创建目录
	CreatDir(temp);

}

//判断一个字符串是否为数字（读取文件判断用）
bool isNum(string str)
{
	stringstream sin(str);
	double d;
	char c;
	if (!(sin >> d))
		return false;
	if (sin >> c)
		return false;
	return true;
}
