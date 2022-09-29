#include "Scene.h"
#include <iostream>
#include <algorithm>

bool operator<(const BoxDotProd &bdp1, const BoxDotProd &bdp2){
    return bdp1.dotProduct > bdp2.dotProduct;
}

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
        scene_box* box = new scene_box(i, walkable::WALKABLE, currentX + BOX_WIDTH + BOX_PADDING, currentX - BOX_PADDING, currentY + BOX_HEIGHT + BOX_PADDING, currentY - BOX_PADDING, currentX, currentY);
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

        box->northEast = FindEastBox(box->originX, box->originY - BOX_HEIGHT);
        box->southEast = FindEastBox(box->originX, box->originY + BOX_HEIGHT);
        box->southWest = FindWestBox(box->originX, box->originY + BOX_HEIGHT);
        box->northWest = FindWestBox(box->originX, box->originY - BOX_HEIGHT);

        //Fill out the boxNeighbours array
        box->boxNeighbours.push_back(box->north);
        box->boxNeighbours.push_back(box->northEast);
        box->boxNeighbours.push_back(box->east);
        box->boxNeighbours.push_back(box->southEast);
        box->boxNeighbours.push_back(box->south);
        box->boxNeighbours.push_back(box->southWest);
        box->boxNeighbours.push_back(box->west);
        box->boxNeighbours.push_back(box->northWest);
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

scene_box* Scene::GetBoxById(int id)
{
    if (!boxes.contains(id))
    {
        std::cout << "GetBoxById: invalid id: " << id << std::endl;
        return nullptr;
    }
    else
    {
        return boxes.at(id);
    }
}

