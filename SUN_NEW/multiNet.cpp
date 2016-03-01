#include "multiNet.h"

//���������
multiNet::multiNet()
{
	linkNum = 0;				//��ʼ����������
	nodeNum = 0;				//��ʼ���ڵ�����
	netNum = 0;
	network n(10, UN_DIRECT_UN_WEIGHT);
	nets.insert(nets.begin(),n);
	type = MULTI;
}

//˫���繹�캯��
multiNet::multiNet(network A,network B				//��������
					,multiType type)				//��������
{
	netNum=2;										//��ʼ������������
	A.netid = 1;									//����ID0
	B.netid = 2;									//����ID1
	
	nets.clear();

	//��ʼ������������
	nets.insert(nets.end(),A);
	nets.insert(nets.end(),B);

	nodeNum=nets[0].nodeNum+ nets[1].nodeNum;		//��ʼ���ڵ�����
	linkNum = nets[0].linkNum + nets[1].linkNum;	//��ʼ���ߵ�����
	this->type=type;								//��ʼ����������
}


//����N��������ɶ����磬���캯��
multiNet::multiNet(int N			//����������
				, network n1		//Ҫ���Ƶ��������
				, multiType type)	//��������
{
	netNum = N;										//��ʼ������������
	nodeNum = 0;
	linkNum = 0;
	this->type = 0;
	
	//��ʼ������������������������ΪN
	for (int i = 1; i <= netNum; i++)
	{
		n1.netid = i;
		nets.insert(nets.end(),n1);
	}

	//��ʼ���������������ͽڵ���
	for (auto it : nets)
	{
		nodeNum += it.nodeNum;
		linkNum += it.linkNum;
	}
	this->type=type;								//��ʼ����������
}

//��ȡ�������������������
multiNet::multiNet(vector<network> temp				//��������
					, multiType type)				//��������
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
		nets.insert(nets.begin()+ netNum,it);	//����ڵ�
		
		//ÿ����һ���ڵ㣬���������Ϣ
		nodeNum += it.nodeNum;
		linkNum += it.linkNum;
	}
	this->type = type;
}

