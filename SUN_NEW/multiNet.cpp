#include "multiNet.h"

#pragma region ������

//����յĶ�����
multiNet::multiNet()
{
	linkNum = 0;				//��ʼ����������
	nodeNum = 0;				//��ʼ���ڵ�����
	netNum = 0;
	nets.insert(make_pair(0, network(10, 0)));
	type = MULTI;
}

//˫���繹�캯��
multiNet::multiNet(network A, network B)			//��������
{
	netNum = 2;									//��ʼ������������
	A.netid = 1;									//����ID0
	B.netid = 2;									//����ID1

	nets.clear();

	//��ʼ������������
	nets.insert(make_pair(A.netid, A));
	nets.insert(make_pair(B.netid, B));

	nodeNum = nets[A.netid].nodeNum + nets[B.netid].nodeNum;		//��ʼ���ڵ�����
	linkNum = nets[A.netid].linkNum + nets[B.netid].linkNum;	//��ʼ���ߵ�����
																//this->type=type;								//��ʼ����������
}


//����N��������ɶ����磬���캯��
multiNet::multiNet(int N			//����������
	, network n1		//Ҫ���Ƶ��������
	)
{
	netNum = N;										//��ʼ������������
	nodeNum = 0;
	linkNum = 0;
	this->type = 0;

	//��ʼ������������������������ΪN
	for (int i = 1; i <= netNum; i++)
	{
		n1.netid = i;
		nets.insert(make_pair(n1.netid, n1));
	}

	//��ʼ���������������ͽڵ���
	for (auto it : nets)
	{
		nodeNum += it.second.nodeNum;
		linkNum += it.second.linkNum;
	}
}

//��ȡ�������������������
multiNet::multiNet(vector<network> temp)			//��������

{
	//��ʼ����������
	nets.clear();
	netNum = 0;

	//��ʼ�����Ӻͽڵ���Ϣ
	nodeNum = 0;
	linkNum = 0;

	for (auto it : temp)
	{

		netNum++;								//���ӽڵ���Ϣ
		it.netid = netNum;						//����������
		nets.insert(make_pair(netNum, it));	//����ڵ�

											//ÿ����һ���ڵ㣬���������Ϣ
		nodeNum += it.nodeNum;
		linkNum += it.linkNum;
	}
}

//��ȡĿ¼�е��ļ�Ⱥ�������������󣬸�ʽΪ	net1.txt	net2.txt	......	links.txt
multiNet::multiNet(string in)
{
	if (in.empty())
	{
		CCol(0, 2);
		cout << "Ŀ¼������Ϊ�գ����Զ�����ԭʼ�����硣" << endl;
		CCol(0, 0);
		new (this) multiNet();
	}

	//�ı�������ɫΪ��
	CCol(0, 1);
	cout << "���ڶ�ȡ��������Ϣ�ļ�Ⱥ��·��Ϊ�� " << in.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//�ı�������ɫΪ��ͨ
	CCol(0, 0);

	//��������
	linkNum = 0;				//��ʼ����������
	nodeNum = 0;				//��ʼ���ڵ�����
	netNum = 0;

	//��links�������
	string inlink = in + "/links.txt";
	ifstream filelink(inlink.data(), ios::in);

	//����ļ��Ƿ��ȡʧ��
	if (filelink.fail())
	{
		CCol(0, 2);
		cout << "��ȡ�����������ļ�ʧ�ܣ�·��Ϊ�� " << inlink.c_str() << "�����Զ�����ԭʼ�����硣" << endl;
		CCol(0, 0);
		new (this) multiNet();
		return;
	}

	vector<int> netsF;			//����������
	string line;					//������ʱһ��

	getline(filelink, line);		//��ʼ��ȡ�ڵ���Ϣ������һ��
	stringstream stream(line);	//����һ����Ϣ���Կո�Ϊ�ָ���

								//���������ţ����洴����
	int netnum;
	while (stream >> netnum)
	{
		netsF.insert(netsF.end(), netnum);
	}


	//ѭ��fileNum - 1�Σ��Ȱ�����д��
	for (auto it : netsF)
	{
		//��intת��Ϊstring
		stringstream ss;
		string tempNum;
		ss << it;
		ss >> tempNum;

		//��netx.txt�ļ�������ʱ������󣬲����������
		addNetworkToMulti(network(in + "/net" + tempNum + ".txt"), it);
	}

	//���һ��д�����������

	inputLinksToMulti(in + "/links.txt");

	//����������ļ���ȡ�����������ȡ���������磨�ò�����ע����
	/*
	//int error=inputLinksToMulti(in + "/links.txt");
	if (error)
	{
	CCol(0,2);
	cout << "Ŀ¼�ļ���Ϣ������ȷ����Ϣ��������ԣ����Զ�����ԭʼ�����硣" << endl;
	CCol(0,0);
	new (this) multiNet();
	return;
	}
	*/

	//�ı�������ɫΪ��
	CCol(0, 1);
	cout << endl << "����ִ����ϣ���������Ķ�ȡ��Ϣ��·��Ϊ�� " << in.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//�ı�������ɫΪ��ͨ
	CCol(0, 0);

}

