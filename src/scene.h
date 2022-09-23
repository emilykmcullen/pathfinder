#pragma once
#include <string>
#include <unordered_map>
#include "constants.h"
#include <cstdio>


enum class walkable { WALKABLE, NOT_WALKABLE };
struct scene_box 
{
    int id;

    //walking boxes to north, east, south, west
    int n, e, s, w;

    walkable walkable_status;

    //extreme x/y values covered by the box
    float topX, lowX, topY, lowY, originX, originY;

    scene_box(int id, int n, int e, int s, int w, walkable walkable_status, float topX, float lowX, float topY, float lowY, float originX, float originY)
    : id(id), n(n), e(e), s(s), w(w), walkable_status(walkable_status), originX(originX), originY(originY)
    {
        topX = topX;
        lowX = lowX;
        topY = topY;
        lowY = lowY;

        //When creating boxes we will add padding to make them overlap
        //Clamp the outer edges of the perimeter boxes here so they don't go outside of the window
        if (topX > WINDOW_WIDTH) topX = WINDOW_WIDTH;
        if (topY > WINDOW_HEIGHT) topY = WINDOW_HEIGHT;
        if (lowX < 0) lowX = 0;
        if (lowY < 0) lowY = 0;

    }
};

class Scene
{
    private:
        std::string name;
        
        std::unordered_map<int, int> boxes_by_midpoint;

        // Find the boxes to the N/E/S/W of a particular box
        // Enter the originX and originY of the box      
        int FindNorthBox(int x, int y);
        int FindSouthBox(int x, int y);
        int FindEastBox(int x, int y);
        int FindWestBox(int x, int y);
        int FindMidpoint(int originX, int originY);


    public:
        Scene(std::string name);
        void PrintBoxInfo();
        void DrawBoxPoints();
        // TO DO: move back to private at some point
        std::unordered_map<int, scene_box*> boxes;

        // Enter the coordinates of a point in the window, return value is the current box id
        // Coords can be floats, but we need them to be cast to ints here for the rounding calculation
        int FindCurrentBoxFromCoord(int x, int y);


    

};

