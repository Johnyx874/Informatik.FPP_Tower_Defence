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

// Struktur f�r einen 2D-Vektor
typedef struct {
    int x, y;
} Vector2;

// Struktur f�r einen Pfad mit mehreren Punkten
typedef struct {
    Vector2* points;
    int count;
} Path;

// Struktur f�r Player
typedef struct {

    char name[50];            // Name des Players
    int cash;                 // Geld des Players
    int health;               // Leben des Players

} Player;

// Struktur f�r ein einzelnes Entity
typedef struct {

    char type[50];            // Type des Entitys
    int textureIndex;         // Von dem Entity verwendete Textur
    Vector2 position;         // Aktuelle Position
    int currentTargetIndex;   // Index des n�chsten Wegpunkts
    int speed;                // Bewegungsgeschwindigkeit (Pixel pro Frame)
    int health;               // Leben des Entitys
    int bonus;                // Preis f�r das besiegen eines Entitys
    int damage;               // Damage den der Spieler f�r jedes durchgelaufene Entity kassiert

    bool attr_air;            // Attribut: Fliegend
    bool attr_armored;        // Attribut: Gepanzert

    bool kill_it;             // bool, ob das Entity get�tet werden muss

} EntityData;

// Struktur f�r einen Tower
typedef struct {

    char type[50];            // Type des Towers
    char description[100];    // Beschreibung des Towers
    int textureIndex;         // Von dem Tower verwendete Textur
    Vector2 position;         // Aktuelle Position

    int price;                // Preis des Towers
    int damage;               // Schaden an Entities
    int reload_time;          // Anzahl an Frames bis Tower reloaded ist
    int range;                // Reichweite in Pixeln
    bool hits_air;            // Attribut: Fliegend
    bool hits_armored;        // Attribut: Gepanzert
    
} TowerData;