#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <iostream>
#include "error.h"
#include "windows.h"
#include "stdafx.h"
#include "network.h"

using namespace std;

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
	vector<network> nets;		//子网络容器
	int  nodeNum;				//节点数量
	int  linkNum;				//边的数量
	int type;					//网络类型

	multiNet();
	multiNet(network A, network B, multiType type);
	multiNet(int N, network n1, multiType type);
	multiNet(vector<network> temp, multiType type);
	multiNet(string in, int fileNum);
	~multiNet();
	void showMultiNet();
	int addLinkBetweenNets(int net1, int node1, int net2, int node2, double weight);
	int delLinkBetweenNets(int net1, int node1, int net2, int node2);
	int delNodeFromMultiNet(int netid, int nodeid);
	int addNetworkToMulti(network n1);
	void outputLinksFromMulti(string out);
	int inputLinksToMulti(string in);
	void outputMultiNet(string out);
	void outputMultiNet_pajek(string out);
};

