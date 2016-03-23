#pragma once
#include "stdafx.h"
using namespace std;

class multiNet
{
public:
	int netNum;					//����������
	map <int, network> nets;		//����������
	int  nodeNum;				//�ڵ�����
	int  linkNum;				//�ߵ�����
	int type;					//��������

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
