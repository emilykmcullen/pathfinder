#include <iostream>
#include "./game.h"
#include "./constants.h"
using namespace std;

int main(int argc, char *args[]) {
    Game game;
    
    game.Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

    while (game.IsRunning()){
        game.ProcessInput();
        game.Update();
        game.Render();
    } 

    game.Destroy();

    return 0;
}