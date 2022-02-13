#include "graph.h"

#include <SDL2/SDL.h>

#include <vector>
#include <iostream>

#include "entities/vector.h"

void Graph::render(SDL_Renderer *renderer) {

  for (unsigned int i = 1; i < puntiDelGrafico.size(); i++) {
    //SDL_RenderDrawPoint(renderer, static_cast<int>(punto.x), static_cast<int>(punto.y));
    SDL_RenderDrawLine(renderer, static_cast<int>(puntiDelGrafico[i-1].x), static_cast<int>(puntiDelGrafico[i-1].y), static_cast<int>(puntiDelGrafico[i].x), static_cast<int>(puntiDelGrafico[i].y));
  };
};

