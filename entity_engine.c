// entity_engine.c

#include "library.h"
#include <stdio.h>
#include <stdbool.h>

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

// Struktur für ein einzelnes "Huhn"/Entity
typedef struct {
    Vector2 position;         // Aktuelle Position
    int currentTargetIndex;  // Index des nächsten Wegpunkts
    int speed;               // Bewegungsgeschwindigkeit (Pixel pro Frame)
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

// Array zur Speicherung mehrerer Entitys
EntityData entities[MAX_ENTITIES];
int entityCount = 0; // Aktuelle Anzahl an Entitys


// Fügt ein neues Entity zum Entity-Array hinzu
void addEntity(Vector2 startPos, int speed) {
    if (entityCount >= MAX_ENTITIES) return;  // Sicherheitscheck

    entities[entityCount].position = startPos;         // Startposition setzen
    entities[entityCount].currentTargetIndex = 0;      // Beginnt bei Wegpunkt 0
    entities[entityCount].speed = speed;               // Geschwindigkeit setzen
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


void cloneEntity(int amount, int spacing) {

    // OPTION A: 10 Hühner mit Abstand generieren (einmalig bei Start)
    if (MAX_ENTITIES - amount >= 0) {  // Verhindert mehrfaches Hinzufügen
        for (int i = 0; i < amount; i++) {

            Vector2 startPos = { 0 - i * spacing, 590 };  // Abstand auf X-Achse

            addEntity(startPos, 5);
        }
    }

}


// Logik für alle Hühner aufrufen (mehrere gleichzeitig)
void chickenBrain() {

    for (int i = 0; i < entityCount; i++) {

        moveEntityAlongPath(&entities[i]);

        printf("Got executed!");

        renderEntity(1, entities[i].position.x - 32, entities[i].position.y - 32);
    }
}


// Dummy-Funktion für eine Kanone (kann später erweitert werden)
void cannonBrain(int x_position, int y_position) {

    renderEntity(2, x_position, y_position);
}


// Haupt-Entity-Manager: verwaltet alle "Brains"
void entityManager() {

    cloneEntity(5, 100);

    chickenBrain();

    cannonBrain(500, 500);
}
