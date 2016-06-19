#pragma once
#include "stdafx.h"
using namespace std;

class node
{
public:
	int id;											//节点ID号
    int level;                                      //节点层级
	int ci;											//节点CI值
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