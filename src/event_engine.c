// event_engine.c

#include "../include/library.h"
bool spawnEntity_this_frame;

#include "../include/structs.h"

#include <SDL3/SDL.h>


void checkEvents(InputState* input) {

    // Aktuelle Maus-Position
    uint32_t buttons = SDL_GetMouseState(&input->x_mouse_position, &input->y_mouse_position);

    
    // Aktuelle Events ( Generelle Events, Tastendrücke && Maustastendrücke )
    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        switch (event.type) {

        case SDL_EVENT_QUIT:
            input->quit = true;

        case SDL_EVENT_KEY_DOWN:

            if (event.key.key == SDLK_C) {
                input->key_c = true;
            }
            else if (event.key.key == SDLK_1) {
                input->key_1 = true;
            }
            else if (event.key.key == SDLK_2) {
                input->key_2 = true;
            }
            else if (event.key.key == SDLK_3) {
                input->key_3 = true;
            }
            else if (event.key.key == SDLK_4) {
                input->key_4 = true;
            }
            else if (event.key.key == SDLK_5) {
                input->key_5 = true;
            }
            else if (event.key.key == SDLK_6) {
                input->key_6 = true;
            }
            else if (event.key.key == SDLK_SPACE) {
                input->key_space = true;
            }
            else if (event.key.key == SDLK_ESCAPE) {
                input->key_escape = true;
            }

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {

                input->button_left = true;
            }
        }
    }
}
