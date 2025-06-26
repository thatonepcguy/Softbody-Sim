#include "main.h"
#include "utils.h"
#include <SDL2/SDL.h>
#include <math.h>

inline float distanceTo(vector2_t pos1, vector2_t pos2) {
    return sqrtf(((pos2.x-pos1.x)*(pos2.x-pos1.x)) + ((pos2.y-pos1.y)*(pos2.y-pos1.y)));
}

inline float magnitude(vector2_t vec) {
    return sqrtf(vec.x*vec.x + vec.y*vec.y);
}

inline vector2_t normalize(vector2_t vec) {
    float mag = magnitude(vec);
    if (mag < 0.0001f)
        return (vector2_t){1, 0};
    return (vector2_t){vec.x / mag, vec.y / mag};
}

inline float dotVec2(vector2_t vec1, vector2_t vec2) {
    return ((vec1.x*vec2.x) + (vec1.y*vec2.y));
}