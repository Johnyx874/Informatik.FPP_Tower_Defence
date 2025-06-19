// graphics_engine.c

#define SDL_MAIN_HANDLED

#include "../include/library.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
// #include <SDL3/SDL_main.h>

#define WINDOW_WIDTH 1300   // std: 1000                  // Breite & Höhe des Fensters festlegen
#define WINDOW_HEIGHT 900  // std: 700

static SDL_Window* window = NULL;       // Wertespeicher für Fenster 
static SDL_Renderer* renderer = NULL;   // Wertespeicher für Renderer
static TTF_Font* font = NULL;

static SDL_Color white = { 255, 255, 255, 255 };

// Wertespeicher für Texturen
static SDL_Texture* texture1 = NULL;
static SDL_Texture* texture2 = NULL;
static SDL_Texture* texture3 = NULL;
static SDL_Texture* texture4 = NULL;
static SDL_Texture* texture5 = NULL;
static SDL_Texture* texture6 = NULL;

// Breite und Höhe der Texturen
static int texture1_width = 0;
static int texture1_height = 0;
static int texture2_width = 0;
static int texture2_height = 0;
static int texture3_width = 0;
static int texture3_height = 0;
static int texture4_width = 0;
static int texture4_height = 0;
static int texture5_width = 0;
static int texture5_height = 0;
static int texture6_width = 0;
static int texture6_height = 0;

static int frame_counter = 0;


// Funktion: insgesamt gerenderte Frames anzeigen
void totalFrames(void) {

    printf("----------------------------------------------------------\n");
    if (frame_counter == 100000) { printf("\n   Anzahl gerenderter Frames: >100000"); }
    else { printf("\n   Anzahl gerenderter Frames: %d\n", frame_counter); }
    printf("\n----------------------------------------------------------");

    fflush(stdout);
}


void countFrame(void) {
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


bool startSDL(void){

    SDL_SetAppMetadata("Centered Texture Example", "1.0", "com.example.centeredtexture");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    if (!TTF_Init()) {
        SDL_Log("TTF_Init fehlgeschlagen: %s\n", SDL_GetError());
        return false;
    }

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

    font = TTF_OpenFont("assets/ByteBounce.ttf", 50);
    if (!font) {
        SDL_Log("Fehler beim Laden der Schriftart: %s\n", SDL_GetError());
        return false;
    }


    texture1 = LoadTexture("assets/map_2.0.bmp", &texture1_width, &texture1_height);
    if (!texture1) { printf("Error Loading Texture"); return false; }

    texture2 = LoadTexture("assets/chicken.bmp", &texture2_width, &texture2_height);
    if (!texture2) { printf("Error Loading Texture"); return false; }

    texture3 = LoadTexture("assets/boar.bmp", &texture3_width, &texture3_height);
    if (!texture3) { printf("Error Loading Texture"); return false; }

    texture4 = LoadTexture("assets/cannon128.bmp", &texture4_width, &texture4_height);
    if (!texture3) { printf("Error Loading Texture"); return false; }

    texture5 = LoadTexture("assets/crossbow128.bmp", &texture5_width, &texture5_height);
    if (!texture3) { printf("Error Loading Texture"); return false; }

    texture6 = LoadTexture("assets/place_indicator.bmp", &texture6_width, &texture6_height);
    if (!texture1) { printf("Error Loading Texture"); return false; }

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

    case 1: renderTexture(texture2, 0 - 32, 0 - 32, 64, 64, x_offset, y_offset); break;
    
    case 2: renderTexture(texture3, 0 - 32, 0 - 32, 64, 64, x_offset, y_offset); break;
    
    }

}


void renderTower(int index, int x_offset, int y_offset) {

    switch (index) {

  //case index: renderTexture(SDL_Texture, X Coordinate, Y Coordinate, width, height, X Offset, Y Offset)

    case 1: renderTexture(texture4, 0 - 64, 0 - 64, 128, 128, x_offset, y_offset); break;

    case 2: renderTexture(texture5, 0 - 64, 0 - 64, 128, 128, x_offset, y_offset); break;

    case 3: renderTexture(texture6, 0, 0, 1300, 900, 0, 0); break;
    }
}


void renderText(const char* message, int x, int y, bool center) {

    size_t len = strlen(message);

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, message, len, white);
    if (!textSurface) {
        SDL_Log("Fehler beim Rendern des Textes: %s", SDL_GetError());
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_DestroySurface(textSurface);
    if (!textTexture) {
        SDL_Log("Fehler beim Erstellen der Textur: %s", SDL_GetError());
        return;
    }

    int x_final = x;

    if (center) {
        x_final = x - (textSurface->w / 2);
    }

    SDL_FRect dst_rect = { x_final, y, textSurface->w, textSurface->h };
    SDL_RenderTexture(renderer, textTexture, NULL, &dst_rect);
    SDL_DestroyTexture(textTexture);
}


void renderClear(void) {
    SDL_RenderClear(renderer);
}


void renderStatic(void) {
    renderTexture(texture1, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);
}


void renderPresent(void) {
    SDL_RenderPresent(renderer);
}



void quitSDL(void) {

    if (font) { TTF_CloseFont(font); }
    TTF_Quit();
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
    SDL_DestroyTexture(texture3);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
}
