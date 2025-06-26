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
    
    unsigned int softbodyCount = 2;
    softbody_t *softbodies;
    
    softbodies = malloc(sizeof(softbody_t)*softbodyCount);
    
    initSoftbody(softbodies, 0, 5, 10, 20, 300, 300, DAMP_COEF, STIFFNESS, (vector3I_t){255, 100, 255});
    initSoftbody(softbodies, 1, 5, 10, 20, 250, 100, DAMP_COEF, STIFFNESS, (vector3I_t){255, 255, 100});

    softbodies[0].nodes[0].fixed = true;
    
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

        // UPDATE SPRINGS
        for (int i = 0; i < softbodyCount; i++) {
            for (int j = 0; j < softbodies[i].springsCount; j++) {
                calculateSpringForces(&softbodies[i].springs[j]);
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
            SDL_RenderDrawRect(renderer, &(SDL_Rect){softbodies[i].boundingBoxTopLeft.x, softbodies[i].boundingBoxTopLeft.y, softbodies[i].boundingBoxBtmRight.x-softbodies[i].boundingBoxTopLeft.x, softbodies[i].boundingBoxBtmRight.y-softbodies[i].boundingBoxTopLeft.y});
        }

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderDrawLine(renderer, 0, FLOOR, SCREEN_WIDTH, FLOOR);

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