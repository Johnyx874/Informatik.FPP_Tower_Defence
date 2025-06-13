// graphics_engine.c

#define SDL_MAIN_HANDLED

#include "library.h"
bool spawnEntity_this_frame;

#include <stdio.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
// #include <SDL3/SDL_main.h>

#define WINDOW_WIDTH 1300   // std: 1000                  // Breite & Höhe des Fensters festlegen
#define WINDOW_HEIGHT 900  // std: 700

static SDL_Window* window = NULL;       // Wertespeicher für Fenster 
static SDL_Renderer* renderer = NULL;   // Wertespeicher für Renderer

// Wertespeicher für Texturen
static SDL_Texture* texture1 = NULL;
static SDL_Texture* texture2 = NULL;
static SDL_Texture* texture3 = NULL;

// Breite und Höhe der Texturen
static int texture1_width = 0;
static int texture1_height = 0;
static int texture2_width = 0;
static int texture2_height = 0;
static int texture3_width = 0;
static int texture3_height = 0;

static int frame_counter = 0;


// Funktion: insgesamt gerenderte Frames anzeigen
void totalFrames() {

    printf("----------------------------------------------------------\n");
    if (frame_counter == 100000) { printf("\n   Anzahl gerenderter Frames: >100000"); }
    else { printf("\n   Anzahl gerenderter Frames: %d\n", frame_counter); }
    printf("\n----------------------------------------------------------");

    fflush(stdout);
}


void countFrame() {
    frame_counter++;
}


// Funktion: Textur laden
SDL_Texture* LoadTexture(const char* path, int* out_width, int* out_height)
{
    SDL_Surface* surface = SDL_LoadBMP(path);         // BMP auf Surface übertragen
    if (!surface) {
        SDL_Log("SDL_LoadBMP failed for '%s': %s", path, SDL_GetError());
        return NULL;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);  // Texture anhand von Surface erstellen
    if (!tex) {
        SDL_Log("CreateTextureFromSurface failed for '%s': %s", path, SDL_GetError());
        SDL_DestroySurface(surface);
        return NULL;
    }

    // Breite & Höhe setzen
    if (out_width) *out_width = surface->w;
    if (out_height) *out_height = surface->h;

    SDL_DestroySurface(surface);  // Surface bereinigen
    return tex;
}


bool InitApp(void)
{
    SDL_SetAppMetadata("Centered Texture Example", "1.0", "com.example.centeredtexture");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    /*
    if (!SDL_CreateWindowAndRenderer("Centered BMP", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("CreateWindowAndRenderer failed: %s", SDL_GetError());
        return false;
    }
    */

    window = SDL_CreateWindow("SDL3 Textures Test", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        return false;
    }


    texture1 = LoadTexture("images/first_map.bmp", &texture1_width, &texture1_height);
    if (!texture1) return false;

    texture2 = LoadTexture("images/cannon128.bmp", &texture2_width, &texture2_height);
    if (!texture2) return false;

    texture3 = LoadTexture("images/chicken.bmp", &texture3_width, &texture3_height);
    if (!texture3) {

        printf("Error Loading Texture");

        return false;
    }
    return true;
}


void renderTexture(SDL_Texture* texture, float x_texture_position, float y_texture_position, float texture_width, float texture_height, int x_offset, int y_offset) {

    SDL_FRect dst_rect;     // Platzhalter für Position & Größe der Textur

    dst_rect.x = x_texture_position + x_offset;      // Abstand von Linker Fensterseite       
    dst_rect.y = y_texture_position + y_offset;      // Abstand von Oberer Fensterseite
    dst_rect.w = texture_width;           // Breite        
    dst_rect.h = texture_height;          // Höhe

    SDL_RenderTexture(renderer, texture, NULL, &dst_rect);              // Textur auf den Renderer übertragen

}


void renderEntity(int index, int x_offset, int y_offset) {

    switch(index) {

  //case index: renderTexture(SDL_Texture, X Coordinate, Y Coordinate, width, height, X Offset, Y Offset)

    case 1: renderTexture(texture3, 0 - 32, 0 - 32, 64, 64, x_offset, y_offset); break;
    
    case 2: renderTexture(texture2, 0 - 64, 0 - 64, 128, 128, x_offset, y_offset); break;

    }

}


void renderClear() {
    SDL_RenderClear(renderer);
}


void renderStatic() {
    renderTexture(texture1, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);
}


void renderPresent() {
    SDL_RenderPresent(renderer);
}



bool quitGame(bool running) {
    
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {

            running = false;

            SDL_DestroyTexture(texture1);
            SDL_DestroyTexture(texture2);
            SDL_DestroyTexture(texture3);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);

        }
    }
    
    return running;
}
