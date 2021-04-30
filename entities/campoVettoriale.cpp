#include "campoVettoriale.h"
#include "sorgente.h"
#include "vector.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

PuntoDelCampo::PuntoDelCampo(float xpos, float ypos) {
  posizione.x = xpos;
  posizione.y = ypos;
}

void PuntoDelCampo::addVector2(vector2 toBeAdded) {
  vettoriDaSommare.push_back(toBeAdded);
}

vector2 PuntoDelCampo::getPosition() {
  return posizione;
}

void PuntoDelCampo::computeVectors() {
  vector2 normalizzando;
  for (it = vettoriDaSommare.begin(); it != vettoriDaSommare.end(); it++) {
    normalizzando = normalize(*it);
    sommaFinale.x += normalizzando.xNormalized;
    sommaFinale.y += normalizzando.yNormalized;
    carica += normalizzando.intensita;
  }
}

void PuntoDelCampo::emptyVectors() {
  vettoriDaSommare.clear();
  sommaFinale.x = 0;
  sommaFinale.y = 0;
  carica = 0;
}

void PuntoDelCampo::render(SDL_Renderer *renderer) {
  if (carica > 0) {
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
  } else {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderDrawLine(renderer, posizione.x, posizione.y, posizione.x + (sommaFinale.x * 5), posizione.y + (sommaFinale.y * 5));
}

void PuntoDelCampo::setCarica(float charge) {
  carica = charge;
}
