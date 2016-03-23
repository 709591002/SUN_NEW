#include "node.h"

#pragma region �ڵ�

node::node()
{

}

//�ڵ��๹�캯��
node::node(int id)
{
	adjOut.clear();			//��ʼ�������ڽ�����
	numAdjOut = 0;			//��ʼ�������ھ�����
	adjIn.clear();			//��ʼ������ڽ�����
	numAdjIn = 0;			//��ʼ������ھ�����

	adjMultiOut.clear();	//��ʼ������������ڽӽڵ���Ϣ
	numAdjMultiOut = 0;		//��ʼ������������ھӽڵ����
	adjMultiIn.clear();		//��ʼ������������ڽӽڵ���Ϣ
	numAdjMultiIn = 0;		//��ʼ������������ھӽڵ����
	strength = 0.0;			//��ʼ���ڵ�Ȩ��
	this->id = id;			//��ʼ���ڵ�ID��
}

//�ڵ�����������
node::~node()
{
	adjOut.clear();		//����ڽ�����
}

//��ӡ��Ϣ
void node::showNode()
{
	cout << "---------------------------------------" << endl;
	//��ӡ�ýڵ������Ϣ

	//��ӡ�ڵ������Ϣ
	cout << "�ڵ��ţ�" << id << " ,�ڵ�Ȩ�أ�" << strength << endl;

	//��ӡ����
	if (numAdjOut != 0)
	{
		cout << endl;
		//�ı�������ɫΪ��
		CCol(0, 2);
		//map������������
		//map <int, double>::iterator it;
		cout << " " << numAdjOut << " �����ȣ�" << endl;
		for (auto it : adjOut)
			cout << id << "  -->>  " << it.first << " ,Ȩ�أ�" << it.second << endl;
	}

	//��ӡ���
	if (numAdjIn != 0)
	{
		cout << endl;
		//�ı�������ɫΪ��
		CCol(0, 1);
		//map������������
		//map <int, double>::iterator it;

		cout << " " << numAdjIn << " ����ȣ�" << endl;
		for (auto it : adjIn)
			cout << id << "  <<--  " << it.first << " ,Ȩ�أ�" << it.second << endl;
	}

	//��ӡ���������
	if (numAdjMultiOut != 0)
	{
		cout << endl;
		//�ı�������ɫΪ��
		CCol(1, 2);
		//map������������
		//map <int, double>::iterator it;
		cout << " " << numAdjMultiOut << " ����������ȣ�" << endl;
		for (auto it : adjMultiOut)
			for (auto it2 : it.second)
				cout << "  -->>  " << " ���� " << it.first << " �� " << it2.first << " ,Ȩ�أ�" << it2.second << endl;
	}

	//��ӡ���������
	if (numAdjMultiIn != 0)
	{
		cout << endl;
		//�ı�������ɫΪ��
		CCol(1, 2);
		//map������������
		//map <int, double>::iterator it;

		cout << " " << numAdjMultiIn << " ����������ȣ�" << endl;
		for (auto it : adjMultiIn)
			for (auto it2 : it.second)
				cout << "  <<--  " << " ���� " << it.first << " �� " << it2.first << " ,Ȩ�أ�" << it2.second << endl;
	}

	cout << endl;
	//�ı�������ɫΪ��ͨ
	CCol(0, 0);

}

//���ӵ�nodeid�ڵ������
int node::addLinkToNode(int nodeid //Ϊ��ǰ�ڵ�����һ����nodeid������ 
	, double weight)//�����ӵ�Ȩ��
{
	if (nodeid == id)
	{
		int error = LINKITSELF;

		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ�������ýڵ� " << nodeid << " ���ӵ��Լ��ˣ�" << endl;
		//����Ĭ������
		CCol(0, 0);

		return error;
	}

	map<int, double>::iterator it;

	//����Ƿ��Ƕ��������
	it = adjOut.find(nodeid);
	if (it != adjOut.end())
	{
		int error = REDUNDANTLINK;

		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ������" << id << " -->> " << nodeid << " �������Ѿ�������" << endl;
		//����Ĭ������
		CCol(0, 0);

		return error;
	}



	//Ϊ������������ھӽڵ���Ϣ��ֻ�������ȣ���Ȳ�����networkʵ��
	//adjOut.insert(map<int, double>::value_type(nodeid,weight));
	adjOut.insert(make_pair(nodeid, weight));

	//�ھ���������һ
	numAdjOut++;

	return 0;
}

