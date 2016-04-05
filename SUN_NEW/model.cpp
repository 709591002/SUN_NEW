#include "model.h"
#include <math.h>
#include <algorithm>

//er模型
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

//BA模型
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


//fitness网络，无标度网络
fitnessModel::fitnessModel(int nodeNum ,int linkNum ,double gamma):network(nodeNum,1)
{
	double w;
	double alpha;
	int error1;
	srand((unsigned)time(0));

	//算出alpha值
	alpha = 1 / (gamma - 1);

	int i, n;
	n = nodeNum;

	//数组存储节点weight
	map<int,double> a;

	//存储每个节点的weight值
	double sump = 0;
	for (i = 1; i <= n; i++)
	{
		a.insert(make_pair(i, pow(i, alpha*(-1.0))));
		sump += a[i];
	}
    
	//标准化
    for(auto it:a)
    {
        a[it.first]/=sump;
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
        
        //生成第一个随机数
		w = (double)rand() / (a[n] * RAND_MAX);
		
        //寻找w的位置，k则是节点数
        auto it = find_if(a.begin(), a.end(),
                          [w](const pair<int, double> & x) -> bool
                          { return x.second > w; }
                          );
        
        
        //看看x是否找到了，没找到则从头开始
        if(it!=a.end()&&w>a[it->first-1])
        {
            i=it->first;
        }
        else
        {
            continue;
        }

        //生成第二个随机数
        w = (double)rand() / (a[n] * RAND_MAX);


        //寻找w的位置，k则是节点数
        it = find_if(a.begin(), a.end(),
                     [w](const pair<int, double> & x) -> bool
                     { return x.second > w; }
                     );

        
        //看看j是否找到了，没找到则从头开始
        if(it!=a.end()&&w>a[it->first-1])
        {
            j=it->first;
        }
        else
        {
            continue;
        }
         
		if (i != j)
		{
			error1 = addLinkToNetwork(i, j, 1.0);

			//如果没错误，则继续
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
