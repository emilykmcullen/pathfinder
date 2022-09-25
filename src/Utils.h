#include <stdio.h>
#include <vector>
#include <unordered_set>


typedef struct node 
{ 
    struct node* parent = nullptr;
    std::vector<node*> children;
    int data = -1;
} node;

class list
{
    public:
    node* first;

    list(int v)
    {
        first = new node();
        first->data = v;
        usedData.insert(v);
    };
    
    // Using this set to store the ids of boxes that have been searched
    // If we try to create a node using an id that is already in this list, it should stop that line of enquiry
    std::unordered_set<int> usedData = {};
                      

    //initialize list with first node

    // given a vector of ints, for each int create a new node for that int (as long as it isn't -1), and init parent to current node
    // add each of those new nodes to the 'children' list of current node
    // return that children vector (so we can loop over)

    std::vector<node*> CreateChildren(std::vector<int> childrenData, node& currentNode)
    {
        for (int data : childrenData) 
        {
            auto searched = usedData.insert(data);
            if (data >= 0 && searched.second)
            {
                node* newNode = new node();
                newNode->data = data;
                newNode->parent = &currentNode;
                //currentNode.children.emplace(newNode);
                currentNode.children.push_back(newNode);
            }
        }
        return currentNode.children;
    }

    node* CheckForDestination(std::vector<node*>& nodes, int desintationBox)
    {
        for (auto& element : nodes)
        {
            if (element->data == desintationBox)
            {
                return element;
            }
        }
        return nullptr;
    }
};
