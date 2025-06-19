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
    {1350, 440}
};

// Initialisierung des Pfads
Path path = { pathPoints, 9 };


EntityData entities[MAX_ENTITIES];  // Array zur Speicherung mehrerer Entitys
int entityCount = 0;                // Aktuelle Anzahl an Entitys

// Initialisierung der Entities
EntityData theChicken = { 
    "chicken",           // Type
    1,                   // Texture Index
    {0, 590},            // Starting Position
    0,                   // Next Waypoint Index (= currentTargetIndex)

    5,                   // Speed
    10,                  // Health
    100,                 // Bonus
    10                   // Damage
};
EntityData theSecond = { 
    "second",   // Type
    2,          // Texture Index
    {0, 590},   // Starting Position
    0,          // Next Waypoint Index (= currentTargetIndex)

    10,         // Speed
    10,         // Health
    50,         // Bonus
    50          // Damage
};


// Fügt ein neues Entity zum Entity-Array hinzu
void spawnEntity(EntityData e, int offset) {

    if (entityCount >= MAX_ENTITIES) {    // Sicherheitscheck
        printf("Too much Entitys!\n");
        return;
    }

    if (running_first_frame) {

        entities[entityCount] = e;  // Sämtliche Daten von e nach entities[entityCount] kopieren

        entities[entityCount].position.x = e.position.x - offset;      // X Position ändern
    
        entityCount++;      // Anzahl erhöhen

        printf("  - Entity '%s' added\n", entities[entityCount - 1].type);  // Debug Text
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


void killEntity(EntityData* e) {

    if (passedFrames(1, 100)) {
        deleteEntity(e);
    }
    else {
        renderEntity((e->textureIndex + 1), e->position.x, e->position.y);
    }
    
}


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


// Bewegt alle Entitys eines Types
void moveEntities(void) {

    for (int i = entityCount - 1; i >= 0; i--) {

        if (entities[i].kill_it) {
            killEntity(&entities[i]);
        }
        else {
            moveAlongPath(&entities[i]);
            renderEntity(entities[i].textureIndex, entities[i].position.x, entities[i].position.y);
        }
    }
}


// Haupt-Entity-Manager: verwaltet alle "Brains"
void entityManager(InputState input) {

    spawnAndCloneEntity(theChicken, 5, 100);
    //spawnEntity(theChicken, 0);

    moveEntities();

    //spawnAndCloneEntity(theSecond, 3, 150);
    /*if (input.key_c) {

        deleteEntity(&entities[0]);
    }*/


    

    //moveEntity(theSecond);
    
    
}


// Maybe gut? Maybe nötig?
// 
//    || 
//    ||
//    \/

//void removeFromList(TowerData* list, int* counter, int indexToRemove) {
//    if (indexToRemove < 0 || indexToRemove >= *counter) {
//        printf("Invalid index!\n");
//        return;
//    }
//
//    // Nachrutschen ab indexToRemove
//    for (int i = indexToRemove; i < (*counter - 1); i++) {
//        list[i] = list[i + 1];  // Kopiere nächsten Eintrag auf die aktuelle Position
//    }
//
//    // Letzten Eintrag leeren (optional, zur Sicherheit)
//    memset(&list[*counter - 1], 0, sizeof(TowerData));
//
//    // Counter anpassen
//    (*counter)--;
//}
