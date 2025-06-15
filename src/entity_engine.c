// entity_engine.c

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "../include/structs.h"
#include "../include/library.h"
bool running_first_frame;

#include "../include/entity_engine.h"
#include "../include/graphics_engine.h"
#include "../include/tower_engine.h"

// Definition der Punkte des Pfads
Vector2 pathPoints[] = {
    {390, 590},
    {390, 380},
    {640, 380},
    {640, 130},
    {960, 130},
    {960, 720},
    {1170, 720},
    {1170, 440},
    {1290, 440}
};

// Initialisierung des Pfads
Path path = { pathPoints, 9 };


EntityData entities[MAX_ENTITIES];  // Array zur Speicherung mehrerer Entitys
int entityCount = 0;                // Aktuelle Anzahl an Entitys

EntityData theChicken = { "chicken", 1, {0, 590}, 0, 5};
EntityData theSecond = { "second", 2, {0, 590}, 0, 10};


// Bewegt eine einzelne Entity entlang des Pfads
void moveAlongPath(EntityData* e) {
    if (path.points == NULL || path.count <= 0) return;
    if (e->currentTargetIndex >= path.count) return;

    Vector2 target = path.points[e->currentTargetIndex];

    Vector2 dir = {
        target.x - e->position.x,
        target.y - e->position.y
    };

    if (dir.x != 0) dir.x = (dir.x > 0) ? 1 : -1;
    if (dir.y != 0) dir.y = (dir.y > 0) ? 1 : -1;

    e->position.x += dir.x * e->speed;
    e->position.y += dir.y * e->speed;

    bool reachedX = (dir.x > 0 && e->position.x >= target.x) ||
        (dir.x < 0 && e->position.x <= target.x) ||
        (dir.x == 0);

    bool reachedY = (dir.y > 0 && e->position.y >= target.y) ||
        (dir.y < 0 && e->position.y <= target.y) ||
        (dir.y == 0);

    if (reachedX && reachedY) {
        e->position = target;
        e->currentTargetIndex++;
    }
}


void moveEntity(EntityData e) {

    for (int i = 0; i <= entityCount; i++) {

        if (strcmp(e.type, entities[i].type) == 0) {
            moveAlongPath(&entities[i]);
            renderEntity(e.textureIndex, entities[i].position.x, entities[i].position.y);
            //printf("  - Entity moved\n");
        }
    }
}

// Fügt ein neues Entity zum Entity-Array hinzu
void spawnEntity(EntityData e, int offset) {

    if (entityCount >= MAX_ENTITIES) {    // Sicherheitscheck
        printf("Too much Entitys!\n");
        return;
    }

    if (running_first_frame) {

        strcpy_s(entities[entityCount].type, 50, e.type);

        entities[entityCount].position.x = e.position.x - offset;      // Startposition setzen
        entities[entityCount].position.y = e.position.y;

        entities[entityCount].currentTargetIndex = 0;      // Beginnt bei Wegpunkt 0
        entities[entityCount].speed = e.speed;            // Geschwindigkeit setzen
        entityCount++;                                     // Anzahl erhöhen

        printf("  - Entity added\n");
    }
}


void spawnAndCloneEntity(EntityData e, int amount, int spacing) {

    for (int i = 0; i < amount; i++) {

        spawnEntity(e, i * spacing);
    }
}


void deleteEntity(EntityData* e) {

    strcpy_s(e->type, 50, "");
    e->textureIndex = 0;
    e->position.x = 0;
    e->position.y = 0;
    e->currentTargetIndex = 0;
    e->speed = 0;
    e->health = 0;

}

// Dummy-Funktion für eine Kanone (kann später erweitert werden)
//void cannonBrain(int x_position, int y_position) {
//
//    renderEntity(2, x_position, y_position);
//}


// Haupt-Entity-Manager: verwaltet alle "Brains"
void entityManager(InputState input) {

    spawnAndCloneEntity(theChicken, 5, 100);

    if (input.spawnEntity) {

        deleteEntity(&entities[0]);
    }

    moveEntity(theChicken);
    
    /*float distance = getDistanceAB(entities[0].position, entities[1].position);

    system("cls");
    printf("     %f     \n", distance);*/

    //printf("-------- entityManger completed--------\n\n");
}
