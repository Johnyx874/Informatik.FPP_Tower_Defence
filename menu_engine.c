#include "library.h"
bool spawnEntity_this_frame;

#include <SDL3/SDL.h>

void checkEvents() {

    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_EVENT_KEY_DOWN) {

            if (event.key.key == SDLK_C) {

                spawnEntity_this_frame = true;

                printf("spawn\n");

            }

            printf("Button!");
        }

        
    }

    //printf("checked\n");
}