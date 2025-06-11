// entity_engine.c

#include "library.h"
bool running_first_frame;

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_ENTITIES 10  // Maximale Anzahl an Entitäten
#define ENTITY_SPACING 100 // Abstand in Pixeln zwischen den Startpositionen der Hühner

// Struktur für einen 2D-Vektor
typedef struct {
    int x, y;
} Vector2;

// Struktur für einen Pfad mit mehreren Punkten
typedef struct {
    Vector2* points;
    int count;
} Path;

// Struktur für ein einzelnes Entity
typedef struct {

    char type[50];            // Type des Entitys
    int textureIndex;         // Von dem Entity verwendete Textur
    Vector2 position;         // Aktuelle Position
    int currentTargetIndex;   // Index des nächsten Wegpunkts
    int speed;                // Bewegungsgeschwindigkeit (Pixel pro Frame)

} EntityData;


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

//EntityData theChicken = { "chicken", { 0, 590 }, 0, 5 };
EntityData theChicken = { "chicken", 1, {0, 590}, 0, 5};
EntityData theSecond = { "second", 2, {0, 590}, 0, 10 };

// Fügt ein neues Entity zum Entity-Array hinzu
void addEntity(char *type, Vector2 position, int speed) {
    if (entityCount >= MAX_ENTITIES) return;  // Sicherheitscheck

    

    strcpy_s(entities[entityCount].type, 50, type);
    //entities[entityCount].type = type;
    entities[entityCount].position = position;      // Startposition setzen
    entities[entityCount].currentTargetIndex = 0;      // Beginnt bei Wegpunkt 0
    entities[entityCount].speed = speed;            // Geschwindigkeit setzen
    entityCount++;                                     // Anzahl erhöhen

}


// Bewegt eine einzelne Entity entlang des Pfads
void moveEntityAlongPath(EntityData* e) {
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


//void spawnAndCloneEntity(EntityData* e, int amount, int spacing) {
//
//    spawnEntity(&e);
//
//    /*
//    // OPTION A: 10 Hühner mit Abstand generieren (einmalig bei Start)
//    if (MAX_ENTITIES - amount >= 0) {  // Verhindert mehrfaches Hinzufügen
//        for (int i = 0; i < amount; i++) {
//
//            Vector2 startPos = { 0 - i * spacing, 590 };  // Abstand auf X-Achse
//
//            addEntity(startPos, 5);
//        }
//    }
//    */
//
//}


void spawnEntity(EntityData* e) {

    char type[50]; strcpy_s(type, 50, e->type);
    Vector2 position = e->position; 
    int speed = e->speed;               

    if (running_first_frame) {
        addEntity(type, position, speed);
        printf("Entity added\n");
    }
    
    for (int i = 0; i <= entityCount; i++) {

        //if (entities[i].type == e->type) {
        if (strcmp(e->type, entities[i].type) == 0) {
            moveEntityAlongPath(&entities[i]);
            renderEntity(e->textureIndex, entities[i].position.x - 32, entities[i].position.y - 32);
            printf("Entity moved\n");
        }


        /*int test = strcmp(e->type, entities[i].type);

        printf("i = %d, entityCount = %d\n", i, entityCount);
        printf("strcmp output: %d\n", test);
        printf("e->type: %s\n", e->type);
        printf("entities[i].type: %s\n\n", entities[i].type);*/
    }
}


 //Logik für alle Hühner aufrufen (mehrere gleichzeitig)
//void chickenBrain() {
//
//    for (int i = 0; i < entityCount; i++) {
//
//        moveEntityAlongPath(&entities[i]);
//
//        
//    }
//}


// Dummy-Funktion für eine Kanone (kann später erweitert werden)
void cannonBrain(int x_position, int y_position) {

    renderEntity(2, x_position, y_position);
}


// Haupt-Entity-Manager: verwaltet alle "Brains"
void entityManager() {

    spawnEntity(&theChicken);
    spawnEntity(&theSecond);

    /*
    cloneEntity(5, 100);

    chickenBrain();

    cannonBrain(500, 500);
    */

    printf("------------------\n");
}
