#pragma once
#include <string>
#include <unordered_map>
#include "Constants.h"
#include <cstdio>
#include "Utils.h"
#include <vector>


enum class walkable { WALKABLE, NOT_WALKABLE };

struct scene_box 
{
    int id;

    //walking boxes to north, east, south, west
    int north, east, south, west;

    // For pathfinding, store all the neighbours (by id) of each box (we don't care what direction they are in)
    std::vector<int> boxNeighbours = {};

    walkable walkable_status;

    //extreme x/y values covered by the box, and origin X/Y
    float maxX, minX, maxY, minY, originX, originY;

    scene_box(int id, int n, int e, int s, int w, walkable walkable_status, float maxX, float minX, float maxY, float minY, float originX, float originY)
    : id(id), north(n), east(e), south(s), west(w), walkable_status(walkable_status), originX(originX), originY(originY)
    {
        maxX = maxX;
        minX = minX;
        maxY = maxY;
        minY = minY;

        //When creating boxes we will add padding to make them overlap
        //Clamp the outer edges of the perimeter boxes here so they don't go outside of the window
        if (maxX > WINDOW_WIDTH) maxX = WINDOW_WIDTH;
        if (maxY > WINDOW_HEIGHT) maxY = WINDOW_HEIGHT;
        if (minX < 0) minX = 0;
        if (minY < 0) minY = 0;

    }
};

class Scene
{
    private:
        std::string name;
        
        std::unordered_map<int, int> boxesByMidpoint;

        Tree* pathLinkedList = nullptr;

        // Find the boxes to the N/E/S/W of a particular box
        // Enter the originX and originY of the box      
        int FindNorthBox(int x, int y);
        int FindSouthBox(int x, int y);
        int FindEastBox(int x, int y);
        int FindWestBox(int x, int y);
        int FindMidpoint(int originX, int originY);
        
        // PATHFINDING STUFF
        // Using this set to store the ids of boxes that have been searched
        // If we try to create a node using an id that is already in this list, it should stop that line of enquiry
        std::unordered_set<int> searchedBoxes = {};
        std::vector<TreeNode*> CreateChildren(std::vector<int> childrenData, TreeNode& currentNode);
        TreeNode* CheckForDestination(std::vector<TreeNode*>& nodes, int desintationBox);


    public:
        Scene(std::string name);
        ~Scene();

        void PrintBoxInfo();
        void DrawBoxPoints();
        // TO DO: move back to private at some point
        std::unordered_map<int, scene_box*> boxes;

        // Enter the coordinates of a point in the window, return value is the current box id
        // Coords can be floats, but we need them to be cast to ints here for the rounding calculation
        int FindCurrentBoxFromCoord(int x, int y);

        //move this elsewhere?
        TreeNode* FindPath(int startBoxId, int destinationBoxId);

        // TO DO: 
        // - handle when invalid box ids are passed to find path
        // - free node memory 
        void DeletePath(){ if (pathLinkedList != nullptr) pathLinkedList->DeleteTree();};

    

};

