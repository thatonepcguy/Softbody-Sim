#pragma once
#include "main.h"
#include <SDL2/SDL.h>

float distanceTo(vector2_t pos1, vector2_t pos2);
vector2_t normalize(vector2_t vec);
float magnitude(vector2_t vec);
float dotVec2(vector2_t vec1, vector2_t vec2);