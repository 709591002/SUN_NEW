#include "multiNet.h"

//构造空网络
multiNet::multiNet()
{
	linkNum = 0;				//初始化链接数量
	nodeNum = 0;				//初始化节点数量
	netNum = 0;
	network n(10, UN_DIRECT_UN_WEIGHT);
	nets.insert(nets.begin(),n);
	type = MULTI;
}

//双网络构造函数
multiNet::multiNet(network A,network B				//两个网络
					,multiType type)				//网络类型
{
	netNum=2;										//初始化子网络数量
	A.netid = 1;									//网络ID0
	B.netid = 2;									//网络ID1
	
	nets.clear();

	//初始化子网络容器
	nets.insert(nets.end(),A);
	nets.insert(nets.end(),B);

	nodeNum=nets[0].nodeNum+ nets[1].nodeNum;		//初始化节点数量
	linkNum = nets[0].linkNum + nets[1].linkNum;	//初始化边的数量
	this->type=type;								//初始化网络类型
}


//复制N个网络组成多网络，构造函数
multiNet::multiNet(int N			//子网络数量
				, network n1		//要复制的网络对象
				, multiType type)	//网络类型
{
	netNum = N;										//初始化子网络数量
	nodeNum = 0;
	linkNum = 0;
	this->type = 0;
	
	//初始化子网络容器，子网络数量为N
	for (int i = 1; i <= netNum; i++)
	{
		n1.netid = i;
		nets.insert(nets.end(),n1);
	}

	//初始化子网络链接数和节点数
	for (auto it : nets)
	{
		nodeNum += it.nodeNum;
		linkNum += it.linkNum;
	}
	this->type=type;								//初始化网络类型
}

//读取网络容器来构造多网络
multiNet::multiNet(vector<network> temp				//网络容器
					, multiType type)				//网络类型
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
		nets.insert(nets.begin()+ netNum,it);	//插入节点
		
		//每插入一个节点，增加相关信息
		nodeNum += it.nodeNum;
		linkNum += it.linkNum;
	}
	this->type = type;
}

//读取目录中的文件群来构造多网络对象，格式为	net1.txt	net2.txt	......	links.txt
multiNet::multiNet(string in,int fileNum)
{
	if (in.empty())
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "目录不允许为空，已自动返回原始多网络。" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
		new (this) multiNet();
	}


	
	//小于2说明不满足最低条件
	if (fileNum < 2)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "目录没有所需文件，请修改路径名称重试，已自动返回原始多网络。" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
		new (this) multiNet();
	}

	//改变字体颜色为红
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "正在读取多网络信息文件群，路径为： " << in.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//改变字体颜色为普通
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

	//满足条件
	linkNum = 0;				//初始化链接数量
	nodeNum = 0;				//初始化节点数量
	netNum = 0;

	//循环fileNum - 1次，先把网络写入
	for (int i = 1; i < fileNum; i++)
	{
		//将int转换为string
		stringstream ss;
		string tempNum;
		ss << i;
		ss >> tempNum;

		//从netx.txt文件创建临时网络对象，并插入多网络
		network temp(in + "/net" + tempNum + ".txt");
		temp.netid=i;
		addNetworkToMulti(temp);
	}

	//最后一次写入多网络链接
	int error=inputLinksToMulti(in + "/links.txt");

	//
	/*
	if (error)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "目录文件信息有误，请确认信息无误后重试，已自动返回原始多网络。" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
		new (this) multiNet();
		return;
	}
	*/

	//改变字体颜色为红
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << endl << "流程执行完毕，请检查上面的读取信息，路径为： " << in.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//改变字体颜色为普通
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

}

//析构函数
multiNet::~multiNet()
{
	//都是自动的
}

//显示网络
void multiNet::showMultiNet()
{
	cout << "该多网络包含子网络 "<<netNum<<" 个，节点数 " <<nodeNum<<" 个，链接数 "<<linkNum<<" 个。"<< endl;
	for (auto it : nets)
	{
		it.showNetwork();
	}
}

//添加不同网络之间的链接，也可以添加本网络之间的连接
int multiNet::addLinkBetweenNets(int net1,int node1,int net2,int node2,double weight)
{
	//判断net1和net2没有越界
	if (net1 > netNum || net2 > netNum || net1<=0||net2<=0)
	{
		int error = BEYONDLIMIT;

		//改变字体颜色为红
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "发生一个错误 ，无法添加多网络链接，网络 " << net1<<" 或 网络 "<<net2<<" 越界。" << endl;
		//改变字体颜色为普通
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

		return error;
	}

	//判断是否是同一网络之间的链接
	if (net1 == net2)
	{
		int error=0;
		error=nets[net1-1].addLinkToNetwork(node1, node2, weight);
		linkNum++;
		return error;
	}

	//查看node1和node2是否越界
	if (node1 > nets[net1-1].nodeNum || node2 > nets[net2-1].nodeNum|| node1<=0||node2<=0)
	{
		int error = BEYONDLIMIT;

		//改变字体颜色为红
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "发生一个错误 ，无法添加 网络"<<net1<<"的 节点 " << node1 << " -->> " << "网络"<<net2<<"的 节点"<< node2 << "，因为越界。" << endl;
		//改变字体颜色为普通
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

		return error;
	}

	//开始添加链接
	int error=0;

	//在node1增加node1到node2的出度链接
	error=nets[net1-1].nodes[node1-1].addLinkToNodeMulti(net2, node2, weight);
	error=nets[net1-1].linkNum++;
	
	//在node2增加node1到node2的入度链接
	error=nets[net2-1].nodes[node2-1].addLinkFromNodeMulti(net1, node1, weight);
	
	linkNum++;
	return error;
}


