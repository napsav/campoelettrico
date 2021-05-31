
#include "lineeDiForza.h"

#include <SDL2/SDL.h>

#include <iostream>
#include <math.h>
#include <vector>
#define LOGVECTOR(stringa, vec) std::cout << stringa << "\tX: " << vec.x << "\tY: " << vec.y << std::endl;

int CaricaLineaDiForza::salto = 1;

CaricaLineaDiForza::CaricaLineaDiForza(float xpos, float ypos) {
  campoelettrico.x = xpos;
  campoelettrico.y = ypos;
  position.x = xpos;
  position.y = ypos;
  std::cout << "COSTRUTTORE Linea di forza posizione x " << position.x
            << " e posizione y " << position.y << std::endl;
}

void CaricaLineaDiForza::addForce(vector2 vettore) {
  vettori.push_back(vettore);
}

void CaricaLineaDiForza::computeVectors(std::vector<Sorgente> Sorgenti) {
  for (currentStep; currentStep < maxStep; currentStep++) {
    for (auto sorgente : Sorgenti) {
      vector2 intensita;
      float valoreForzaCampo;
      vector2 distanzaVettore = distanza(sorgente.getPosition(), campoelettrico);
      valoreForzaCampo =
          costanteColoumb * (sorgente.getCharge() /
                             ((distanzaVettore.modulo * distanzaVettore.modulo) *
                              (1.0f / (scala * scala))));

      std::cout << "Valore(linea): \t" << valoreForzaCampo << std::endl;
      intensita.x = valoreForzaCampo * distanzaVettore.xNormalized;
      intensita.y = valoreForzaCampo * distanzaVettore.yNormalized;
      intensita.modulo = valoreForzaCampo;
      this->addForce(intensita);
    }
    for (it = vettori.begin(); it != vettori.end(); it++) {
      campoelettrico.x += it->x;
      campoelettrico.y += it->y;
    }
    campoelettrico.modulo = sqrt((campoelettrico.x * campoelettrico.x) + (campoelettrico.y * campoelettrico.y));
    position.x += CaricaLineaDiForza::salto * ((campoelettrico.x) / (campoelettrico.modulo));
    position.y += CaricaLineaDiForza::salto * ((campoelettrico.y) / (campoelettrico.modulo));
    Point delta(position.y, position.x);
    LOGVECTOR("Punto linea " << currentStep, delta);
    ramo.puntiDelGrafico.push_back(delta);
  }
}

void CaricaLineaDiForza::render(SDL_Renderer *renderer) {
  ramo.render(renderer);
}

void CaricaLineaDiForza::emptyVectors() {
  vettori.clear();
  campoelettrico.x = 0;
  campoelettrico.y = 0;
  campoelettrico.modulo = 0;
}
