#include "multiNet.h"

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