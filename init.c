#include <math.h>
#include <stdbool.h>
#include "utils.h"
#include "main.h"
#include "init.h"

void initPointBlock(int rows, int cols, float spacing, float offsetx, float offsety, node_t *nodes) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int i = r * cols + c;
            nodes[i] = (node_t){0};
            nodes[i].halfSideLen    = 1;
            nodes[i].mass           = 6;
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
    springs[si].dampCoef = dampCoef;
    springs[si].restLength = distanceTo(nodes[0].center, nodes[rows*cols-1].center);
    springs[si].stiffness = stiffness*0.5;
    springs[si].node1 = &nodes[0];
    springs[si].node2 = &nodes[rows*cols-1];
    si++;
    springs[si].dampCoef = dampCoef;
    springs[si].restLength = distanceTo(nodes[cols].center, nodes[rows*cols-1-cols].center);
    springs[si].stiffness = stiffness*0.5;
    springs[si].node1 = &nodes[cols-1];
    springs[si].node2 = &nodes[rows*cols-cols];
    si++;
}

void initSoftbody(softbody_t *softbodies, int index, int rows, int cols, int spacing, int offsetX, int offsetY, float dampCoef, float stiffness, vector3I_t color) {
    softbodies[index].nodesCount = rows * cols;
    softbodies[index].springsCount = (rows - 1) * cols + rows * (cols - 1) + 2 * (rows - 1) * (cols - 1) + 2/*for corner springs*/;

    // NODES
    softbodies[index].nodes = malloc(sizeof(node_t)*softbodies[index].nodesCount); 
    memset(softbodies[index].nodes, 0, sizeof(node_t)*softbodies[index].nodesCount);
    initPointBlock(rows, cols, spacing, offsetX, offsetY, softbodies[index].nodes);

    // EDGE NODE INDEXES
    softbodies[index].edgeNodeCount = rows*2+cols*2 - 4;
    softbodies[index].edgeNodeIndexes = malloc(sizeof(unsigned int)*softbodies[index].edgeNodeCount);
    
    // CONNECTORS
    softbodies[index].springs = malloc(sizeof(spring_t)*softbodies[index].springsCount);
    memset(softbodies[index].springs, 0, sizeof(spring_t)*softbodies[index].springsCount);
    initSpringBlock(rows, cols, spacing, dampCoef, stiffness, softbodies[index].springs, softbodies[index].nodes);

    int count = 0;
    for (int c = 0; c < cols; c++) {
        softbodies[index].edgeNodeIndexes[count] = 0 * cols + c;
        count++;
    }
        
    for (int r = 1; r < rows - 1; r++) {
        softbodies[index].edgeNodeIndexes[count] = r * cols + (cols - 1);
        count++;
    }

    for (int c = cols - 1; c >= 0; c--) {
        softbodies[index].edgeNodeIndexes[count] = (rows - 1) * cols + c;
        count++;
    }

    for (int r = rows - 2; r > 0; r--) {
        softbodies[index].edgeNodeIndexes[count] = r * cols + 0;
        count++;
    }

    softbodies[index].color = color;
}