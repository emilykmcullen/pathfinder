#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


class Game {
    private:
        bool isRunning;
        SDL_Window *window;

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
};

#endif