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


Place place;

TowerData towers[MAX_TOWERS];
int towerCount = 0;

TowerData activeTowers[MAX_ACTIVE_TOWERS];
int activeTowerCount = 0;

TowerData cannon = { "cannon", 1, {0, 0} };
TowerData crossbow = { "crossbow", 2, {0, 0} };

// Abstand zwischen Punkt A & B berechnen
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

// Tower zur generellen Towerliste hinzufügen
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

// Tower zur Liste der aktiven Towers hinzufügen
void addToActiveTowers(TowerData t) {

	if (activeTowerCount >= MAX_ACTIVE_TOWERS) {
		printf("Too many active towers!\n");
		return;
	}

	activeTowers[activeTowerCount] = t;
	activeTowerCount++;
}


void cannonBrain(int index) {

	for (int e = 0; e < entityCount; e++) {

		if (entities[e].textureIndex == 1) {

			float distance = getDistanceAB(towers[index].position, entities[e].position);

			if (distance <= 200) {
				deleteEntity(&entities[e]);
			}

			printf("%f\n", distance);
		}
	}

}


// Liste der aktiven Towers durchgehen und alle Einträge berechnen
void processActiveTowers(void) {

	for (int i = 0; i < activeTowerCount; i++) {		// Liste der aktiven Towers durchgehen
		
		if (strcmp(activeTowers[i].type, "cannon") == 0) {  // wenn aktiver Tower == Cannon
			cannonBrain(i);									// dann führe dessen Funktion aus
		}

		// aktiven Tower rendern
		renderTower(activeTowers[i].textureIndex, activeTowers[i].position.x, activeTowers[i].position.y);
	}

}


// Platziere Tower mit Maus
void placeTower(TowerData t, InputState input) {

	int unused_tower = -1;

	for (int i = 0; i <= towerCount; i++) {

		if (strcmp(towers[i].type, t.type) == 0) {
			if (towers[i].position.x == 0 && towers[i].position.y == 0) {
				unused_tower = i;
				break;
			}
		}
	}
	if (unused_tower == -1) { place = (Place){ 0 }; }


	if (unused_tower >= 0) {
		renderTower(towers[unused_tower].textureIndex, input.x_mouse_position, input.y_mouse_position);
	}
	
	if (input.button_left) {
		if (unused_tower >= 0) {

			towers[unused_tower].position.x = input.x_mouse_position;
			towers[unused_tower].position.y = input.y_mouse_position;

			addToActiveTowers(towers[unused_tower]);

			place = (Place){ 0 };
		}
	}
}

// Verwaltung des Tower Placing Systems
void placeController(InputState input) {

	// Wenn ESC gedrückt: Platzieren beenden
	if (input.key_escape) {
		place = (Place){ 0 }; // (Place) wichtig für Compiler, handelt sich um Place-Struct
	}

	// Wenn 1 gedrückt: Cannon Tower platzieren
	if (input.key_1) {
		place.cannon = true;
	}
	if (place.cannon) { placeTower(cannon, input); }

	// Wenn 2 gedrückt: Crossbow Tower platzieren
	if (input.key_2) {
		place.crossbow = true;
	}
	if (place.crossbow) { placeTower(crossbow, input); }
	
}

// Tower verwalten
void towerManager(InputState input) {

	addTower(cannon, 5);
	
	addTower(crossbow, 5);

	placeController(input);

	processActiveTowers();
}