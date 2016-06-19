// SUN_NEW.cpp : 定义控制台应用程序的入口点。
// 重新编写的复杂网络模型

#include "stdafx.h"
#include "model.h"

int main()
{
	//开始计时器
	time_t start = 0, end = 0;
	time(&start);

	//设置默认字体
	CCol(0, 0);
	////////////////////////////////////////////////////////////////////////////////////////////
	
	fitnessModel et(50,1000,0.7);
	//et.showNetwork();
	et.refreshCI(2);
	for (auto it : et.nodes)
	{
		cout << it.first << "," << it.second.ci << endl;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////
    
	//结束计时
	time(&end);
    
    //计算时间差
	double timeAll;
	timeAll = difftime(end , start);
	cout << "程序整体运行了：" <<(int)timeAll/3600 << " 小时 "
                            <<(int)timeAll/60%60 << " 分钟 "
                            <<(int)timeAll%60 << " 秒 。" << endl;
	return 0;
}

