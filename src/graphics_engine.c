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

#define MAX_FRAME_TRACKERS 200  // 0-99 -> für Entities
                                // 100-200 -> für Tower

static SDL_Window* window = NULL;       // Wertespeicher für Fenster 
static SDL_Renderer* renderer = NULL;   // Wertespeicher für Renderer
static TTF_Font* font = NULL;

static SDL_Color white = { 255, 255, 255, 255 };

// Wertespeicher für Texturen
static SDL_Texture* map_tex = NULL;
static SDL_Texture* chicken_tex = NULL;
static SDL_Texture* boar_tex = NULL;
static SDL_Texture* cannon_tex = NULL;
static SDL_Texture* crossbow_tex = NULL;
static SDL_Texture* place_indicator_tex = NULL;
static SDL_Texture* killed_chicken_tex = NULL;

// Breite und Höhe der Texturen
static int map_tex_width = 0;
static int map_tex_height = 0;
static int chicken_tex_width = 0;
static int chicken_tex_height = 0;
static int boar_tex_width = 0;
static int boar_tex_height = 0;
static int cannon_tex_width = 0;
static int cannon_tex_height = 0;
static int crossbow_tex_width = 0;
static int crossbow_tex_height = 0;
static int place_indicator_tex_width = 0;
static int place_indicator_tex_height = 0;
static int killed_chicken_tex_width = 0;
static int killed_chicken_tex_height = 0;

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


// Funktion: an beliebig vielen Stellen im Code X Frames abzuwarten
bool passedFrames(int id, int waitFrames) {

    static int counters[MAX_FRAME_TRACKERS] = { 0 };

    if (id < 0 || id >= MAX_FRAME_TRACKERS) {
        SDL_Log("passedFrames: Ungültige ID");
        return false;
    }

    if (counters[id] == -1) {
        counters[id] = 0;
        return false;
    }

    if (counters[id] < waitFrames) {
        counters[id]++;
        return false;
    }

    counters[id] = -1; // Reset für nächste Nutzung
    return true;
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


    map_tex = LoadTexture("assets/map_2.0.bmp", &map_tex_width, &map_tex_height);
    if (!map_tex) { printf("Error Loading Texture"); return false; }

    chicken_tex = LoadTexture("assets/chicken.bmp", &chicken_tex_width, &chicken_tex_height);
    if (!chicken_tex) { printf("Error Loading Texture"); return false; }

    boar_tex = LoadTexture("assets/boar.bmp", &boar_tex_width, &boar_tex_height);
    if (!boar_tex) { printf("Error Loading Texture"); return false; }

    cannon_tex = LoadTexture("assets/cannon128.bmp", &cannon_tex_width, &cannon_tex_height);
    if (!cannon_tex) { printf("Error Loading Texture"); return false; }

    crossbow_tex = LoadTexture("assets/crossbow128.bmp", &crossbow_tex_width, &crossbow_tex_height);
    if (!crossbow_tex) { printf("Error Loading Texture"); return false; }

    place_indicator_tex = LoadTexture("assets/place_indicator.bmp", &place_indicator_tex_width, &place_indicator_tex_height);
    if (!place_indicator_tex) { printf("Error Loading Texture"); return false; }

    killed_chicken_tex = LoadTexture("assets/killed_chicken.bmp", &killed_chicken_tex_width, &killed_chicken_tex_height);
    if (!killed_chicken_tex) { printf("Error Loading Texture"); return false; }


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

    case 1: renderTexture(chicken_tex, 0 - 32, 0 - 32, 64, 64, x_offset, y_offset); break;
    
    case 2: renderTexture(boar_tex, 0 - 32, 0 - 32, 64, 64, x_offset, y_offset); break;
    

    case 101: renderTexture(killed_chicken_tex, 0 - 32, 0 - 32, 64, 64, x_offset, y_offset); break;

    }

}


void renderTower(int index, int x_offset, int y_offset) {

    switch (index) {

  //case index: renderTexture(SDL_Texture, X Coordinate, Y Coordinate, width, height, X Offset, Y Offset)

    case 1: renderTexture(cannon_tex, 0 - 64, 0 - 64, 128, 128, x_offset, y_offset); break;

    case 2: renderTexture(crossbow_tex, 0 - 64, 0 - 64, 128, 128, x_offset, y_offset); break;

    case 3: renderTexture(place_indicator_tex, 0, 0, 1300, 900, 0, 0); break;
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

    SDL_DestroySurface(textSurface);
    SDL_DestroyTexture(textTexture);
}


void renderClear(void) {
    SDL_RenderClear(renderer);
}


void renderStatic(void) {
    renderTexture(map_tex, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);
}


void renderPresent(void) {
    SDL_RenderPresent(renderer);
}



void quitSDL(void) {

    if (font) { TTF_CloseFont(font); }
    TTF_Quit();
    SDL_DestroyTexture(map_tex);
    SDL_DestroyTexture(chicken_tex);
    SDL_DestroyTexture(boar_tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
}
