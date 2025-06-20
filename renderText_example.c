#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string.h>
#include <stdbool.h>

static TTF_Font* font = NULL;
static SDL_Color white = { 255, 255, 255, 255 };
static SDL_Renderer* renderer = NULL;

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
        SDL_DestroySurface(textSurface);
        return;
    }

    int x_final = center ? x - (textSurface->w / 2) : x;
    SDL_FRect dst_rect = { x_final, y, textSurface->w, textSurface->h };
    SDL_RenderTexture(renderer, textTexture, NULL, &dst_rect);

    SDL_DestroySurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

int main(void) {

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("TTF_Init fehlgeschlagen: %s\n", SDL_GetError());
        return false;
    }

    if (!TTF_Init()) {
        SDL_Log("TTF_Init fehlgeschlagen: %s\n", SDL_GetError());
        return false;
    }

    font = TTF_OpenFont("your font path", 50);
    if (!font) {
        SDL_Log("Fehler beim Laden der Schriftart: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Textanzeige", 800, 600, 0);
    renderer = SDL_CreateRenderer(window, NULL);

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);
    renderText("Hallo SDL3!", 400, 280, true);
    SDL_RenderPresent(renderer);

    SDL_Delay(2000);

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
