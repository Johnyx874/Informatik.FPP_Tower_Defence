// structs.h
#pragma once

#include <stdbool.h>
#include <SDL3/SDL.h>


// input management
typedef struct {

    float x_mouse_position, y_mouse_position;

    bool quit;
    
    bool key_c;
    bool key_1;
    bool key_2;
    bool key_space;
    bool key_escape;

    bool button_left;
} InputState;

// Struktur zur Verwaltung des Placing Systems
typedef struct {

    bool cannon;
    bool crossbow;
    
} Place;

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
    int health;

} EntityData;

// Struktur für einen Tower
typedef struct {

    char type[50];            // Type des Towers
    int textureIndex;         // Von dem Tower verwendete Textur
    Vector2 position;         // Aktuelle Position
    
    //int speed;                // Angriffsgeschwindigkeit ?
    
} TowerData;