//ɾ���Ӹýڵ㵽nodeid�����ӣ���ɾ������
int node::delLinkFromNode(int nodeid)
{
	//����Ƿ���ɾ������
	if (nodeid == id)
	{
		int error = LINKITSELF;

		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ�����󣬲���ɾ���ڵ�����" << endl;
		//����Ĭ������
		CCol(0, 0);

		return error;
	}

	map <int, double>::iterator it;

	//����Ƿ�������

	it = adjOut.find(nodeid);
	if (it != adjOut.end())
	{
		adjOut.erase(it);	//ɾ��Ԫ��
		numAdjOut--;		//�ڽ���������1
	}
	else
	{
		int error = NOLINK;

		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ������," << id << " -->> " << nodeid << " �����Ӹ��������ڣ�" << endl;
		//����Ĭ������
		CCol(0, 0);

		return error;
	}

	return 0;

}

//���Ӵ�nodeid��ȵ�������Ϣ
int node::addLinkFromNode(int nodeid		//�ӱ��Ϊnodeid�Ľڵ����
	, double weight)	//�����ӵ�Ȩ��
{
	//��������Ƿ�ָ���Լ�
	if (nodeid == id)
	{
		int error = LINKITSELF;

		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ�������ýڵ� " << nodeid << " ���Լ�ָ���ˣ�" << endl;
		//����Ĭ������
		CCol(0, 0);

		return error;
	}

	map <int, double>::iterator it;
	it = adjIn.find(nodeid);
	if (it != adjIn.end())
	{
		int error = REDUNDANTLINK;

		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ������" << id << " <<-- " << nodeid << " ����������Ѿ�������" << endl;
		//����Ĭ������
		CCol(0, 0);

		return error;
	}

	adjIn.insert(make_pair(nodeid, weight));
	numAdjIn++;
	return 0;
}

//ɾ����nodeid���ýڵ�����ӣ���ɾ�����
int node::delLinkToNode(int nodeid)
{
	//����Ƿ���ɾ������
	if (nodeid == id)
	{
		int error = LINKITSELF;

		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ�����󣬲���ɾ���ڵ�����" << endl;
		//����Ĭ������
		CCol(0, 0);

		return error;
	}

	map <int, double>::iterator it;

	//����Ƿ�������

	it = adjIn.find(nodeid);
	if (it != adjIn.end())
	{
		adjIn.erase(it);	//ɾ��Ԫ��
		numAdjIn--;		//�ڽ���������1
	}
	else
	{
		int error = NOLINK;

		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ������,�޷�ɾ�� " << id << " -->> " << nodeid << " �����Ӳ����ڡ�" << endl;

		//����Ĭ������
		CCol(0, 0);

		return error;
	}

	return 0;

}

//���ӵ�netid�����nodeid�ڵ�Ķ������������ӣ������Ӷ��������
int node::addLinkToNodeMulti(int netid			//Ŀ������ID
	, int nodeid, double weight)	//Ϊ��ǰ�ڵ�����һ����nodeid�Ķ������������ӣ�ȨֵΪweight������ 
{
	//����Ƿ���ָ��netid��������ݣ����û������
	auto it1 = adjMultiOut.find(netid);
	if (it1 == adjMultiOut.end())
	{
		map<int, double> temp;						//��ʱ��map����
		adjMultiOut.insert(make_pair(netid, temp));	//��ʼ��ָ��netid�ļ�¼
	}

	//���ָ��netid�����nodeid�ڵ�������Ƿ����

	auto it = adjMultiOut[netid].find(nodeid);
	if (it != adjMultiOut[netid].end())
	{
		int error = REDUNDANTLINK;

		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ�����󣬱��ڵ� -->> ���� " << netid << " �Ľڵ� " << nodeid << " �������Ѿ�������" << endl;
		//����Ĭ������
		CCol(0, 0);

		return error;
	}
	else
	{
		//��������ڣ�����Ӽ�¼
		adjMultiOut[netid].insert(make_pair(nodeid, weight));
		numAdjMultiOut++;
	}
	return 0;

}

