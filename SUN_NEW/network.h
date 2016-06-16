﻿#pragma once
#include "stdafx.h"
using namespace std;

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
    vector<int> getLevelNodes(int nodeid, int d);           //获得指定层级的节点编号
};



//多网络网络类型
enum multiType {
	INTER_DEPENDENT = 0,  //相依网络，双网络
	INTER_CONNECT = 1,	//相连网络，双网络
	MULTI = 2				//纯多网络，数量无上限
};