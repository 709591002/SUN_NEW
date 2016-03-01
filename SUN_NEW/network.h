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
#include "other.h"
using namespace std;

//��������ö��
enum netType
{
	UN_DIRECT_UN_WEIGHT = 0,		//������Ȩ��
	UN_DIRECT_WITH_WEIGHT = 1,		//�����Ȩ����
	DIRECT_UN_WEIGHT = 2,			//������Ȩ����
	DIRECT_WITH_WEIGHT = 3			//�����Ȩ����
};

class network
{
public:
	int netid;					//�����ţ�Ĭ��Ϊ0
	vector<node> nodes;			//�ڵ�����
	int  nodeNum;				//�ڵ�����
	int  linkNum;				//�ߵ�����
	int type;					//��������
	network(int N, netType type);	//Ĭ�Ϲ��캯��
	network(string in);			//��ȡ�ļ��������磬֧���Զ�ʶ��txt��ʽ

	network(int N, netType type,int netid);	//Ĭ�Ϲ��캯������������
	network(string in, int netid) ;			//��ȡ�ļ��������磬֧���Զ�ʶ��txt��ʽ����������

	//network(network & n2);		//�������캯��
	~network();					//��������
	void showNetwork();			//��ʾ����
	int addLinkToNetwork(int from, int to, double weight);	//��from��to���ȨֵΪweight������
	int delLinkFromNetwork(int from, int to);				//��from��toɾ������
	int addNodesToNetwork(int N);							//�������ΪN�Ľڵ�
	int delNodeFromNetwork(int nodeid);						//ɾ�����Ϊnodeid������ڵ�
	int isValidLink(int from, int to);						//��֤��from��to�������Ƿ����
	int setLinkWeight(int from, int to, double weight);		//�޸Ĵ�from��to������Ȩֵ
	double getLinkWeight(int from, int to);					//��ȡ��from��to������Ȩֵ
	void outputNetwork(string out);							//�������Ϊ��ͨ��ʽ��֧���Զ�����Ŀ¼(xxx/xxx.txt)
	void outputNetwork_pajek(string out);					//�������Ϊ��ͨpajek��ʽ��֧���Զ�����Ŀ¼(xxx/xxx.txt)
	void outputNetwork_pajek_frac(string out);				//�������Ϊ�߼�pajek��ʽ��֧���Զ�����Ŀ¼(xxx/xxx.txt)
};

