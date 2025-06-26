#include <math.h>
#include "utils.h"
#include "main.h"
#include "phys.h"

void calculateSpringForces(spring_t *spring) {
    spring->currentLength = distanceTo(spring->node1->center, spring->node2->center);

    // CALCULATE A DIRECTION VECTOR
    vector2_t direction = (vector2_t){spring->node1->center.x - spring->node2->center.x, spring->node1->center.y - spring->node2->center.y};
    direction = normalize(direction);
    // CALCULATE DAMPING
    float dampForce = spring->dampCoef * dotVec2(direction, (vector2_t){spring->node1->velocity.x-spring->node2->velocity.x, spring->node1->velocity.y-spring->node2->velocity.y});
    // CALCULATE THE TOTAL FORCE APPLIED
    float force = spring->stiffness * (spring->restLength-spring->currentLength) - dampForce;
    
    spring->node1->accelleration.x += direction.x*force;
    spring->node1->accelleration.y += direction.y*force;
    spring->node2->accelleration.x += -direction.x*force;
    spring->node2->accelleration.y += -direction.y*force;
}

void applyForcesWithGrav(node_t *node) {
    // APPLY GRAVITY
    node->accelleration.x /= node->mass;
    node->accelleration.y /= node->mass;
    node->accelleration.y += GRAVITY;
    node->velocity.x += node->accelleration.x;
    node->velocity.y += node->accelleration.y;
    node->center.x += node->velocity.x * FRICTION;
    node->center.y += node->velocity.y * FRICTION;
    if (node->center.y > FLOOR) {
        node->center.y = FLOOR;
        node->velocity.x = 0;
        node->velocity.y = 0;
    }
    node->accelleration.x = 0;
    node->accelleration.y = 0;
}

bool pointInsideObject(vector2_t *pointToCheck, softbody_t *bodyToCheck) {
    bool inside = false;
    for (int i = 1; i < bodyToCheck->edgeNodeCount; i++) {
        float ax = bodyToCheck->nodes[bodyToCheck->edgeNodeIndexes[i]].center.x;
        float ay = bodyToCheck->nodes[bodyToCheck->edgeNodeIndexes[i]].center.y;
        float bx = bodyToCheck->nodes[bodyToCheck->edgeNodeIndexes[i-1]].center.x;
        float by = bodyToCheck->nodes[bodyToCheck->edgeNodeIndexes[i-1]].center.y;
        if (pointToCheck->x == ax && pointToCheck->y == ay) {
            return true;
        }
        if ((ay > pointToCheck->y) != (by > pointToCheck->y)) {
            float slope = (pointToCheck->x - ax) * (by - ay) - (bx - ax) * (pointToCheck->y - ay);
            if ((slope < MIN_DIF && slope > 0) || (slope > -MIN_DIF && slope < 0)) {
                return true;
            }
            if ((slope < MIN_DIF) != (by < ay)) {
                inside ^= 1;
            }
        }
    }
    return inside;
}

void performSoftbodyCollision(softbody_t *body1, softbody_t *body2) {
    for (int i = 0; i < body1->nodesCount; i++) {
        if (body1->nodes[i].center.x > body2->boundingBoxTopLeft.x &&
            body1->nodes[i].center.x < body2->boundingBoxBtmRight.x &&
            body1->nodes[i].center.y > body2->boundingBoxTopLeft.y &&
            body1->nodes[i].center.y < body2->boundingBoxBtmRight.y) {
            if (pointInsideObject(&body1->nodes[i].center, body2)) {
                int closestk;
                float distance = SCREEN_WIDTH; // good enough
                for (int k = 0; k < body2->edgeNodeCount; k++) {
                    float dist = distanceTo(body1->nodes[i].center, body2->nodes[body2->edgeNodeIndexes[k]].center);
                    if (dist < distance) {
                        distance = dist;
                        closestk = k;
                    }
                }
                vector2_t direction = (vector2_t){body1->nodes[i].center.x - body1->nodes[body2->edgeNodeIndexes[closestk]].center.x, body1->nodes[i].center.y - body1->nodes[body2->edgeNodeIndexes[closestk]].center.y};
                direction = normalize(direction);
                float force = REPEL_FORCE * (distance);
                body1->nodes[i].accelleration.x += -direction.x*force;
                body1->nodes[i].accelleration.y += -direction.y*force;
                body1->nodes[i].velocity.x = 0;
                body1->nodes[i].velocity.y = 0;
            }
        }
    }
}

        