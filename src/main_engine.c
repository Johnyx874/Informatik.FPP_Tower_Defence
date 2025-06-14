// main_engine.c

#include "../include/library.h"		// Zugriff auf Header-Datei
bool running_first_frame;

#include "../include/structs.h"		// Zugriff auf Header-Datei

#include <stdio.h>		// Standart I/O Library
#include <stdbool.h>		// Verwenden von Boolians


void GameLoop() {

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


        SDL_Delay(20);

    }
}


int main() {

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
