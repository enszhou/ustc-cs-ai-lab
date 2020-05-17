#include <queue>
#include <vector>
#include <unordered_set>
#include <set>
#include <string>
#include <utility>

const std::vector<std::pair<int, int>> actions{ {0, 1}, {1, 0}, {0, -1}, {-1, 0} };
const char direcions[4] = { 'l', 'u', 'r', 'd' };

typedef struct node //32 Bytes
{
	unsigned char state[23]; //state[0], state[22] is 0's location, state[7] is the right-down 7's location
	unsigned char action;    // 4 * num + direcion
	unsigned char num_children_fringe;
	bool is_in_closed;
	unsigned short cost_f;
	unsigned short cost_g;
	struct node* parent;
	//struct node* child;
	//struct node* sibling;
} Node;

struct cmp_gt
{
	bool operator()(Node const* a, Node const* b) const
	{
		return a->cost_f > b->cost_f; //min heap or descending order
	}
};

struct myhash
{
public:
	std::size_t operator()(Node const* node) const
	{
		std::string s((char*)(node->state), 23);
		return std::hash<std::string>()(s);
	}
};

struct myeuqal
{
	bool operator()(Node const* lhs, Node const* rhs) const
	{
		for (int j = 0; j < 23; j++)
		{
			if (lhs->state[j] != rhs->state[j])
				return false;
		}
		return true;
	}
};

class AStar
{
private:
	unsigned char dest_state[23];
	std::priority_queue<Node*, std::vector<Node*>, cmp_gt> fringe;
	// std::set<Node, cmp_gt> fringe; //descending order
	std::unordered_set<Node*, myhash, myeuqal> closed;
	// std::unordered_set<Node *, myhash, myeuqal> hash_fringe;

public:
	AStar(unsigned char(&init_state)[23], unsigned char(&dest_state)[23]);
	~AStar();
	// int F(Node &node);
	// int G(Node &node);
	// int H(Node &node);
	unsigned short H(unsigned char(&state)[23]);
	unsigned short Distance(int corr1, int corr2);
	void NormalState(unsigned char(&state)[23]);
	Node PopMinNode();
	void Expand(Node& parent, std::vector<Node>& children);
	void FreshClosed(std::vector<Node>& children);
	void DealRemovedParent(Node* p);
	//void SetChildrenTag(Node* parent);
	void FreshFringe(Node& parent, std::vector<Node>& children);
	bool isStateEqual(unsigned char(&state1)[23], unsigned char(&state2)[23]);
	Node Solve();
};