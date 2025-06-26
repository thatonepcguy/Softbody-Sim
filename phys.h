#pragma once
#include <math.h>
#include "utils.h"
#include "main.h"

void calculateSpringForces(spring_t *spring);
void applyForcesWithGrav(node_t *node);