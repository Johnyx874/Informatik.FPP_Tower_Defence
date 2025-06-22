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
static SDL_Texture* chicken_tex = NULL; static int chicken_tex_width = 0; static int chicken_tex_height = 0;
static SDL_Texture* killed_chicken_tex = NULL; static int killed_chicken_tex_width = 0; static int killed_chicken_tex_height = 0;

static SDL_Texture* boar_tex = NULL; static int boar_tex_width = 0; static int boar_tex_height = 0;
static SDL_Texture* killed_boar_tex = NULL; static int killed_boar_tex_width = 0; static int killed_boar_tex_height = 0;

static SDL_Texture* hopper_tex = NULL; static int hopper_tex_width = 0; static int hopper_tex_height = 0;
static SDL_Texture* killed_hopper_tex = NULL; static int killed_hopper_tex_width = 0; static int killed_hopper_tex_height = 0;

static SDL_Texture* ballon_tex = NULL; static int ballon_tex_width = 0; static int ballon_tex_height = 0;
static SDL_Texture* killed_ballon_tex = NULL; static int killed_ballon_tex_width = 0; static int killed_ballon_tex_height = 0;

static SDL_Texture* car_tex = NULL; static int car_tex_width = 0; static int car_tex_height = 0;
static SDL_Texture* killed_car_tex = NULL; static int killed_car_tex_width = 0; static int killed_car_tex_height = 0;

static SDL_Texture* warthog_tex = NULL; static int warthog_tex_width = 0; static int warthog_tex_height = 0;
static SDL_Texture* killed_warthog_tex = NULL; static int killed_warthog_tex_width = 0; static int killed_warthog_tex_height = 0;

static SDL_Texture* titan_tex = NULL; static int titan_tex_width = 0; static int titan_tex_height = 0;
static SDL_Texture* killed_titan_tex = NULL; static int killed_titan_tex_width = 0; static int killed_titan_tex_height = 0;


static SDL_Texture* crossbow_tex = NULL; static int crossbow_tex_width = 0; static int crossbow_tex_height = 0;
static SDL_Texture* gray_crossbow_tex = NULL; static int gray_crossbow_tex_width = 0; static int gray_crossbow_tex_height = 0;

static SDL_Texture* cannon_tex = NULL; static int cannon_tex_width = 0; static int cannon_tex_height = 0;
static SDL_Texture* gray_cannon_tex = NULL; static int gray_cannon_tex_width = 0; static int gray_cannon_tex_height = 0;

static SDL_Texture* minigun_tex = NULL; static int minigun_tex_width = 0; static int minigun_tex_height = 0;
static SDL_Texture* gray_minigun_tex = NULL; static int gray_minigun_tex_width = 0; static int gray_minigun_tex_height = 0;

static SDL_Texture* launcher_tex = NULL; static int launcher_tex_width = 0; static int launcher_tex_height = 0;
static SDL_Texture* gray_launcher_tex = NULL; static int gray_launcher_tex_width = 0; static int gray_launcher_tex_height = 0;

static SDL_Texture* saw_tex = NULL; static int saw_tex_width = 0; static int saw_tex_height = 0;
static SDL_Texture* gray_saw_tex = NULL; static int gray_saw_tex_width = 0; static int gray_saw_tex_height = 0;

static SDL_Texture* sniper_tex = NULL; static int sniper_tex_width = 0; static int sniper_tex_height = 0;
static SDL_Texture* gray_sniper_tex = NULL; static int gray_sniper_tex_width = 0; static int gray_sniper_tex_height = 0;

static SDL_Texture* map_tex = NULL; static int map_tex_width = 0; static int map_tex_height = 0;

static SDL_Texture* place_indicator_tex = NULL; static int place_indicator_tex_width = 0; static int place_indicator_tex_height = 0;

