

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_time.h>

#define SDL_FLAGS SDL_INIT_VIDEO

#define WINDOW_TITLE "Test Window"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

struct Game {
    SDL_Window* window;
    SDL_Renderer* renderer;
};

bool game_init_sdl(struct Game* game);
void game_free(struct Game* game);

bool game_init_sdl(struct Game* game) {
    if (!SDL_Init(SDL_FLAGS)) {
        fprintf(stderr, "Error initializing SDL3: %s\n", SDL_GetError());
        return false;
    }

    game->window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (game->window == NULL) {
        fprintf(stderr, "Error creating Window: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void game_free(struct Game* game) {
    if (game->window) {
        SDL_DestroyWindow(game->window);
        game->window = NULL;
    }

    SDL_Quit();

}

int main() {
    bool exit_status = EXIT_FAILURE;

    struct Game game = { 0 };

    if (game_init_sdl(&game)) {
        exit_status = EXIT_SUCCESS;
    }

    SDL_Delay(5000);

    game_free(&game);

    return exit_status;
}
