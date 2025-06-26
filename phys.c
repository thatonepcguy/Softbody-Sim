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

        /* I DINT WNAT TO HAVE THIS IN MAIN LOOP SO ITS HERE
        // DRAW NODES
        
        for (int i = 0; i < softbodyCount; i++) {
            for (int j = 0; j < softbodies[i].nodesCount; j++) {
                SDL_RenderDrawRect(renderer, &(SDL_Rect){(int)softbodies[i].nodes[j].center.x-(int)softbodies[i].nodes[j].halfSideLen, 
                                                         (int)softbodies[i].nodes[j].center.y-(int)softbodies[i].nodes[j].halfSideLen,
                                                         softbodies[i].nodes[j].halfSideLen*2, softbodies[i].nodes[j].halfSideLen*2});
            }
        }

        // DRAW SPRINGS
        for (int i = 0; i < softbodyCount; i++) {
            for (int j = 0; j < softbodies[i].springsCount; j++) {
                SDL_RenderDrawLine(renderer, (int)softbodies[i].springs[j].node1->center.x, (int)softbodies[i].springs[j].node1->center.y, (int)softbodies[i].springs[j].node2->center.x, (int)softbodies[i].springs[j].node2->center.y);
            }
        }
        */