// SUN_NEW.cpp : 定义控制台应用程序的入口点。
// 重新编写的复杂网络模型

#include "stdafx.h"
#include "multiNet.h"
int main()
{
	//开始计时器
	time_t start = 0, end = 0;
	time(&start);

	//设置默认字体
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

	network n1(100000,DIRECT_WITH_WEIGHT);

	cout << "创建完毕" << endl;

	for (int i = 1; i < n1.nodeNum; i++)
	{
		n1.addLinkToNetwork(i, i + 1, 0.999);
	}

	n1.outputNetwork("aaa/sss.txt");

	//结束计时
	time(&end);
	cout << "程序整体运行了：" << (end - start) << "秒" << endl;
	return 0;
}

