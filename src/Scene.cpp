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
        boxes_by_midpoint.emplace(midpoint, i);
        
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

int Scene::FindNorthBox(int originX, int originY)
{
    int northOriginY = originY - BOX_HEIGHT;

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

int Scene::FindEastBox(int originX, int originY)
{
    int EastOriginX = originX + BOX_WIDTH;

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

int Scene::FindSouthBox(int originX, int originY)
{
    int southOriginY = originY + BOX_HEIGHT;

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

int Scene::FindWestBox(int originX, int originY)
{
    int westOriginX = originX - BOX_WIDTH;

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
    int boxId = boxes_by_midpoint.at(midpoint);
    printf("MOUSE CLICKED ON BOX: %i \n", boxId);
    return boxId;

    
}

