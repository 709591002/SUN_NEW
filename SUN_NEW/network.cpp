#include "network.h"
#include <map>
//#include "multiNet.h"

using namespace std;


/////////////////////////
/////////////////////////
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


///////////////////////////
///////////////////////////
#pragma region 单网络 
//默认构造函数
network::network()
{

}

//通过多网络构造单网络
network::network(multiNet mul)
{
	//重新分配多网络节点编号

	//从1开始分配
	int num = 1;
	for (auto itnet:mul.nets)
	{
		for (auto itnodes : mul.nets[itnet.first].nodes)
		{
			//如果不存在顺序节点，则规定新节点
			if (!mul.nets[itnet.first].isValidNode(num))
			{
				mul.changeNodeid(itnet.first, itnodes.first, num);
				cout << itnet.first << "," << itnodes.first << "," << num << endl;
			}
			//cout << num << endl;
			num++;
		}
		cout << "next" << endl;
	}

		cout <<"result: "<<num<< " , " <<mul.nodeNum<< endl;

	//逐个添加网络节点
}

//构造函数
network::network(int N		//节点的数量
				,int start)  //网络起节点
{
	linkNum = 0;
	netid = 0;			//默认网络编号为0

	if (start < 0)
	{
		CCol(0, 2);
		cout << "网络初始化无法从负数开始！故本网络从0开始构造。" << endl;
		CCol(0, 0);
		start = 0;
	}
	//如果规定节点数大于0
	if (N>0)
	{
		nodeNum = 0;
		for (int i = start; i < start+N; i++)
		{
			addNodeToNetwork(i);
		}
	}
	//如果节点个数为0
	else if(N==0)
	{
		nodeNum = 0;
	}
	//如果节点数小于0
	else
	{
		CCol(0, 2);
		cout << "网络初始化节点数量无法为负数！已返回默认对象！" << endl;
		CCol(0, 0);
		new (this) network(1, 0);
	}

}

//读取文件构造函数，自动识别普通格式和pajek格式
network::network(string in)
{
	nodeNum = 0;				//初始化节点数量
	linkNum = 0;				//初始化链接数量
	netid = 0;					//默认网络编号为0
	ifstream file(in.data(), ios::in);

	//检查文件是否读取失败
	if (file.fail())
	{
		CCol(0,2);
		cout << "读取文件失败，路径为： " << in.c_str() << "，已自动将本对象替换为 空网络。" << endl;
		CCol(0,0);
		new (this) network(1,0);
	}
	//文件没有读取失败，正常进行
	else 
	{
		int error = 0;				//错误标记
		CCol(0,1);
		cout << "读取文件成功，路径为： " << in.c_str()  << endl;
		CCol(0,0);
		string line;				//储存临时一行
		
		getline(file, line);		//开始读取节点信息，即第一行
		stringstream stream(line);	//处理一行信息，以空格为分隔符
		
		string tempNum;
		stream >> tempNum;			//获取网络节点数量
		
		//判断是否为pajek格式
		if (isNum(tempNum))
		{
			string tempTwo;
			stream >> tempTwo;						//获取网络类型

			//判断第二个是否为数字，如果是则为无权格式，如果不是则为孙老师格式
			if (isNum(tempTwo))
			{
				//继续处理第一行
				int firstNum = atoi(tempNum.c_str());
				addNodeToNetwork(firstNum);
				int secondNum = atoi(tempTwo.c_str());
				addNodeToNetwork(secondNum);
				//添加第一对儿信息
				addLinkToNetwork(firstNum, secondNum, 1);

				int tempNum;
				while (stream >> tempNum)
				{
					//如果不存在节点，则建立节点
					if (!isValidNode(tempNum))
					{
						addNodeToNetwork(tempNum);
					}
					addLinkToNetwork(firstNum, tempNum,1);
				}
				//第一行处理完毕

				//开始处理剩余所有
				while (!file.eof())
				{
					getline(file, line);		//获取临时一行
					stringstream stream(line);	//处理一行信息，以空格为分隔符

					if (file.fail()) {
						break;
					}

					//每一行的第一个节点
					stream >> firstNum;
					if (!isValidNode(firstNum))
					{
						addNodeToNetwork(firstNum);
					}

					while (stream >> tempNum)
					{
						//如果不存在节点，则建立节点
						if (!isValidNode(tempNum))
						{
							addNodeToNetwork(tempNum);
						}
						addLinkToNetwork(firstNum, tempNum, 1);
					}

				}


				CCol(0, 1);
				cout << "无权格式文件读取完毕，路径为： " << in.c_str() << endl;
				CCol(0, 0);

			}
			//孙老师格式
			else if (tempTwo=="sun")
			{
			//int nodeNumF = atoi(tempNum.c_str());		//储存网络节点数，包含string转换为int的过程

			//开始读取链接文件信息，循环“节点数量”次
			while (!file.eof())
			{
				getline(file, line);		//获取临时一行
				stringstream stream(line);	//处理一行信息，以空格为分隔符

				if (file.fail()) {
					break;
				}

				int id;						//储存临时节点id号信息
				int adjNumOut;				//储存临时节点出度信息

				stream >> id;				//获取节点id号信息
				stream >> adjNumOut;		//获取临时节点出度信息

				//每行开始处，向网络中加入编号节点
				if (!isValidNode(id))
				{
					addNodeToNetwork(id);
					
				}

				/*删除线性判断，已改为map容器
				//检查是否出错
				if (id != i)
				{
					error = 1;
					break;
				}
				*/

				//开始加入链接，循环“邻居数量”次
				for (int i = 0; i < adjNumOut; i++)
				{
					//获取邻居和权重
					int neighbor;
					double weight;
					stream >> neighbor;
					stream >> weight;

					if (!isValidNode(neighbor))
					{
						addNodeToNetwork(neighbor);

					}

					//开始加链接
					addLinkToNetwork(id, neighbor, weight);
					//linkNum++;
				}

			}
			CCol(0, 1);
			cout << "普通格式文件读取完毕，路径为： " << in.c_str() << endl;
			CCol(0, 0);
		}
		}
		//判断为pajek格式成功
		else if(tempNum == "*vertices")
		{
			int nodeNumF;
			stream >> nodeNumF; //读取节点个数

			/*修改后非线性增加
			//开始根据节点数量增加节点容器
			for (int i = 1; i <= nodeNum; i++)
			{
				nodes.insert(nodes.end(), i);
			}
			*/

			//循环到*arcs出现，并判断格式是否合法
			for (int i = 1; i <= nodeNumF; i++)
			{
				getline(file, line);		//获取临时一行
				stringstream stream(line);	//处理一行信息，以空格为分隔符
				int tempNum;
				stream >> tempNum;
				if (!isValidNode(tempNum))
				{
					addNodeToNetwork(tempNum);
				}
			}
			//如果还没报错，则继续
			if (!error)
			{
				getline(file, line);		//获取临时一行
				stringstream stream(line);	//处理一行信息，以空格为分隔符

				string tempNum;				//读取一下，看看是不是"*arcs"，
				stream >> tempNum;

				//如果不是说明格式错误
				if (tempNum == "*arcs")
				{
					//开始读取链接信息
					while (!file.eof())
					{
						getline(file, line);		//获取临时一行
						stringstream stream(line);	//处理一行信息，以空格为分隔符

						if (file.fail()) {
							break;
						}

						int from, to;				//储存起止节点
						double weight;				//储存权值信息
						stream >> from;
						stream >> to;
						stream >> weight;
						addLinkToNetwork(from, to, weight);
					}
				}
				else
				{
					error = 1;
				}

			}
			

			CCol(0,1);
			cout << "pajek格式文件读取完毕，路径为： " << in.c_str() << endl;
			CCol(0,0);
		}
		//什么格式都不是，返回错误
		else 
		{
			error = 1;
		}
		
		if (error)
		{
			CCol(0,2);
			cout << "文件格式错误，路径为： " << in.c_str() << "，已自动将本对象替换为 空网络。" << endl;
			CCol(0,0);
			nodes.clear();		//清空节点信息
			new (this) network(1,0);
		}
		
	}


}

