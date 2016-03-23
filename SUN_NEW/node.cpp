#include "node.h"

#pragma region 节点

node::node()
{

}

//节点类构造函数
node::node(int id)
{
	adjOut.clear();			//初始化出度邻接容器
	numAdjOut = 0;			//初始化出度邻居数量
	adjIn.clear();			//初始化入度邻接容器
	numAdjIn = 0;			//初始化入度邻居数量

	adjMultiOut.clear();	//初始化多网络出度邻接节点信息
	numAdjMultiOut = 0;		//初始化多网络出度邻居节点个数
	adjMultiIn.clear();		//初始化多网络入度邻接节点信息
	numAdjMultiIn = 0;		//初始化多网络入度邻居节点个数
	strength = 0.0;			//初始化节点权重
	this->id = id;			//初始化节点ID号
}

//节点类析构函数
node::~node()
{
	adjOut.clear();		//清空邻接容器
}

//打印信息
void node::showNode()
{
	cout << "---------------------------------------" << endl;
	//打印该节点基础信息

	//打印节点基本信息
	cout << "节点编号：" << id << " ,节点权重：" << strength << endl;

	//打印出度
	if (numAdjOut != 0)
	{
		cout << endl;
		//改变字体颜色为红
		CCol(0, 2);
		//map迭代器，备用
		//map <int, double>::iterator it;
		cout << " " << numAdjOut << " 个出度：" << endl;
		for (auto it : adjOut)
			cout << id << "  -->>  " << it.first << " ,权重：" << it.second << endl;
	}

	//打印入度
	if (numAdjIn != 0)
	{
		cout << endl;
		//改变字体颜色为绿
		CCol(0, 1);
		//map迭代器，备用
		//map <int, double>::iterator it;

		cout << " " << numAdjIn << " 个入度：" << endl;
		for (auto it : adjIn)
			cout << id << "  <<--  " << it.first << " ,权重：" << it.second << endl;
	}

	//打印多网络出度
	if (numAdjMultiOut != 0)
	{
		cout << endl;
		//改变字体颜色为红
		CCol(1, 2);
		//map迭代器，备用
		//map <int, double>::iterator it;
		cout << " " << numAdjMultiOut << " 个多网络出度：" << endl;
		for (auto it : adjMultiOut)
			for (auto it2 : it.second)
				cout << "  -->>  " << " 网络 " << it.first << " 的 " << it2.first << " ,权重：" << it2.second << endl;
	}

	//打印多网络入度
	if (numAdjMultiIn != 0)
	{
		cout << endl;
		//改变字体颜色为绿
		CCol(1, 2);
		//map迭代器，备用
		//map <int, double>::iterator it;

		cout << " " << numAdjMultiIn << " 个多网络入度：" << endl;
		for (auto it : adjMultiIn)
			for (auto it2 : it.second)
				cout << "  <<--  " << " 网络 " << it.first << " 的 " << it2.first << " ,权重：" << it2.second << endl;
	}

	cout << endl;
	//改变字体颜色为普通
	CCol(0, 0);

}

//增加到nodeid节点的链接
int node::addLinkToNode(int nodeid //为当前节点增加一个到nodeid的链接 
	, double weight)//该链接的权重
{
	if (nodeid == id)
	{
		int error = LINKITSELF;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误，让节点 " << nodeid << " 连接到自己了！" << endl;
		//设置默认字体
		CCol(0, 0);

		return error;
	}

	map<int, double>::iterator it;

	//检查是否是多余的链接
	it = adjOut.find(nodeid);
	if (it != adjOut.end())
	{
		int error = REDUNDANTLINK;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误，" << id << " -->> " << nodeid << " 的链接已经建立。" << endl;
		//设置默认字体
		CCol(0, 0);

		return error;
	}



	//为相关容器增加邻居节点信息，只包括出度，入度部分在network实现
	//adjOut.insert(map<int, double>::value_type(nodeid,weight));
	adjOut.insert(make_pair(nodeid, weight));

	//邻居数量增加一
	numAdjOut++;

	return 0;
}

//删除从该节点到nodeid的链接，即删除出度
int node::delLinkFromNode(int nodeid)
{
	//检查是否是删除自身
	if (nodeid == id)
	{
		int error = LINKITSELF;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误，不能删除节点自身！" << endl;
		//设置默认字体
		CCol(0, 0);

		return error;
	}

	map <int, double>::iterator it;

	//检查是否有链接

	it = adjOut.find(nodeid);
	if (it != adjOut.end())
	{
		adjOut.erase(it);	//删除元素
		numAdjOut--;		//邻接数量减少1
	}
	else
	{
		int error = NOLINK;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误," << id << " -->> " << nodeid << " 的连接根本不存在！" << endl;
		//设置默认字体
		CCol(0, 0);

		return error;
	}

	return 0;

}

//增加从nodeid入度的链接信息
int node::addLinkFromNode(int nodeid		//从编号为nodeid的节点入度
	, double weight)	//该链接的权重
{
	//检查链接是否指向自己
	if (nodeid == id)
	{
		int error = LINKITSELF;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误，让节点 " << nodeid << " 被自己指向了！" << endl;
		//设置默认字体
		CCol(0, 0);

		return error;
	}

	map <int, double>::iterator it;
	it = adjIn.find(nodeid);
	if (it != adjIn.end())
	{
		int error = REDUNDANTLINK;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误，" << id << " <<-- " << nodeid << " 的入度链接已经建立。" << endl;
		//设置默认字体
		CCol(0, 0);

		return error;
	}

	adjIn.insert(make_pair(nodeid, weight));
	numAdjIn++;
	return 0;
}

