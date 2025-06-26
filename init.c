#include <math.h>
#include "utils.h"
#include "main.h"
#include "init.h"

void initPointBlock(int rows, int cols, float spacing, float offsetx, float offsety, node_t *nodes) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int i = r * cols + c;
            nodes[i] = (node_t){0};
            nodes[i].halfSideLen    = 1;
            nodes[i].mass           = 5;
            nodes[i].center.x       = c * spacing + offsetx;
            nodes[i].center.y       = r * spacing + offsety;
        }
    }
}

void initSpringBlock(int rows, int cols, float spacing, float dampCoef, float stiffness, spring_t *springs, node_t *nodes) {
    int si = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int i = r * cols + c;
            if (r < rows-1) {
                springs[si].dampCoef = dampCoef;
                springs[si].restLength = spacing;
                springs[si].stiffness = stiffness;
                springs[si].node1 = &nodes[i];
                springs[si].node2 = &nodes[(r+1) * cols + c];
                si += 1;
            }
            if (c < cols-1) {
                springs[si].dampCoef = dampCoef;
                springs[si].restLength = spacing;
                springs[si].stiffness = stiffness;
                springs[si].node1 = &nodes[i];
                springs[si].node2 = &nodes[r * cols + (c+1)];
                si += 1;
            }
            if (c < cols-1 && r < rows-1) {
                springs[si].dampCoef = dampCoef;
                springs[si].restLength = sqrtf(spacing*spacing + spacing*spacing);
                springs[si].stiffness = stiffness;
                springs[si].node1 = &nodes[i];
                springs[si].node2 = &nodes[(r+1) * cols + (c+1)];
                si += 1;
            }
            if (c > 0 && r < rows-1) {
                springs[si].dampCoef = dampCoef;
                springs[si].restLength = sqrtf(spacing*spacing + spacing*spacing);
                springs[si].stiffness = stiffness;
                springs[si].node1 = &nodes[i];
                springs[si].node2 = &nodes[(r+1) * cols + (c-1)];
                si += 1;
            }
        }
    }
}