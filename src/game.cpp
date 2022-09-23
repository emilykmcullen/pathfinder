#include <iostream>
#include "./Constants.h"
#include "./Game.h"

SDL_Renderer* Game::renderer;
SDL_Event Game::event;

Game::Game(){
    this->isRunning =false;
}

Game::~Game(){

}

bool Game::IsRunning() const {
    return this->isRunning;
}


void Game::Initialize(int width, int height) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cerr << "Error initializing SDL" << std::endl;
        return;
    }
    if (TTF_Init() !=0) {
        std::cerr << "Error initializing SDL TTF" << std::endl;
    }
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_BORDERLESS
    );
    if(!window){
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer){
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    isRunning = true;

    //for boxes
    color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);
    color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
    my_scene = new Scene("emily sceene");
    //my_scene->PrintBoxInfo();

    return;
}

//for boxes
void Game::RenderColorBuffer()
{
    //final param is the width of our row
    SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int)(WINDOW_WIDTH * sizeof(uint32_t)));
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void Game::DrawRect(int x, int y, int width, int height, uint32_t color) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int current_x = x + i;
            int current_y = y + j;
            color_buffer[(WINDOW_WIDTH * current_y) + current_x] = color;
        }
    }
}

void Game::DrawSceneBoxes()
{
    uint32_t colour = 0xDDFFFFFF;
    for (int i = 0; i < BOXES_PER_ROW_AND_COLUMN * BOXES_PER_ROW_AND_COLUMN; i++)
    {
        scene_box* box = my_scene->boxes.at(i);
        DrawRect(box->originX, box->originY, BOX_WIDTH, BOX_HEIGHT, colour);
        colour = (uint32_t) ((float)colour / 1.2f);
    }
}

void Game::ClearColorBuffer(uint32_t color)
{
    for (int i = 0; i < (WINDOW_WIDTH * WINDOW_HEIGHT); i++)
    {
        color_buffer[i] = color;
    }

    // for (int y = 0; y < window_height; y++)
    // {
    //     for (int x = 0; x < window_width; x++)
    //     {
    //         color_buffer[(window_width * y) + x] = color;
    //     }
    // }
}


void Game::ProcessInput(){
    SDL_PollEvent(&event);
    switch (event.type){
        case SDL_QUIT:{
            isRunning = false;
            break;
        }
        case SDL_KEYDOWN: {
            if (event.key.keysym.sym == SDLK_ESCAPE){
                isRunning =false;
            }
        }
        case SDL_MOUSEBUTTONDOWN: 
        {
            my_scene->FindCurrentBoxFromCoord(event.button.x, event.button.y);
        }
        default: {
            break;
        }
    }
}

void Game::Update(){

    //Sleep the execution until we reach the target frame time in milliseconds
    int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);

    //Only call delay if we are too fast to process this frame
    if(timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME){
        SDL_Delay(timeToWait);
    }
    
    // delta time is the difference in ticks from the last frame, converted to seconds
    float deltaTime =(SDL_GetTicks() - ticksLastFrame)/1000.0f;

    //Clamp deltaTime to a maximum value
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;


    //Sets the new ticks for the current frame to be used in the next pass
    ticksLastFrame = SDL_GetTicks();
    
}

void Game::Render(){
    SDL_SetRenderDrawColor(renderer, 21, 21,21 , 255);
    SDL_RenderClear(renderer);
    //ClearColorBuffer(0xFFFFFF00);
    DrawSceneBoxes();
    RenderColorBuffer();
    SDL_RenderPresent(renderer);
}


void Game::ProcessGameOver() {
    std::cout << "Game Over" << std::endl;
    isRunning = false;
}


void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}