//读取文件构造函数，自动识别普通格式和pajek格式，带网络编号
network::network(string in, int netid)
{

	//调用默认读取文件构造函数
	new (this) network(in);

	//改写网络编号
	this->netid = netid;

}

/*
//拷贝构造函数
//除非特殊需要，用系统默认的即可（vector容器要求）
network::network(network &n2)
{
	nodes.assign(n2.nodes.begin(), n2.nodes.end());	//拷贝节点容器
	nodeNum=n2.nodeNum;								//拷贝节点数量
	linkNum=n2.linkNum;								//拷贝边的数量
	type=n2.type;									//拷贝网络类型
	netid = n2.netid;								//拷贝网络编号
}
*/

//析构函数
network::~network()
{
	//都是容器，没有new，没有什么可以释放的
}

//显示网络
void network::showNetwork() 
{
	cout << "=====================================================" << endl;
	cout <<"网络编号："<<netid <<" ,共有节点 "<<nodeNum<<" 个，链接  "<<linkNum<<" 条。"<< endl;
	
	//遍历所有节点
	for (auto it:nodes)
	{
			it.second.showNode();
	}
	cout << "=====================================================" << endl;
}

//添加网络链接
int network::addLinkToNetwork(int from, int to //从from到to增加链接
	, double weight)   //增加链接的权重
{
	int error;

	//检查from到to是否越界
	if (!isValidNode(from) || from < 0 || !isValidNode(to) || to < 0)
	{
		error = BEYONDLIMIT;

		//改变字体颜色为红
		CCol(0,2);
		cout << "发生一个错误，无法为 网络"<<netid<<" 添加从 " << from << " -->> " << to << " 的链接，因为越界了。" << endl;
		//改变字体颜色为普通
		CCol(0,0);
		
		return error;
	}

	//添加节点链接
	error = nodes[from].addLinkToNode(to, weight);	//在from添加出度信息
	error = nodes[to].addLinkFromNode(from, weight);//在to添加入度信息

	//如果添加节点出错，则返回错误信息
	if (error)
	{
		return error;
	}

	//网络链接数，加一
	linkNum++;
	return 0;
}

//删除网络链接
int network::delLinkFromNetwork(int from, int to)//从from到to删除链接
{
	int error=0;
	
	//检查from到to是否越界
	if (!isValidNode(from) || from < 0 || !isValidNode(to) || to < 0)
	{
		error = BEYONDLIMIT;

		//改变字体颜色为红
		CCol(0,2);
		cout << "发生一个错误 ，无法删除 网络 "<<netid<<"的 " << from << " --> " << to << " 的链接，因为越界了。" << endl;
		//改变字体颜色为普通
		CCol(0,0);
		
		return error;
	}
	
	if(isValidLink(from,to))
	{ 
	//删除节点之间链接
	error = nodes[from].delLinkFromNode(to);	//删除from的出度信息
	error = nodes[to].delLinkToNode(from);	//删除to的入度信息
	}
	else
	{
		error = NOLINK;

		//改变字体颜色为红
		CCol(0,2);
		cout << "发生一个错误 ，无法删除从 " << from << " --> " << to << " 的链接，因为没有链接。" << endl;
		//改变字体颜色为普通
		CCol(0,0);

		return error;
	}
	//如果没有错误，链接数减一
	if (error == 0) 
	{
		linkNum--;
	}

	return error;
}

//为网络增加节点
int network::addNodeToNetwork(int N)//为网络增加编号为N的节点
{
	if (N < 0)
	{
		int error=1;
		//改变字体颜色为红
		CCol(0,2);
		cout << "发生一个错误 ，无法为 网络 "<<netid<<" 增加编号为 " << N << " 的节点，它编号小于0" << endl;
		//改变字体颜色为普通
		CCol(0,0);

		return error;
	}

	if (isValidNode(N))
	{
		int error = 1;
		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误 ，无法为 网络 " << netid << " 增加编号为 " << N << " 的节点，它已存在！" << endl;
		//改变字体颜色为普通
		CCol(0, 0);
        
        return error;
	}

	//增加编号为N的节点
	node temp(N);
		nodes.insert(make_pair(N,node(N)));

	//增加节点数
	nodeNum ++;
	return 0;
}