//删除不同网络之间的链接，也可以删除本网络之间的连接
int multiNet::delLinkBetweenNets(int net1, int node1, int net2, int node2)
{
	//判断net1和net2没有越界
	if (net1 > netNum || net2 > netNum || net1<=0 || net2<=0)
	{
		int error = BEYONDLIMIT;

		//改变字体颜色为红
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "发生一个错误 ，无法删除多网络链接，网络 " << net1 << " 或 网络 " << net2 << " 越界。" << endl;
		//改变字体颜色为普通
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

		return error;
	}

	//判断是否是同一网络之间的链接
	if (net1 == net2)
	{
		int error=0;
		error = nets[net1-1].delLinkFromNetwork(node1, node2);
		linkNum--;
		return error;
	}

	//查看node1和node2是否越界
	if (node1 > nets[net1-1].nodeNum || node2 > nets[net2-1].nodeNum|| node1<=0|| node2<=0)
	{
		int error = BEYONDLIMIT;

		//改变字体颜色为红
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "发生一个错误 ，无法删除多网络链接，节点 " << node1 << " 或 节点 " << node2 << " 越界。" << endl;
		//改变字体颜色为普通
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

		return error;
	}

	//开始删除链接
	int error=0;

	//在node1增加node1到node2的出度链接
	error = nets[net1-1].nodes[node1].delLinkToNodeMulti(net2, node2);
	error = nets[net1-1].linkNum--;

	//在node2增加node1到node2的入度链接
	error = nets[net2-1].nodes[node2].delLinkFromNodeMulti(net1, node1);

	linkNum--;
	return error;
}

//删除多网络节点
int multiNet::delNodeFromMultiNet(int netid, int nodeid)
{
	//判断netid是否越界
	if (netid > netNum|| netid<=0 )
	{
		int error = BEYONDLIMIT;

		//改变字体颜色为红
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "发生一个错误 ，无法删除多网络节点，网络 " << netid <<" 越界。" << endl;
		//改变字体颜色为普通
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

		return error;
	}

	//查看nodeid是否越界
	if (nodeid > nets[netid-1].nodeNum||nodeid<=0)
	{
		int error = BEYONDLIMIT;

		//改变字体颜色为红
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "发生一个错误 ，无法删除多网络节点，节点 " << nodeid << " 越界。" << endl;
		//改变字体颜色为普通
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

		return error;
	}

	//开始删除节点

	//如果该节点存留多网络链接关联，则全部删除
	
	//清除多网络出度
	if (nets[netid-1].nodes[nodeid-1].numAdjMultiOut > 0)
	{
		for (auto it : nets[netid - 1].nodes[nodeid - 1].adjMultiOut)
			for (auto it2 : it.second)
			{
				nets[it.first - 1].nodes[it2.first - 1].delLinkFromNodeMulti(netid, nodeid);
				//删除，网络it.first的节点it2.first <<-- 网络netid的节点nodeid
				//指向该节点链接的起点所在网络链接数量减少
				nets[it.first - 1].linkNum--;
				nets[netid - 1].nodes[nodeid - 1].numAdjMultiOut--;

				//多网络链接总数量减少
				linkNum--;
			}
		//检查是否清理干净
		if (nets[netid - 1].nodes[nodeid - 1].numAdjMultiOut == 0)
		{
			//出度清理干净了
			nets[netid - 1].nodes[nodeid - 1].adjMultiOut.clear();
		}
	}

	//清除多网络入度
	if (nets[netid-1].nodes[nodeid-1].numAdjMultiIn > 0)
	{
		for(auto it: nets[netid-1].nodes[nodeid-1].adjMultiIn)
			for (auto it2 : it.second)
			{
				nets[it.first-1].nodes[it2.first-1].delLinkToNodeMulti(netid, nodeid);
				//删除，网络it.first的节点it2.first -->> 网络netid的节点nodeid
				//指向该节点链接的起点所在网络链接数量减少
				nets[it.first-1].linkNum--;
				nets[netid-1].nodes[nodeid-1].numAdjMultiIn--;

				//多网络链接总数量减少
				linkNum--;
			}
		//检查是否清理干净
		if (nets[netid-1].nodes[nodeid-1].numAdjMultiIn == 0)
		{
			//入度清理干净了
			nets[netid-1].nodes[nodeid-1].adjMultiIn.clear();
		}
	}


	//入度和出度清除完毕，开始从网络层面清除节点
	int error = nets[netid-1].delNodeFromNetwork(nodeid);

	//多网络总链接数减少数量，等于该节点所有链接数量总和
	linkNum -= +nets[netid-1].nodes[nodeid-1].numAdjOut + nets[netid-1].nodes[nodeid-1].numAdjIn;

	if (!error)
	{
		nodeNum--;
	}

	return error;
}

