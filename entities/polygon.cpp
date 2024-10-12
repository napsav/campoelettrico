#include <SDL2/SDL.h>
#include <iostream>
#include "polygon.h"
#include "../macros.h"
#include "point.h"


Polygon::Polygon(unsigned int sides) {
    points = new Point[sides];
    nSides = sides;
}

Polygon::Polygon(unsigned int sides, Point* newPoints) {
    points = new Point[sides];
    sdl_points = new SDL_Point[sides+1];
    for(unsigned int i = 0; i < sides; i++) {
        points[i] = newPoints[i];
    }

    nSides = sides;
}

void Polygon::render ( SDL_Renderer* renderer, float scale, Point origin )
{
    for(unsigned int i = 0; i < nSides; i++) {
        sdl_points[i] = points[i]*scale;
        sdl_points[i].x += origin.x*scale;
        sdl_points[i].y += origin.y*scale;
    }
    sdl_points[nSides]=points[0]*scale;
    sdl_points[nSides].x += origin.x*scale;
    sdl_points[nSides].y += origin.y*scale;
    SDL_RenderDrawLines(renderer, sdl_points, nSides+1);
}

SDL_Point operator +(SDL_Point a, SDL_Point b) {
    return SDL_Point{a.x + b.x, a.y + b.y};
}

SDL_Point operator *(SDL_Point a, float b) {
    return SDL_Point{static_cast<int>(a.x*b), static_cast<int>(a.y*b)};
}
