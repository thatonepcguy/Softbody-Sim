#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GRAVITY 0.04
#define FLOOR SCREEN_HEIGHT-80

#define STIFFNESS 0.6
#define DAMP_COEF 0.7

#define ROWS 10
#define COLS 10
#define SPACING 30.0
#define OFFSETX 100
#define OFFSETY 100

#define FRICTION 0.9f

typedef struct {
    float x;
    float y;
} vector2_t;

typedef struct {
    union {
        int x;
        int r;
    };
    union {
        int y;
        int g;
    };
    union {
        int z;
        int b;
    };
} vector3I_t;

typedef struct {
    int mass;
    vector2_t velocity;
    vector2_t accelleration;
    vector2_t center;
    bool fixed;
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
    unsigned int springsCount;
    node_t *nodes;
    unsigned int nodesCount;
    unsigned int *edgeNodeIndexes;
    unsigned int edgeNodeCount;
    vector3I_t color;
    vector2_t boundingBoxTopLeft;
    vector2_t boundingBoxBtmRight;
} softbody_t;

