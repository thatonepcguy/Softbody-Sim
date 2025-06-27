#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GRAVITY 0.04f
#define FLOOR SCREEN_HEIGHT

#define REPEL_FORCE 1.0f
#define SELF_REPEL_FORCE 10.0f
#define SELF_COLLISION_RADIUS 3

#define STIFFNESS 1.0f
#define DAMP_COEF 0.7f

#define ROWS 10
#define COLS 10
#define SPACING 30.0f
#define OFFSETX 100
#define OFFSETY 100

#define FRICTION 0.9f

#define MIN_DIF 0.0001f

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

