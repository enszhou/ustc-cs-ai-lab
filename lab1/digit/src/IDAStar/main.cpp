#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <time.h>
#include "IDAStar.h"

using namespace std;

unsigned char init_cells[5][5];
unsigned char dest_cells[5][5];
unsigned char init_state[23];
unsigned char dest_state[23];

void output_result(Node &dest_node, string outfile_path, int time_run)
{
	ofstream outfile(outfile_path);
	vector<int> path;
	Node *p;
	for (p = &dest_node; p->parent != NULL; p = p->parent)
	{
		path.emplace_back(p->action);
	}
	reverse(path.begin(), path.end());

	cout << "running time: " << time_run / (3600000) << "h " << time_run % 3600000 / 60000 << "m " << time_run % 60000 / 1000 << "s " << time_run % 1000 << "ms" << endl;
	cout << path.size() << " steps";
	// outfile << path.size() << " steps" << endl;

	for (auto step : path)
	{
		// cout << '(' << step / 4 << ',' << direcions[step % 4] << ')' << endl;
		outfile << '(' << step / 4 << ',' << direcions[step % 4] << ')' << endl;
	}
}

int main(int argc, char **argv)
{
	string init_infile_path, dest_infile_path, outfile_path;
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

	outfile_path = init_infile_path;
	string filename = init_infile_path.substr(init_infile_path.find_last_of('/') + 1);
	string filedir = init_infile_path.substr(0, init_infile_path.find_last_of('/'));
	outfile_path = filedir + "/../output/idastar_" + filename;
	// cout << outfile_path << endl;

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

	clock_t time_start = clock();
	IDAStar idastar_solver(init_state, dest_state);
	Node dest_node = idastar_solver.Solve();
	clock_t time_end = clock();
	int time_run = (int)(((double)(time_end - time_start)) / CLOCKS_PER_SEC * 1000);

	output_result(dest_node, outfile_path, time_run);

	return 0;
}