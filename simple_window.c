#include <stdio.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_time.h>

struct Game {
    SDL_Window *window;
};

bool game_init_sdl(struct Game *game) {
 
    SDL_Init(SDL_INIT_VIDEO);
 
    game->window = SDL_CreateWindow("Test Window", 800, 600, 0);

    return true;
}

void game_free(struct Game *game) {
    
    SDL_DestroyWindow(game->window);
 
    SDL_Quit();
}

int main() {
 
    struct Game game = {0};

    game_init_sdl(&game);

    SDL_Delay(5000);

    game_free(&game);

    return 0;
}
