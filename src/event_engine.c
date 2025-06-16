// event_engine.c

#include "../include/library.h"
bool spawnEntity_this_frame;

#include "../include/structs.h"

#include <SDL3/SDL.h>


void checkEvents(InputState* input) {

    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_EVENT_QUIT) {
            input->quit = true;
        }

        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.key == SDLK_C) {

                input->spawnEntity = true;
            }
        }

        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.key == SDLK_SPACE) {

                input->space_pressed = true;
            }
        }
    }
}
