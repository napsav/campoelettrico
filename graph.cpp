#include "graph.h"

#include <SDL2/SDL.h>

#include <vector>

#include "entities/vector.h"

void Graph::render(SDL_Renderer *renderer) {
  for (Point &punto : puntiDelGrafico) {
    SDL_RenderDrawPoint(renderer, static_cast<int>(punto.x),
                        static_cast<int>(punto.y));
  };
};

