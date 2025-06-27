#pragma once
#include <math.h>
#include "utils.h"
#include "main.h"

void calculateSpringForces(spring_t *spring);
void applyForcesWithGrav(node_t *node);
void performSoftbodyCollision(softbody_t *body1, softbody_t *body2);
void performSelfCollision(node_t *node1, node_t *node2);