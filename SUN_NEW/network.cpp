#include "network.h"


//���캯��
network::network(int N		//�ڵ������
				,netType type)  //��������
{
	//����涨�ڵ�������0
	if (N>0)
	{
		nodeNum = N;
		for (int i = 1; i <= N; i++)
		{
			nodes.insert(nodes.end(),i);
		}
	}
	//����ڵ����Ϊ0
	else 
	{
		nodeNum = 0;
	}
	linkNum = 0;
	netid = 0;			//Ĭ��������Ϊ0
	this->type = type;

}

//���캯������������
network::network(int N, netType type, int netid)
{

	//����Ĭ�Ϲ��캯��
	new (this) network(N, DIRECT_WITH_WEIGHT);

	//��д������
	this->netid = netid;
}


//��ȡ�ļ����캯�����Զ�ʶ����ͨ��ʽ��pajek��ʽ
network::network(string in)
{
	linkNum = 0;				//��ʼ����������
	netid = 0;					//Ĭ��������Ϊ0
	ifstream file(in.data(), ios::in);

	//����ļ��Ƿ��ȡʧ��
	if (file.fail())
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "��ȡ�ļ�ʧ�ܣ�·��Ϊ�� " << in.c_str() << "�����Զ����������滻Ϊ �����硣" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
		new (this) network(1, DIRECT_WITH_WEIGHT);
	}
	//�ļ�û�ж�ȡʧ�ܣ���������
	else 
	{
		int error = 0;				//������
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "��ȡ�ļ��ɹ���·��Ϊ�� " << in.c_str()  << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
		string line;				//������ʱһ��
		
		getline(file, line);		//��ʼ��ȡ�ڵ���Ϣ������һ��
		stringstream stream(line);	//����һ����Ϣ���Կո�Ϊ�ָ���
		
		string tempNum;
		stream >> tempNum;			//��ȡ����ڵ�����
		
		//�ж��Ƿ�Ϊpajek��ʽ
		if (isNum(tempNum))
		{
			stream >> type;						//��ȡ��������
			nodeNum= atoi(tempNum.c_str());		//��������ڵ���������stringת��Ϊint�Ĺ���
			
			//��ʼ���ݽڵ��������ӽڵ�����
			for (int i = 1; i <= nodeNum; i++)
			{
				nodes.insert(nodes.end(), i);
			}

			//��ʼ��ȡ�����ļ���Ϣ��ѭ�����ڵ���������
			for (int i = 1; i <= nodeNum;i++)
			{
				getline(file, line);		//��ȡ��ʱһ��
				stringstream stream(line);	//����һ����Ϣ���Կո�Ϊ�ָ���
			
				int id;						//������ʱ�ڵ�id����Ϣ
				int adjNumOut;				//������ʱ�ڵ������Ϣ

				stream >> id;				//��ȡ�ڵ�id����Ϣ
				stream >> adjNumOut;		//��ȡ��ʱ�ڵ������Ϣ
			
				//����Ƿ����
				if (id != i)
				{
					error = 1;
					break;
				}
				//��ʼ�������ӣ�ѭ�����ھ���������
				for (int i = 0; i < adjNumOut;i++)
				{
					//��ȡ�ھӺ�Ȩ��
					int neighbor;
					double weight;
					stream >> neighbor;
					stream >> weight;

					//��ʼ������
					addLinkToNetwork(id, neighbor, weight);
					linkNum++;
				}

		}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
			cout << "��ͨ��ʽ�ļ���ȡ��ϣ�·��Ϊ�� " << in.c_str() << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
		}
		//�ж�Ϊpajek��ʽ�ɹ�
		else if(tempNum == "*vertices")
		{
			stream >> nodeNum; //��ȡ�ڵ����

			//��ʼ���ݽڵ��������ӽڵ�����
			for (int i = 1; i <= nodeNum; i++)
			{
				nodes.insert(nodes.end(), i);
			}

			//ѭ����*arcs���֣����жϸ�ʽ�Ƿ�Ϸ�
			for (int i = 1; i <= nodeNum; i++)
			{
				getline(file, line);		//��ȡ��ʱһ��
				stringstream stream(line);	//����һ����Ϣ���Կո�Ϊ�ָ���
				int tempNum;
				stream >> tempNum;
				if (tempNum != i)
				{
					error = 1;
					break;
				}
			}
			//�����û���������
			if (!error)
			{
				getline(file, line);		//��ȡ��ʱһ��
				stringstream stream(line);	//����һ����Ϣ���Կո�Ϊ�ָ���
				
				string tempNum;				//��ȡһ�£������ǲ���"*arcs"��
				stream >> tempNum;

				//�������˵����ʽ����
				if (tempNum == "*arcs")
				{
					//��ʼ��ȡ������Ϣ
					while (!file.eof())
					{
						getline(file, line);		//��ȡ��ʱһ��
						stringstream stream(line);	//����һ����Ϣ���Կո�Ϊ�ָ���

						if (file.fail()) {
							break;
						}

						int from, to;				//������ֹ�ڵ�
						double weight;				//����Ȩֵ��Ϣ
						stream >> from;
						stream >> to;
						stream >> weight;
						addLinkToNetwork(from, to, weight);
						linkNum++;

					}
				}
				else
				{
					error = 1;
				}



			}
			
			type = DIRECT_WITH_WEIGHT;		//Ĭ����������

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
			cout << "pajek��ʽ�ļ���ȡ��ϣ�·��Ϊ�� " << in.c_str() << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

		}
		//ʲô��ʽ�����ǣ����ش���
		else 
		{
			error = 1;
		}

		if (error)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "�ļ���ʽ����·��Ϊ�� " << in.c_str() << "�����Զ����������滻Ϊ �����硣" << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
			nodes.clear();		//��սڵ���Ϣ
			new (this) network(0, UN_DIRECT_WITH_WEIGHT);
		}
		
	}


}

