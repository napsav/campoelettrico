#include "graph.h"
#include "settings.h"
#include <SDL2/SDL.h>

#include <vector>
#include <iostream>

#include "entities/vector.h"

void Graph::render(SDL_Renderer *renderer) {

  for (unsigned int i = 1; i < puntiDelGrafico.size(); i++) {
    //SDL_RenderDrawPoint(renderer, static_cast<int>(punto.x), static_cast<int>(punto.y));
    Settings::canvas.drawLine(puntiDelGrafico[i-1], puntiDelGrafico[i]);
  };
};

void Graph::renderByPoints(SDL_Renderer *renderer) {
  for(Point &point : puntiDelGrafico) {
    SDL_RenderDrawPoint(renderer, static_cast<int>(point.x), static_cast<int>(point.y));
  }
}
