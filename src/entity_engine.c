// entity_engine.c

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "../include/structs.h"
#include "../include/library.h"

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
    
}

// spawnt [amount] Entities mit dem Abstand [spacing]
void spawnAndCloneEntity(EntityData e, int amount, int spacing) {

    for (int i = 0; i < amount; i++) {

        spawnEntity(e, i * spacing);
    }
}


void removeFromList(EntityData* list, int* counter, int indexToRemove) {
    if (indexToRemove < 0 || indexToRemove >= *counter) { // Sicherheitscheck
        printf("Invalid index!\n");
        return;
    }

    // Nachrutschen ab indexToRemove
    for (int i = indexToRemove; i < (*counter - 1); i++) {
        list[i] = list[i + 1];  // Kopiere nächsten Eintrag auf die aktuelle Position
    }

    // Letzten Eintrag leeren (zur Sicherheit)
    memset(&list[*counter - 1], 0, sizeof(EntityData));

    // Counter anpassen
    (*counter)--;
}

// killt ein Entity mit Kill-Animation
void killEntity(EntityData* e, int i) {

    if (passedFrames(15)) {     // Nach 15 Frames:
        
        giveBonus(entities[i].bonus);   // Bonus an Spieler übertragen

        *e = (EntityData){ 0 }; // Entity technisch löschen

        removeFromList(entities, &entityCount, i);  // Liste neu sortieren
        printf("Entity killed..\n");    // Debug Text
    }
    else {
        renderEntity((e->textureIndex + 100), e->position.x, e->position.y);    // Kill-Textur des Entitys anzeigen
    }
    
}


// Bewegt eine einzelne Entity entlang des Pfads
void moveAlongPath(EntityData* e) {
    // Sicherheitschecks
    if (path.points == NULL || path.count <= 0) return;
    if (e->currentTargetIndex >= path.count) return;

    // Ziel Wegpunkt festlegen
    Vector2 target = path.points[e->currentTargetIndex];

    // Richtung bestimmen
    Vector2 dir = {
        target.x - e->position.x,
        target.y - e->position.y
    };
    if (dir.x != 0) dir.x = (dir.x > 0) ? 1 : -1;
    if (dir.y != 0) dir.y = (dir.y > 0) ? 1 : -1;

    // Mit speed in die Richtung bewegen
    e->position.x += dir.x * e->speed;
    e->position.y += dir.y * e->speed;

    // Wenn Ziel Wegpunkt erreicht, Entity auf genaue Wegpunkt koordinaten setzen
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

    // Entity Liste iterieren
    for (int i = entityCount - 1; i >= 0; i--) {

        // Entity killen wenn Leben zu niedrig sind
        if (entities[i].health <= 0) {
            killEntity(&entities[i], i);
        }
        else {

            // Ansonsten Entity über Pfad bewegen
            moveAlongPath(&entities[i]);
            renderEntity(entities[i].textureIndex, entities[i].position.x, entities[i].position.y);
            
        }
    }
}


// Haupt-Entity-Manager
void entityManager(void) {

    // bewegt zurzeit nur die Entitys
    moveEntities();    
}





// This File contains Code provided by ChatGPT