//��ȡ�ļ����캯�����Զ�ʶ����ͨ��ʽ��pajek��ʽ����������
network::network(string in, int netid)
{

	//����Ĭ�϶�ȡ�ļ����캯��
	new (this) network(in);

	//��д������
	this->netid = netid;

}

/*
//�������캯��
//����������Ҫ����ϵͳĬ�ϵļ��ɣ�vector����Ҫ��
network::network(network &n2)
{
	nodes.assign(n2.nodes.begin(), n2.nodes.end());	//�����ڵ�����
	nodeNum=n2.nodeNum;								//�����ڵ�����
	linkNum=n2.linkNum;								//�����ߵ�����
	type=n2.type;									//������������
	netid = n2.netid;								//����������
}
*/

//��������
network::~network()
{
	//����������û��new��û��ʲô�����ͷŵ�
}

//��ʾ����
void network::showNetwork() 
{
	cout << "=====================================================" << endl;
	cout <<"�����ţ�"<<netid <<" ,���нڵ� "<<nodeNum<<" ��������  "<<linkNum<<" ����"<< endl;
	
	//�������нڵ�
	for (auto it:nodes)
	{
		if (it.status != NODE_DEAD)
		{
			it.showNode();
		}
	}
}

//�����������
int network::addLinkToNetwork(int from, int to //��from��to��������
	, double weight)   //�������ӵ�Ȩ��
{
	int error;

	//���from��to�Ƿ�Խ��
	if (from>nodeNum || from <= 0 || to>nodeNum || to <= 0)
	{
		error = BEYONDLIMIT;

		//�ı�������ɫΪ��
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "����һ�������޷�Ϊ ����"<<netid<<" ��Ӵ� " << from << " -->> " << to << " �����ӣ���ΪԽ���ˡ�" << endl;
		//�ı�������ɫΪ��ͨ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
		
		return error;
	}

	//��ӽڵ�����
	error = nodes[from-1].addLinkToNode(to, weight);	//��from��ӳ�����Ϣ
	error = nodes[to-1].addLinkFromNode(from, weight);//��to��������Ϣ

	//�����ӽڵ�����򷵻ش�����Ϣ
	if (error)
	{
		return error;
	}

	//��������������һ
	linkNum++;
	return 0;
}

