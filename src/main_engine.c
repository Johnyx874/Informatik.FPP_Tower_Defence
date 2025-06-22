// main_engine.c

#include <stdio.h>		// Standart I/O Library
#include <stdbool.h>		// Verwenden von Boolians
#include <stdlib.h>		// Verwendung von system();

#include <SDL3/SDL.h>

#include "../include/structs.h"		
#include "../include/library.h"
	bool running_first_frame = true;
#include "../include/graphics_engine.h"
#include "../include/entity_engine.h"
#include "../include/tower_engine.h"
#include "../include/player_engine.h"
#include "../include/level_engine.h"

// Game Loop ¯\_(ツ)_/¯
void GameLoop(void) {

    bool running = true;

    while (running) {

		InputState input = { 0 };	// alle Inputs reseten
		checkEvents(&input);		// nach Inputs suchen

		if (input.quit) { running = false; }		// spiel bei Quit-Input beenden

		renderClear();	// Bildschirm leeren

		renderStatic();	// Statische Objekte rendern

		levelManager();	// Level berechnen

		entityManager();	// Entitys berechnen

		towerManager(input); // Tower berechnen

		playerManager(); // Spielerdaten berechnen

		renderPresent();	// Bildschirm füllen

		running_first_frame = false;	// erster Frame ab hier vorbei

		countFrame();	// gerenderten Frame zählen


        SDL_Delay(20); // Delay um menschliche Wahrnehmungsrate nicht zu überfordern

    }
}

// Haubtfunktion
int main(void) {

	// SDL hochfahren
	startSDL();

	// Game Loop ausführen
	GameLoop();

	// SDL herunterfahren
	quitSDL();

	// Anzeigen von insgesamt gerenderten Frames
	totalFrames();
	
}