//从网络中删除节点
int network::delNodeFromNetwork(int nodeid) //要删除节点编号
{
	int error = 0;


	if (!isValidNode(nodeid))
	{
		error = REDUNDANTLINK;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误 ，要删除的节点不存在 " << nodeid << endl;
		//改变字体颜色为普通
		CCol(0, 0);

		return error;
	}


	//判断是否和其它网络有联系，如果有，则无法删除
	if (nodes[nodeid].numAdjMultiIn!=0|| nodes[nodeid].numAdjMultiOut!=0)
	{
		error = BEYONDLIMIT;

		//改变字体颜色为红
		CCol(0,2);
		cout << "发生一个错误 ，无法删除 网络"<<netid<<" 的 节点"<<nodeid<<" ，它还存在多网络链接 " << endl;
		//改变字体颜色为普通
		CCol(0,0);

		return error;
	}



	//map迭代器，备用
	//map <int, double>::iterator it;

	//删除所有指向nodeid的链接，即删除nodeid的所有入度
	for (auto it : nodes[nodeid].adjIn)
	{
		//访问nodeid的入度表里的每一个节点，将所有有关nodeid信息删除
		nodes[it.first].delLinkFromNode(nodeid);	
	}

	//遍历所有从nodeid指向的节点的入度信息，让nodeid不再指向它们
	for (auto it : nodes[nodeid].adjOut)
	{
		nodes[it.first].delLinkToNode(nodeid);
	}

	//网络链接数减少，差值为被删除节点的邻居数和
	linkNum -= nodes[nodeid].numAdjOut + nodes[nodeid].numAdjIn;

	//删除所有在nodeid保存的出度和入度的链接
	nodes[nodeid].adjOut.clear();
	nodes[nodeid].adjIn.clear();

	//彻底删除该节点
	nodes.erase(nodeid);

	//减少网络节点数
	nodeNum--;

	return 0;
}

//验证是否有从from到to的链接
int network::isValidLink(int from, int to)
{
	//验证节点是否存在，不存在则返回0
	if (!isValidNode(from) || !isValidNode(to))
	{
		return 0;
	}

	//验证是否相同，如果相同就返回0
	if (from == to)
	{
		return 0;
	}

	//如果form或to超出范围，则返回0
	if (from < 0 || to < 0)
	{
		return 0;
	}

	//看看能不能找到
	auto it=nodes[from].adjOut.find(to);
	
	//如果找到了就返回1
	if (it != nodes[from].adjOut.end())
	{
		return 1;
	}
	return 0;
}


int network::isValidNode(int nodeid)
{

	//要找的节点小于0，则返回错误
	if (nodeid < 0)
	{
		return 0;
	}


	//看看能不能找到
	auto it = nodes.find(nodeid);

	//如果找到了就返回1
	if (it != nodes.end())
	{
		return 1;
	}
	return 0;
}


//设置从from到to的权值为weight
int network::setLinkWeight(int from, int to, double weight)
{
	//如果搜索越界，则显示错误信息
	if (!isValidNode(from) || from < 0 || !isValidNode(to) || to < 0)
	{
		int error = BEYONDLIMIT;
		//改变字体颜色为红
		CCol(0,2);
		cout << "发生一个错误，无法设置 网络" <<netid<< " 的 " << from << " -->> " << to << " 的权值，因为节点不存在。" << endl;
		//改变字体颜色为普通
		CCol(0,0);
		return error;
	}

	//尝试在from的出度列表寻找从from到to的链接
	auto it = nodes[from].adjOut.find(to);
	if (it != nodes[from].adjOut.end())
	{
		it->second = weight;
	}
	else
	{
		int error = NOLINK;
		
		//改变字体颜色为红
		CCol(0,2);
		cout << "发生一个错误，无法设置 网络"<<netid<<" 的 " << from << " -->> " << to << " 的权值，因为出度信息不存在。" << endl;
		//改变字体颜色为普通
		CCol(0,0);

		return error;
	}

	//尝试在to的入度列表寻找从from到to的链接
	it = nodes[to].adjIn.find(from);
	if (it != nodes[to].adjIn.end())
	{
		it->second = weight;
	}
	else
	{
		int error = NOLINK;

		//改变字体颜色为红
		CCol(0,2);
		cout << "发生一个错误，无法设置 网络" << netid << " 的 " << from << " -->> " << to << " 的权值，因为入度信息不存在。" << endl;
		//改变字体颜色为普通
		CCol(0,0);

		return error;
	}
	return 0;
}

//获取从from到to的网络链接权值
double network::getLinkWeight(int from, int to)
{
	//如果搜索越界，则显示错误信息
	if (!isValidNode(from) || from < 0 || !isValidNode(to) || to < 0)
	{
		int error = BEYONDLIMIT;
		//改变字体颜色为红
		CCol(0,2);
		cout << "发生一个错误，无法获取从 网络" << netid << " 的 " << from << " -->> " << to << " 的权值，因为越界了。" << endl;
		//改变字体颜色为普通
		CCol(0,0);
		return error;
	}

	auto it = nodes[from].adjOut.find(to);
	if (it != nodes[from].adjOut.end())
	{
		return it->second;
	}
	else
	{
		int error = NOLINK;

		//改变字体颜色为红
		CCol(0,2);
		cout << "发生一个错误，无法获取从 网络" << netid << " 的 " << from << " -->> " << to << " 的权值，因为没有链接了。" << endl;
		//改变字体颜色为普通
		CCol(0,0);

		return error;
	}
}

//网络对象输出为普通文件格式，支持自动创建路径(格式为 XX/XX/XXX/XXX.txt)
void network::outputNetwork(string out) //输出流
{
	//重新定义编号，pajek强制从1开始
	redistribute();

	if (out.empty())
	{
		CCol(0,2);
		cout << "路径不允许为空，已自动返回。" << endl;
		CCol(0,0);
		return;
	}

	//检查是否为多级目录
	if (out.find('/') < out.length())
	{
		GetDirAndCreate(out);	//输出目录名称 
	}
	

	//创建数据流
	ofstream  file(out,std::ios_base::ate);

	if (file.fail())
	{

		//改变字体颜色为红
		CCol(0,2);
		cout << "创建文件路径错误： " << out.c_str() <<" ，请检查路径是否正确！"<< endl;
		//改变字体颜色为普通
		CCol(0,0);

		return;
	}

	//改变字体颜色为红
	CCol(0,1);
	cout << "正在创建普通格式的网络信息文件： " << out.c_str() <<  endl;
	//改变字体颜色为普通
	CCol(0,0);

	//开始写入文件
	file << nodeNum <<" "<< "sun" << endl;

	for (auto it1:nodes)
	{
		file<<it1.second.id<<" "<< it1.second.numAdjOut<<" ";
		for (auto it2:it1.second.adjOut)
		{
			//weight会保留两位小数点
			file << it2.first << " " << setiosflags(ios::fixed) << setprecision(2) << it2.second << " ";
		}
		file << endl;
	}

}

