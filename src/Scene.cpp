#include "Scene.h"
#include <iostream>

Scene::Scene(std::string name) : name(name)
{
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
        int midpoint = (midX * 1000) + midY; //Gives us the key to use
        boxes_by_midpoint.emplace(midpoint, i);

        currentX += BOX_WIDTH;
        if (currentX == WINDOW_WIDTH)
        {
            currentX = 0;
            currentY += BOX_HEIGHT;
        }
        if (currentY == WINDOW_HEIGHT)
        {
            printf("REACHED WINDOW WIDTH, BOXES COMPLETED %i\n", i);
            //After all boxes are completed, go through each box and find it's nesw and assign them
            break;
        }
    }

    for (int i = 0; i < total_boxes; i++)
    {   
        scene_box* box = boxes.at(i);
        //Find n,e,s,w of the current box
        box->n = FindNorthBox(box->originX, box->originY);
        box->e = FindEastBox(box->originX, box->originY);
        box->s = FindSouthBox(box->originX, box->originY);
        box->w = FindWestBox(box->originX, box->originY);
    }
}

void Scene::PrintBoxInfo()
{
    for (int i = 0; i < (BOXES_PER_ROW_AND_COLUMN*BOXES_PER_ROW_AND_COLUMN); i++)
    {
        scene_box* box = boxes.at(i);
        std::cout << "BOX ID: " << box->id << ", BOX N: " << box->n << ", BOX E: " << box->e << ", BOX S: " << box->s << ", BOX W" << box->w << std::endl; 
    }
}

int Scene::FindNorthBox(float originX, float originY)
{
    float northOriginY = originY - BOX_HEIGHT;

    int midpoint = FindMidpoint(originX, northOriginY);  

    if (boxes_by_midpoint.contains(midpoint) == false)
    {
        return -1;
    }
    else
    {
        return boxes_by_midpoint.at(midpoint);
    }
}

int Scene::FindEastBox(float originX, float originY)
{
    float EastOriginX = originX + BOX_WIDTH;

    int midpoint = FindMidpoint(EastOriginX, originY);  

    if (boxes_by_midpoint.contains(midpoint) == false)
    {
        return -1;
    }
    else
    {
        return boxes_by_midpoint.at(midpoint);
    }
}

int Scene::FindSouthBox(float originX, float originY)
{
    float southOriginY = originY + BOX_HEIGHT;

    int midpoint = FindMidpoint(originX, southOriginY);  

    if (boxes_by_midpoint.contains(midpoint) == false)
    {
        return -1;
    }
    else
    {
        return boxes_by_midpoint.at(midpoint);
    }
}

int Scene::FindWestBox(float originX, float originY)
{
    float westOriginX = originX - BOX_WIDTH;

    int midpoint = FindMidpoint(westOriginX, originY);  

    if (boxes_by_midpoint.contains(midpoint) == false)
    {
        return -1;
    }
    else
    {
        return boxes_by_midpoint.at(midpoint);
    }
}

int Scene::FindMidpoint(float originX, float originY)
{
    int midX = originX + (BOX_WIDTH/2);
    int midY = originY + (BOX_HEIGHT/2);
    int midpoint = (midX * 1000) + midY;
    return midpoint;
}