//��������
multiNet::~multiNet()
{
	//�����Զ���
}

//��ʾ����
void multiNet::showMultiNet()
{
	CCol(1, 3);
	cout << "�ö�������������� " << netNum << " �����ڵ��� " << nodeNum << " ���������� " << linkNum << " ����" << endl;
	CCol(0, 0);
	for (auto it : nets)
	{
		it.second.showNetwork();
	}
}

//��Ӳ�ͬ����֮������ӣ�Ҳ������ӱ�����֮�������
int multiNet::addLinkBetweenNets(int net1, int node1, int net2, int node2, double weight)
{
	//�ж�net1��net2û��Խ��
	if (!isValidNet(net1) || !isValidNet(net2) || net1 <= 0 || net2 <= 0)
	{
		int error = BEYONDLIMIT;

		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ������ ���޷���Ӷ��������ӣ����� " << net1 << " �� ���� " << net2 << " Խ�硣" << endl;
		//�ı�������ɫΪ��ͨ
		CCol(0, 0);

		return error;
	}

	//�ж��Ƿ���ͬһ����֮�������
	if (net1 == net2)
	{
		int error = 0;
		error = nets[net1].addLinkToNetwork(node1, node2, weight);
		linkNum++;
		return error;
	}

	//�鿴node1��node2�Ƿ�Խ��
	if (!nets[net1].isValidNode(node1) || !nets[net2].isValidNode(node2) || node1 <= 0 || node2 <= 0)
	{
		int error = BEYONDLIMIT;

		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ������ ���޷���� ����" << net1 << "�� �ڵ� " << node1 << " -->> " << "����" << net2 << "�� �ڵ�" << node2 << "����ΪԽ�硣" << endl;
		//�ı�������ɫΪ��ͨ
		CCol(0, 0);

		return error;
	}

	//��ʼ�������
	int error = 0;

	//��node1����node1��node2�ĳ�������
	error = nets[net1].nodes[node1].addLinkToNodeMulti(net2, node2, weight);
	error = nets[net1].linkNum++;

	//��node2����node1��node2���������
	error = nets[net2].nodes[node2].addLinkFromNodeMulti(net1, node1, weight);

	linkNum++;
	return error;
}


//ɾ����ͬ����֮������ӣ�Ҳ����ɾ��������֮�������
int multiNet::delLinkBetweenNets(int net1, int node1, int net2, int node2)
{
	//�ж�net1��net2û��Խ��
	if (isValidNet(net1) || isValidNet(net2) || net1 <= 0 || net2 <= 0)
	{
		int error = BEYONDLIMIT;

		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ������ ���޷�ɾ�����������ӣ����� " << net1 << " �� ���� " << net2 << " Խ�硣" << endl;
		//�ı�������ɫΪ��ͨ
		CCol(0, 0);

		return error;
	}

	//�ж��Ƿ���ͬһ����֮�������
	if (net1 == net2)
	{
		int error = 0;
		error = nets[net1].delLinkFromNetwork(node1, node2);
		linkNum--;
		return error;
	}

	//�鿴node1��node2�Ƿ�Խ��
	if (!nets[net1].isValidNode(node1) || !nets[net2].isValidNode(node2) || node1 <= 0 || node2 <= 0)
	{
		int error = BEYONDLIMIT;

		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ������ ���޷�ɾ�����������ӣ��ڵ� " << node1 << " �� �ڵ� " << node2 << " Խ�硣" << endl;
		//�ı�������ɫΪ��ͨ
		CCol(0, 0);

		return error;
	}

	//��ʼɾ������
	int error = 0;

	//��node1����node1��node2�ĳ�������
	error = nets[net1].nodes[node1].delLinkToNodeMulti(net2, node2);
	error = nets[net1].linkNum--;

	//��node2����node1��node2���������
	error = nets[net2].nodes[node2].delLinkFromNodeMulti(net1, node1);

	linkNum--;
	return error;
}