//ɾ����������
int network::delLinkFromNetwork(int from, int to)//��from��toɾ������
{
	int error=0;
	
	//���from��to�Ƿ�Խ��
	if (from>nodeNum || from <= 0 || to>nodeNum || to <= 0)
	{
		error = BEYONDLIMIT;

		//�ı�������ɫΪ��
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "����һ������ ���޷�ɾ�� ���� "<<netid<<"�� " << from << " --> " << to << " �����ӣ���ΪԽ���ˡ�" << endl;
		//�ı�������ɫΪ��ͨ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
		
		return error;
	}
	
	if(isValidLink(from,to))
	{ 
	//ɾ���ڵ�֮�����ӣ���ע�⣺�����źͽڵ��Ų�1��
	error = nodes[from-1].delLinkFromNode(to);	//ɾ��from�ĳ�����Ϣ
	error = nodes[to-1].delLinkToNode(from);	//ɾ��to�������Ϣ
	}
	else
	{
		error = NOLINK;

		//�ı�������ɫΪ��
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "����һ������ ���޷�ɾ���� " << from << " --> " << to << " �����ӣ���Ϊû�����ӡ�" << endl;
		//�ı�������ɫΪ��ͨ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

		return error;
	}
	//���û�д�����������һ
	if (error == 0) 
	{
		linkNum--;
	}

	return error;
}

//Ϊ�������ӽڵ㣬ֻ���ӽڵ㣬����������
int network::addNodesToNetwork(int N)//Ϊ��������N���ڵ�
{
	if (N <= 0)
	{
		int error=1;
		//�ı�������ɫΪ��
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "����һ������ ���޷�Ϊ ���� "<<netid<<" ���� " << N << " ���ڵ㡣" << endl;
		//�ı�������ɫΪ��ͨ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

		return error;
	}
	//��ԭ�нڵ���nodeNum������N�����ܽ����ΪnodeNum + N
	for (int i = nodeNum; i < nodeNum + N; i++)
	{
		nodes.insert(nodes.end(), i+1);
	}

	//���ӽڵ���
	nodeNum += N;
	return 0;
}

//��������ɾ���ڵ�
int network::delNodeFromNetwork(int nodeid) //Ҫɾ���ڵ���
{
	int error = 0;

	//�ж��Ƿ��������������ϵ������У����޷�ɾ��
	if (nodes[nodeid-1].numAdjMultiIn!=0|| nodes[nodeid-1].numAdjMultiOut!=0)
	{
		error = BEYONDLIMIT;

		//�ı�������ɫΪ��
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "����һ������ ���޷�ɾ�� ����"<<netid<<" �� �ڵ�"<<nodeid<<" ���������ڶ��������� " << endl;
		//�ı�������ɫΪ��ͨ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

		return error;
	}

	//���nodeid�Ƿ�Խ��
	if (nodeid>nodeNum || nodeid <= 0)
	{
		error = BEYONDLIMIT;

		//�ı�������ɫΪ��
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "����һ������ ���޷�ɾ���ڵ� " << nodeid <<"��Ϊ��Խ����"<< endl;
		//�ı�������ɫΪ��ͨ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

		return error;
	}

	//ȷ����û��"DEAD"
	if (nodes[nodeid-1].status == NODE_DEAD)
	{
		error = REDUNDANTLINK;

		//�ı�������ɫΪ��
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "����һ������ ���޷��ٴ�ɾ����ɾ���Ľڵ� " << nodeid << endl;
		//�ı�������ɫΪ��ͨ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

		return error;
	}

	//map������������
	//map <int, double>::iterator it;

	//ɾ������ָ��nodeid�����ӣ���ɾ��nodeid���������
	for (auto it : nodes[nodeid-1].adjIn)
	{
		//����nodeid����ȱ����ÿһ���ڵ㣬�������й�nodeid��Ϣɾ��
		nodes[it.first-1].delLinkFromNode(nodeid);	
	}

	//�������д�nodeidָ��Ľڵ�������Ϣ����nodeid����ָ������
	for (auto it : nodes[nodeid-1].adjOut)
	{
		nodes[it.first-1].delLinkToNode(nodeid);
	}

	//�������������٣���ֵΪ��ɾ���ڵ���ھ�����
	linkNum -= nodes[nodeid-1].numAdjOut + nodes[nodeid-1].numAdjIn;

	//ɾ��������nodeid����ĳ��Ⱥ���ȵ�����
	nodes[nodeid-1].adjOut.clear();
	nodes[nodeid-1].adjIn.clear();

	//��������ڵ���
	nodeNum--;

	//���ڵ����Ը�дΪ"DEAD"
	nodes[nodeid-1].status = NODE_DEAD;
	return 0;
}

