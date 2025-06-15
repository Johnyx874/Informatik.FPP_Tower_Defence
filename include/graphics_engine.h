// graphics_engine.h
#pragma once

// Funktionen
void renderClear(); // Renderer leeren
void renderStatic(); // Statische Objekte rendern (zB: Map)
void renderPresent(); // Alle geladenen Objekte rendern
void renderEntity(int index, int x_offset, int y_offset);  // die Entity-Texture Nr.'index' rendern 
void renderTower(int index, int x_offset, int y_offset); // die Tower-Texture Nr.'index' rendern 
void totalFrames(); // anzeigen von insgesamt gerenderten Frames
void countFrame(); // frame_counter + 1
bool startSDL();  // Initialisieren, Texturen laden
void quitSDL(); // Beenden des Spiels

