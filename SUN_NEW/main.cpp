// SUN_NEW.cpp : �������̨Ӧ�ó������ڵ㡣
// ���±�д�ĸ�������ģ��

#include "stdafx.h"
#include "multiNet.h"
int main()
{
	//��ʼ��ʱ��
	time_t start = 0, end = 0;
	time(&start);

	//����Ĭ������
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

	network n1(100000,DIRECT_WITH_WEIGHT);

	cout << "�������" << endl;

	for (int i = 1; i < n1.nodeNum; i++)
	{
		n1.addLinkToNetwork(i, i + 1, 0.999);
	}

	n1.outputNetwork("aaa/sss.txt");

	//������ʱ
	time(&end);
	cout << "�������������ˣ�" << (end - start) << "��" << endl;
	return 0;
}

