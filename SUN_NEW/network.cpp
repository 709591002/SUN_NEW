#include "network.h"
#include <map>

using namespace std;

//默认构造函数
network::network()
{

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
			int nodeNumF = atoi(tempNum.c_str());		//储存网络节点数，包含string转换为int的过程

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