//网络对象输出为普通pajek文件格式，支持自动创建路径(格式为 XX/XX/XXX/XXX.txt)
void network::outputNetwork_pajek(string out)
{
	//重新定义编号，pajek强制从1开始
	redistribute();

	if (out.empty())
	{
		CCol(0,2);
		cout << "路径不允许为空，已自动返回。" << endl;
		CCol(0,0);
		return;
	}

	//检查是否为多级目录
	if (out.find('/') < out.length())
	{
		GetDirAndCreate(out);	//输出目录名称 
	}


	//创建数据流
	ofstream  file(out, std::ios_base::ate);

	if (file.fail())
	{

		//改变字体颜色为红
		CCol(0,2);
		cout << "创建文件路径错误： " << out.c_str() << " ，请检查路径是否正确！" << endl;
		//改变字体颜色为普通
		CCol(0,0);
		return;
	}
	//改变字体颜色为红
	CCol(0,1);
	cout << "正在创建普通Pajek格式的网络信息文件： " << out.c_str() << endl;
	//改变字体颜色为普通
	CCol(0,0);

	//开始写入文件
	file << "*vertices "<< nodeNum<<" " << endl;
	for (auto it:nodes)
	{
		file << it.second.id<<" " << endl;
	}
	file << "*arcs " << endl;
	for (auto it1:nodes)
	{
		for (auto it2:it1.second.adjOut)
		{
			//weight保留三位小数点
			file << it1.second.id << " " << it2.first << " " << setiosflags(ios::fixed) << setprecision(3) << it2.second<<endl;
		}
	}

}

/* 高级pajek格式信息如下：
output the information of a network to a *.net file, so it can be used as
an input file of pajek

format in the output file:
*vertices n                  //n is the number of vertices
1 "name"                   // "name"  the label of vertex 1
......
*arcs
i j v_{ij}                         // the arc from i to j has value v_{ij}
. . .
*edges
p q v_{pq}                         // the edge from p to q has value v_{pq}

*/

//网络对象输出为带颜色的pajek文件格式，支持自动创建路径(格式为 XX/XX/XXX/XXX.txt)
void network::outputNetwork_pajek_frac(string out)
{
	//重新定义编号，pajek强制从1开始
	redistribute();

	if (out.empty())
	{
		CCol(0,2);
		cout << "路径不允许为空，已自动返回。" << endl;
		CCol(0,0);
		return;
	}

	//检查是否为多级目录
	if (out.find('/') < out.length())
	{
		GetDirAndCreate(out);	//输出目录名称 
	}


	//创建数据流
	ofstream  file(out, std::ios_base::ate);

	if (file.fail())
	{

		//改变字体颜色为红
		CCol(0,2);
		cout << "创建文件路径错误： " << out.c_str() << " ，请检查路径是否正确！" << endl;
		//改变字体颜色为普通
		CCol(0,0);
		return;
	}
	//改变字体颜色为红
	CCol(0,1);
	cout << "正在创高级Pajek格式的网络信息文件： " << out.c_str() << endl;
	//改变字体颜色为普通
	CCol(0,0);

	//开始写入文件
	file << "*vertices " << nodeNum << " " << endl;
	for (auto it : nodes)
	{
		file << it.second.id << " " << endl;
	}
	
	file << "*arcs " << endl;

	for (auto it1 : nodes)
	{
		for (auto it2 : it1.second.adjOut)
		{
			//获取邻居编号
			int neighbor = it2.first;

			//如果该节点出度 和 邻居的出度 相同，则black ，否则 gray
			if (it1.second.numAdjOut == nodes[neighbor-1].numAdjOut)
			{
				file << it1.second.id<<" "<< neighbor <<"  1 c black"<<endl;
			}
			else {
				file << it1.second.id << " " << neighbor << "  -1 c gray" << endl;
			}
		}
	}
}

//改变网络编号，不改变结构
int network::changeNodeid(int oldid, int newid)
{
	if (oldid == newid)
	{
		int error = 1;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误 ，无法改变 网络 " << netid << "的 节点编号 从 " << oldid << " --> " << newid << "，因为编号相同 。" << endl;
		//改变字体颜色为普通
		CCol(0, 0);

		return error;
	}

	if (!isValidNode(oldid))
	{
		int error = 1;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误 ，无法改变 网络 " << netid << "的 节点编号 从 " << oldid << " --> " << newid << "，因为原节点不存在 。" << endl;
		//改变字体颜色为普通
		CCol(0, 0);

		return error;
	}

	if (isValidNode(newid))
	{
		int error = 1;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误 ，无法改变 网络 " << netid << "的 节点编号 从 " << oldid << " --> " << newid << "，因为新节点已被占用 。" << endl;
		//改变字体颜色为普通
		CCol(0, 0);

		return error;
	}

	//找到原有节点所在位置
	auto it = nodes.find(oldid);

	//改变原有节点的id信息
	it->second.id = newid;

	//插入新节点
	nodes.insert(make_pair(newid, it->second));

	//删除原有节点
	nodes.erase(it);

	//开始改变节点原有属性
	
	//遍历所有出度
	for (auto it : nodes[newid].adjOut)
	{
		//对于每一个出度节点，改变他们的入度节点
		auto itout = nodes[it.first].adjIn.find(oldid);
		//插入新的节点信息
		nodes[it.first].adjIn.insert(make_pair(newid, itout->second));
		//删除旧的节点信息
		nodes[it.first].adjIn.erase(itout);
	}

	//遍历所有入度
	for (auto it : nodes[newid].adjIn)
	{
		//对于每一个入度节点，改变他们的出度信息
		auto itin = nodes[it.first].adjOut.find(oldid);
		//插入新的节点信息
		nodes[it.first].adjOut.insert(make_pair(newid, itin->second));
		//删除旧的节点信息
		nodes[it.first].adjOut.erase(itin);
	}
	return 0;
}


