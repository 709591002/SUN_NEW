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


baModel::baModel(int m0,int m,int N):erModel( m0, p)
{
	int i, j, l, x1;

	int error;
	int newly;

	int trueN;
	double p;
	int old;

	vector<int> array;
	array.resize(sizeof(int)*(int)(N*m*2.1) + 1);

	trueN = 0;
	// 1) start with a small number(m0) nodes
	/* or start with a few nodes and links*/
	// a fully connected network
	p = 1;
	

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
