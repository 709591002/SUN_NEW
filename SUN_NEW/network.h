#pragma once

#include "network.h"
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <iostream>
#include "error.h"
#include <fstream> 
#include <iomanip>
#include <string>
#include <sstream>
#include "other.h"

#ifdef _WIN32
#include "windows.h"
#endif

using namespace std;

class multiNet;
class network;
class node;

class node
{
public:
	int id;											//节点ID号
	map<int, double> adjOut;						//出度邻接节点信息，编号和权重
	int  numAdjOut;									//出度邻居节点个数
	map<int, double> adjIn;							//入度邻接节点信息，编号和权重
	int  numAdjIn;									//入度邻居节点个数

	map<int, map<int, double>> adjMultiOut;			//多网络出度邻接节点信息，网络号，编号和权重
	int  numAdjMultiOut;							//多网络出度邻居节点个数
	map<int, map<int, double>> adjMultiIn;			//多网络入度邻接节点信息，网络号，编号和权重
	int  numAdjMultiIn;								//多网络入度邻居节点个数

	double strength;								//节点权值
	int cid;										//簇团id
	node();										//构造函数
	node(int id);								//构造函数
	~node();										//析构函数
	void showNode();								//显示节点信息
	int addLinkToNode(int nodeid, double weight);	//为节点增加出度邻居
	int delLinkFromNode(int nodeid);				//为节点删除出度邻居
	int addLinkFromNode(int nodeid, double weight);	//为节点增加入度邻居
	int delLinkToNode(int nodeid);					//为节点删除入度邻居
	int addLinkToNodeMulti(int netid, int nodeid, double weight);	//为节点增加多网络出度邻居
	int delLinkToNodeMulti(int netid, int nodeid);					//为节点删除多网络出度邻居
	int addLinkFromNodeMulti(int netid, int nodeid, double weight);	//为节点增加多网络入度邻居
	int delLinkFromNodeMulti(int netid, int nodeid);				//为节点删除多网络入度邻居
};


class network
{
public:
	int netid;					//网络编号，默认为0
	map <int,node> nodes;		//节点容器
	int  nodeNum;				//节点数量
	int  linkNum;				//边的数量
	//network(int N, netType type);	//默认构造函数
	network(string in);
	network();						//默认构造函数
	network(multiNet mul);			//通过多网络构造单网络
	//读取文件构造网络，支持自动识别txt格式
	network(int N, int start);	//默认构造函数
	network(string in, int netid) ;			//读取文件构造网络，支持自动识别txt格式，带网络编号
	//network(network & n2);		//拷贝构造函数
	~network();					//析构函数
	void showNetwork();			//显示网络
	int addLinkToNetwork(int from, int to, double weight);	//从from到to添加权值为weight的链接
	int delLinkFromNetwork(int from, int to);				//从from到to删除链接
	int addNodeToNetwork(int N);							//添加数量为N的节点
	int delNodeFromNetwork(int nodeid);						//删除编号为nodeid的网络节点
	int isValidLink(int from, int to);						//验证从from到to的链接是否存在
	int isValidNode(int num);								//验证num节点是否存在
	int setLinkWeight(int from, int to, double weight);		//修改从from到to的链接权值
	double getLinkWeight(int from, int to);					//获取从from到to的链接权值
	void outputNetwork(string out);							//输出网络为普通格式，支持自动创建目录(xxx/xxx.txt)
	void outputNetwork_pajek(string out);					//输出网络为普通pajek格式，支持自动创建目录(xxx/xxx.txt)
	void outputNetwork_pajek_frac(string out);				//输出网络为高级pajek格式，支持自动创建目录(xxx/xxx.txt)
	int changeNodeid(int oldid, int newid);					//改变网络编号
	int redistribute();										//重新分配网络编号
	//重新分配节点编号，从1开始
};



//多网络网络类型
enum multiType {
	INTER_DEPENDENT = 0,  //相依网络，双网络
	INTER_CONNECT = 1,	//相连网络，双网络
	MULTI = 2				//纯多网络，数量无上限
};

class multiNet
{
public:
	int netNum;					//子网络数量
	map <int, network> nets;		//子网络容器
	int  nodeNum;				//节点数量
	int  linkNum;				//边的数量
	int type;					//网络类型

	multiNet();
	multiNet(network A, network B);
	multiNet(int N, network n1);
	multiNet(vector<network> temp);
	multiNet(string in);
	~multiNet();
	void showMultiNet();
	int addLinkBetweenNets(int net1, int node1, int net2, int node2, double weight);
	int delLinkBetweenNets(int net1, int node1, int net2, int node2);
	int delNodeFromMultiNet(int netid, int nodeid);
	int addNetworkToMulti(network n1, int netid);
	int isValidNet(int netid);
	void outputLinksFromMulti(string out);
	int inputLinksToMulti(string in);
	void outputMultiNet(string out);
	void outputMultiNet_pajek(string out);
	int changeNetid(int oldid, int newid);
	int changeNodeid(int netid, int oldid, int newid);
	int redistribute();
};