//添加一个网络到多网络
int multiNet::addNetworkToMulti(network n1)
{
	if (n1.nodeNum == 0)
	{
		int error = 1;
		//改变字体颜色为红
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "发生一个错误 ，无法添加空网络到多网络"<< endl;
		//改变字体颜色为普通
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

		return error;
	}

	//网络数量加一
	netNum++;

	//增加总体节点和链接数量
	linkNum += n1.linkNum;
	nodeNum += n1.nodeNum;

	//改变netid
	n1.netid = netNum;

	//nets容器中添加网络
	nets.insert(nets.end(),n1);
	
	return 0;
}

//输出多网络链接到txt文件，格式为 网络1 节点1 网络2 节点2
void multiNet::outputLinksFromMulti(string out)
{
	if (out.empty())
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "路径不允许为空，已自动返回。" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
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
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "创建文件路径错误： " << out.c_str() << " ，请检查路径是否正确！" << endl;
		//改变字体颜色为普通
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

		return;
	}

	//改变字体颜色为红
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "正在创建普通格式的多网络链接信息文件： " << out.c_str() << endl;
	//改变字体颜色为普通
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

	//开始写入文件
	file << nodeNum << " " << type << endl;

	for(auto it:nets)
		for (auto it1 : it.nodes)
		{
			//it.netid	网络1
			//it1.id	节点1
			for (auto it2 : it1.adjMultiOut)
			{
				//it2.first	网络2
				for (auto it3 : it2.second)
				{
					//it3.first		节点2
					//it3.second	权值

					//weight会保留两位小数点
					file << it.netid << " " << it1.id << " " << it2.first << " " << it3.first << " " << setiosflags(ios::fixed) << setprecision(2) << it3.second << endl;
				}
			}
		}
}

//读取多网络链接txt文件，添加到当前多网络
int multiNet::inputLinksToMulti(string in)
{
	if (in.empty())
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "路径不允许为空，已自动返回。" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
		return 2;
	}

	int error_return = 0;
	ifstream file(in.data(), ios::in);

	//检查文件是否读取失败
	if (file.fail())
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "读取多网络链接文件失败，路径为： " << in.c_str() << "，已自动返回错误。" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
		return 2;
	}
	//文件没有读取失败，正常进行
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "读取多网络链接文件成功，路径为： " << in.c_str() << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
		string line;				//储存临时一行

		int tempNodeNum = 0;

		getline(file, line);		//开始读取节点信息，即第一行
		stringstream stream(line);	//处理一行信息，以空格为分隔符
		stream >> tempNodeNum;		//获取多网络链接数量
		stream >> type;				//获取多网类型

		//下面开始添加多网络链接
		while (!file.eof())
		{

			getline(file, line);		//获取临时一行
			stringstream stream(line);	//处理一行信息，以空格为分隔符

			if (file.fail()) {
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
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "读取多网络链接文件完毕，但部分信息有误，路径为： " << in.c_str() << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
	}

	return error_return;
}

void multiNet::outputMultiNet(string out)
{
	if (out.empty())
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "目录不允许为空，已自动返回。" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
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
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "目录创建失败，路径为： " << out << "，已自动返回。" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
		return;
	}
	*/

	//改变字体颜色为红
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "正在创建普通格式的多网络信息文件群，路径为： " << out.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//改变字体颜色为普通
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

	//开始输出网络文件，到out路径
	for (auto it : nets)
	{
		//int转换为string
		stringstream ss;
		string tempNum;
		ss << it.netid;
		ss >> tempNum;
		
		//开始输出
		it.outputNetwork(out + "/net" + tempNum+".txt");
	}

	//开始输出多网络链接文件
	outputLinksFromMulti(out + "/links.txt");

	//改变字体颜色为红
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout <<endl<< "所有文件创建完毕，请检查上面的创建信息，路径为： " << out.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//改变字体颜色为普通
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);



}

void multiNet::outputMultiNet_pajek(string out)
{
	if (out.empty())
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "目录不允许为空，已自动返回。" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
		return;
	}

	//转换读取目录的格式
	char* temp;
	const int len = out.length();
	temp = new char[len + 1];
	strcpy(temp, out.c_str());

	//改变字体颜色为红
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "正在创建pajek格式的多网络信息文件群，路径为： " << out.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//改变字体颜色为普通
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

	//开始输出网络文件，到out路径
	for (auto it : nets)
	{
		//int转换为string
		stringstream ss;
		string tempNum;
		ss << it.netid;
		ss >> tempNum;

		//开始输出
		it.outputNetwork_pajek(out + "/net" + tempNum + ".txt");
	}

	//开始输出多网络链接文件
	outputLinksFromMulti(out + "/links.txt");

	//改变字体颜色为红
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << endl << "流程执行完毕，请检查上面的创建信息，路径为： " << out.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//改变字体颜色为普通
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

}