//ɾ��������ڵ�
int multiNet::delNodeFromMultiNet(int netid, int nodeid)
{
	//�ж�netid�Ƿ�Խ��
	if (netid > netNum || netid <= 0)
	{
		int error = BEYONDLIMIT;

		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ������ ���޷�ɾ��������ڵ㣬���� " << netid << " Խ�硣" << endl;
		//�ı�������ɫΪ��ͨ
		CCol(0, 0);

		return error;
	}

	//�鿴nodeid�Ƿ�Խ��
	if (!nets[netid].isValidNode(nodeid) || nodeid <= 0)
	{
		int error = BEYONDLIMIT;

		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ������ ���޷�ɾ��������ڵ㣬�ڵ� " << nodeid << " Խ�硣" << endl;
		//�ı�������ɫΪ��ͨ
		CCol(0, 0);

		return error;
	}

	//��ʼɾ���ڵ�

	//����ýڵ�������������ӹ�������ȫ��ɾ��

	//������������
	if (nets[netid].nodes[nodeid].numAdjMultiOut > 0)
	{
		for (auto it : nets[netid].nodes[nodeid].adjMultiOut)
			for (auto it2 : it.second)
			{
				nets[it.first].nodes[it2.first].delLinkFromNodeMulti(netid, nodeid);
				//ɾ��������it.first�Ľڵ�it2.first <<-- ����netid�Ľڵ�nodeid
				//ָ��ýڵ����ӵ������������������������
				nets[it.first].linkNum--;
				nets[netid].nodes[nodeid].numAdjMultiOut--;

				//��������������������
				linkNum--;
			}
		//����Ƿ�����ɾ�
		if (nets[netid].nodes[nodeid].numAdjMultiOut == 0)
		{
			//��������ɾ���
			nets[netid].nodes[nodeid].adjMultiOut.clear();
		}
	}

	//������������
	if (nets[netid].nodes[nodeid].numAdjMultiIn > 0)
	{
		for (auto it : nets[netid].nodes[nodeid].adjMultiIn)
			for (auto it2 : it.second)
			{
				nets[it.first].nodes[it2.first].delLinkToNodeMulti(netid, nodeid);
				//ɾ��������it.first�Ľڵ�it2.first -->> ����netid�Ľڵ�nodeid
				//ָ��ýڵ����ӵ������������������������
				nets[it.first].linkNum--;
				nets[netid].nodes[nodeid].numAdjMultiIn--;

				//��������������������
				linkNum--;
			}
		//����Ƿ�����ɾ�
		if (nets[netid].nodes[nodeid].numAdjMultiIn == 0)
		{
			//�������ɾ���
			nets[netid].nodes[nodeid].adjMultiIn.clear();
		}
	}

	//���������������������������ڸýڵ��������������ܺ�
	linkNum -= +nets[netid].nodes[nodeid].numAdjOut + nets[netid].nodes[nodeid].numAdjIn;

	//��Ⱥͳ��������ϣ���ʼ�������������ڵ�
	int error = nets[netid].delNodeFromNetwork(nodeid);



	if (!error)
	{
		nodeNum--;
	}

	return error;
}

