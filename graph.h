
#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "entities/vector.h"
#include "entities/point.h"


class Graph {
public:
  void render(SDL_Renderer *renderer);
  void renderByPoints(SDL_Renderer *renderer);
  std::vector<Point> puntiDelGrafico;
};
