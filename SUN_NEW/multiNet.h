#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <iostream>
#include "error.h"
#ifdef _WIN32
#include "windows.h"
#endif
#include "stdafx.h"
#include "network.h"

using namespace std;

//��������������
enum multiType {
	INTER_DEPENDENT = 0,  //�������磬˫����
	INTER_CONNECT = 1,	//�������磬˫����
	MULTI = 2				//�������磬����������
};

class multiNet
{
public:
	int netNum;					//����������
	vector<network> nets;		//����������
	int  nodeNum;				//�ڵ�����
	int  linkNum;				//�ߵ�����
	int type;					//��������

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