//���һ�����絽������
int multiNet::addNetworkToMulti(network n1, int netid)
{

	if (n1.nodeNum == 0)
	{
		int error = 1;
		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ������ ���޷���ӿ����絽������" << endl;
		//�ı�������ɫΪ��ͨ
		CCol(0, 0);

		return error;
	}

	if (isValidNet(netid))
	{
		int error = 1;
		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ������ ���������Ѵ������� " << netid << " �����޷���ӡ�" << endl;
		//�ı�������ɫΪ��ͨ
		CCol(0, 0);
		return error;
	}

	//����������һ
	netNum++;

	//��������ڵ����������
	linkNum += n1.linkNum;
	nodeNum += n1.nodeNum;

	//�ı�netid
	n1.netid = netid;

	//nets�������������
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

//������������ӵ�txt�ļ�����ʽΪ ����1 �ڵ�1 ����2 �ڵ�2
void multiNet::outputLinksFromMulti(string out)
{
	if (out.empty())
	{
		CCol(0, 2);
		cout << "·��������Ϊ�գ����Զ����ء�" << endl;
		CCol(0, 0);
		return;
	}

	//����Ƿ�Ϊ�༶Ŀ¼
	if (out.find('/') < out.length())
	{
		GetDirAndCreate(out);	//���Ŀ¼���� 
	}


	//����������
	ofstream  file(out, std::ios_base::ate);

	if (file.fail())
	{

		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "�����ļ�·������ " << out.c_str() << " ������·���Ƿ���ȷ��" << endl;
		//�ı�������ɫΪ��ͨ
		CCol(0, 0);

		return;
	}

	//�ı�������ɫΪ��
	CCol(0, 1);
	cout << "���ڴ�����ͨ��ʽ�Ķ�����������Ϣ�ļ��� " << out.c_str() << endl;
	//�ı�������ɫΪ��ͨ
	CCol(0, 0);

	//��һ��д��������
	for (auto it : nets)
	{
		file << it.second.netid << " ";
	}

	file << endl;
	//��ʼд�������
	for (auto it : nets)
		for (auto it1 : it.second.nodes)
		{
			//it.netid	����1
			//it1.id	�ڵ�1
			for (auto it2 : it1.second.adjMultiOut)
			{
				//it2.first	����2
				for (auto it3 : it2.second)
				{
					//it3.first		�ڵ�2
					//it3.second	Ȩֵ

					//weight�ᱣ����λС����
					file << it.second.netid << " " << it1.second.id << " " << it2.first << " " << it3.first << " " << setiosflags(ios::fixed) << setprecision(2) << it3.second << endl;
				}
			}
		}
}

//��ȡ����������txt�ļ�����ӵ���ǰ������
int multiNet::inputLinksToMulti(string inlink)
{
	if (inlink.empty())
	{
		CCol(0, 2);
		cout << "·��������Ϊ�գ����Զ����ء�" << endl;
		CCol(0, 0);
		return 2;
	}

	int error_return = 0;
	ifstream filelink(inlink.data(), ios::in);

	//����ļ��Ƿ��ȡʧ��
	if (filelink.fail())
	{
		CCol(0, 2);
		cout << "��ȡ�����������ļ�ʧ�ܣ�·��Ϊ�� " << inlink.c_str() << "�����Զ����ش���" << endl;
		CCol(0, 0);
		return 2;
	}
	//�ļ�û�ж�ȡʧ�ܣ���������
	else
	{
		CCol(0, 1);
		cout << "��ȡ�����������ļ��ɹ���·��Ϊ�� " << inlink.c_str() << endl;
		CCol(0, 0);
		string line;				//������ʱһ��


		getline(filelink, line);		//��ʼ��ȡ�ڵ���Ϣ������һ��
		stringstream stream(line);	//����һ����Ϣ���Կո�Ϊ�ָ���

		int netnum;
		while (stream >> netnum)
		{
			if (!isValidNet(netnum))
			{
				CCol(0, 2);
				cout << "���ڶ�ȡ�����������ļ������� " << netnum << " ȱʧ�����Զ�������·��Ϊ�� " << inlink.c_str() << endl;
				CCol(0, 0);
				addNetworkToMulti(network(1, 0), netNum);
				error_return = 1;
			}
		}


		//���濪ʼ��Ӷ���������
		while (!filelink.eof())
		{

			getline(filelink, line);		//��ȡ��ʱһ��
			stringstream stream(line);	//����һ����Ϣ���Կո�Ϊ�ָ���

			if (filelink.fail()) {
				break;
			}

			int net1, node1, net2, node2;
			double weight;

			//��ȡ������Ϣ
			stream >> net1;
			stream >> node1;
			stream >> net2;
			stream >> node2;
			stream >> weight;

			//������ӣ��ɹ�����������Ӽ�һ
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
		cout << "��ȡ�����������ļ���ϣ���������Ϣ����·��Ϊ�� " << inlink.c_str() << endl;
		CCol(0, 0);
	}

	return error_return;
}

void multiNet::outputMultiNet(string out)
{
	if (out.empty())
	{
		CCol(0, 2);
		cout << "Ŀ¼������Ϊ�գ����Զ����ء�" << endl;
		CCol(0, 0);
		return;
	}

	//ת����ȡĿ¼�ĸ�ʽ
	char* temp;
	const int len = out.length();
	temp = new char[len + 1];
	strcpy(temp, out.c_str());

	/*
	//����Ŀ¼
	if (CreatDir(temp) == -1)
	{
	CCol(0,2);
	cout << "Ŀ¼����ʧ�ܣ�·��Ϊ�� " << out << "�����Զ����ء�" << endl;
	CCol(0,0);
	return;
	}
	*/

	//�ı�������ɫΪ��
	CCol(0, 1);
	cout << "���ڴ�����ͨ��ʽ�Ķ�������Ϣ�ļ�Ⱥ��·��Ϊ�� " << out.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//�ı�������ɫΪ��ͨ
	CCol(0, 0);

	//��ʼ��������ļ�����out·��
	for (auto it : nets)
	{
		//intת��Ϊstring
		stringstream ss;
		string tempNum;
		ss << it.second.netid;
		ss >> tempNum;

		//��ʼ���
		it.second.outputNetwork(out + "/net" + tempNum + ".txt");
	}

	//��ʼ��������������ļ�
	outputLinksFromMulti(out + "/links.txt");

	//�ı�������ɫΪ��
	CCol(0, 1);
	cout << endl << "�����ļ�������ϣ���������Ĵ�����Ϣ��·��Ϊ�� " << out.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//�ı�������ɫΪ��ͨ
	CCol(0, 0);



}

void multiNet::outputMultiNet_pajek(string out)
{
	if (out.empty())
	{
		CCol(0, 2);
		cout << "Ŀ¼������Ϊ�գ����Զ����ء�" << endl;
		CCol(0, 0);
		return;
	}

	//ת����ȡĿ¼�ĸ�ʽ
	char* temp;
	const int len = out.length();
	temp = new char[len + 1];
	strcpy(temp, out.c_str());

	//�ı�������ɫΪ��
	CCol(0, 1);
	cout << "���ڴ���pajek��ʽ�Ķ�������Ϣ�ļ�Ⱥ��·��Ϊ�� " << out.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//�ı�������ɫΪ��ͨ
	CCol(0, 0);

	//��ʼ��������ļ�����out·��
	for (auto it : nets)
	{
		//intת��Ϊstring
		stringstream ss;
		string tempNum;
		ss << it.second.netid;
		ss >> tempNum;

		//��ʼ���
		it.second.outputNetwork_pajek(out + "/net" + tempNum + ".txt");
	}

	//��ʼ��������������ļ�
	outputLinksFromMulti(out + "/links.txt");

	//�ı�������ɫΪ��
	CCol(0, 1);
	cout << endl << "����ִ����ϣ���������Ĵ�����Ϣ��·��Ϊ�� " << out.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//�ı�������ɫΪ��ͨ
	CCol(0, 0);

}

int multiNet::changeNetid(int oldid, int newid)
{
	//����޸�idһ�����򷵻ش���
	if (oldid == newid)
	{
		CCol(0, 2);
		cout << "�޸�����ID���ִ����޷��޸�Ϊ�Լ��� " << endl;
		CCol(0, 0);

		return 1;
	}

	//�鿴ԭ�����Ƿ����
	if (!isValidNet(oldid))
	{
		CCol(0, 2);
		cout << "�޸�����ID���ִ���ԭ ���� " << oldid << " �������ڡ�" << endl;
		CCol(0, 0);

		return 1;
	}

	//�鿴�������Ƿ�ռ��
	if (isValidNet(newid))
	{
		CCol(0, 2);
		cout << "�޸�����ID���ִ����� ���� " << newid << " �ѱ�ռ�á�" << endl;
		CCol(0, 0);

		return 1;
	}

	//�����µ�����
	nets.insert(make_pair(newid, nets[oldid]));
	//�ı�����ID
	nets[newid].netid = newid;
	//ɾ��������
	nets.erase(oldid);

	//����ÿһ���ڵ�
	for (auto it : nets[newid].nodes)
	{
		//������������ȣ��ı��������������
		for (auto itout : it.second.adjMultiOut)
		{
			//itout.first�Ƕ���������
			for (auto itin : itout.second)
			{
				//itin.first�ǽڵ���
				//itin.second������Ȩֵ

				//���Ҷ�������ľ���Ϣ
				auto target = nets[itout.first].nodes[itin.first].adjMultiIn.find(oldid);

				//��������Ϣ
				nets[itout.first].nodes[itin.first].adjMultiIn.insert(make_pair(newid, nets[itout.first].nodes[itin.first].adjMultiIn[oldid]));

				//ɾ������Ϣ
				nets[itout.first].nodes[itin.first].adjMultiIn.erase(target);

				//cout << "�޸��� ����" << itout.first << " �� �ڵ� " << itin.first << " ."<< endl;
			}
		}
		//������������ȣ��ı����ĳ���������
		for (auto itin : it.second.adjMultiIn)
		{

			//itin.first�Ƕ���������
			for (auto itout : itin.second)
			{
				//itout.first�ǽڵ���
				//itout.second������Ȩֵ

				//���Ҷ�������ľ���Ϣ
				auto target = nets[itin.first].nodes[itout.first].adjMultiOut.find(oldid);

				//��������Ϣ
				nets[itin.first].nodes[itout.first].adjMultiOut.insert(make_pair(newid, nets[itin.first].nodes[itout.first].adjMultiOut[oldid]));

				//ɾ������Ϣ
				nets[itin.first].nodes[itout.first].adjMultiOut.erase(target);

				//cout << "�޸��� ����" << itin.first << " �� �ڵ� " << itin.first << " ." << endl;
			}

		}

	}
	return 0;
}

int multiNet::changeNodeid(int netid, int oldid, int newid)
{
	//netid�Ƿ���ڣ�
	if (!isValidNet(netid))
	{
		CCol(0, 2);
		cout << "�޸Ķ����� �ڵ�ID���ִ������� " << netid << " �������ڡ�" << endl;
		CCol(0, 0);

		return 1;
	}

	//�Ͻڵ����Ƿ����
	if (!nets[netid].isValidNode(oldid))
	{
		CCol(0, 2);
		cout << "�޸Ķ����� �ڵ�ID���ִ������� " << netid << " �� �ڵ� " << oldid << " �������ڡ�" << endl;
		CCol(0, 0);

		return 1;
	}

	//�½ڵ����Ƿ�ռ�ã�
	if (nets[netid].isValidNode(newid))
	{
		CCol(0, 2);
		cout << "�޸Ķ����� �ڵ�ID���ִ������� " << netid << " �� �ڵ� " << newid << " �Ѿ����ڡ�" << endl;
		CCol(0, 0);

		return 1;
	}

	//����������޸Ľڵ���
	nets[netid].changeNodeid(oldid, newid);

	//��������ȣ��޸Ķ��汣��ľ���Ϣ
	for (auto itout : nets[netid].nodes[newid].adjMultiOut)
	{
		//itout.first�Ƕ�������ı��
		for (auto itin : itout.second)
		{
			//itin.first�Ƕ���ڵ�ı��
			//itin.second�����ӵ�Ȩֵ

			//����µ���Ϣ
			nets[itout.first].nodes[itin.first].adjMultiIn[netid].insert(make_pair(newid, nets[itout.first].nodes[itin.first].adjMultiIn[netid][oldid]));

			//ɾ���ϵ���Ϣ
			nets[itout.first].nodes[itin.first].adjMultiIn[netid].erase(oldid);

		}
	}

	//��������ȣ��޸Ķ��汣��ľ���Ϣ
	for (auto itin : nets[netid].nodes[newid].adjMultiIn)
	{

		//itin.first�Ƕ�������ı��
		for (auto itout : itin.second)
		{
			//itout.first�Ƕ���ڵ�ı��
			//itout.second�����ӵ�Ȩֵ

			//����µ���Ϣ
			nets[itin.first].nodes[itout.first].adjMultiOut[netid].insert(make_pair(newid, nets[itin.first].nodes[itout.first].adjMultiOut[netid][oldid]));

			//ɾ���ϵ���Ϣ
			nets[itin.first].nodes[itout.first].adjMultiOut[netid].erase(oldid);

		}
	}
	return 0;
}

int multiNet::redistribute()
{

	CCol(0, 1);
	cout << "����Ϊ�������硱���·���ڵ��š�" << endl;
	CCol(0, 0);

	//���û�����磬ֱ�ӷ���
	if (netNum == 0)
	{
		return 0;
	}

	//�������0���ڣ��������������
	if (nets.find(0) != nets.end())
	{
		auto end = nets.rbegin();
		changeNetid(0, end->first + 1);
	}

	for (int i = 1; i <= netNum; i++)
	{
		//��������Ƿ���ڣ�����п�ȱ���ö�β�
		if (!isValidNet(i))
		{
			auto end = nets.rbegin();
			changeNetid(end->first, i);
		}

		//��ʼ�����Ų������ӽڵ�
		for (int j = 1; j <= nets[i].nodeNum; j++)
		{
			//����ڵ��п�ȱ�����ö�β�
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