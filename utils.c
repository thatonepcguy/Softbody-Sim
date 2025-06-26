#include "main.h"
#include "utils.h"
#include <SDL2/SDL.h>
#include <math.h>

float distanceTo(vector2_t pos1, vector2_t pos2) {
    return sqrtf(((pos2.x-pos1.x)*(pos2.x-pos1.x)) + ((pos2.y-pos1.y)*(pos2.y-pos1.y)));
}

float magnitude(vector2_t vec) {
    return sqrtf(vec.x*vec.x + vec.y*vec.y);
}

vector2_t normalize(vector2_t vec) {
    return (vector2_t){vec.x / magnitude(vec), vec.y / magnitude(vec)};
}

float dotVec2(vector2_t vec1, vector2_t vec2) {
    return ((vec1.x*vec2.x) + (vec1.y*vec2.y));
}