static SDL_Texture* range_200_tex = NULL; static int range_200_tex_width = 0; static int range_200_tex_height = 0;


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

    font = TTF_OpenFont("assets/misc/ByteBounce.ttf", 50);
    if (!font) {
        SDL_Log("Fehler beim Laden der Schriftart: %s\n", SDL_GetError());
        return false;
    }


    

    // === Entity Texturen ===
    chicken_tex = LoadTexture("assets/entities/chicken.bmp", &chicken_tex_width, &chicken_tex_height);
    if (!chicken_tex) { printf("Error Loading Texture"); return false; }
    killed_chicken_tex = LoadTexture("assets/damaged_entities/chicken_damaged.bmp", &killed_chicken_tex_width, &killed_chicken_tex_height);
    if (!killed_chicken_tex) { printf("Error Loading Texture"); return false; }

    boar_tex = LoadTexture("assets/entities/boar.bmp", &boar_tex_width, &boar_tex_height);
    if (!boar_tex) { printf("Error Loading Texture"); return false; }
    killed_boar_tex = LoadTexture("assets/damaged_entities/boar_damaged.bmp", &killed_boar_tex_width, &killed_boar_tex_height);
    if (!killed_boar_tex) { printf("Error Loading Texture"); return false; }

    hopper_tex = LoadTexture("assets/entities/grasshopper.bmp", &hopper_tex_width, &hopper_tex_height);
    if (!hopper_tex) { printf("Error Loading Texture"); return false; }
    killed_hopper_tex = LoadTexture("assets/damaged_entities/grasshopper_damaged.bmp", &killed_hopper_tex_width, &killed_hopper_tex_height);
    if (!killed_hopper_tex) { printf("Error Loading Texture"); return false; }

    ballon_tex = LoadTexture("assets/entities/ballon.bmp", &ballon_tex_width, &ballon_tex_height);
    if (!ballon_tex) { printf("Error Loading Texture"); return false; }
    killed_ballon_tex = LoadTexture("assets/damaged_entities/ballon_damaged.bmp", &killed_ballon_tex_width, &killed_ballon_tex_height);
    if (!killed_ballon_tex) { printf("Error Loading Texture"); return false; }

    car_tex = LoadTexture("assets/entities/car.bmp", &car_tex_width, &car_tex_height);
    if (!car_tex) { printf("Error Loading Texture"); return false; }
    killed_car_tex = LoadTexture("assets/damaged_entities/car_damaged.bmp", &killed_car_tex_width, &killed_car_tex_height);
    if (!killed_car_tex) { printf("Error Loading Texture"); return false; }

    warthog_tex = LoadTexture("assets/entities/warthog.bmp", &warthog_tex_width, &warthog_tex_height);
    if (!warthog_tex) { printf("Error Loading Texture"); return false; }
    killed_warthog_tex = LoadTexture("assets/damaged_entities/warthog_damaged.bmp", &killed_warthog_tex_width, &killed_warthog_tex_height);
    if (!killed_warthog_tex) { printf("Error Loading Texture"); return false; }

    titan_tex = LoadTexture("assets/entities/Dark Dirigible Titan.bmp", &titan_tex_width, &titan_tex_height);
    if (!titan_tex) { printf("Error Loading Texture"); return false; }
    killed_titan_tex = LoadTexture("assets/damaged_entities/Dark Dirigible Titan_damaged.bmp", &killed_titan_tex_width, &killed_titan_tex_height);
    if (!killed_titan_tex) { printf("Error Loading Texture"); return false; }

    // === Tower Texturen ===
    cannon_tex = LoadTexture("assets/towers/cannon.bmp", &cannon_tex_width, &cannon_tex_height);
    if (!cannon_tex) { printf("Error Loading Texture"); return false; }
    gray_cannon_tex = LoadTexture("assets/gray_towers/gray_cannon.bmp", &gray_cannon_tex_width, &gray_cannon_tex_height);
    if (!gray_cannon_tex) { printf("Error Loading Texture"); return false; }

    crossbow_tex = LoadTexture("assets/towers/crossbow.bmp", &crossbow_tex_width, &crossbow_tex_height);
    if (!crossbow_tex) { printf("Error Loading Texture"); return false; }
    gray_crossbow_tex = LoadTexture("assets/gray_towers/gray_crossbow.bmp", &gray_crossbow_tex_width, &gray_crossbow_tex_height);
    if (!gray_crossbow_tex) { printf("Error Loading Texture"); return false; }

    minigun_tex = LoadTexture("assets/towers/minigun.bmp", &minigun_tex_width, &minigun_tex_height);
    if (!minigun_tex) { printf("Error Loading Texture"); return false; }
    gray_minigun_tex = LoadTexture("assets/gray_towers/gray_minigun.bmp", &gray_minigun_tex_width, &gray_minigun_tex_height);
    if (!gray_minigun_tex) { printf("Error Loading Texture"); return false; }

    launcher_tex = LoadTexture("assets/towers/launcher.bmp", &launcher_tex_width, &launcher_tex_height);
    if (!launcher_tex) { printf("Error Loading Texture"); return false; }
    gray_launcher_tex = LoadTexture("assets/gray_towers/gray_launcher.bmp", &gray_launcher_tex_width, &gray_launcher_tex_height);
    if (!gray_launcher_tex) { printf("Error Loading Texture"); return false; }

    saw_tex = LoadTexture("assets/towers/saw.bmp", &saw_tex_width, &saw_tex_height);
    if (!saw_tex) { printf("Error Loading Texture"); return false; }
    gray_saw_tex = LoadTexture("assets/gray_towers/gray_saw.bmp", &gray_saw_tex_width, &gray_saw_tex_height);
    if (!gray_saw_tex) { printf("Error Loading Texture"); return false; }

    sniper_tex = LoadTexture("assets/towers/sniper.bmp", &sniper_tex_width, &sniper_tex_height);
    if (!sniper_tex) { printf("Error Loading Texture"); return false; }
    gray_sniper_tex = LoadTexture("assets/gray_towers/gray_sniper.bmp", &gray_sniper_tex_width, &gray_sniper_tex_height);
    if (!gray_sniper_tex) { printf("Error Loading Texture"); return false; }



    map_tex = LoadTexture("assets/misc/map_2.0.bmp", &map_tex_width, &map_tex_height);
    if (!map_tex) { printf("Error Loading Texture"); return false; }

    place_indicator_tex = LoadTexture("assets/misc/place_indicator.bmp", &place_indicator_tex_width, &place_indicator_tex_height);
    if (!place_indicator_tex) { printf("Error Loading Texture"); return false; }

    range_200_tex = LoadTexture("assets/ranges/range_indicator_200.bmp", &range_200_tex_width, &range_200_tex_height);
    if (!range_200_tex) { printf("Error Loading Texture"); return false; }

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

    case 1: renderTexture(chicken_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;
    
    case 2: renderTexture(boar_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;
    
    case 3: renderTexture(hopper_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;

    case 4: renderTexture(ballon_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;

    case 5: renderTexture(car_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;

    case 6: renderTexture(warthog_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;

    case 7: renderTexture(titan_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;


    case 101: renderTexture(killed_chicken_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;

    case 102: renderTexture(killed_boar_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;

    case 103: renderTexture(killed_hopper_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;

    case 104: renderTexture(killed_ballon_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;

    case 105: renderTexture(killed_car_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;

    case 106: renderTexture(killed_warthog_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;

    case 107: renderTexture(killed_titan_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;

    }

}


void renderTower(int index, int x_offset, int y_offset) {

    switch (index) {

  //case index: renderTexture(SDL_Texture, X Coordinate, Y Coordinate, width, height, X Offset, Y Offset)

    case 1: renderTexture(crossbow_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;

    case 2: renderTexture(cannon_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;

    case 3: renderTexture(minigun_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;

    case 4: renderTexture(launcher_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;

    case 5: renderTexture(saw_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;

    case 6: renderTexture(sniper_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;


    case 101: renderTexture(gray_crossbow_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;

    case 102: renderTexture(gray_cannon_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;

    case 103: renderTexture(gray_minigun_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;

    case 104: renderTexture(gray_launcher_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;

    case 105: renderTexture(gray_saw_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;

    case 106: renderTexture(gray_sniper_tex, 0 - 48, 0 - 48, 96, 96, x_offset, y_offset); break;
    }
}


void renderIndicator(int index, int x_offset, int y_offset) {

    switch (index) {

        //case index: renderTexture(SDL_Texture, X Coordinate, Y Coordinate, width, height, X Offset, Y Offset)

    case 1: renderTexture(place_indicator_tex, 0, 0, 1300, 900, 0, 0); break;

    case 200: renderTexture(range_200_tex, 0 - 200, 0 - 168, 400, 400, x_offset, y_offset); break;


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