//��֤�Ƿ��д�from��to������
int network::isValidLink(int from, int to)
{

	//��֤�Ƿ���ͬ�������ͬ�ͷ���0
	if (from == to)
	{
		return 0;
	}

	//���form��to������Χ���򷵻�0
	if (from>nodeNum || from <= 0 || to>nodeNum || to <= 0)
	{
		return 0;
	}

	//�����ܲ����ҵ�
	auto it=nodes[from-1].adjOut.find(to);
	
	//����ҵ��˾ͷ���1
	if (it != nodes[from-1].adjOut.end())
	{
		return 1;
	}
	return 0;
}

//���ô�from��to��ȨֵΪweight
int network::setLinkWeight(int from, int to, double weight)
{
	//�������Խ�磬����ʾ������Ϣ
	if (from>nodeNum || from <= 0 || to>nodeNum || to <= 0)
	{
		int error = BEYONDLIMIT;
		//�ı�������ɫΪ��
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "����һ�������޷����� ����" <<netid<< " �� " << from << " -->> " << to << " ��Ȩֵ����ΪԽ�硣" << endl;
		//�ı�������ɫΪ��ͨ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
		return error;
	}

	//������from�ĳ����б�Ѱ�Ҵ�from��to������
	auto it = nodes[from-1].adjOut.find(to);
	if (it != nodes[from-1].adjOut.end())
	{
		it->second = weight;
	}
	else
	{
		int error = NOLINK;
		
		//�ı�������ɫΪ��
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "����һ�������޷����� ����"<<netid<<" �� " << from << " -->> " << to << " ��Ȩֵ����Ϊ������Ϣ�����ڡ�" << endl;
		//�ı�������ɫΪ��ͨ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

		return error;
	}

	//������to������б�Ѱ�Ҵ�from��to������
	it = nodes[to-1].adjIn.find(from);
	if (it != nodes[to-1].adjIn.end())
	{
		it->second = weight;
	}
	else
	{
		int error = NOLINK;

		//�ı�������ɫΪ��
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "����һ�������޷����� ����" << netid << " �� " << from << " -->> " << to << " ��Ȩֵ����Ϊ�����Ϣ�����ڡ�" << endl;
		//�ı�������ɫΪ��ͨ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

		return error;
	}
	return 0;
}

//��ȡ��from��to����������Ȩֵ
double network::getLinkWeight(int from, int to)
{
	//�������Խ�磬����ʾ������Ϣ
	if (from>nodeNum || from <= 0 || to>nodeNum || to <= 0)
	{
		int error = BEYONDLIMIT;
		//�ı�������ɫΪ��
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "����һ�������޷���ȡ�� ����" << netid << " �� " << from << " -->> " << to << " ��Ȩֵ����ΪԽ���ˡ�" << endl;
		//�ı�������ɫΪ��ͨ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
		return error;
	}

	auto it = nodes[from-1].adjOut.find(to);
	if (it != nodes[from-1].adjOut.end())
	{
		return it->second;
	}
	else
	{
		int error = NOLINK;

		//�ı�������ɫΪ��
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "����һ�������޷���ȡ�� ����" << netid << " �� " << from << " -->> " << to << " ��Ȩֵ����Ϊû�������ˡ�" << endl;
		//�ı�������ɫΪ��ͨ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

		return error;
	}
}

