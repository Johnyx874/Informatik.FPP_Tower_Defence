// tower_engine.c

// Zugriff auf Bibliothecken
#include <stdio.h>		// Standart I/O Library
#include <stdbool.h>		// Verwenden von Boolians
#include <string.h>		// Verwenden von Strings
#include <math.h>	// Verwenden von sqrt();

#include <SDL3/SDL.h>

// Zugriff auf eigene Header
#include "../include/structs.h"
#include "../include/library.h"		
	bool running_first_frame;
	bool placing = false;
	float old_x = 0;
	float old_y = 0;
#include "../include/entity_engine.h"	
	int entityCount;
#include "../include/graphics_engine.h"	
#include "../include/tower_engine.h"	


TowerData towers[MAX_TOWERS];
int towerCount = 0;

TowerData activeTowers[MAX_ACTIVE_TOWERS];
int activeTowerCount = 0;

TowerData cannon = { "cannon", 1, {0, 0} };
TowerData crossbow = { "crossbow", 2, {0, 0} };


float getDistanceAB(Vector2 A, Vector2 B) {

	Vector2 C;

	C.x = A.x;
	C.y = B.y;

	int dis_ac = A.y - C.y;
	int dis_bc = B.x - C.x;

	if (dis_ac < 0) { dis_ac *= -1; }
	if (dis_bc < 0) { dis_bc *= -1; }

	float dis_ab = sqrt( (dis_ac * dis_ac) + (dis_bc * dis_bc) );

	return dis_ab;
}


void addTower(TowerData t, int amount) {

	if (running_first_frame) {

		for (int i = 0; i < amount; i++) {

			if (towerCount >= MAX_TOWERS) {    // Sicherheitscheck
				printf("Too much Towers!\n");
				return;
			}

			// Daten auf Listeneintrag kopieren
			strcpy_s(towers[towerCount].type, 50, t.type);
			towers[towerCount].textureIndex = t.textureIndex;
			towers[towerCount].position.x = 0;
			towers[towerCount].position.y = 0;

			towerCount++;	// Anzahl erhöhen
		}
		
	}                                    
}


void everyDistance(void) {

	for (int Ti = 0; Ti < activeTowerCount; Ti++) {
		for (int Ei = 0; Ei < entityCount; Ei++) {

			float distance = getDistanceAB(activeTowers[Ti].position, entities[Ei].position);

			//printf("\nTower %d -> Entity %d = %f\n", Ti, Ei, distance);

			if (distance <= 110) {
				deleteEntity(&entities[Ei]);
			}
		}
	}
}


void addToActiveTowers(TowerData t) {

	if (activeTowerCount >= MAX_ACTIVE_TOWERS) {
		printf("Too many active towers!\n");
		return;
	}

	activeTowers[activeTowerCount] = t;
	activeTowerCount++;
}


void processActiveTowers(void) {

	for (int i = 0; i < activeTowerCount; i++) {
		
		renderTower(activeTowers[i].textureIndex, activeTowers[i].position.x, activeTowers[i].position.y);
		
	}

	// Liste leeren nach Bearbeitung
	/*activeTowerCount = 0;*/
}


void placeController(InputState input) {

	if (input.key_1) {
		placing = true;
		
	}

	if (placing) {
		
		int unused_tower = -1;

		for (int i = 0; i <= towerCount; i++) {

			if (strcmp(towers[i].type, "cannon") == 0) {
				if (towers[i].position.x == 0 && towers[i].position.y == 0) {
					unused_tower = i;
					break;
				}
			}
		}
		
		if (unused_tower >= 0) {
			renderTower(towers[unused_tower].textureIndex, input.x_mouse_position, input.y_mouse_position);	
		}	

		if (input.button_left) {
			if (unused_tower >= 0) {

				towers[unused_tower].position.x = input.x_mouse_position;
				towers[unused_tower].position.y = input.y_mouse_position;

				addToActiveTowers(towers[unused_tower]);
			}

			placing = false;
		}

		if (input.key_escape) {
			placing = false;
		}
	}
}


void towerManager(InputState input) {

	addTower(cannon, 5);
	
	placeController(input);

	processActiveTowers();

	everyDistance();
}