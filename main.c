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

    // NODES
    unsigned int nodesCount = ROWS * COLS;
    node_t *nodes;
    nodes = malloc(sizeof(node_t)*nodesCount); 

    initPointBlock(ROWS, COLS, SPACING, OFFSETX, OFFSETY, nodes);

    // CONNECTORS
    unsigned int springsCount = (ROWS - 1) * COLS + ROWS * (COLS - 1) + 2 * (ROWS - 1) * (COLS - 1);
    spring_t *springs;
    springs = malloc(sizeof(spring_t)*springsCount);
    
    initSpringBlock(ROWS, COLS, SPACING, DAMP_COEF, STIFFNESS, springs, nodes);
    

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
        for (int i = 0; i < springsCount; i++) {
            calculateSpringForces(&springs[i]);
        }

        // UPDATE NODES
        for (int i = 0; i < nodesCount; i++) {
            applyForcesWithGrav(&nodes[i]);
        }

        // Draw
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        // DRAW NODES
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        for (int i = 0; i < nodesCount; i++) {
            SDL_RenderDrawRect(renderer, &(SDL_Rect){(int)nodes[i].center.x-(int)nodes[i].halfSideLen, 
                                                     (int)nodes[i].center.y-(int)nodes[i].halfSideLen,
                                                     nodes[i].halfSideLen*2, nodes[i].halfSideLen*2});
        }

        // DRAW SPRINGS
        for (int i = 0; i < springsCount; i++) {
            SDL_RenderDrawLine(renderer, (int)springs[i].node1->center.x, (int)springs[i].node1->center.y, (int)springs[i].node2->center.x, (int)springs[i].node2->center.y);
        }

        SDL_RenderDrawLine(renderer, 0, FLOOR, SCREEN_WIDTH, FLOOR);

        SDL_RenderPresent(renderer);
        SDL_Delay(1000/60); // eh good enough

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    free(nodes);
    free(springs);

    return 0;
}