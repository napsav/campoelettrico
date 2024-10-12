#pragma once
#include <SDL2/SDL.h>


struct Point {
    Point(float xpos = 0, float ypos = 0) {
        x = xpos;
        y = ypos;
    }

    float x;
    float y;

    operator SDL_Point() const {
        return SDL_Point{static_cast<int>(x), static_cast<int>(y)};
    }

    Point operator +(Point b)  {
        return Point{x + b.x, y + b.y};
    }

    Point operator -(Point b)  {
        return Point{x - b.x, y - b.y};
    }

    Point operator *(float b) {
        return Point{x*b, y*b};
    }

    Point operator /(float b) {
        return Point{x/b, y/b};
    }
};
