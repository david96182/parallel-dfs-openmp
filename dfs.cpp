#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <queue>
#include <sstream>
#include <omp.h>
#include <fstream>
#include <vector>
#include <chrono>
using namespace std;
vector<int> output;
vector<bool> visited;
vector<vector<int>> graph;
int threads_num, from, to;
bool stop = false;
bool result;

//Initilize graph
void s_init()
{
	int res, nodesNum, node1, node2;
	FILE *fin = fopen("./graph/graph2.in", "r");
	fscanf(fin, "%d", &nodesNum);

	graph.resize(nodesNum + 1);

	while (fscanf(fin, "%d%d", &node1, &node2) != EOF)
	{
		bool checkNode = node1 < 0 || node2 < 0 || node1 > nodesNum || node2 > nodesNum;

		graph[node1].push_back(node2);
		graph[node2].push_back(node1);
	}
	res = fclose(fin);
}

// Serial DFS
bool s_dfs(int v, int dest)
{
	visited[v] = true;
	output.push_back(v);
	if (v != dest)
	{
		for (int i = 0; i < graph[v].size(); ++i)
		{
			if (!visited[graph[v][i]])
			{
				bool stop = s_dfs(graph[v][i], dest);
				if (stop)
				{
					return true;
				}
			}
		}
	}
	else
	{
		return true;
	}
	return false;
}

// Parallel DFS
bool p_dfs(int v, int dest)
{
#pragma omp critical
	visited[v] = true;
	output.push_back(v);

	if (v != dest)
	{
		for (int i = 0; i < graph[v].size(); ++i)
		{
			if (!visited[graph[v][i]])
			{
#pragma omp task
				stop = p_dfs(graph[v][i], dest);
#pragma omp taskwait
				if (stop)
				{
					return true;
				}
			}
		}
	}
	else
	{
		return true;
	}
	return false;
}

void setVisitedFalse()
{
	for (int i = 0; i < graph.size() - 1; ++i)
	{
		visited.push_back(false);
	}
}

void displayResult()
{
	for (unsigned int j = 0; j < output.size(); ++j)
	{
		cout << output[j] << " ";
	}
	cout << "\n";
}

int main(int argc, char const *argv[])
{
	threads_num = atoi(argv[1]);
	from = atoi(argv[2]);
	to = atoi(argv[3]);

	if( !to ){
		cout << "a";
	}

	// initialize the graph
	s_init();
	setVisitedFalse();

	//check input parameters
	if ( from >= graph.size() - 1 || from < 0 ) {
		cout << "From must be greater than or equal to 0 and smaller than " << graph.size() - 1 << "\n";
		return 1;
	} else if(to >= graph.size() - 1 || to < 0 ) {
		cout << "To must be greater than or equal 0 and smaller than " << graph.size() - 1 << "\n";
		return 1;
	}

	/* Serial DFS */
	auto start = chrono::steady_clock::now();

	result = s_dfs(from, to);
	auto end = chrono::steady_clock::now();
	auto dur = chrono::duration_cast<chrono::microseconds>(end - start).count();

	// Display results
	cout << "\nResult of Serial DFS: \n";
	if (result)
	{
		displayResult();
		cout << "Serial DFS time: " << dur << "ms\n";
	}
	else
	{
		cout << "It was not possible to find a path between the given nodes\n";
	}

	cout << string(80, '-');
	cout << string(80, '-');

	// Clean visited nodes after serial DFS
	for (int i = 0; i < visited.size(); ++i)
	{
		visited[i] = false;
	}

	//Clear result of Serial DFS
	output.clear();

	/* Perform Parallel DFS */
	omp_set_num_threads(threads_num);
	auto pstart = chrono::steady_clock::now();

#pragma omp parallel
	{
#pragma omp single
		{
			result = p_dfs(from, to);
		}
	}

	auto pend = chrono::steady_clock::now();
	auto pdur = chrono::duration_cast<chrono::microseconds>(pend - pstart).count();
	cout << "\nResult of Parallel DFS: \n";
	if (result)
	{
		displayResult();
		cout << "Parallel DFS time: " << pdur << "ms" << endl;
	}
	else
	{
		cout << "It was not possible to find a path between the given nodes\n";
	}

	return 0;
}
