#include <stdio.h>
#include <vector>
#include <unordered_set>


typedef struct TreeNode 
{ 
    struct TreeNode* parent = nullptr;
    std::vector<TreeNode*> children;
    int data = -1;
} TreeNode;

class Tree
{
    public:
        TreeNode* first;

        Tree(int v)
        {
            first = new TreeNode();
            first->data = v;
        };

        ~Tree()
        {
            DeleteTree();
        }
 
        void DeleteTree()
        {
            if (first == nullptr) return;

            TreeNode* temp = first;

            // get the list of children
            std::vector<TreeNode*> kids = first->children;

            if (kids.size() == 0)
            {
                // If it doesn't have any children just free the memory of the first node
                printf("Deleting: %i\n", temp->data);
                delete temp;
                return;
            }

            for (auto& node : kids)
            {
                // For each node, point parent to nullptr
                node->parent = nullptr;
            }

            // Delete the parent
            printf("Deleting: %i\n", temp->data);
            delete temp;


            std::vector<std::vector<TreeNode*>> listsToDelete = {};
            // Now go through the the kids
            for (auto& node : kids)
            {
                TreeNode* temp = node;
                std::vector<TreeNode*> childNodes = temp->children;
                for (auto& node2 : childNodes)
                {
                    node2->parent = nullptr;
                }
                printf("Deleting: %i\n", temp->data);
                delete temp;
                listsToDelete.push_back(childNodes);

            }

            std::vector<std::vector<TreeNode*>> listsToDelete2 = {};
            bool stop = false;
            while (stop == false)
            {
                for (auto& list: listsToDelete)
                {
                    for (auto& node : list)
                    {
                        TreeNode* temp = node;
                        std::vector<TreeNode*> childNodes = temp->children;
                        for (auto& node2 : childNodes)
                        {
                            node2->parent = nullptr;
                        }
                        printf("Deleting: %i\n", temp->data);
                        delete temp;
                        listsToDelete2.push_back(childNodes);
                    }
                }

                listsToDelete = listsToDelete2;
                listsToDelete2.clear();
                if (listsToDelete.empty())
                {
                    stop = true;
                }
            }
        }
        
    
};
