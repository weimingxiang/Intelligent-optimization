#include <bits/stdc++.h>

using namespace std;

int node_size = 31; //城市的个数

typedef struct Node
{
	int x;
	int y;
	double operator-(const Node& node) //计算两点之间的距离
	{
		int xx = x - node.x;
		int yy = y - node.y;
		return sqrt(pow(xx, 2) + pow(yy, 2));
	}
}Node;

Node* node;

double Distance(char* path) //计算一个路径的总长度
{
	double distance = 0;
	for (int i = 0; i < node_size; i++)
	{
		distance += node[path[i]] - node[path[(i + 1) % 31]];
	}
	return distance;
}

char* Change_path(char* full_path_best, int x, int y) // 交换两个点并返回交换后的路径
{
	char *full_path_one_near = new char[node_size];
	memcpy(full_path_one_near, full_path_best, node_size * sizeof(char));
	char temp = full_path_one_near[x];
	full_path_one_near[x] = full_path_one_near[y];
	full_path_one_near[y] = temp;
	return full_path_one_near;
}

int main()
{
	int point[] = { 1304, 2312, 3639, 1315, 4177, 2244, 3712, 1399, 3488, 1535, 3326, 1556, 3238, 1229, 4196, 1004, 4312, 790, 4386, 570, 3007, 1970, 2562, 1756, 2788, 1491, 2381, 1676, 1332, 695, 3715, 1678, 3918, 2179, 4061, 2370, 3780, 2212, 3676, 2578, 4029, 2838, 4263, 2931, 3429, 1908, 3507, 2367, 3394, 2643, 3439, 3201, 2935, 3240, 3140, 3550, 2545, 2357, 2778, 2826, 2370, 2975 }; // 城市的坐标

	node = (Node*)point;
	unsigned seed;

	int tabu_length = 6;  //禁忌长度

	

	char** full_path_near = new char* [node_size * node_size];
	double dis_near;

	/*
	for (int i = 0; i < (node_size - 1) * (node_size - 2); i++)
	{
		full_path_near[i] = new char[node_size];
	}
	*/

	char* full_path_best = new char[node_size]; double dis_best = 0;
	char* full_path_near_best;
	int count = 0;



	//char full_path[] = { 0, 28, 30, 29, 26, 27, 25, 24, 23, 19, 20, 21, 17, 2, 16, 18, 22, 10, 5, 4, 15, 3, 1, 7, 8, 9, 6, 12, 11, 13, 14};
	//memcpy(full_path_best, full_path, node_size * sizeof(char));
	
	vector<char> full_path(node_size);

	for (int i = 0; i < node_size; i++)
	{
		full_path[i] = i;
	}
	
	while (full_path.size()) //path create 初始化一个路径
	{
		seed = time(NULL);
		srand(seed);
		int index = rand() % full_path.size();
		full_path_best[count++] = full_path[index];
		full_path.erase(full_path.begin() + index);
	}
	
	

	dis_best = Distance(full_path_best); // 计算初始化路径的总长度



	/*
	tabu_table 
	   2   3   4
	1  x   x   x
	2      x   x
	3          x
	4
	*/

	int* tabu_table = new int[node_size * node_size]; //创建禁忌表
	memset(tabu_table, 0, node_size * node_size * sizeof(int));
	

	int i = 0; //答案未变次数 
	char* full_path_best_temp = new char[node_size]; //临时存储最好路径
	memcpy(full_path_best_temp, full_path_best, node_size * sizeof(char)); 
	while (i != 20) { //答案未变次数超过一定限度后停止循环
		i++;
		double min_dis = 1000000;
		seed = time(NULL);
		srand(seed);
		int random = rand() % (2 * node_size - 2); //随机特赦
		int tabu_x, tabu_y;
		for (int x = 0; x < node_size; x++) //计算领域中的路径
		{
			for (int y = 0; y < node_size; y++)
			{
				if (x < y && (tabu_table[x * node_size + y] == 0 || x + y == random))
				{
					full_path_near[x * node_size + y] = Change_path(full_path_best_temp, x, y);
					dis_near = Distance(full_path_near[x * node_size + y]);
					if (min_dis > dis_near)
					{
						min_dis = dis_near;
						full_path_near_best = full_path_near[x * node_size + y];
						tabu_x = x;
						tabu_y = y;
					}

				}
			}
		}

		
		memcpy(full_path_best_temp, full_path_near_best, node_size * sizeof(char)); // 挑选领域中的最短路径

		if (dis_best > min_dis) // 如是最短路径，保存最短路径
		{
			i = 0;
			dis_best = min_dis;
			tabu_table[tabu_x * node_size + tabu_y] = tabu_length;
			memcpy(full_path_best, full_path_best_temp, node_size * sizeof(char));
		}

		for (int x = 0; x < node_size; x++) // 更新禁忌表
		{
			for (int y = 0; y < node_size; y++)
			{
				if (x < y)
				{
					if (tabu_table[x * node_size + y] == 0 || x + y == random) delete[] full_path_near[x * node_size + y];
					if (tabu_table[x * node_size + y] > 0) tabu_table[x * node_size + y]--;
				}
			}
		}

		for (int j = 0; j < node_size; j++) //输出路径及长度
		{
			cout << (int)(full_path_best[j]) << "\t";
		}
		cout << endl << dis_best << endl;
	}

	return 0;
}