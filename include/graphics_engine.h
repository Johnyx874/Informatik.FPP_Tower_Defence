// graphics_engine.h

#pragma once

void renderClear(); // Renderer leeren

void renderStatic(); // Statische Objekte rendern (zB: Map)

void renderPresent(); // Alle geladenen Objekte rendern

void renderEntity(int index, int x_offset, int y_offset);  // das Entity Nr.'index' rendern 

void totalFrames(); // anzeigen von insgesamt gerenderten Frames

void countFrame(); // frame_counter + 1

void quitSDL(); // Beenden des Spiels

bool startSDL();  // Initialisieren, Texturen laden
