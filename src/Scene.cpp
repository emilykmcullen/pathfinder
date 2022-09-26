#include "Scene.h"
#include <iostream>

Scene::Scene(std::string name) : name(name)
{
    if (WINDOW_WIDTH % BOXES_PER_ROW_AND_COLUMN != 0 || WINDOW_HEIGHT % BOXES_PER_ROW_AND_COLUMN != 0)
    {
        // We want the boxes to have non-decimal origins so if they don't divide into whole numbers, finish the program
        // TO DO: stop program execution here
        printf("ERROR!!! BOXES DON'T DIVIDE INTO THE WINDOW CLEANLY!\n");
    }

    //Initialize boxes
    int total_boxes = BOXES_PER_ROW_AND_COLUMN * BOXES_PER_ROW_AND_COLUMN;

    float currentX = 0;
    float currentY = 0;

    for (int i = 0; i < total_boxes; i++)
    {
        scene_box* box = new scene_box(i, 0, 0, 0, 0, walkable::NOT_WALKABLE, currentX + BOX_WIDTH + BOX_PADDING, currentX - BOX_PADDING, currentY + BOX_HEIGHT + BOX_PADDING, currentY - BOX_PADDING, currentX, currentY);
        boxes.emplace(box->id, box);

        //This will round any float values, but that's what we want. We just want an approximate value to use as the key
        int midX = currentX + (BOX_WIDTH/2); 
        int midY = currentY + (BOX_HEIGHT/2);
        int midpoint = (midX * BOX_MIDPOINT_MULTIPLIER) + midY; //Gives us the key to use
        boxesByMidpoint.emplace(midpoint, i);
        
        currentX += BOX_WIDTH;
        // Previously this checked if currentX == WINDOW_WIDTH, but it might not always exactly equal it
        // So just to be sure changed it to near enough to WINDOW_WIDTH
        if (currentX >= WINDOW_WIDTH - (BOX_WIDTH/2))
        {
            currentX = 0;
            currentY += BOX_HEIGHT;
        }
        // Previously this checked if currentY == WINDOW_HEIGHT, but it might not always exactly equal it
        // So just to be sure changed it to near enough to WINDOW_HEIGHT
        if (currentY >= WINDOW_HEIGHT - (BOX_HEIGHT/2))
        {
            printf("REACHED WINDOW END, BOXES COMPLETED %i\n", i);
            //After all boxes are completed, go through each box and find it's nesw and assign them
            break;
        }
    }

    for (int i = 0; i < total_boxes; i++)
    {   
        scene_box* box = boxes.at(i);
        //Find n,e,s,w of the current box
        // TO DO: are the nesw needed now i have boxneighbours?
        box->north = FindNorthBox(box->originX, box->originY);
        box->east = FindEastBox(box->originX, box->originY);
        box->south = FindSouthBox(box->originX, box->originY);
        box->west = FindWestBox(box->originX, box->originY);
        //Fill out the boxNeighbours array
        box->boxNeighbours.push_back(box->north);
        box->boxNeighbours.push_back(box->east);
        box->boxNeighbours.push_back(box->south);
        box->boxNeighbours.push_back(box->west);
    }
}

Scene::~Scene()
{
    DeletePath();
}

void Scene::PrintBoxInfo()
{
    for (int i = 0; i < (BOXES_PER_ROW_AND_COLUMN*BOXES_PER_ROW_AND_COLUMN); i++)
    {
        scene_box* box = boxes.at(i);
        std::cout << "BOX ID: " << box->id << ", BOX N: " << box->north << ", BOX E: " << box->east << ", BOX S: " << box->south << ", BOX W" << box->west << std::endl; 
    }
}

int Scene::FindNorthBox(int originX, int originY)
{
    int northOriginY = originY - BOX_HEIGHT;

    int midpoint = FindMidpoint(originX, northOriginY);  

    if (boxesByMidpoint.contains(midpoint) == false)
    {
        return -1;
    }
    else
    {
        return boxesByMidpoint.at(midpoint);
    }
}

int Scene::FindEastBox(int originX, int originY)
{
    int EastOriginX = originX + BOX_WIDTH;

    int midpoint = FindMidpoint(EastOriginX, originY);  

    if (boxesByMidpoint.contains(midpoint) == false)
    {
        return -1;
    }
    else
    {
        return boxesByMidpoint.at(midpoint);
    }
}

int Scene::FindSouthBox(int originX, int originY)
{
    int southOriginY = originY + BOX_HEIGHT;

    int midpoint = FindMidpoint(originX, southOriginY);  

    if (boxesByMidpoint.contains(midpoint) == false)
    {
        return -1;
    }
    else
    {
        return boxesByMidpoint.at(midpoint);
    }
}

int Scene::FindWestBox(int originX, int originY)
{
    int westOriginX = originX - BOX_WIDTH;

    int midpoint = FindMidpoint(westOriginX, originY);  

    if (boxesByMidpoint.contains(midpoint) == false)
    {
        return -1;
    }
    else
    {
        return boxesByMidpoint.at(midpoint);
    }
}

