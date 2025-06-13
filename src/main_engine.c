// main_engine.c

#include "../include/library.h"		// Zugriff auf Header-Datei
bool running_first_frame;
bool spawnEntity_this_frame;
bool running = true;

#include <stdio.h>		// Standart I/O Library
#include <stdbool.h>		// Verwenden von Boolians


void GameLoop() {

    running = true;

    while (running) {

		spawnEntity_this_frame = false;


		checkEvents();

		renderClear();

		renderStatic();

        entityManager();

		renderPresent();


		countFrame();


		running_first_frame = false;


        SDL_Delay(20);

    }
}


int main() {

	// Initialisierung und Texturen laden
	InitApp();

	running_first_frame = true;

	// Game Loop
	GameLoop();

	// Anzeigen von insgesamt gerenderten Frames
	totalFrames();
	
}
