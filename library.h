// library.h
#pragma once

#include <stdbool.h>

extern bool running_first_frame;

extern bool spawnEntity_this_frame;

bool InitApp();  // Initialisieren, Texturen laden

void GameLoop(); // Haubt Game Loop

void checkEvents(); // nach Events suchen

void entityManager(); // Verwalten von Entitys

void renderClear(); // Renderer leeren

void renderStatic(); // Statische Objekte rendern (zB: Map)

void renderPresent(); // Alle geladenen Objekte rendern

void renderEntity(int index, int x_offset, int y_offset);  // das Entity Nr.'index' rendern 

void totalFrames(); // anzeigen von insgesamt gerenderten Frames

void countFrame(); // frame_counter + 1

bool quitGame(bool running); // Beenden des Spiels