#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include "AStar.h"

using namespace std;

unsigned char init_cells[5][5];
unsigned char dest_cells[5][5];
unsigned char init_state[23];
unsigned char dest_state[23];

void test_output()
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			printf("%d ", init_cells[i][j]);
		}
		cout << endl;
	}
	cout << endl;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			printf("%d ", dest_cells[i][j]);
		}
		cout << endl;
	}
	cout << endl;
	for (int i = 0; i < 23; i++)
	{
		printf("%d ", dest_state[i]);
	}
}

void output_result(Node& dest_node)
{
	vector<int> path;
	Node* p;
	for (p = &dest_node; p->parent != NULL; p = p->parent)
	{
		path.emplace_back(p->action);
	}
	reverse(path.begin(), path.end());
	cout << path.size() << " steps" << endl;
	for (auto step : path)
	{
		cout << '(' << step / 4 << ',' << direcions[step % 4] << ')' << endl;
	}
}

int main(int argc, char** argv)
{
	string init_infile_path, dest_infile_path;
	if (argc < 3)
	{
	    cout << "Please specify the input file: ";
	    cin >> init_infile_path >> dest_infile_path;
	}
	else
	{
	    init_infile_path = argv[1];
	    dest_infile_path = argv[2];
	}

	// init_infile_path = "D:/2020Spring/AI/lab/lab1/digit/input/init3.txt";
	// dest_infile_path = "D:/2020Spring/AI/lab/lab1/digit/input/dest.txt";

	ifstream init_infile(init_infile_path);
	ifstream dest_infile(dest_infile_path);

	if (!init_infile || !dest_infile)
	{
		cout << "file doesn't exist";
		exit(1);
	}
	int temp;
	memset(init_state, 'a', 23);
	memset(dest_state, 'a', 23);

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			init_infile >> temp;
			init_cells[i][j] = temp;
			if (temp == 0 && init_state[0] != 'a')
				init_state[22] = i * 5 + j;
			else
				init_state[temp] = i * 5 + j;

			dest_infile >> temp;
			dest_cells[i][j] = temp;
			if (temp == 0 && dest_state[0] != 'a')
				dest_state[22] = i * 5 + j;
			else
				dest_state[temp] = i * 5 + j;

			init_infile.ignore(1, ',');
			dest_infile.ignore(1, ',');
		}
	}
	init_infile.close();
	dest_infile.close();

	AStar astar_solver(init_state, dest_state);
	Node dest_node = astar_solver.Solve();
	output_result(dest_node);

	// test_output();

   /* vector<Node> children;
	Node node = astar_solver.PopMinNode();
	astar_solver.Expand(node, children);
	for (auto child : children)
	{
		for (int i = 0; i < 23; i++)
		{
			printf("%d ", child.state[i]);
		}
		cout << endl;
	}*/
	return 0;
}