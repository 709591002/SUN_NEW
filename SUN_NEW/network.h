#pragma once

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <iostream>
#include "error.h"
#include "node.h"
#include <fstream> 
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;

//网络类型枚举
enum netType
{
	UN_DIRECT_UN_WEIGHT = 0,		//无向无权网
	UN_DIRECT_WITH_WEIGHT = 1,		//无向加权网络
	DIRECT_UN_WEIGHT = 2,			//有向无权网络
	DIRECT_WITH_WEIGHT = 3			//有向加权网络
};

class network
{
public:
	int netid;					//网络编号，默认为0
	vector<node> nodes;			//节点容器
	int  nodeNum;				//节点数量
	int  linkNum;				//边的数量
	int type;					//网络类型
	network(int N, netType type);	//默认构造函数
	network(string in);			//读取文件构造网络，支持自动识别txt格式

	network(int N, netType type,int netid);	//默认构造函数，带网络编号
	network(string in, int netid) ;			//读取文件构造网络，支持自动识别txt格式，带网络编号

	//network(network & n2);		//拷贝构造函数
	~network();					//析构函数
	void showNetwork();			//显示网络
	int addLinkToNetwork(int from, int to, double weight);	//从from到to添加权值为weight的链接
	int delLinkFromNetwork(int from, int to);				//从from到to删除链接
	int addNodesToNetwork(int N);							//添加数量为N的节点
	int delNodeFromNetwork(int nodeid);						//删除编号为nodeid的网络节点
	int isValidLink(int from, int to);						//验证从from到to的链接是否存在
	int setLinkWeight(int from, int to, double weight);		//修改从from到to的链接权值
	double getLinkWeight(int from, int to);					//获取从from到to的链接权值
	void outputNetwork(string out);							//输出网络为普通格式，支持自动创建目录(xxx/xxx.txt)
	void outputNetwork_pajek(string out);					//输出网络为普通pajek格式，支持自动创建目录(xxx/xxx.txt)
	void outputNetwork_pajek_frac(string out);				//输出网络为高级pajek格式，支持自动创建目录(xxx/xxx.txt)
};