int network::redistribute()
{
	CCol(0, 1);
	cout << "正在为“单网络”重新分配节点编号。" << endl;
	CCol(0, 0);
	
	//没有节点，则返回
	if (nodeNum == 0)
	{
		return 0;
	}

	//如果存在0号节点
	if (nodes.find(0) != nodes.end())
	{
		//将0节点插入到末尾后面
		auto end = nodes.rbegin();
		changeNodeid(0, end->first + 1);
	}

	for (int i = 1; i <= nodeNum; i++)
	{
		//如果不存在，则改变序号
		if (nodes.find(i) == nodes.end())
		{
			auto end = nodes.rbegin();
			changeNodeid(end->first, i);
		}
	}

	return 0;
}

#pragma endregion

////////////////////////////
////////////////////////////
#pragma region 多网络

//构造空的多网络
multiNet::multiNet()
{
	linkNum = 0;				//初始化链接数量
	nodeNum = 0;				//初始化节点数量
	netNum = 0;
	nets.insert(make_pair(0, network(10, 0)));
	type = MULTI;
}

//双网络构造函数
multiNet::multiNet(network A, network B)			//两个网络
{
	netNum = 2;									//初始化子网络数量
	A.netid = 1;									//网络ID0
	B.netid = 2;									//网络ID1

	nets.clear();

	//初始化子网络容器
	nets.insert(make_pair(A.netid, A));
	nets.insert(make_pair(B.netid, B));

	nodeNum = nets[A.netid].nodeNum + nets[B.netid].nodeNum;		//初始化节点数量
	linkNum = nets[A.netid].linkNum + nets[B.netid].linkNum;	//初始化边的数量
																//this->type=type;								//初始化网络类型
}


//复制N个网络组成多网络，构造函数
multiNet::multiNet(int N			//子网络数量
	, network n1		//要复制的网络对象
	)
{
	netNum = N;										//初始化子网络数量
	nodeNum = 0;
	linkNum = 0;
	this->type = 0;

	//初始化子网络容器，子网络数量为N
	for (int i = 1; i <= netNum; i++)
	{
		n1.netid = i;
		nets.insert(make_pair(n1.netid, n1));
	}

	//初始化子网络链接数和节点数
	for (auto it : nets)
	{
		nodeNum += it.second.nodeNum;
		linkNum += it.second.linkNum;
	}
}

//读取网络容器来构造多网络
multiNet::multiNet(vector<network> temp)			//网络容器

{
	//初始化网络容器
	nets.clear();
	netNum = 0;

	//初始化链接和节点信息
	nodeNum = 0;
	linkNum = 0;

	for (auto it : temp)
	{

		netNum++;								//增加节点信息
		it.netid = netNum;						//分配网络编号
		nets.insert(make_pair(netNum, it));	//插入节点

											//每插入一个节点，增加相关信息
		nodeNum += it.nodeNum;
		linkNum += it.linkNum;
	}
}

//读取目录中的文件群来构造多网络对象，格式为	net1.txt	net2.txt	......	links.txt
multiNet::multiNet(string in)
{
	if (in.empty())
	{
		CCol(0, 2);
		cout << "目录不允许为空，已自动返回原始多网络。" << endl;
		CCol(0, 0);
		new (this) multiNet();
	}

	//改变字体颜色为红
	CCol(0, 1);
	cout << "正在读取多网络信息文件群，路径为： " << in.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//改变字体颜色为普通
	CCol(0, 0);

	//满足条件
	linkNum = 0;				//初始化链接数量
	nodeNum = 0;				//初始化节点数量
	netNum = 0;

	//从links添加网络
	string inlink = in + "/links.txt";
	ifstream filelink(inlink.data(), ios::in);

	//检查文件是否读取失败
	if (filelink.fail())
	{
		CCol(0, 2);
		cout << "读取多网络链接文件失败，路径为： " << inlink.c_str() << "，已自动返回原始多网络。" << endl;
		CCol(0, 0);
		new (this) multiNet();
		return;
	}

	vector<int> netsF;			//储存网络编号
	string line;					//储存临时一行

	getline(filelink, line);		//开始读取节点信息，即第一行
	stringstream stream(line);	//处理一行信息，以空格为分隔符

								//保存网络编号，后面创建用
	int netnum;
	while (stream >> netnum)
	{
		netsF.insert(netsF.end(), netnum);
	}


	//循环fileNum - 1次，先把网络写入
	for (auto it : netsF)
	{
		//将int转换为string
		stringstream ss;
		string tempNum;
		ss << it;
		ss >> tempNum;

		//从netx.txt文件创建临时网络对象，并插入多网络
		addNetworkToMulti(network(in + "/net" + tempNum + ".txt"), it);
	}

	//最后一次写入多网络链接

	inputLinksToMulti(in + "/links.txt");

	//如果多网络文件读取错误，则放弃读取整个多网络（该部分已注销）
	/*
	//int error=inputLinksToMulti(in + "/links.txt");
	if (error)
	{
	CCol(0,2);
	cout << "目录文件信息有误，请确认信息无误后重试，已自动返回原始多网络。" << endl;
	CCol(0,0);
	new (this) multiNet();
	return;
	}
	*/

	//改变字体颜色为红
	CCol(0, 1);
	cout << endl << "流程执行完毕，请检查上面的读取信息，路径为： " << in.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//改变字体颜色为普通
	CCol(0, 0);

}

//析构函数
multiNet::~multiNet()
{
	//都是自动的
}

//显示网络
void multiNet::showMultiNet()
{
	CCol(1, 3);
	cout << "该多网络包含子网络 " << netNum << " 个，节点数 " << nodeNum << " 个，链接数 " << linkNum << " 个。" << endl;
	CCol(0, 0);
	for (auto it : nets)
	{
		it.second.showNetwork();
	}
}

