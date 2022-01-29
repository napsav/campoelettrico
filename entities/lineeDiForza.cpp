
#include "lineeDiForza.h"
#include "settings.h"
#define PI 3.141592
#include <SDL2/SDL.h>
#include <iostream>
#include <math.h>
#include <vector>

#define LOGVECTOR(stringa, vec) std::cout << stringa << "\tX: " << vec.x << "\tY: " << vec.y << std::endl;

float salto = 1;
int maxStep = 1000;

CaricaLineaDiForza::CaricaLineaDiForza(float xpos, float ypos) {
  campoelettrico.x = xpos;
  campoelettrico.y = ypos;
  posizioneIniziale.x = xpos;
  posizioneIniziale.y = ypos;
  position.x = xpos;
  position.y = ypos;
  if (abilitaLog) {
    std::cout << "COSTRUTTORE Linea di forza posizione x " << position.x
              << " e posizione y " << position.y << std::endl;
  }
}

void CaricaLineaDiForza::addForce(vector2 vettore) {
  vettori.push_back(vettore);
}

void CaricaLineaDiForza::computeVectors(std::vector<Sorgente> &Sorgenti) {
  for (currentStep = 0; currentStep < maxStep; currentStep++) {

    // Più lo step è alto, più la curva sarà precisa

    // Per ogni sorgente, viene calcolata l'intensità in un punto discreto dell'approssimazione della curva

    for (auto sorgente : Sorgenti) {
      vector2 intensita;
      float valoreForzaCampo;
      vector2 distanzaVettore = distanza(sorgente.getPosition(), position);
      if (distanzaVettore.modulo != 0) {
        valoreForzaCampo =
            costanteColoumb * (sorgente.getCharge() /
                               ((distanzaVettore.modulo * distanzaVettore.modulo) *
                                (1.0f / (scala * scala))));
      } else {
        valoreForzaCampo = 0;
      }

      intensita.x = valoreForzaCampo * distanzaVettore.xNormalized;
      intensita.y = valoreForzaCampo * distanzaVettore.yNormalized;
      intensita.modulo = valoreForzaCampo;
      this->addForce(intensita);
    }

    // Per il principio di sovrapposizione, tutti i vettori campo elettrico si sommano, quindi qui calcolo la risultante di tutte le sorgenti

    for (it = vettori.begin(); it != vettori.end(); it++) {
      campoelettrico.x += it->x;
      campoelettrico.y += it->y;
    }
    campoelettrico.modulo = sqrt((campoelettrico.x * campoelettrico.x) + (campoelettrico.y * campoelettrico.y));
    position.x += salto * ((campoelettrico.x) / (campoelettrico.modulo));
    position.y += salto * ((campoelettrico.y) / (campoelettrico.modulo));

    // Rendering linee di campo

    ramo.puntiDelGrafico.push_back(Point(position.x, position.y));
    this->emptyVectors();
  }
  position.x = posizioneIniziale.x;
  position.y = posizioneIniziale.y;
}

void CaricaLineaDiForza::render(SDL_Renderer *renderer) {
  ramo.render(renderer);
  ramo.puntiDelGrafico.clear();
}

void CaricaLineaDiForza::emptyVectors() {
  vettori.clear();
  campoelettrico.x = 0;
  campoelettrico.y = 0;
  campoelettrico.modulo = 0;
}

void spawnLinee(std::vector<CaricaLineaDiForza> &linee, std::vector<Sorgente>::iterator &it) {
  float x = it->getPosition().x;
  float y = it->getPosition().y;
  float carica = it->getCharge();
  float angoloStep = (2 * PI) / ((2 * PI) + (abs(carica) * densitaLinee));
  for (float angoloCorrente = 0; angoloCorrente < 2 * PI; angoloCorrente += angoloStep) {
    if (carica > 0) {
      linee.push_back(CaricaLineaDiForza(x + (raggio * cos(angoloCorrente)), y + (raggio * sin(angoloCorrente))));
    } else {
      linee.push_back(CaricaLineaDiForza((x + ((raggio + (maxStep / 2)) * cos(angoloCorrente))), (y + ((raggio + (maxStep / 2)) * sin(angoloCorrente)))));
    }
  }
}
