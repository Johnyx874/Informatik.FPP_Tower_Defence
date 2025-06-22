// entity_engine.c

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "../include/structs.h"
#include "../include/library.h"
    bool add_entities;

#include "../include/entity_engine.h"
#include "../include/graphics_engine.h"
#include "../include/tower_engine.h"
#include "../include/player_engine.h"

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

    4,                   // Speed
    15,                  // Health
    2,                   // Bonus
    false,               // Fliegt?
    false               // Gepanzert?

};
EntityData theBoar = { 
    "boar",   // Type
    2,          // Texture Index
    {0, 590},   // Starting Position
    0,          // Next Waypoint Index (= currentTargetIndex)

    3,         // Speed
    40,         // Health
    5,         // Bonus
    false,               // Fliegt?
    false               // Gepanzert?

};
EntityData theHopper = {
    "hopper",   // Type
    3,          // Texture Index
    {0, 590},   // Starting Position
    0,          // Next Waypoint Index (= currentTargetIndex)

    5,         // Speed
    10,         // Health
    4,         // Bonus
    false,               // Fliegt?
    false               // Gepanzert?

};
EntityData theBallon = {
    "ballon",   // Type
    4,          // Texture Index
    {0, 590},   // Starting Position
    0,          // Next Waypoint Index (= currentTargetIndex)

    3,         // Speed
    30,         // Health
    8,         // Bonus
    true,               // Fliegt?
    false               // Gepanzert?

};
EntityData theCar = {
    "car",   // Type
    5,          // Texture Index
    {0, 590},   // Starting Position
    0,          // Next Waypoint Index (= currentTargetIndex)

    1,         // Speed
    100,         // Health
    20,         // Bonus
    false,               // Fliegt?
    false               // Gepanzert?

};
EntityData theWarthog = {
    "warthog",   // Type
    6,          // Texture Index
    {0, 590},   // Starting Position
    0,          // Next Waypoint Index (= currentTargetIndex)

    2,         // Speed
    70,         // Health
    15,         // Bonus
    false,               // Fliegt?
    true               // Gepanzert?

};
EntityData theTitan = {
    "titan",   // Type
    7,          // Texture Index
    {0, 590},   // Starting Position
    0,          // Next Waypoint Index (= currentTargetIndex)

    4,         // Speed
    50,         // Health
    20,         // Bonus
    true,               // Fliegt?
    true               // Gepanzert?

};

// Fügt ein neues Entity zum Entity-Array hinzu
void spawnEntity(EntityData e, int offset) {

    if (entityCount >= MAX_ENTITIES) {    // Sicherheitscheck
        printf("Too much Entitys!\n");
        return;
    }

    entities[entityCount] = e;  // Sämtliche Daten von e nach entities[entityCount] kopieren

    entities[entityCount].position.x = e.position.x - offset;      // X Position ändern
    
    entityCount++;      // Anzahl erhöhen

    printf("  - Entity '%s' added\n", entities[entityCount - 1].type);  // Debug Text

    add_entities = false;
    
}


void spawnAndCloneEntity(EntityData e, int amount, int spacing) {

    for (int i = 0; i < amount; i++) {

        spawnEntity(e, i * spacing);
    }
}


void removeFromList(EntityData* list, int* counter, int indexToRemove) {
    if (indexToRemove < 0 || indexToRemove >= *counter) {
        printf("Invalid index!\n");
        return;
    }

    // Nachrutschen ab indexToRemove
    for (int i = indexToRemove; i < (*counter - 1); i++) {
        list[i] = list[i + 1];  // Kopiere nächsten Eintrag auf die aktuelle Position
    }

    // Letzten Eintrag leeren (optional, zur Sicherheit)
    memset(&list[*counter - 1], 0, sizeof(EntityData));

    // Counter anpassen
    (*counter)--;
}


void killEntity(EntityData* e, int i) {

    if (passedFrames(15)) {
        
        *e = (EntityData){ 0 };

        giveBonus(entities[i].bonus);

        removeFromList(entities, &entityCount, i);
        printf("Entity killed..\n");
    }
    else {
        renderEntity((e->textureIndex + 100), e->position.x, e->position.y);
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

        if (entities[i].health <= 0) {

            killEntity(&entities[i], i);

        }
        else {

            moveAlongPath(&entities[i]);
            renderEntity(entities[i].textureIndex, entities[i].position.x, entities[i].position.y);
            
        }
    }
}


// Haupt-Entity-Manager: verwaltet alle "Brains"
void entityManager(void) {

    //spawnAndCloneEntity(theChicken, 5, 100);
    //spawnEntity(theWarthog, 0);

    
    /*spawnAndCloneEntity(theChicken, 10, 200);*/

    //if (entityCount == 0) {
    //    if (passedFrames(150)) {

    //    }

    //}



    //if (passedFrames(2000)) {

    //    spawnAndCloneEntity(theHopper, 5, 100);

    //    if (entityCount == 0) {



    //        spawnAndCloneEntity(theChicken, 10, 100);
    //        spawnEntity(theBoar, 0);
    //    }
    //}






    //if (level.3) {

    //    spawnAndCloneEntity(theChicken, 15, 0);

    //}
    //if (level.4) {

    //    spawnAndCloneEntity(theChicken, 20, 100);

    //    spawnAndCloneEntity(theBallon, 3, 100);

    //    // wait 5 sec

    //    spawnAndCloneEntity(theChicken, 20, 50);

    //    spawnAndCloneEntity(theHopper, 7, 100);
    //}
    //if (level.5) {

    //    spawnAndCloneEntity(theWarthog, 5, 100);
    //}
    //if (level.6) {

    //    spawnAndCloneEntity(theCar, 3, 200);

    //    // wait 5 sec

    //    spawnAndCloneEntity(theChicken, 30, 50);
    //}
    //if (level.7) {

    //    spawnAndCloneEntity(theWarthog, 12, 200);

    //    spawnAndCloneEntity(theBallon, 40, 50);

    //    // wait 5 sec

    //    spawnAndCloneEntity(theTitan, 3, 200);
    //}



    moveEntities();

    //printf("---------\n");
    
    
}
