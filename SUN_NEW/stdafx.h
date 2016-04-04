// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

//不注销则显示错误信息
//#define SHOWERR

#include <vector>
#include <map>
#include <iostream>
#include <fstream> 
#include <iomanip>
#include <string>
#include <sstream>

#ifdef _WIN32
#include "windows.h"
#endif

#include "other.h"
#include "network.h"
#include "node.h"
#include "multiNet.h"

using namespace std;

class multiNet;
class network;
class node;

// TODO:  在此处引用程序需要的其他头文件