TreeNode* Scene::FindPath(int startBoxId, int destinationBoxId)
{
    // Handle invalid parameters
    if (startBoxId == destinationBoxId || !boxes.contains(startBoxId) || !boxes.contains(destinationBoxId))
    {
        std::cout << "Cannot create a path between these locations: " << startBoxId << " and " << destinationBoxId << std::endl;
        return nullptr;
    }

    if (boxes.at(destinationBoxId)->walkable_status == walkable::NOT_WALKABLE)
    {
        std::cout << "Entered a non-walkable destination" << std::endl;
        return nullptr;
    }

    // If a path already exists delete it
    if (pathLinkedList != nullptr)
    {
        pathLinkedList->DeleteTree();
    }

    // Clear searched boxes
    if (searchedBoxes.size() != 0)
    {
        searchedBoxes.clear();
    }

    pathIds.clear();

    pathLinkedList = new Tree(startBoxId);

    searchedBoxes.insert(startBoxId);
    
    scene_box* currentBox = boxes.at(startBoxId);

    TreeNode* currentNode = pathLinkedList->first;
    // For the first node, we can just do a simple check if any of the neighbours are the destination
    // We don't need to be concerned about the direction etc.
    for (int i = 0; i < currentBox->boxNeighbours.size(); i++)
    {
        if (currentBox->boxNeighbours.at(i) == destinationBoxId)
        {
            TreeNode* newNode = new TreeNode();
            newNode->data = destinationBoxId;
            newNode->parent = currentNode;
            currentNode->children.push_back(newNode); 
            CreatePathIdList(newNode);
            return newNode; 
        }
    }

    /** If the first box's neighbours did not contain the destination, then we start a bigger search... */

    // Get the direction vector from current box to destination box 
    scene_box* destinationBox = boxes.at(destinationBoxId);
    vec2 vectorFromCurrentBoxToDestination = { destinationBox->originX - currentBox->originX, destinationBox->originY - currentBox->originY};
    std::vector<BoxDotProd> orderedNeighbourBoxes = {};

    // Before we search the children nodes, we need to get them into the best order for searching
    // I'm ordering them based on how similar the direction is between the 
    // vectors of: currentBox->destinationBox  and : currentBox->neighbourBox
    for (int boxId : currentBox->boxNeighbours)
    {
        searchedBoxes.insert(boxId);
        if (boxId != -1)
        {
            scene_box* neighbourBox = boxes.at(boxId);
            if (neighbourBox->walkable_status != walkable::NOT_WALKABLE)
            {
                // get direction vec to each neighbour box from current box
                vec2 vectorFromCurrentBoxToNeighbour = { neighbourBox->originX - currentBox->originX, neighbourBox->originY - currentBox->originY };
                // then get the dot product of that vector against the vectorFromStartToDest, to see how similar they are
                float dotProd = NormalizedVecsDotProduct(vectorFromCurrentBoxToDestination , vectorFromCurrentBoxToNeighbour);
                BoxDotProd boxDotProd = { neighbourBox->id, dotProd };
                orderedNeighbourBoxes.push_back(boxDotProd);
            }
        }       
    }
    std::sort(orderedNeighbourBoxes.begin(), orderedNeighbourBoxes.end());
    // Create child nodes of the first node
    std::vector<TreeNode*> childNodes = CreateChildren(orderedNeighbourBoxes, *currentNode);


    // We didn't find the destination node in the child nodes of the first nodes
    // So now we search the child nodes of the child nodes and so on, until we reach destination node
    std::vector<std::vector<TreeNode*>> nodeListsToSearch = {};
    nodeListsToSearch.push_back(childNodes);
    TreeNode* destination = nullptr;
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
                currentBox = boxes.at(currentNode->data);
                std::vector<BoxDotProd> orderedNeighbourBoxes = {};
                for (int boxId : currentBox->boxNeighbours)
                {
                    auto searched = searchedBoxes.insert(boxId);
                    if (boxId != -1 && searched.second)
                    {
                        scene_box* neighbourBox = boxes.at(boxId);
                        if (neighbourBox->walkable_status != walkable::NOT_WALKABLE)
                        {
                            vectorFromCurrentBoxToDestination = { destinationBox->originX - currentBox->originX, destinationBox->originY - currentBox->originY};
                        
                            // get direction vec to each neighbour box from current box
                            vec2 vectorFromCurrentBoxToNeighbour = { neighbourBox->originX - currentBox->originX, neighbourBox->originY - currentBox->originY };
                            // then get the dot product of that vector against the vectorFromStartToDest, to see how similar they are
                            float dotProd = NormalizedVecsDotProduct(vectorFromCurrentBoxToDestination, vectorFromCurrentBoxToNeighbour);
                            BoxDotProd boxDotProd = { neighbourBox->id, dotProd };
                            orderedNeighbourBoxes.push_back(boxDotProd);
                        }  
                    }       
                }
                // sort the boxes by largest dot product first
                std::sort(orderedNeighbourBoxes.begin(), orderedNeighbourBoxes.end());

                // Create the node's children nodes
                std::vector<TreeNode*> nodeKids = CreateChildren(orderedNeighbourBoxes, *currentNode);
                // Check if the destination is in it's children nodes
                destination = CheckForDestination(nodeKids, destinationBoxId);
                if (destination != nullptr)
                {
                    CreatePathIdList(destination);
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
    CreatePathIdList(destination);
    return destination;
}

void Scene::CreatePathIdList(TreeNode* destination)
{
    TreeNode* currentNode = destination;
    if (currentNode != nullptr)
    {
        while(currentNode->parent != nullptr)
        {
            pathIds.push_back(currentNode->data);
            currentNode = currentNode->parent;
        }
        pathIds.push_back(currentNode->data);

    }

}

std::vector<TreeNode*> Scene::CreateChildren(std::vector<BoxDotProd> boxNeighbours, TreeNode& currentNode)
{
    for (BoxDotProd data : boxNeighbours) 
    {
        //auto searched = searchedBoxes.insert(data.boxId);
        if (data.boxId >= 0)
        {
            TreeNode* newNode = new TreeNode();
            newNode->data = data.boxId;
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

void Scene::NonWalkableBoxes(std::vector<int> boxIds)
{
    for (int id : boxIds)
    {
        if (boxes.contains(id))
        {
            boxes.at(id)->walkable_status = walkable::NOT_WALKABLE;
        }
    }
}