//添加不同网络之间的链接，也可以添加本网络之间的连接
int multiNet::addLinkBetweenNets(int net1, int node1, int net2, int node2, double weight)
{
	//判断net1和net2没有越界
	if (!isValidNet(net1) || !isValidNet(net2) || net1 <= 0 || net2 <= 0)
	{
		int error = BEYONDLIMIT;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误 ，无法添加多网络链接，网络 " << net1 << " 或 网络 " << net2 << " 越界。" << endl;
		//改变字体颜色为普通
		CCol(0, 0);

		return error;
	}

	//判断是否是同一网络之间的链接
	if (net1 == net2)
	{
		int error = 0;
		error = nets[net1].addLinkToNetwork(node1, node2, weight);
		linkNum++;
		return error;
	}

	//查看node1和node2是否越界
	if (!nets[net1].isValidNode(node1) || !nets[net2].isValidNode(node2) || node1 <= 0 || node2 <= 0)
	{
		int error = BEYONDLIMIT;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误 ，无法添加 网络" << net1 << "的 节点 " << node1 << " -->> " << "网络" << net2 << "的 节点" << node2 << "，因为越界。" << endl;
		//改变字体颜色为普通
		CCol(0, 0);

		return error;
	}

	//开始添加链接
	int error = 0;

	//在node1增加node1到node2的出度链接
	error = nets[net1].nodes[node1].addLinkToNodeMulti(net2, node2, weight);
	error = nets[net1].linkNum++;

	//在node2增加node1到node2的入度链接
	error = nets[net2].nodes[node2].addLinkFromNodeMulti(net1, node1, weight);

	linkNum++;
	return error;
}


//删除不同网络之间的链接，也可以删除本网络之间的连接
int multiNet::delLinkBetweenNets(int net1, int node1, int net2, int node2)
{
	//判断net1和net2没有越界
	if (isValidNet(net1) || isValidNet(net2) || net1 <= 0 || net2 <= 0)
	{
		int error = BEYONDLIMIT;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误 ，无法删除多网络链接，网络 " << net1 << " 或 网络 " << net2 << " 越界。" << endl;
		//改变字体颜色为普通
		CCol(0, 0);

		return error;
	}

	//判断是否是同一网络之间的链接
	if (net1 == net2)
	{
		int error = 0;
		error = nets[net1].delLinkFromNetwork(node1, node2);
		linkNum--;
		return error;
	}

	//查看node1和node2是否越界
	if (!nets[net1].isValidNode(node1) || !nets[net2].isValidNode(node2) || node1 <= 0 || node2 <= 0)
	{
		int error = BEYONDLIMIT;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误 ，无法删除多网络链接，节点 " << node1 << " 或 节点 " << node2 << " 越界。" << endl;
		//改变字体颜色为普通
		CCol(0, 0);

		return error;
	}

	//开始删除链接
	int error = 0;

	//在node1增加node1到node2的出度链接
	error = nets[net1].nodes[node1].delLinkToNodeMulti(net2, node2);
	error = nets[net1].linkNum--;

	//在node2增加node1到node2的入度链接
	error = nets[net2].nodes[node2].delLinkFromNodeMulti(net1, node1);

	linkNum--;
	return error;
}

//删除多网络节点
int multiNet::delNodeFromMultiNet(int netid, int nodeid)
{
	//判断netid是否越界
	if (netid > netNum || netid <= 0)
	{
		int error = BEYONDLIMIT;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误 ，无法删除多网络节点，网络 " << netid << " 越界。" << endl;
		//改变字体颜色为普通
		CCol(0, 0);

		return error;
	}

	//查看nodeid是否越界
	if (!nets[netid].isValidNode(nodeid) || nodeid <= 0)
	{
		int error = BEYONDLIMIT;

		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误 ，无法删除多网络节点，节点 " << nodeid << " 越界。" << endl;
		//改变字体颜色为普通
		CCol(0, 0);

		return error;
	}

	//开始删除节点

	//如果该节点存留多网络链接关联，则全部删除

	//清除多网络出度
	if (nets[netid].nodes[nodeid].numAdjMultiOut > 0)
	{
		for (auto it : nets[netid].nodes[nodeid].adjMultiOut)
			for (auto it2 : it.second)
			{
				nets[it.first].nodes[it2.first].delLinkFromNodeMulti(netid, nodeid);
				//删除，网络it.first的节点it2.first <<-- 网络netid的节点nodeid
				//指向该节点链接的起点所在网络链接数量减少
				nets[it.first].linkNum--;
				nets[netid].nodes[nodeid].numAdjMultiOut--;

				//多网络链接总数量减少
				linkNum--;
			}
		//检查是否清理干净
		if (nets[netid].nodes[nodeid].numAdjMultiOut == 0)
		{
			//出度清理干净了
			nets[netid].nodes[nodeid].adjMultiOut.clear();
		}
	}

	//清除多网络入度
	if (nets[netid].nodes[nodeid].numAdjMultiIn > 0)
	{
		for (auto it : nets[netid].nodes[nodeid].adjMultiIn)
			for (auto it2 : it.second)
			{
				nets[it.first].nodes[it2.first].delLinkToNodeMulti(netid, nodeid);
				//删除，网络it.first的节点it2.first -->> 网络netid的节点nodeid
				//指向该节点链接的起点所在网络链接数量减少
				nets[it.first].linkNum--;
				nets[netid].nodes[nodeid].numAdjMultiIn--;

				//多网络链接总数量减少
				linkNum--;
			}
		//检查是否清理干净
		if (nets[netid].nodes[nodeid].numAdjMultiIn == 0)
		{
			//入度清理干净了
			nets[netid].nodes[nodeid].adjMultiIn.clear();
		}
	}

	//多网络总链接数减少数量，等于该节点所有链接数量总和
	linkNum -= +nets[netid].nodes[nodeid].numAdjOut + nets[netid].nodes[nodeid].numAdjIn;

	//入度和出度清除完毕，开始从网络层面清除节点
	int error = nets[netid].delNodeFromNetwork(nodeid);



	if (!error)
	{
		nodeNum--;
	}

	return error;
}

//添加一个网络到多网络
int multiNet::addNetworkToMulti(network n1, int netid)
{

	if (n1.nodeNum == 0)
	{
		int error = 1;
		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误 ，无法添加空网络到多网络" << endl;
		//改变字体颜色为普通
		CCol(0, 0);

		return error;
	}

	if (isValidNet(netid))
	{
		int error = 1;
		//改变字体颜色为红
		CCol(0, 2);
		cout << "发生一个错误 ，多网络已存在网络 " << netid << " ，故无法添加。" << endl;
		//改变字体颜色为普通
		CCol(0, 0);
		return error;
	}

	//网络数量加一
	netNum++;

	//增加总体节点和链接数量
	linkNum += n1.linkNum;
	nodeNum += n1.nodeNum;

	//改变netid
	n1.netid = netid;

	//nets容器中添加网络
	nets.insert(make_pair(n1.netid, n1));

	return 0;
}


