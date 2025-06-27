#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "main.h"
#include "utils.h"
#include "phys.h"
#include "init.h"

int main (int argc, char *argv[]) {
    

    // INIT SDL
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        printf("SDL INIT FAILED");
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("idk man", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    
    Uint32 renderFlags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, renderFlags);

    // SOFTBODY 1
    
    unsigned int softbodyCount = 6;
    softbody_t *softbodies;
    
    softbodies = malloc(sizeof(softbody_t)*softbodyCount);
    
    initSoftbody(softbodies, 0, 5, 20, 20, 50, 600, DAMP_COEF, STIFFNESS, (vector3I_t){255, 255, 100});
    initSoftbody(softbodies, 1, 5, 20, 20, 450, 600, DAMP_COEF, STIFFNESS, (vector3I_t){255, 100, 255});
    initSoftbody(softbodies, 2, 5, 20, 20, 850, 600, DAMP_COEF, STIFFNESS, (vector3I_t){255, 100, 100});
    initSoftbody(softbodies, 3, 5, 20, 20, 250, 450, DAMP_COEF, STIFFNESS, (vector3I_t){100, 255, 255});
    initSoftbody(softbodies, 4, 5, 20, 20, 650, 450, DAMP_COEF, STIFFNESS, (vector3I_t){100, 255, 100});
    initSoftbody(softbodies, 5, 5, 20, 20, 450, 300, DAMP_COEF, STIFFNESS, (vector3I_t){100, 100, 255});
    
    bool close = 0;
    while (!close) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    close = 1;
                    break;
            }
        }

        // CHECK FOR INSIDE BOUNDING BOX
        for (int i = 0; i < softbodyCount; i++) {
            for (int j = 0; j < softbodyCount; j++) {
                if ((softbodies[i].boundingBoxTopLeft.x < softbodies[j].boundingBoxBtmRight.x &&
                    softbodies[i].boundingBoxBtmRight.x > softbodies[j].boundingBoxTopLeft.x &&
                    softbodies[i].boundingBoxTopLeft.y < softbodies[j].boundingBoxBtmRight.y &&
                    softbodies[i].boundingBoxBtmRight.y > softbodies[j].boundingBoxTopLeft.y) && i != j) {
                    performSoftbodyCollision(&softbodies[i], &softbodies[j]);
                }
            }
        }

        // UPDATE SPRINGS
        for (int i = 0; i < softbodyCount; i++) {
            for (int j = 0; j < softbodies[i].springsCount; j++) {
                calculateSpringForces(&softbodies[i].springs[j]);
            }
        }

        // DO SELF COLLISION SHIT
        // peak programming
        for (int i = 0; i < softbodyCount; i++) {
            for (int j = 0; j < softbodies[i].nodesCount; j++) {
                for (int k = 0; k < softbodies[i].nodesCount; k++) {
                    if (j != k && distanceTo(softbodies[i].nodes[j].center, softbodies[i].nodes[k].center) < SELF_COLLISION_RADIUS){
                        performSelfCollision(&softbodies[i].nodes[j], &softbodies[i].nodes[k]);
                    } 
                }
            }
        }
        
        // UPDATE NODES
        for (int i = 0; i < softbodyCount; i++) {
            softbodies[i].boundingBoxTopLeft = (vector2_t){SCREEN_WIDTH, SCREEN_HEIGHT};
            softbodies[i].boundingBoxBtmRight = (vector2_t){0, 0};
            for (int j = 0; j < softbodies[i].nodesCount; j++) {
                if (!softbodies[i].nodes[j].fixed)
                    applyForcesWithGrav(&softbodies[i].nodes[j]);
                if (softbodies[i].nodes[j].center.x > softbodies[i].boundingBoxBtmRight.x) softbodies[i].boundingBoxBtmRight.x = softbodies[i].nodes[j].center.x;
                if (softbodies[i].nodes[j].center.x < softbodies[i].boundingBoxTopLeft.x) softbodies[i].boundingBoxTopLeft.x = softbodies[i].nodes[j].center.x;

                if (softbodies[i].nodes[j].center.y > softbodies[i].boundingBoxBtmRight.y) softbodies[i].boundingBoxBtmRight.y = softbodies[i].nodes[j].center.y;
                if (softbodies[i].nodes[j].center.y < softbodies[i].boundingBoxTopLeft.y) softbodies[i].boundingBoxTopLeft.y = softbodies[i].nodes[j].center.y;
            }
        }
        
        // Draw
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        // DRAW EDGES  
        for (int i = 0; i < softbodyCount; i++) {
            SDL_SetRenderDrawColor(renderer, softbodies[i].color.r, softbodies[i].color.g, softbodies[i].color.b, 255);
            for (int j = 0; j < softbodies[i].edgeNodeCount; j++) {
                if (j < softbodies[i].edgeNodeCount-1) {
                    SDL_RenderDrawLineF(renderer, softbodies[i].nodes[softbodies[i].edgeNodeIndexes[j]].center.x, softbodies[i].nodes[softbodies[i].edgeNodeIndexes[j]].center.y, softbodies[i].nodes[softbodies[i].edgeNodeIndexes[j+1]].center.x, softbodies[i].nodes[softbodies[i].edgeNodeIndexes[j+1]].center.y);
                } else {
                    SDL_RenderDrawLineF(renderer, softbodies[i].nodes[softbodies[i].edgeNodeIndexes[j]].center.x, softbodies[i].nodes[softbodies[i].edgeNodeIndexes[j]].center.y, softbodies[i].nodes[softbodies[i].edgeNodeIndexes[0]].center.x, softbodies[i].nodes[softbodies[i].edgeNodeIndexes[0]].center.y);
                }
            }
            //SDL_RenderDrawRectF(renderer, &(SDL_FRect){softbodies[i].boundingBoxTopLeft.x, softbodies[i].boundingBoxTopLeft.y, softbodies[i].boundingBoxBtmRight.x-softbodies[i].boundingBoxTopLeft.x, softbodies[i].boundingBoxBtmRight.y-softbodies[i].boundingBoxTopLeft.y});
        }

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderDrawLine(renderer, 0, FLOOR, SCREEN_WIDTH, FLOOR);

        /*
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
            
        

        SDL_RenderPresent(renderer);
        SDL_Delay(10); // eh good enough

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    for (int i = 0; i < softbodyCount; i++) {
        free(softbodies[i].nodes);
        free(softbodies[i].springs);
        free(softbodies[i].edgeNodeIndexes);
    }
    
    free(softbodies);

    return 0;
}