//��ȡĿ¼�е��ļ�Ⱥ�������������󣬸�ʽΪ	net1.txt	net2.txt	......	links.txt
multiNet::multiNet(string in,int fileNum)
{
	if (in.empty())
	{
		CCol(0,2);
		cout << "Ŀ¼������Ϊ�գ����Զ�����ԭʼ�����硣" << endl;
		CCol(0,0);
		new (this) multiNet();
	}


	
	//С��2˵���������������
	if (fileNum < 2)
	{
		CCol(0,2);
		cout << "Ŀ¼û�������ļ������޸�·���������ԣ����Զ�����ԭʼ�����硣" << endl;
		CCol(0,0);
		new (this) multiNet();
	}

	//�ı�������ɫΪ��
	CCol(0,1);
	cout << "���ڶ�ȡ��������Ϣ�ļ�Ⱥ��·��Ϊ�� " << in.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//�ı�������ɫΪ��ͨ
	CCol(0,0);

	//��������
	linkNum = 0;				//��ʼ����������
	nodeNum = 0;				//��ʼ���ڵ�����
	netNum = 0;

	//ѭ��fileNum - 1�Σ��Ȱ�����д��
	for (int i = 1; i < fileNum; i++)
	{
		//��intת��Ϊstring
		stringstream ss;
		string tempNum;
		ss << i;
		ss >> tempNum;

		//��netx.txt�ļ�������ʱ������󣬲����������
		network temp(in + "/net" + tempNum + ".txt");
		temp.netid=i;
		addNetworkToMulti(temp);
	}

	//���һ��д�����������
	int error=inputLinksToMulti(in + "/links.txt");

	//
	/*
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
	CCol(0,1);
	cout << endl << "����ִ����ϣ���������Ķ�ȡ��Ϣ��·��Ϊ�� " << in.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//�ı�������ɫΪ��ͨ
	CCol(0,0);

}

//��������
multiNet::~multiNet()
{
	//�����Զ���
}

//��ʾ����
void multiNet::showMultiNet()
{
	cout << "�ö�������������� "<<netNum<<" �����ڵ��� " <<nodeNum<<" ���������� "<<linkNum<<" ����"<< endl;
	for (auto it : nets)
	{
		it.showNetwork();
	}
}

//��Ӳ�ͬ����֮������ӣ�Ҳ������ӱ�����֮�������
int multiNet::addLinkBetweenNets(int net1,int node1,int net2,int node2,double weight)
{
	//�ж�net1��net2û��Խ��
	if (net1 > netNum || net2 > netNum || net1<=0||net2<=0)
	{
		int error = BEYONDLIMIT;

		//�ı�������ɫΪ��
		CCol(0,2);
		cout << "����һ������ ���޷���Ӷ��������ӣ����� " << net1<<" �� ���� "<<net2<<" Խ�硣" << endl;
		//�ı�������ɫΪ��ͨ
		CCol(0,0);

		return error;
	}

	//�ж��Ƿ���ͬһ����֮�������
	if (net1 == net2)
	{
		int error=0;
		error=nets[net1-1].addLinkToNetwork(node1, node2, weight);
		linkNum++;
		return error;
	}

	//�鿴node1��node2�Ƿ�Խ��
	if (node1 > nets[net1-1].nodeNum || node2 > nets[net2-1].nodeNum|| node1<=0||node2<=0)
	{
		int error = BEYONDLIMIT;

		//�ı�������ɫΪ��
		CCol(0,2);
		cout << "����һ������ ���޷���� ����"<<net1<<"�� �ڵ� " << node1 << " -->> " << "����"<<net2<<"�� �ڵ�"<< node2 << "����ΪԽ�硣" << endl;
		//�ı�������ɫΪ��ͨ
		CCol(0,0);

		return error;
	}

	//��ʼ�������
	int error=0;

	//��node1����node1��node2�ĳ�������
	error=nets[net1-1].nodes[node1-1].addLinkToNodeMulti(net2, node2, weight);
	error=nets[net1-1].linkNum++;
	
	//��node2����node1��node2���������
	error=nets[net2-1].nodes[node2-1].addLinkFromNodeMulti(net1, node1, weight);
	
	linkNum++;
	return error;
}


//ɾ����ͬ����֮������ӣ�Ҳ����ɾ��������֮�������
int multiNet::delLinkBetweenNets(int net1, int node1, int net2, int node2)
{
	//�ж�net1��net2û��Խ��
	if (net1 > netNum || net2 > netNum || net1<=0 || net2<=0)
	{
		int error = BEYONDLIMIT;

		//�ı�������ɫΪ��
		CCol(0,2);
		cout << "����һ������ ���޷�ɾ�����������ӣ����� " << net1 << " �� ���� " << net2 << " Խ�硣" << endl;
		//�ı�������ɫΪ��ͨ
		CCol(0,0);

		return error;
	}

	//�ж��Ƿ���ͬһ����֮�������
	if (net1 == net2)
	{
		int error=0;
		error = nets[net1-1].delLinkFromNetwork(node1, node2);
		linkNum--;
		return error;
	}

	//�鿴node1��node2�Ƿ�Խ��
	if (node1 > nets[net1-1].nodeNum || node2 > nets[net2-1].nodeNum|| node1<=0|| node2<=0)
	{
		int error = BEYONDLIMIT;

		//�ı�������ɫΪ��
		CCol(0,2);
		cout << "����һ������ ���޷�ɾ�����������ӣ��ڵ� " << node1 << " �� �ڵ� " << node2 << " Խ�硣" << endl;
		//�ı�������ɫΪ��ͨ
		CCol(0,0);

		return error;
	}

	//��ʼɾ������
	int error=0;

	//��node1����node1��node2�ĳ�������
	error = nets[net1-1].nodes[node1].delLinkToNodeMulti(net2, node2);
	error = nets[net1-1].linkNum--;

	//��node2����node1��node2���������
	error = nets[net2-1].nodes[node2].delLinkFromNodeMulti(net1, node1);

	linkNum--;
	return error;
}

//ɾ��������ڵ�
int multiNet::delNodeFromMultiNet(int netid, int nodeid)
{
	//�ж�netid�Ƿ�Խ��
	if (netid > netNum|| netid<=0 )
	{
		int error = BEYONDLIMIT;

		//�ı�������ɫΪ��
		CCol(0,2);
		cout << "����һ������ ���޷�ɾ��������ڵ㣬���� " << netid <<" Խ�硣" << endl;
		//�ı�������ɫΪ��ͨ
		CCol(0,0);

		return error;
	}

	//�鿴nodeid�Ƿ�Խ��
	if (nodeid > nets[netid-1].nodeNum||nodeid<=0)
	{
		int error = BEYONDLIMIT;

		//�ı�������ɫΪ��
		CCol(0,2);
		cout << "����һ������ ���޷�ɾ��������ڵ㣬�ڵ� " << nodeid << " Խ�硣" << endl;
		//�ı�������ɫΪ��ͨ
		CCol(0,0);

		return error;
	}

	//��ʼɾ���ڵ�

	//����ýڵ�������������ӹ�������ȫ��ɾ��
	
	//������������
	if (nets[netid-1].nodes[nodeid-1].numAdjMultiOut > 0)
	{
		for (auto it : nets[netid - 1].nodes[nodeid - 1].adjMultiOut)
			for (auto it2 : it.second)
			{
				nets[it.first - 1].nodes[it2.first - 1].delLinkFromNodeMulti(netid, nodeid);
				//ɾ��������it.first�Ľڵ�it2.first <<-- ����netid�Ľڵ�nodeid
				//ָ��ýڵ����ӵ������������������������
				nets[it.first - 1].linkNum--;
				nets[netid - 1].nodes[nodeid - 1].numAdjMultiOut--;

				//��������������������
				linkNum--;
			}
		//����Ƿ�����ɾ�
		if (nets[netid - 1].nodes[nodeid - 1].numAdjMultiOut == 0)
		{
			//��������ɾ���
			nets[netid - 1].nodes[nodeid - 1].adjMultiOut.clear();
		}
	}

	//������������
	if (nets[netid-1].nodes[nodeid-1].numAdjMultiIn > 0)
	{
		for(auto it: nets[netid-1].nodes[nodeid-1].adjMultiIn)
			for (auto it2 : it.second)
			{
				nets[it.first-1].nodes[it2.first-1].delLinkToNodeMulti(netid, nodeid);
				//ɾ��������it.first�Ľڵ�it2.first -->> ����netid�Ľڵ�nodeid
				//ָ��ýڵ����ӵ������������������������
				nets[it.first-1].linkNum--;
				nets[netid-1].nodes[nodeid-1].numAdjMultiIn--;

				//��������������������
				linkNum--;
			}
		//����Ƿ�����ɾ�
		if (nets[netid-1].nodes[nodeid-1].numAdjMultiIn == 0)
		{
			//�������ɾ���
			nets[netid-1].nodes[nodeid-1].adjMultiIn.clear();
		}
	}


	//��Ⱥͳ��������ϣ���ʼ�������������ڵ�
	int error = nets[netid-1].delNodeFromNetwork(nodeid);

	//���������������������������ڸýڵ��������������ܺ�
	linkNum -= +nets[netid-1].nodes[nodeid-1].numAdjOut + nets[netid-1].nodes[nodeid-1].numAdjIn;

	if (!error)
	{
		nodeNum--;
	}

	return error;
}

//���һ�����絽������
int multiNet::addNetworkToMulti(network n1)
{
	if (n1.nodeNum == 0)
	{
		int error = 1;
		//�ı�������ɫΪ��
		CCol(0,2);
		cout << "����һ������ ���޷���ӿ����絽������"<< endl;
		//�ı�������ɫΪ��ͨ
		CCol(0,0);

		return error;
	}

	//����������һ
	netNum++;

	//��������ڵ����������
	linkNum += n1.linkNum;
	nodeNum += n1.nodeNum;

	//�ı�netid
	n1.netid = netNum;

	//nets�������������
	nets.insert(nets.end(),n1);
	
	return 0;
}

//������������ӵ�txt�ļ�����ʽΪ ����1 �ڵ�1 ����2 �ڵ�2
void multiNet::outputLinksFromMulti(string out)
{
	if (out.empty())
	{
		CCol(0,2);
		cout << "·��������Ϊ�գ����Զ����ء�" << endl;
		CCol(0,0);
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
		CCol(0,2);
		cout << "�����ļ�·������ " << out.c_str() << " ������·���Ƿ���ȷ��" << endl;
		//�ı�������ɫΪ��ͨ
		CCol(0,0);

		return;
	}

	//�ı�������ɫΪ��
	CCol(0,1);
	cout << "���ڴ�����ͨ��ʽ�Ķ�����������Ϣ�ļ��� " << out.c_str() << endl;
	//�ı�������ɫΪ��ͨ
	CCol(0,0);

	//��ʼд���ļ�
	file << nodeNum << " " << type << endl;

	for(auto it:nets)
		for (auto it1 : it.nodes)
		{
			//it.netid	����1
			//it1.id	�ڵ�1
			for (auto it2 : it1.adjMultiOut)
			{
				//it2.first	����2
				for (auto it3 : it2.second)
				{
					//it3.first		�ڵ�2
					//it3.second	Ȩֵ

					//weight�ᱣ����λС����
					file << it.netid << " " << it1.id << " " << it2.first << " " << it3.first << " " << setiosflags(ios::fixed) << setprecision(2) << it3.second << endl;
				}
			}
		}
}

//��ȡ����������txt�ļ�����ӵ���ǰ������
int multiNet::inputLinksToMulti(string in)
{
	if (in.empty())
	{
		CCol(0,2);
		cout << "·��������Ϊ�գ����Զ����ء�" << endl;
		CCol(0,0);
		return 2;
	}

	int error_return = 0;
	ifstream file(in.data(), ios::in);

	//����ļ��Ƿ��ȡʧ��
	if (file.fail())
	{
		CCol(0,2);
		cout << "��ȡ�����������ļ�ʧ�ܣ�·��Ϊ�� " << in.c_str() << "�����Զ����ش���" << endl;
		CCol(0,0);
		return 2;
	}
	//�ļ�û�ж�ȡʧ�ܣ���������
	else
	{
		CCol(0,1);
		cout << "��ȡ�����������ļ��ɹ���·��Ϊ�� " << in.c_str() << endl;
		CCol(0,0);
		string line;				//������ʱһ��

		int tempNodeNum = 0;

		getline(file, line);		//��ʼ��ȡ�ڵ���Ϣ������һ��
		stringstream stream(line);	//����һ����Ϣ���Կո�Ϊ�ָ���
		stream >> tempNodeNum;		//��ȡ��������������
		stream >> type;				//��ȡ��������

		//���濪ʼ��Ӷ���������
		while (!file.eof())
		{

			getline(file, line);		//��ȡ��ʱһ��
			stringstream stream(line);	//����һ����Ϣ���Կո�Ϊ�ָ���

			if (file.fail()) {
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
		CCol(0,2);
		cout << "��ȡ�����������ļ���ϣ���������Ϣ����·��Ϊ�� " << in.c_str() << endl;
		CCol(0,0);
	}

	return error_return;
}

void multiNet::outputMultiNet(string out)
{
	if (out.empty())
	{
		CCol(0,2);
		cout << "Ŀ¼������Ϊ�գ����Զ����ء�" << endl;
		CCol(0,0);
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
	CCol(0,1);
	cout << "���ڴ�����ͨ��ʽ�Ķ�������Ϣ�ļ�Ⱥ��·��Ϊ�� " << out.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//�ı�������ɫΪ��ͨ
	CCol(0,0);

	//��ʼ��������ļ�����out·��
	for (auto it : nets)
	{
		//intת��Ϊstring
		stringstream ss;
		string tempNum;
		ss << it.netid;
		ss >> tempNum;
		
		//��ʼ���
		it.outputNetwork(out + "/net" + tempNum+".txt");
	}

	//��ʼ��������������ļ�
	outputLinksFromMulti(out + "/links.txt");

	//�ı�������ɫΪ��
	CCol(0,1);
	cout <<endl<< "�����ļ�������ϣ���������Ĵ�����Ϣ��·��Ϊ�� " << out.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//�ı�������ɫΪ��ͨ
	CCol(0,0);



}

void multiNet::outputMultiNet_pajek(string out)
{
	if (out.empty())
	{
		CCol(0,2);
		cout << "Ŀ¼������Ϊ�գ����Զ����ء�" << endl;
		CCol(0,0);
		return;
	}

	//ת����ȡĿ¼�ĸ�ʽ
	char* temp;
	const int len = out.length();
	temp = new char[len + 1];
	strcpy(temp, out.c_str());

	//�ı�������ɫΪ��
	CCol(0,1);
	cout << "���ڴ���pajek��ʽ�Ķ�������Ϣ�ļ�Ⱥ��·��Ϊ�� " << out.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//�ı�������ɫΪ��ͨ
	CCol(0,0);

	//��ʼ��������ļ�����out·��
	for (auto it : nets)
	{
		//intת��Ϊstring
		stringstream ss;
		string tempNum;
		ss << it.netid;
		ss >> tempNum;

		//��ʼ���
		it.outputNetwork_pajek(out + "/net" + tempNum + ".txt");
	}

	//��ʼ��������������ļ�
	outputLinksFromMulti(out + "/links.txt");

	//�ı�������ɫΪ��
	CCol(0,1);
	cout << endl << "����ִ����ϣ���������Ĵ�����Ϣ��·��Ϊ�� " << out.c_str() << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	//�ı�������ɫΪ��ͨ
	CCol(0,0);

}