int multiNet::isValidNet(int netid)
{
	if (netid < 0)
	{
		return 0;
	}
	auto it = nets.find(netid);
	if (it != nets.end())
	{
		return 1;
	}
	return 0;
}

//输出多网络链接到txt文件，格式为 网络1 节点1 网络2 节点2
void multiNet::outputLinksFromMulti(string out)
{
	if (out.empty())
	{
		CCol(0, 2);
		cout << "路径不允许为空，已自动返回。" << endl;
		CCol(0, 0);
		return;
	}

	//检查是否为多级目录
	if (out.find('/') < out.length())
	{
		GetDirAndCreate(out);	//输出目录名称 
	}


	//创建数据流
	ofstream  file(out, std::ios_base::ate);

	if (file.fail())
	{

		//改变字体颜色为红
		CCol(0, 2);
		cout << "创建文件路径错误： " << out.c_str() << " ，请检查路径是否正确！" << endl;
		//改变字体颜色为普通
		CCol(0, 0);

		return;
	}

	//改变字体颜色为红
	CCol(0, 1);
	cout << "正在创建普通格式的多网络链接信息文件： " << out.c_str() << endl;
	//改变字体颜色为普通
	CCol(0, 0);

	//第一行写入多网络号
	for (auto it : nets)
	{
		file << it.second.netid << " ";
	}

	file << endl;
	//开始写入多链接
	for (auto it : nets)
		for (auto it1 : it.second.nodes)
		{
			//it.netid	网络1
			//it1.id	节点1
			for (auto it2 : it1.second.adjMultiOut)
			{
				//it2.first	网络2
				for (auto it3 : it2.second)
				{
					//it3.first		节点2
					//it3.second	权值

					//weight会保留两位小数点
					file << it.second.netid << " " << it1.second.id << " " << it2.first << " " << it3.first << " " << setiosflags(ios::fixed) << setprecision(2) << it3.second << endl;
				}
			}
		}
}

//读取多网络链接txt文件，添加到当前多网络
int multiNet::inputLinksToMulti(string inlink)
{
	if (inlink.empty())
	{
		CCol(0, 2);
		cout << "路径不允许为空，已自动返回。" << endl;
		CCol(0, 0);
		return 2;
	}

	int error_return = 0;
	ifstream filelink(inlink.data(), ios::in);

	//检查文件是否读取失败
	if (filelink.fail())
	{
		CCol(0, 2);
		cout << "读取多网络链接文件失败，路径为： " << inlink.c_str() << "，已自动返回错误。" << endl;
		CCol(0, 0);
		return 2;
	}
	//文件没有读取失败，正常进行
	else
	{
		CCol(0, 1);
		cout << "读取多网络链接文件成功，路径为： " << inlink.c_str() << endl;
		CCol(0, 0);
		string line;				//储存临时一行


		getline(filelink, line);		//开始读取节点信息，即第一行
		stringstream stream(line);	//处理一行信息，以空格为分隔符

		int netnum;
		while (stream >> netnum)
		{
			if (!isValidNet(netnum))
			{
				CCol(0, 2);
				cout << "正在读取多网络链接文件，网络 " << netnum << " 缺失，已自动创建，路径为： " << inlink.c_str() << endl;
				CCol(0, 0);
				addNetworkToMulti(network(1, 0), netNum);
				error_return = 1;
			}
		}


		//下面开始添加多网络链接
		while (!filelink.eof())
		{

			getline(filelink, line);		//获取临时一行
			stringstream stream(line);	//处理一行信息，以空格为分隔符

			if (filelink.fail()) {
				break;
			}

			int net1, node1, net2, node2;
			double weight;

			//获取链接信息
			stream >> net1;
			stream >> node1;
			stream >> net2;
			stream >> node2;
			stream >> weight;

			//添加链接，成功则多网络链接加一
			if (addLinkBetweenNets(net1, node1, net2, node2, weight) == 0)
			{
				linkNum++;
			}
			else
			{
				error_return = 1;
			}

		}
	}

	if (error_return)
	{
		CCol(0, 2);
		cout << "读取多网络链接文件完毕，但部分信息有误，路径为： " << inlink.c_str() << endl;
		CCol(0, 0);
	}

	return error_return;
}

void multiNet::outputMultiNet(string out)
{
	if (out.empty())
	{
		CCol(0, 2);
		cout << "目录不允许为空，已自动返回。" << endl;
		CCol(0, 0);
		return;
	}

	//转换读取目录的格式
	char* temp;
	const int len = out.length();
	temp = new char[len + 1];
	strcpy(temp, out.c_str());

	/*
	//创建目录
	if (CreatDir(temp) == -1)
	{
	CCol(0,2);
	cout << "目录创建失败，路径为： " << out << "，已自动返回。" << endl;
	CCol(0,0);
	return;
	}
	*/

	//改变字体颜色为红
	CCol(0, 1);
	cout << "正在创建普通格式的多网络信息文件群，路径为： " << out.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//改变字体颜色为普通
	CCol(0, 0);

	//开始输出网络文件，到out路径
	for (auto it : nets)
	{
		//int转换为string
		stringstream ss;
		string tempNum;
		ss << it.second.netid;
		ss >> tempNum;

		//开始输出
		it.second.outputNetwork(out + "/net" + tempNum + ".txt");
	}

	//开始输出多网络链接文件
	outputLinksFromMulti(out + "/links.txt");

	//改变字体颜色为红
	CCol(0, 1);
	cout << endl << "所有文件创建完毕，请检查上面的创建信息，路径为： " << out.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//改变字体颜色为普通
	CCol(0, 0);



}

void multiNet::outputMultiNet_pajek(string out)
{
	if (out.empty())
	{
		CCol(0, 2);
		cout << "目录不允许为空，已自动返回。" << endl;
		CCol(0, 0);
		return;
	}

	//转换读取目录的格式
	char* temp;
	const int len = out.length();
	temp = new char[len + 1];
	strcpy(temp, out.c_str());

	//改变字体颜色为红
	CCol(0, 1);
	cout << "正在创建pajek格式的多网络信息文件群，路径为： " << out.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//改变字体颜色为普通
	CCol(0, 0);

	//开始输出网络文件，到out路径
	for (auto it : nets)
	{
		//int转换为string
		stringstream ss;
		string tempNum;
		ss << it.second.netid;
		ss >> tempNum;

		//开始输出
		it.second.outputNetwork_pajek(out + "/net" + tempNum + ".txt");
	}

	//开始输出多网络链接文件
	outputLinksFromMulti(out + "/links.txt");

	//改变字体颜色为红
	CCol(0, 1);
	cout << endl << "流程执行完毕，请检查上面的创建信息，路径为： " << out.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//改变字体颜色为普通
	CCol(0, 0);

}

