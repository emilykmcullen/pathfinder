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
        std::unordered_map<int, scene_box*> boxes;
        std::unordered_map<int, int> boxes_by_midpoint;
        int FindNorthBox(float x, float y);
        int FindSouthBox(float x, float y);
        int FindEastBox(float x, float y);
        int FindWestBox(float x, float y);
        int FindMidpoint(float x, float y);

    public:
        Scene(std::string name);
        void PrintBoxInfo();


    

};

