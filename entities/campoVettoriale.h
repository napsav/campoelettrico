#pragma once
#include "../settings.h"
#include "vector.h"
#include <SDL2/SDL.h>
#include <vector>


#include <cmath>
#include <iostream>
#include <vector>
#define COLORE(color) \
  color[0] * 0xFF, color[1] * 0xFF, color[2] * 0xFF, color[3] * 0xFF
#include "../settings.h"
#include "sorgente.h"

class PuntoDelCampo {
public:
  PuntoDelCampo(float xpos, float ypos);

  vector2 getPosition();
  void addVector2(vector2 toBeAdded);
  void computeVectors();
  void emptyVectors();
  void render(SDL_Renderer *renderer);
  void setCarica(float charge);
  ~PuntoDelCampo();

private:
  float carica;
  vector2 posizione;
  vector2 sommaFinale;
  std::vector<vector2>::iterator it;
  std::vector<vector2> vettoriDaSommare;
};
