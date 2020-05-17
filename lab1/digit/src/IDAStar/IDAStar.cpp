#include "IDAStar.h"
#include <cstring>
#include <iostream>
#include <climits>

using namespace std;

IDAStar::IDAStar(unsigned char (&init_state)[23], unsigned char (&dest_state)[23])
{
	Node *init_node = new Node;
	memcpy(this->dest_state, dest_state, 23);
	memcpy(init_node->state, init_state, 23);
	init_node->action = 255;
	init_node->cost_g = 0;
	init_node->cost_f = init_node->cost_g + H(init_node->state);
	init_node->parent = NULL;
	fringe.emplace(init_node);
	all_nodes.emplace(init_node);
}

IDAStar::~IDAStar()
{
	FreeList();
}

void IDAStar::FreeList()
{
	Node *p;
	closed.clear();
	for (auto p : all_nodes)
		delete p;
	all_nodes.clear();
}

unsigned short IDAStar::Distance(int corr1, int corr2)
{
	return abs(corr1 / 5 - corr2 / 5) + abs(corr1 % 5 - corr2 % 5);
}

unsigned short IDAStar::H(unsigned char (&state)[23])
{
	int sum = 0;
	sum += 3 * Distance(dest_state[7], state[7]);

	for (int i = 1; i < 22; i++)
	{

		if (i == 7)
			continue;
		sum += Distance(dest_state[i], state[i]);
	}
	return sum;
}

void IDAStar::NormalState(unsigned char (&state)[23])
{
	int temp;
	if (state[0] > state[22])
	{
		temp = state[0];
		state[0] = state[22];
		state[22] = temp;
	}
}

Node IDAStar::PopMinNode()
{
	Node *p_min_node;
	Node min_node;
	while (true)
	{
		p_min_node = fringe.top();
		fringe.pop();

		if (p_min_node->parent != NULL)
		{
			p_min_node->parent->num_children_fringe--;
			if (p_min_node->parent->is_in_closed == false)
			{
				// if (p_min_node->parent->num_children_fringe == 0)
				// 	delete p_min_node->parent;
				// delete p_min_node;
				continue;
			}
		}

		auto search = closed.find(p_min_node);
		if (search != closed.end()) //exist
		{
			// delete p_min_node;
		}
		else
		{
			min_node = *p_min_node;
			// delete p_min_node;
			break;
		}
	}

	return min_node;
}

void IDAStar::Expand(Node &parent, vector<Node> &children)
{
	vector<pair<int, int>> sevens{{parent.state[7] / 5, parent.state[7] % 5},		  // right-down 0
								  {parent.state[7] / 5 - 1, parent.state[7] % 5 - 1}, // left-up 1
								  {parent.state[7] / 5 - 1, parent.state[7] % 5}};	  // center 2
	bool flag;
	//the two spaces
	int zero_row, zero_col, row, col, zero_corr, k;
	int spaces[2] = {0, 22};
	for (auto space : spaces)
	{
		zero_corr = parent.state[space]; // 0 or 22
		zero_row = zero_corr / 5;
		zero_col = zero_corr % 5;
		for (int i = 0; i < 4; i++)
		{
			row = zero_row + actions[i].first;
			col = zero_col + actions[i].second;
			if (row >= 0 && row <= 4 && col >= 0 && col <= 4 && parent.state[22 - space] != row * 5 + col) // exist and not the other zero
			{
				flag = true;
				for (auto seven : sevens)
				{
					if (seven.first == row && seven.second == col)
					{
						flag = false;
						break;
					}
				}
				if (flag) // not seven, not the other zero
				{
					Node child = parent;
					for (k = 0; k < 23; k++)
						if (child.state[k] == row * 5 + col)
							break;
					child.state[space] = row * 5 + col;
					child.state[k] = zero_corr;
					NormalState(child.state);
					child.action = 4 * k + i;
					child.cost_g = parent.cost_g + 1;
					child.cost_f = child.cost_g + H(child.state);
					child.parent = &parent;
					children.emplace_back(child);
				}
			}
		}
	}

	// sevens move

	int zero_larger_corr, zero_smaller_corr;
	if (parent.state[0] > parent.state[22])
	{
		zero_larger_corr = parent.state[0];
		zero_smaller_corr = parent.state[22];
	}
	else
	{
		zero_larger_corr = parent.state[22];
		zero_smaller_corr = parent.state[0];
	}

	Node child = parent;
	child.cost_g = parent.cost_g + 1;
	child.parent = &parent;

	flag = false;
	if (sevens[0].first == zero_larger_corr / 5 && sevens[0].second - 1 == zero_larger_corr % 5 && sevens[1].first == zero_smaller_corr / 5 && sevens[1].second - 1 == zero_smaller_corr % 5)
	{ // move left
		flag = true;
		child.state[7] = parent.state[7] - 1;
		child.state[0] = parent.state[7] - 5; // 2
		child.state[22] = parent.state[7];	  // 0
		child.action = 4 * 7 + 0;
	}
	else if (sevens[2].first - 1 == zero_larger_corr / 5 && sevens[2].second == zero_larger_corr % 5 && sevens[1].first - 1 == zero_smaller_corr / 5 && sevens[1].second == zero_smaller_corr % 5)
	{ // move up
		flag = true;
		child.state[7] = parent.state[7] - 5;
		child.state[0] = parent.state[7] - 6; // 1
		child.state[22] = parent.state[7];	  // 0
		child.action = 4 * 7 + 1;
	}
	else if (sevens[0].first == zero_larger_corr / 5 && sevens[0].second + 1 == zero_larger_corr % 5 && sevens[2].first == zero_smaller_corr / 5 && sevens[2].second + 1 == zero_smaller_corr % 5)
	{ // move right
		flag = true;
		child.state[7] = parent.state[7] + 1;
		child.state[0] = parent.state[7] - 6; // 1
		child.state[22] = parent.state[7];	  // 0
		child.action = 4 * 7 + 2;
	}
	else if (sevens[0].first + 1 == zero_larger_corr / 5 && sevens[0].second == zero_larger_corr % 5 && sevens[1].first + 1 == zero_smaller_corr / 5 && sevens[1].second == zero_smaller_corr % 5)
	{ // move down
		flag = true;
		child.state[7] = parent.state[7] + 5;
		child.state[0] = parent.state[7] - 6;  // 1
		child.state[22] = parent.state[7] - 5; // 2
		child.action = 4 * 7 + 3;
	}

	if (flag)
	{
		NormalState(child.state);
		child.cost_f = child.cost_g + H(child.state);
		children.emplace_back(child);
	}
}