//ɾ����netid�����nodeid�ڵ�Ķ������������ӣ���ɾ�����������
int node::delLinkToNodeMulti(int netid				//Ŀ������ID
	, int nodeid)					//ɾ����ǰ�ڵ㵽nodeid�Ķ������������ӣ���ɾ������
{
	//����Ƿ���ָ��netid��������ݣ����û���򷵻ش���
	auto it1 = adjMultiOut.find(netid);
	if (it1 == adjMultiOut.end())
	{
		int error = NOLINK;

		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ������,���ڵ� -->> ���� " << netid << " �Ľڵ� " << nodeid << " �����Ӳ����ڣ��޷�ɾ����" << endl;
		//����Ĭ������
		CCol(0, 0);

		return error;
	}

	//���ָ��netid�����nodeid�ڵ�������Ƿ���ڣ����������޷�ɾ�������ش���

	auto it = adjMultiOut[netid].find(nodeid);
	if (it != adjMultiOut[netid].end())
	{
		//����ҵ���¼����ɾ����¼
		adjMultiOut[netid].erase(it);
		numAdjMultiOut--;
	}
	else
	{
		int error = NOLINK;

		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ������,���ڵ� -->> ���� " << netid << " �Ľڵ� " << nodeid << " �����Ӳ����ڣ��޷�ɾ����" << endl;
		//����Ĭ������
		CCol(0, 0);

		return error;
	}
	return 0;
}

//���Ӵ�netid�����nodeid�ڵ㵽��ǰ�ڵ�Ķ������������ӣ������Ӷ��������
int node::addLinkFromNodeMulti(int netid			//Ŀ������ID
	, int nodeid, double weight)	//����һ����nodeid����ǰ�ڵ�Ķ������������ӣ�ȨֵΪweight����� 
{
	//����Ƿ��д�netid�����������ݣ����û���򷵻ش���
	auto it1 = adjMultiIn.find(netid);
	if (it1 == adjMultiIn.end())
	{
		map<int, double> temp;						//��ʱ��map����
		adjMultiIn.insert(make_pair(netid, temp));	//��ʼ��ָ��netid�ļ�¼
	}
	//����netid�����nodeid�ڵ����������Ƿ���ڣ���������򷵻ش���
	auto it = adjMultiIn[netid].find(nodeid);
	if (it != adjMultiIn[netid].end())
	{
		int error = REDUNDANTLINK;

		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ������ ���ڵ� <<-- ���� " << netid << " �Ľڵ� " << nodeid << " �������Ѿ�������" << endl;
		//����Ĭ������
		CCol(0, 0);

		return error;
	}
	else
	{
		//��������ڣ�����Ӽ�¼
		adjMultiIn[netid].insert(make_pair(nodeid, weight));
		numAdjMultiIn++;
	}
	return 0;
}

//ɾ����netid�����nodeid�ڵ㵽��ǰ�ڵ�Ķ������������ӣ���ɾ�����������
int node::delLinkFromNodeMulti(int netid			//Ŀ������ID
	, int nodeid)			//ɾ����nodeid����ǰ�ڵ�Ķ������������ӣ���ɾ�����
{
	//����Ƿ��д�netid�����������ݣ����û���򷵻ش���
	auto it1 = adjMultiIn.find(netid);
	if (it1 == adjMultiIn.end())
	{
		int error = NOLINK;

		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ������,���ڵ� <<-- ���� " << netid << " �Ľڵ� " << nodeid << " �����Ӳ����ڣ��޷�ɾ����" << endl;
		//����Ĭ������
		CCol(0, 0);

		return error;
	}
	//����netid�����nodeid�ڵ����������Ƿ���ڣ��������򷵻ش���
	auto it = adjMultiIn[netid].find(nodeid);
	if (it != adjMultiIn[netid].end())
	{
		//����ҵ���¼����ɾ����¼
		adjMultiIn[netid].erase(it);
		numAdjMultiIn--;
	}
	else
	{
		int error = NOLINK;

		//�ı�������ɫΪ��
		CCol(0, 2);
		cout << "����һ������,���ڵ� <<-- ���� " << netid << " �Ľڵ� " << nodeid << " �����Ӳ����ڣ��޷�ɾ����" << endl;
		//����Ĭ������
		CCol(0, 0);

		return error;
	}
	return 0;
}

#pragma endregion