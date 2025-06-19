// player_engine.c

#include <stdio.h>
#include <stdbool.h>

#include "../include/structs.h"

#include "../include/entity_engine.h"
#include "../include/graphics_engine.h"

Player player = { "Player 1", 0, 100 };		// Initialisierung von player

// wenn Entity die gesamte Map durchlaufen hat, Damage von Entity auf Player übertragen
void checkDamage(void) {

	for (int i = 0; i <= entityCount; i++) {

		if (entities[i].position.x >= 1300) {

			player.health -= entities[i].damage;

			printf("HP: %d\n", player.health);

			deleteEntity(&entities[i]);
		}
	}
}

// Bonus von Entity auf Player übertragen
void giveBonus(EntityData e) {

	player.cash += e.bonus;

	printf("Cash: %d\n", player.cash);
}

// Verwaltung von player
void playerManager(void) {
	
	checkDamage();

	char buffer[64];

	snprintf(buffer, sizeof(buffer), "HP: %d", player.health);
	renderText(buffer, 70, 30, false);

	snprintf(buffer, sizeof(buffer), "Cash: %d", player.cash);
	renderText(buffer, 1000, 30, false);

}