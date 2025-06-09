// main_engine.c

#include "library.h"		// Zugriff auf Header-Datei

#include <stdio.h>		// Standart I/O Library
#include <stdbool.h>		// Verwenden von Boolians


void GameLoop() {

    bool running = true;

    while (running) {

        running = quitGame(running);


		renderClear();

		renderStatic();

        entityManager();

		renderPresent();


		countFrame();

        SDL_Delay(20);

    }
}


int main() {

	// Initialisierung und Texturen laden
	InitApp();

	// Game Loop
	GameLoop();

	// Anzeigen von insgesamt gerenderten Frames
	totalFrames();
	
}