//删除从nodeid到该节点的链接，即删除入度
int node::delLinkToNode(int nodeid)
{
	//检查是否是删除自身
	if (nodeid == id)
	{
		int error = LINKITSELF;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误，不能删除节点自身！" << endl;
		//设置默认字体
		CCol(0, 0);

		return error;
	}

	map <int, double>::iterator it;

	//检查是否有链接

	it = adjIn.find(nodeid);
	if (it != adjIn.end())
	{
		adjIn.erase(it);	//删除元素
		numAdjIn--;		//邻接数量减少1
	}
	else
	{
		int error = NOLINK;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误,无法删除 " << id << " -->> " << nodeid << " ，链接不存在。" << endl;

		//设置默认字体
		CCol(0, 0);

		return error;
	}

	return 0;

}

//增加到netid网络的nodeid节点的多网络类型链接，即增加多网络出度
int node::addLinkToNodeMulti(int netid			//目标网络ID
	, int nodeid, double weight)	//为当前节点增加一个到nodeid的多网络类型链接，权值为weight，出度 
{
	//检查是否有指向netid网络的数据，如果没有则建立
	auto it1 = adjMultiOut.find(netid);
	if (it1 == adjMultiOut.end())
	{
		map<int, double> temp;						//临时空map容器
		adjMultiOut.insert(make_pair(netid, temp));	//初始化指向netid的记录
	}

	//检查指向netid网络的nodeid节点的数据是否存在

	auto it = adjMultiOut[netid].find(nodeid);
	if (it != adjMultiOut[netid].end())
	{
		int error = REDUNDANTLINK;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误，本节点 -->> 网络 " << netid << " 的节点 " << nodeid << " 的链接已经建立。" << endl;
		//设置默认字体
		CCol(0, 0);

		return error;
	}
	else
	{
		//如果不存在，则添加记录
		adjMultiOut[netid].insert(make_pair(nodeid, weight));
		numAdjMultiOut++;
	}
	return 0;

}

//删除到netid网络的nodeid节点的多网络类型链接，即删除多网络出度
int node::delLinkToNodeMulti(int netid				//目标网络ID
	, int nodeid)					//删除当前节点到nodeid的多网络类型链接，即删除出度
{
	//检查是否有指向netid网络的数据，如果没有则返回错误
	auto it1 = adjMultiOut.find(netid);
	if (it1 == adjMultiOut.end())
	{
		int error = NOLINK;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误,本节点 -->> 网络 " << netid << " 的节点 " << nodeid << " 的链接不存在！无法删除。" << endl;
		//设置默认字体
		CCol(0, 0);

		return error;
	}

	//检查指向netid网络的nodeid节点的数据是否存在，不存在则无法删除，返回错误

	auto it = adjMultiOut[netid].find(nodeid);
	if (it != adjMultiOut[netid].end())
	{
		//如果找到记录，则删除记录
		adjMultiOut[netid].erase(it);
		numAdjMultiOut--;
	}
	else
	{
		int error = NOLINK;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误,本节点 -->> 网络 " << netid << " 的节点 " << nodeid << " 的链接不存在！无法删除。" << endl;
		//设置默认字体
		CCol(0, 0);

		return error;
	}
	return 0;
}

//增加从netid网络的nodeid节点到当前节点的多网络类型链接，即增加多网络入度
int node::addLinkFromNodeMulti(int netid			//目标网络ID
	, int nodeid, double weight)	//增加一个从nodeid到当前节点的多网络类型链接，权值为weight，入度 
{
	//检查是否有从netid网络来的数据，如果没有则返回错误
	auto it1 = adjMultiIn.find(netid);
	if (it1 == adjMultiIn.end())
	{
		map<int, double> temp;						//临时空map容器
		adjMultiIn.insert(make_pair(netid, temp));	//初始化指向netid的记录
	}
	//检查从netid网络的nodeid节点来的数据是否存在，如果存在则返回错误
	auto it = adjMultiIn[netid].find(nodeid);
	if (it != adjMultiIn[netid].end())
	{
		int error = REDUNDANTLINK;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误， 本节点 <<-- 网络 " << netid << " 的节点 " << nodeid << " 的链接已经建立。" << endl;
		//设置默认字体
		CCol(0, 0);

		return error;
	}
	else
	{
		//如果不存在，则添加记录
		adjMultiIn[netid].insert(make_pair(nodeid, weight));
		numAdjMultiIn++;
	}
	return 0;
}

//删除从netid网络的nodeid节点到当前节点的多网络类型链接，即删除多网络入度
int node::delLinkFromNodeMulti(int netid			//目标网络ID
	, int nodeid)			//删除从nodeid到当前节点的多网络类型链接，即删除入度
{
	//检查是否有从netid网络来的数据，如果没有则返回错误
	auto it1 = adjMultiIn.find(netid);
	if (it1 == adjMultiIn.end())
	{
		int error = NOLINK;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误,本节点 <<-- 网络 " << netid << " 的节点 " << nodeid << " 的链接不存在！无法删除。" << endl;
		//设置默认字体
		CCol(0, 0);

		return error;
	}
	//检查从netid网络的nodeid节点来的数据是否存在，不存在则返回错误
	auto it = adjMultiIn[netid].find(nodeid);
	if (it != adjMultiIn[netid].end())
	{
		//如果找到记录，则删除记录
		adjMultiIn[netid].erase(it);
		numAdjMultiIn--;
	}
	else
	{
		int error = NOLINK;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误,本节点 <<-- 网络 " << netid << " 的节点 " << nodeid << " 的链接不存在！无法删除。" << endl;
		//设置默认字体
		CCol(0, 0);

		return error;
	}
	return 0;
}

#pragma endregion