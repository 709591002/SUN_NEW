#include "model.h"


erModel::erModel(int nodeNum, double p):network(nodeNum,1)
{
	int i, j, n;
	double w;

	n = nodeNum;
	for (i = 1; i <= n; i++)
	{
		for (j = 1; j <= n; j++)
		{
			w = (double)rand() / (1.0*RAND_MAX);
			if (w>(1.0 - p))
			{
				addLinkToNetwork(i,j,1.0);
				addLinkToNetwork(j,i,1.0);
			}
		}
	}
}


baModel::baModel(int m0,int m,int N):erModel( m0, 1)
{
	int i, j, l, x1;

	int error;
	int newly;

	int trueN;
	int old;

	vector<int> array;
	array.resize(sizeof(int)*(int)(N*m*2.1) + 1);

	trueN = 0;
	// 1) start with a small number(m0) nodes
	/* or start with a few nodes and links*/
	// a fully connected network
	

	for (i = 1; i <= nodeNum; i++)
	{
		if (nodes[i].numAdjOut == 0)
		{
			array[trueN + 1] = i;
			trueN = trueN + 1;
		}
		else {
			for (j = 1; j <= nodes[i].numAdjOut; j++)
			{
				array[trueN + 1] = i;
				trueN = trueN + 1;
			}
		}
	}

	// 2) growth 
	for (l = 1; l <= N - m0; l++)
	{
		addNodeToNetwork(1);
		newly = nodeNum;
		for (i = 1; i <= m; i++)
		{
			while (1)
			{
				uniformly_random_int(&x1, 1, trueN);
				old = array[x1];
				error = addLinkToNetwork(newly, old, 1.0);
				error = addLinkToNetwork(old, newly, 1.0);
				if (error == 0)
				{
					array[trueN + 1] = old;
					array[trueN + 2] = newly;
					trueN = trueN + 2;
					break;
				}
			}
		}
	}
}

fitnessModel::fitnessModel(int nodeNum ,int linkNum ,double gamma):network(nodeNum,1)
{
	int x;
	double w;
	double alpha;
	int error1, error2;
	srand((unsigned)time(0));

	//算出alpha值
	alpha = 1 / (gamma - 1);

	//printf("alpha是%f \n", alpha);

	int i, n;
	n = nodeNum;

	//数组存储节点weight
	vector<double> a;

	//初始化0位
	a.insert(a.end(),0);

	//存储每个节点的weight值
	double sump = 0;
	for (i = 1; i <= n; i++)
	{
		a.insert(a.end(), pow(i, alpha*(-1.0)));
		sump += a[i];
	}

	double sumtemp = 0.0;


	//标准化
	for (i = 1; i <= n; i++)
	{
		a[i] /= sump;
	}

	//序列化
	for (i = 1; i <= n; i++)
	{
		if (i == 1)
		{
			a[i] = a[i];
		}
		else
		{
			a[i] = a[i] + a[i - 1];
		}
	}

	//开始加边
	int Num = linkNum;

	while (Num > 0)
	{
		int i, j;

		w = (double)rand() / (a[n] * RAND_MAX);
		x = -999;
		for (int k = 1; k <= n; k++)
		{
			if (w <= a[k] && w > a[k - 1])
			{
				x = k;
				break;
			}
		}

		if (x > 0)
		{
			i = x;
		}
		else
		{
			continue;
		}


		w = (double)rand() / (a[n] * RAND_MAX);

		for (int k = 1; k <= n; k++)
		{
			if (w <= a[k] && w > a[k - 1])
			{
				x = k;
				break;
			}
		}


		if (x > 0)
		{
			j = x;
		}
		else
		{
			continue;
		}



		if (i != j)
		{
			error1 = addLinkToNetwork(i, j, 1.0);
			//error2 = addLinkToNetwork(j, i, 1.0); //因为要单向加边，故注销

			//如果没错误，则继续
			//if (error1 != REDUNDANTLINK && error2 != REDUNDANTLINK) //因为要单向加边，故注销
			if (error1 != REDUNDANTLINK)
			{
			Num--;	
			}
		}
	}

}

void
uniformly_random_int(int *v, /*output: return one random value, that is uniformly
							 distributed  between [begin, end]*/
	int begin,
	int end)
{
	double w;
	w = (double)rand() / (1.0*RAND_MAX);
	if (w == 1.0)
	{
		(*v) = end;
	}
	else {
		(*v) = (int)(w*(end - begin + 1) + begin);
	}
}
