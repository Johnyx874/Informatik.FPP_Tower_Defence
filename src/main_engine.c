// main_engine.c

#include <stdio.h>		// Standart I/O Library
#include <stdbool.h>		// Verwenden von Boolians
#include <stdlib.h>		// Verwendung von system();

#include <SDL3/SDL.h>

#include "../include/structs.h"		// Zugriff auf Header-Datei
#include "../include/library.h"		// Zugriff auf Header-Datei
	bool running_first_frame;

#include "../include/graphics_engine.h"
#include "../include/entity_engine.h"



void GameLoop(void) {

    bool running = true;

    while (running) {

		InputState input = { 0 };		// alle Inputs reseten
		checkEvents(&input);		// nach Inputs suchen

		if (input.quit) { running = false; }		// spiel bei Quit-Input beenden


		renderClear();	// Bildschirm leeren

		renderStatic();	// Statische Objekte rendern

        entityManager(input);	// Entitys berechnen und rendern

		renderPresent();	// Bildschirm füllen


		countFrame();	// gerenderten Frame zählen


		running_first_frame = false;

        SDL_Delay(50);

    }
}


int main(void) {

	// SDL hochfahren
	startSDL();

	running_first_frame = true;

	// Game Loop
	GameLoop();

	// SDL herunterfahren
	quitSDL();

	// Anzeigen von insgesamt gerenderten Frames
	totalFrames();
	
}
