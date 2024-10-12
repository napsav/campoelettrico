#pragma once
#include "../ui/imgui.h"
#include "point.h"

class Polygon {
public:
    Polygon(unsigned int sides);
    Polygon ( unsigned int sides, Point* newPoints );


    void render(SDL_Renderer * renderer, float scale, Point origin);
private:
    unsigned int nSides = 0;
    Point *points;
    SDL_Point *sdl_points;
};
