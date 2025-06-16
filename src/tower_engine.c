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
	bool place = true;
	float old_x = 0;
	float old_y = 0;
#include "../include/entity_engine.h"	
	int entityCount;
#include "../include/graphics_engine.h"	
#include "../include/tower_engine.h"	

TowerData towers[MAX_TOWERS];
int towerCount = 0;

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


void positionTowers(InputState input) {


	float x_mouse_position, y_mouse_position;
	
	if (input.left_click) {
		place = false;
	}

	if (place) {

		uint32_t buttons = SDL_GetMouseState(&x_mouse_position, &y_mouse_position);

		old_x = x_mouse_position;
		old_y = y_mouse_position;

		renderTower(towers[0].textureIndex, x_mouse_position, y_mouse_position);

		towers[0].position.x = x_mouse_position;
		towers[0].position.y = y_mouse_position;
	}
	else {
		renderTower(towers[0].textureIndex, old_x, old_y);

		towers[0].position.x = old_x;
		towers[0].position.y = old_y;
	}
	
	

	/*x = x / 10000000;
	y = y / 10000000;*/

	
	//printf("X: %f Y: %f\n", x_mouse_position, y_mouse_position);

	/*for (int i = 0; i <= towerCount; i++) {

		if (strcmp(towers[i].type, "") != 0) {

			renderTower(towers[i].textureIndex, towers[i].position.x, towers[i].position.y);
		}
	}*/
}


void addTower(TowerData t, int x_position, int y_position) {

	if (running_first_frame) {

		if (towerCount >= MAX_TOWERS) {    // Sicherheitscheck
			printf("Too much Towers!\n");
			return;
		}

		// Daten auf Listeneintrag kopieren
		strcpy_s(towers[towerCount].type, 50, t.type);
		towers[towerCount].textureIndex = t.textureIndex;
		towers[towerCount].position.x = x_position;
		towers[towerCount].position.y = y_position;

		towerCount++;	// Anzahl erhöhen
	}                                    
}


void everyDistance(void) {

	for (int Ti = 0; Ti < towerCount; Ti++) {
		for (int Ei = 0; Ei < entityCount; Ei++) {

			float distance = getDistanceAB(towers[Ti].position, entities[Ei].position);

			//printf("\nTower %d -> Entity %d = %f\n", Ti, Ei, distance);

			if (distance <= 110) {
				deleteEntity(&entities[Ei]);
			}
		}
	}
}


void towerManager(InputState input) {

	addTower(cannon, 600, 500);

	positionTowers(input);

	if (!place) {
		everyDistance();
	}
	

	
	

	//placeTower(crossbow, 1000, 100);
	
}