#pragma once
#include "settings.h"
#include "vector.h"
#include <SDL2/SDL.h>
#include <vector>
class PuntoDelCampo {
public:
  PuntoDelCampo(float xpos, float ypos);

  vector2 getPosition();
  void addVector2(vector2 toBeAdded);
  void computeVectors();
  void emptyVectors();
  void render(SDL_Renderer *renderer);
  void setCarica(float charge);

private:
  float carica;
  vector2 posizione;
  vector2 sommaFinale;
  std::vector<vector2>::iterator it;
  std::vector<vector2> vettoriDaSommare;
};