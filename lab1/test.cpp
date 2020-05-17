#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <unordered_set>

using namespace std;

typedef struct node //32 Bytes
{
    unsigned char state[23]; //state[0], state[22] is 0's location, state[7] is the central 7's location
    unsigned char action;
    unsigned short cost_f;
    unsigned short cost_g;
    struct node *parent;

} Node;

struct myhash
{
public:
    std::size_t operator()(Node const *node) const
    {
        std::string s((char *)(node->state), 23);
        return std::hash<std::string>()(s);
    }
};

struct myeuqal
{
    bool operator()(Node const *lhs, Node const *rhs) const
    {
        for (int j = 0; j < 23; j++)
        {
            if (lhs->state[j] != rhs->state[j])
                return false;
        }
        return true;
    }
};

Node node;

Node &test()
{
    return node;
}

int main(int argc, char **argv)
{
    for (int i = 0; i < 23; i++)
    {
        node.state[i] = 'a' + i;
    }
    unordered_set<Node *, myhash, myeuqal> closed;
    closed.insert(&node);
    cout << myhash()(&node);

    // Node node1 = test();
    // Node &node2 = test();
    // cout << &node << endl
    //      << &node1 << endl
    //      << &node2 << endl;

    // const std::vector<std::pair<int, int>> actions{{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
    // for (auto action : actions)
    // {
    //     cout << '(' << action.first << ',' << action.second << ')' << endl;
    // }
}