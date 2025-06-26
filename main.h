#pragma once
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GRAVITY 0.1
#define FLOOR SCREEN_HEIGHT-80

#define STIFFNESS 0.9
#define DAMP_COEF 0.01

#define ROWS 10
#define COLS 3
#define SPACING 30.0
#define OFFSETX 100
#define OFFSETY 100

typedef struct {
    float x;
    float y;
} vector2_t;

typedef struct {
    int mass;
    vector2_t velocity;
    vector2_t accelleration;
    vector2_t center;
    int halfSideLen;
} node_t;

typedef struct {
    float restLength;
    float currentLength;
    float stiffness;
    float dampCoef;
    node_t *node1;
    node_t *node2;
} spring_t;

typedef struct {
    spring_t *springs;
    node_t *nodes;
} softbody_t;

