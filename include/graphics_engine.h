// graphics_engine.h
#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#define MAX_FRAME_TRACKERS 64

#define passedFrames(waitFrames) passedFramesInternal(__LINE__, waitFrames)

// Funktionen
bool passedFramesInternal(int id, int waitFrames);
void renderClear(); // Renderer leeren
void renderStatic(); // Statische Objekte rendern (zB: Map)
void renderPresent(); // Alle geladenen Objekte rendern
void renderEntity(int index, int x_offset, int y_offset);  // die Entity-Texture Nr.'index' rendern 
void renderTower(int index, int x_offset, int y_offset);   // die Tower-Texture Nr.'index' rendern 
void renderIndicator(int index, int x_offset, int y_offset);   // die Range-Textur Nr. 'index' rendern
void renderText(const char* message, int x, int y, bool center); // den Text 'message' rendern 
void totalFrames(); // anzeigen von insgesamt gerenderten Frames
void countFrame(); // frame_counter + 1
bool startSDL();  // Initialisieren, Texturen laden
void quitSDL(); // Beenden des Spiels