//����������Ϊ��ͨ�ļ���ʽ��֧���Զ�����·��(��ʽΪ XX/XX/XXX/XXX.txt)
void network::outputNetwork(string out) //�����
{
	if (out.empty())
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "·��������Ϊ�գ����Զ����ء�" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
		return;
	}

	//����Ƿ�Ϊ�༶Ŀ¼
	if (out.find('/') < out.length())
	{
		GetDirAndCreate(out);	//���Ŀ¼���� 
	}
	

	//����������
	ofstream  file(out,std::ios_base::ate);

	if (file.fail())
	{

		//�ı�������ɫΪ��
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "�����ļ�·������ " << out.c_str() <<" ������·���Ƿ���ȷ��"<< endl;
		//�ı�������ɫΪ��ͨ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

		return;
	}

	//�ı�������ɫΪ��
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "���ڴ�����ͨ��ʽ��������Ϣ�ļ��� " << out.c_str() <<  endl;
	//�ı�������ɫΪ��ͨ
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

	//��ʼд���ļ�
	file << nodeNum <<" "<< type << endl;

	for (auto it1:nodes)
	{
		file<<it1.id<<" "<< it1.numAdjOut<<" ";
		for (auto it2:it1.adjOut)
		{
			//weight�ᱣ����λС����
			file << it2.first << " " << setiosflags(ios::fixed) << setprecision(2) << it2.second << " ";
		}
		file << endl;
	}

}

//����������Ϊ��ͨpajek�ļ���ʽ��֧���Զ�����·��(��ʽΪ XX/XX/XXX/XXX.txt)
void network::outputNetwork_pajek(string out)
{
	if (out.empty())
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "·��������Ϊ�գ����Զ����ء�" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
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
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "�����ļ�·������ " << out.c_str() << " ������·���Ƿ���ȷ��" << endl;
		//�ı�������ɫΪ��ͨ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
		return;
	}
	//�ı�������ɫΪ��
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "���ڴ�����ͨPajek��ʽ��������Ϣ�ļ��� " << out.c_str() << endl;
	//�ı�������ɫΪ��ͨ
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

	//��ʼд���ļ�
	file << "*vertices "<< nodeNum<<" " << endl;
	for (auto it:nodes)
	{
		file << it.id<<" " << endl;
	}
	file << "*arcs " << endl;
	for (auto it1:nodes)
	{
		for (auto it2:it1.adjOut)
		{
			//weight������λС����
			file << it1.id << " " << it2.first << " " << setiosflags(ios::fixed) << setprecision(3) << it2.second<<endl;
		}
	}

}

/* �߼�pajek��ʽ��Ϣ���£�
output the information of a network to a *.net file, so it can be used as
an input file of pajek

format in the output file:
*vertices n                  //n is the number of vertices
1 "name"                   // "name"  the label of vertex 1
......
*arcs
i j v_{ij}                         // the arc from i to j has value v_{ij}
. . .
*edges
p q v_{pq}                         // the edge from p to q has value v_{pq}

*/

//����������Ϊ����ɫ��pajek�ļ���ʽ��֧���Զ�����·��(��ʽΪ XX/XX/XXX/XXX.txt)
void network::outputNetwork_pajek_frac(string out)
{
	if (out.empty())
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "·��������Ϊ�գ����Զ����ء�" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
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
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "�����ļ�·������ " << out.c_str() << " ������·���Ƿ���ȷ��" << endl;
		//�ı�������ɫΪ��ͨ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
		return;
	}
	//�ı�������ɫΪ��
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "���ڴ��߼�Pajek��ʽ��������Ϣ�ļ��� " << out.c_str() << endl;
	//�ı�������ɫΪ��ͨ
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

	//��ʼд���ļ�
	file << "*vertices " << nodeNum << " " << endl;
	for (auto it : nodes)
	{
		file << it.id << " " << endl;
	}
	
	file << "*arcs " << endl;

	for (auto it1 : nodes)
	{
		for (auto it2 : it1.adjOut)
		{
			//��ȡ�ھӱ��
			int neighbor = it2.first;

			//����ýڵ���� �� �ھӵĳ��� ��ͬ����black ������ gray
			if (it1.numAdjOut == nodes[neighbor-1].numAdjOut)
			{
				file << it1.id<<" "<< neighbor <<"  1 c black"<<endl;
			}
			else {
				file << it1.id << " " << neighbor << "  -1 c gray" << endl;
			}
		}
	}
}