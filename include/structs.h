// structs.h
#pragma once

#include <stdbool.h>

// event_engine.h
typedef struct {
    bool quit;
    bool spawnEntity;
    bool space_pressed;
    bool left_click;
} InputState;

// Struktur f�r einen 2D-Vektor
typedef struct {
    int x, y;
} Vector2;

// Struktur f�r einen Pfad mit mehreren Punkten
typedef struct {
    Vector2* points;
    int count;
} Path;

// Struktur f�r ein einzelnes Entity
typedef struct {

    char type[50];            // Type des Entitys
    int textureIndex;         // Von dem Entity verwendete Textur
    Vector2 position;         // Aktuelle Position
    int currentTargetIndex;   // Index des n�chsten Wegpunkts
    int speed;                // Bewegungsgeschwindigkeit (Pixel pro Frame)
    int health;

} EntityData;

// Struktur f�r einen Tower
typedef struct {

    char type[50];            // Type des Towers
    int textureIndex;         // Von dem Tower verwendete Textur
    Vector2 position;         // Aktuelle Position
    
    //int speed;                // Angriffsgeschwindigkeit ?
    
} TowerData;