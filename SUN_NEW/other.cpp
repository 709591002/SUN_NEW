#include "other.h"

//����Ŀ¼��֧��linux
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

	// �����м�Ŀ¼  
	for (i = 0; i < iLen; i++)
	{
		if (pszDir[i] == '\\' || pszDir[i] == '/')
		{
			pszDir[i] = '\0';

			//���������,����  
			iRet = ACCESS(pszDir, 0);
			if (iRet != 0)
			{
				iRet = MKDIR(pszDir);
				if (iRet != 0)
				{
					return -1;
				}
			}
			//֧��linux,������\����/  
			pszDir[i] = '/';
		}
	}

	iRet = MKDIR(pszDir);
	free(pszDir);
	return iRet;
}

//���ļ����л�ȡ·������������·��
void GetDirAndCreate(string FullName)
{
	//��ȡĿ¼dir
	FullName = FullName.substr(0, FullName.find_last_of("/"));

	//ת����ȡĿ¼�ĸ�ʽ
	char* temp;
	const int len = FullName.length();
	temp = new char[len + 1];
	strcpy(temp, FullName.c_str());

	//����Ŀ¼
	CreatDir(temp);

}

//�ж�һ���ַ����Ƿ�Ϊ���֣���ȡ�ļ��ж��ã�
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

//�޸Ŀ���̨��ɫ
//������0=�ޣ�1=��
//��ͨ=0����ɫ=1����ɫ=2����ɫ=3
void CCol(int back, int col)
{
#ifdef _WIN32 

	if (back == 0)
	{
		if(col==2)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		else if (col == 1)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		else if (col == 0)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
		else if (col == 3)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	}
	else
	{
		if (col == 2)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_RED);
		else if (col == 1)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_GREEN);
		else if (col == 0)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_INTENSITY);
		else if (col == 3)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_BLUE);

	}
#endif
}