int multiNet::changeNetid(int oldid, int newid)
{
	//如果修改id一样，则返回错误
	if (oldid == newid)
	{
		CCol(0, 2);
		cout << "修改网络ID出现错误，无法修改为自己。 " << endl;
		CCol(0, 0);

		return 1;
	}

	//查看原网络是否存在
	if (!isValidNet(oldid))
	{
		CCol(0, 2);
		cout << "修改网络ID出现错误，原 网络 " << oldid << " 并不存在。" << endl;
		CCol(0, 0);

		return 1;
	}

	//查看新网络是否被占用
	if (isValidNet(newid))
	{
		CCol(0, 2);
		cout << "修改网络ID出现错误，新 网络 " << newid << " 已被占用。" << endl;
		CCol(0, 0);

		return 1;
	}

	//插入新的网络
	nets.insert(make_pair(newid, nets[oldid]));
	//改变网络ID
	nets[newid].netid = newid;
	//删除旧网络
	nets.erase(oldid);

	//遍历每一个节点
	for (auto it : nets[newid].nodes)
	{
		//遍历多网络出度，改变对面的入度网络编号
		for (auto itout : it.second.adjMultiOut)
		{
			//itout.first是对面网络编号
			for (auto itin : itout.second)
			{
				//itin.first是节点编号
				//itin.second是链接权值

				//查找对面网络的旧信息
				auto target = nets[itout.first].nodes[itin.first].adjMultiIn.find(oldid);

				//插入新信息
				nets[itout.first].nodes[itin.first].adjMultiIn.insert(make_pair(newid, nets[itout.first].nodes[itin.first].adjMultiIn[oldid]));

				//删除旧信息
				nets[itout.first].nodes[itin.first].adjMultiIn.erase(target);

				//cout << "修改了 网络" << itout.first << " 的 节点 " << itin.first << " ."<< endl;
			}
		}
		//遍历多网络入度，改变对面的出度网络编号
		for (auto itin : it.second.adjMultiIn)
		{

			//itin.first是对面网络编号
			for (auto itout : itin.second)
			{
				//itout.first是节点编号
				//itout.second是链接权值

				//查找对面网络的旧信息
				auto target = nets[itin.first].nodes[itout.first].adjMultiOut.find(oldid);

				//插入新信息
				nets[itin.first].nodes[itout.first].adjMultiOut.insert(make_pair(newid, nets[itin.first].nodes[itout.first].adjMultiOut[oldid]));

				//删除旧信息
				nets[itin.first].nodes[itout.first].adjMultiOut.erase(target);

				//cout << "修改了 网络" << itin.first << " 的 节点 " << itin.first << " ." << endl;
			}

		}

	}
	return 0;
}

int multiNet::changeNodeid(int netid, int oldid, int newid)
{
	//netid是否存在？
	if (!isValidNet(netid))
	{
		CCol(0, 2);
		cout << "修改多网络 节点ID出现错误，网络 " << netid << " 并不存在。" << endl;
		CCol(0, 0);

		return 1;
	}

	//老节点编号是否存在
	if (!nets[netid].isValidNode(oldid))
	{
		CCol(0, 2);
		cout << "修改多网络 节点ID出现错误，网络 " << netid << " 的 节点 " << oldid << " 并不存在。" << endl;
		CCol(0, 0);

		return 1;
	}

	//新节点编号是否被占用？
	if (nets[netid].isValidNode(newid))
	{
		CCol(0, 2);
		cout << "修改多网络 节点ID出现错误，网络 " << netid << " 的 节点 " << newid << " 已经存在。" << endl;
		CCol(0, 0);

		return 1;
	}

	//单网络层面修改节点编号
	nets[netid].changeNodeid(oldid, newid);

	//多网络出度，修改对面保存的旧信息
	for (auto itout : nets[netid].nodes[newid].adjMultiOut)
	{
		//itout.first是对面网络的编号
		for (auto itin : itout.second)
		{
			//itin.first是对面节点的编号
			//itin.second是链接的权值

			//添加新的信息
			nets[itout.first].nodes[itin.first].adjMultiIn[netid].insert(make_pair(newid, nets[itout.first].nodes[itin.first].adjMultiIn[netid][oldid]));

			//删除老的信息
			nets[itout.first].nodes[itin.first].adjMultiIn[netid].erase(oldid);

		}
	}

	//多网络入度，修改对面保存的旧信息
	for (auto itin : nets[netid].nodes[newid].adjMultiIn)
	{

		//itin.first是对面网络的编号
		for (auto itout : itin.second)
		{
			//itout.first是对面节点的编号
			//itout.second是链接的权值

			//添加新的信息
			nets[itin.first].nodes[itout.first].adjMultiOut[netid].insert(make_pair(newid, nets[itin.first].nodes[itout.first].adjMultiOut[netid][oldid]));

			//删除老的信息
			nets[itin.first].nodes[itout.first].adjMultiOut[netid].erase(oldid);

		}
	}
	return 0;
}

int multiNet::redistribute()
{

	CCol(0, 1);
	cout << "正在为“多网络”重新分配节点编号。" << endl;
	CCol(0, 0);

	//如果没有网络，直接返回
	if (netNum == 0)
	{
		return 0;
	}

	//如果网络0存在，则将其排在最后面
	if (nets.find(0) != nets.end())
	{
		auto end = nets.rbegin();
		changeNetid(0, end->first + 1);
	}

	for (int i = 1; i <= netNum; i++)
	{
		//检查网络是否存在，如果有空缺则用队尾填补
		if (!isValidNet(i))
		{
			auto end = nets.rbegin();
			changeNetid(end->first, i);
		}

		//开始重新排布网络子节点
		for (int j = 1; j <= nets[i].nodeNum; j++)
		{
			//如果节点有空缺，则用队尾填补
			if (!nets[i].isValidNode(j))
			{
				auto endnode = nets[i].nodes.rbegin();
				changeNodeid(i, endnode->first, j);
			}
		}
	}
	return 0;
}

#pragma endregion