#pragma once

#include "stdafx.h"

using namespace std;


class erModel :public network
{
public:

	double p;
	erModel(int nodeNum, double p);
};


class baModel :public erModel
{
public:

	baModel(int m0, int m, int N);

};

void uniformly_random_int(int * v, int begin, int end); //model会用到的随机算法
