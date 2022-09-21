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
    float topX, lowX, topY, lowY;

    scene_box(int id, int n, int e, int s, int w, walkable walkable_status, float topX, float lowX, float topY, float lowY)
    : id(id), n(n), e(e), s(s), w(w), walkable_status(walkable_status)
    {
        topX = topX;
        lowX = lowX;
        topY = topY;
        lowY = lowY;

        //When we are creating boxes we will add padding to make them overlap
        //We clamp the outer edges of the perimeter boxes here so they don't go outside of the window
        if (topX > WINDOW_WIDTH) topX = WINDOW_WIDTH;
        if (topY > WINDOW_HEIGHT) topY = WINDOW_HEIGHT;
        if (lowX < 0) lowX = 0;
        if (lowY < 0) lowY = 0;

    }
};

struct scene
{
    std::string name;
    std::unordered_map<int, scene_box*> boxes;

    scene(std::string name) : name(name)
    {
        //Initialize boxes
        int total_boxes = BOXES_PER_ROW_AND_COLUMN * BOXES_PER_ROW_AND_COLUMN;

        float box_width = WINDOW_WIDTH / BOXES_PER_ROW_AND_COLUMN;
        float box_height = WINDOW_HEIGHT / BOXES_PER_ROW_AND_COLUMN;

        int currentX = 0;
        int currentY = 0;

        for (int i = 0; i < total_boxes; i++)
        {
            scene_box* box = new scene_box(i, 0, 0, 0, 0, walkable::NOT_WALKABLE, currentX + box_width + BOX_PADDING, currentX - BOX_PADDING, currentY + box_height + BOX_PADDING, currentY - BOX_PADDING);
            
            boxes.emplace(box->id, box);

            currentX += box_width;
            if (currentX == WINDOW_WIDTH)
            {
                currentX = 0;
                currentY += box_height;
            }
            if (currentY == WINDOW_HEIGHT)
            {
                printf("REACHED WINDOW WIDTH, BOXES COMPLETED %i\n", i);
                break;
            }
        }

    }



};