void IDAStar::FreshClosed(vector<Node> &children)
{
	Node *p;
	for (auto child = children.begin(); child != children.end();)
	{
		auto search = closed.find(&(*child));
		if (search != closed.end())
		{
			p = *search;
			if (child->cost_g < p->cost_g)
			{
				closed.erase(search);
				DealRemovedParent(p);
			}
			else
			{
				child = children.erase(child);
				continue;
			}
		}
		child++;
	}
}

void IDAStar::DealRemovedParent(Node *p)
{
	// TODO
	//delete p;
	p->is_in_closed = false;
}

void IDAStar::FreshFringe(Node &parent, vector<Node> &children)
{
	Node *new_node;
	int n = children.size();
	parent.num_children_fringe = n;
	parent.is_in_closed = true;
	for (int i = 0; i < n; i++)
	{
		new_node = new Node;
		*new_node = children[i];
		fringe.emplace(new_node);
		all_nodes.emplace(new_node);
	}
	return;
}

bool IDAStar::isStateEqual(unsigned char (&state1)[23], unsigned char (&state2)[23])
{
	for (int j = 0; j < 23; j++)
	{
		if (state1[j] != state2[j])
			return false;
	}
	return true;
}


Node IDAStar::Solve()
{
	Node *new_min_node;
	Node init_node, min_node;
	int f_limit, next_f_limit;

	init_node = PopMinNode();
	f_limit = init_node.cost_f;
	while (f_limit < INT_MAX)
	{
		cout << f_limit << endl;
		next_f_limit = INT_MAX;
		new_min_node = new Node;
		*new_min_node = init_node;
		fringe.emplace(new_min_node);
		all_nodes.emplace(new_min_node);
		while (!fringe.empty())
		{
			min_node = PopMinNode();
			if (min_node.cost_f > f_limit)
			{
				next_f_limit = next_f_limit > min_node.cost_f ? min_node.cost_f : next_f_limit;
			}
			else
			{
				if (isStateEqual(min_node.state, dest_state))
					return min_node;
				vector<Node> children;
				new_min_node = new Node;
				*new_min_node = min_node;
				closed.emplace(new_min_node);
				all_nodes.emplace(new_min_node);
				Expand(*new_min_node, children);
				FreshClosed(children);
				FreshFringe(*new_min_node, children);
			}
		}
		FreeList();
		f_limit = next_f_limit;
	}
	return init_node;
}