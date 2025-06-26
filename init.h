#pragma once
#include <math.h>
#include "utils.h"
#include "main.h"

void initPointBlock(int rows, int cols, float spacing, float offsetx, float offsety, node_t *nodes);
void initSpringBlock(int rows, int cols, float spacing, float dampCoef, float stiffness, spring_t *springs, node_t *nodes);