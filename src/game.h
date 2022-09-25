#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Scene.h"


class Game {
    private:
        bool isRunning;
        SDL_Window *window;
        Scene* my_scene;
        
        //for boxes
        // TO DO: tidy these up elsewhere
        uint32_t* color_buffer = NULL;
        SDL_Texture* color_buffer_texture = NULL;
        void RenderColorBuffer();
        void ClearColorBuffer(uint32_t color);
        void DrawRect(int x, int y, int width, int height, uint32_t color);
        void DrawSceneBoxes();
        //

    public:
        Game();
        ~Game();
        int ticksLastFrame = 0;
        bool IsRunning() const;
        static SDL_Renderer *renderer;
        static SDL_Event event;
        void Initialize(int width, int height);
        void ProcessInput();
        void ProcessGameOver();
        void Update();
        void Render();
        void Destroy();
        void Test();
};

#endif