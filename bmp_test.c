#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>

#define WINDOW_WIDTH 640   // std: 640                  // Breite & Höhe des Fensters festlegen
#define WINDOW_HEIGHT 480  // std: 480

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

void renderedFrames() {

    printf("----------------------------------------------------------\n");
    if (frame_counter == 100000) { printf("\n   Anzahl gerenderter Frames: >100000"); }
    else { printf("\n   Anzahl gerenderter Frames: %d\n", frame_counter); }
    printf("\n----------------------------------------------------------");

    fflush(stdout);
}


// Hilfsfunktion zum Laden einer BMP-Datei als Textur
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

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("Centered Texture Example", "1.0", "com.example.centeredtexture");  // Metadaten des Programms festlegen

    if (!SDL_Init(SDL_INIT_VIDEO)) {                         // Initialisieren von SDL_INIT_VIDEO
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Centered BMP", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {  // Erstellen des Fensters und des Renderers
        SDL_Log("CreateWindowAndRenderer failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Textur 1 laden
    texture1 = LoadTexture("D:/Informatik/Additional Files/Images/test_map.bmp", &texture1_width, &texture1_height);
    if (!texture1) return SDL_APP_FAILURE;

    // Textur 2 laden
    texture2 = LoadTexture("D:/Informatik/Additional Files/Images/testing_shape.bmp", &texture2_width, &texture2_height);
    if (!texture2) return SDL_APP_FAILURE;

    // Textur 3 laden
    texture3 = LoadTexture("D:/Informatik/Additional Files/Images/sample.bmp", &texture3_width, &texture3_height);
    if (!texture3) return SDL_APP_FAILURE;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)  // Beenden des Programms bei Quit-Event
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    SDL_FRect dst_rect;     // Platzhalter für Position & Größe der Textur

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  // Hintergrundfarbe des Renderers festlegen
    SDL_RenderClear(renderer);

    // -------- Textur 1: zentriert zeichnen --------
    dst_rect.x = ((float)(WINDOW_WIDTH - texture1_width)) / 2.0f;        // Abstand von Linker Fensterseite       
    dst_rect.y = ((float)(WINDOW_HEIGHT - texture1_height)) / 2.0f;      // Abstand von Oberer Fensterseite
    dst_rect.w = (float)texture1_width;                                  // Breite        
    dst_rect.h = (float)texture1_height;                                 // Höhe
    SDL_RenderTexture(renderer, texture1, NULL, &dst_rect);              // Textur auf den Renderer übertragen

    // -------- Textur 2: links oben anzeigen --------
    dst_rect.x = 20.0f;
    dst_rect.y = 20.0f;
    dst_rect.w = (float)texture2_width;
    dst_rect.h = (float)texture2_height;
    SDL_RenderTexture(renderer, texture2, NULL, &dst_rect);

    // -------- Textur 3: links oben anzeigen --------
    dst_rect.x = 500.0f;
    dst_rect.y = 20.0f;
    dst_rect.w = (float)(5 * texture3_width);
    dst_rect.h = (float)(5 * texture3_height);
    SDL_RenderTexture(renderer, texture3, NULL, &dst_rect);

    SDL_RenderPresent(renderer);   // Alles auf dem Bildschirm anzeigen

    if (frame_counter < 100000) { frame_counter++; }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)     // Texturen bei Programm-Ende löschen
{
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
    SDL_DestroyTexture(texture3);

    renderedFrames();
}