int Scene::FindMidpoint(int originX, int originY)
{
    // Will not find the exact midpoint, we just need an approx midpoint to use as a key
    int midX = originX + (BOX_WIDTH/2);
    int midY = originY + (BOX_HEIGHT/2);
    int midpoint = (midX * 1000) + midY;
    return midpoint;
}


int Scene::FindCurrentBoxFromCoord(int x, int y)
{
    if (x > WINDOW_WIDTH || x < 0 || y > WINDOW_HEIGHT || y < 0)
    {
        printf("Scene::FindCurrentBoxFromCoord - invalid coordinates %f, %f\n", x, y);
        return -1;
    }

    // Round the x,y coords down to the nearest multiple of the box_width/height
    // Separate statements because compiler was optimizing the div/mult out
    // TO DO: IS THERE A BETTER WAY TO DO THIS
    int dividedX = (x/BOX_WIDTH);
    int originX = dividedX * BOX_WIDTH;
    int dividedY = (y/BOX_HEIGHT);
    int originY = dividedY * BOX_HEIGHT;

    int midpoint = FindMidpoint(originX, originY);
    int boxId = boxesByMidpoint.at(midpoint);
    printf("MOUSE CLICKED ON BOX: %i \n", boxId);
    return boxId;

    
}

TreeNode* Scene::FindPath(int startBoxId, int destinationBoxId)
{
    // TO DO:
    // when we call FindPath I need to free the memory previously stored in pathLinkedList
    // AND need to empty searchedBoxes
    // Also what if start and dest are the same, handle this
    if (startBoxId == destinationBoxId || !boxes.contains(startBoxId) || !boxes.contains(destinationBoxId))
    {
        std::cout << "Cannot create a path between these locations: " << startBoxId << " and " << destinationBoxId << std::endl;
        return nullptr;
    }

    if (pathLinkedList != nullptr)
    {
        pathLinkedList->DeleteTree();
    }

    pathLinkedList = new Tree(startBoxId);

    searchedBoxes.insert(startBoxId);
    
    scene_box* box = boxes.at(startBoxId);

    // We have our list and the first node is initialized
    // Get the box neighbours and send that data to CreateChildren
    TreeNode* currentNode = pathLinkedList->first;
    // Create child nodes of the first node
    std::vector<TreeNode*> childNodes = CreateChildren(box->boxNeighbours, *currentNode);
    // Check if any of those nodes is the destination
    TreeNode* destination = CheckForDestination(childNodes, destinationBoxId);

    if (destination != nullptr)
    {
        return destination;
    }

    
    std::vector<std::vector<TreeNode*>> nodeListsToSearch = {};
    // For each of the child nodes of our first node, create their child nodes, check for desintation, then if destination is 
    // not found, add their child nodes to nodeListsToSearch
    for (auto& node1 : childNodes)
    {
        currentNode = node1;
        box = boxes.at(currentNode->data);
        std::vector<TreeNode*> nodeKids = CreateChildren(box->boxNeighbours, *currentNode);
        destination = CheckForDestination(nodeKids, destinationBoxId);
        if (destination != nullptr)
        {
           return destination;
        }
        nodeListsToSearch.push_back(nodeKids);
    }

    // Now we can repeat a similar pattern
    // If we get here, we haven't found the destination in the child nodes of the first node OR in their children's child nodes 
    // eg. we haven't found them in three 'levels' of nodes
    while (destination == nullptr)
    {
        // Make a second list holding vectors of child nodes
        std::vector<std::vector<TreeNode*>> nodeListsToSearch2 = {};

        // For each list in our original list of nodeListsToSearch
        for (auto& list : nodeListsToSearch)
        {
            // Check each node in the list of child nodes
            for (auto& node1 : list)
            {
                currentNode = node1;
                box = boxes.at(currentNode->data);
                // Create the node's children nodes
                std::vector<TreeNode*> nodeKids = CreateChildren(box->boxNeighbours, *currentNode);
                // Check if the destination is in it's children nodes
                destination = CheckForDestination(nodeKids, destinationBoxId);
                if (destination != nullptr)
                {
                    return destination;
                }
                // Add it's child nodes to our second list of lists to search
                nodeListsToSearch2.push_back(nodeKids);
                
            }
        }
        // Once all the lists in our original list of lists have been searched, we can reassign our original list
        // to the new list of lists we created
        // Then we will start over again and repeat our search on those lists of lists of nodes
        nodeListsToSearch = nodeListsToSearch2;
        nodeListsToSearch2.clear();
        nodeListsToSearch2.shrink_to_fit();

        
    }
    return destination;
}

std::vector<TreeNode*> Scene::CreateChildren(std::vector<int> childrenData, TreeNode& currentNode)
{
    for (int data : childrenData) 
    {
        auto searched = searchedBoxes.insert(data);
        if (data >= 0 && searched.second)
        {
            TreeNode* newNode = new TreeNode();
            newNode->data = data;
            newNode->parent = &currentNode;
            //currentNode.children.emplace(newNode);
            currentNode.children.push_back(newNode);
        }
    }
    return currentNode.children;
}

TreeNode* Scene::CheckForDestination(std::vector<TreeNode*>& nodes, int desintationBox)
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

