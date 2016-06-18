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
	
	fitnessModel et(500,10000,0.7);
	//et.showNetwork();
    auto l=et.getLevelNodes(2, 3);
    for(auto it:l)
    {
        cout<<it<<endl;
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

