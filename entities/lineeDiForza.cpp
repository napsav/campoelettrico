
#include "lineeDiForza.h"

#include <SDL2/SDL.h>

#include <iostream>
#include <vector>

#include "../draw.h"
#include "settings.h"
#include "sorgente.h"
int raggio = 4;
CaricaLineaDiForza::CaricaLineaDiForza(float xpos, float ypos, float carica) {
  charge = carica;

  position.x = xpos;
  position.y = ypos;
  std::cout << "COSTRUTTORE carica con posizione x " << position.x
            << " e posizione y " << position.y << " - CARICA: " << charge
            << std::endl;
}

vector2 CaricaLineaDiForza::getVelocity() { return velocity; }

vector2 CaricaLineaDiForza::getPosition() { return position; }

vector2 CaricaLineaDiForza::getAcceleration() { return acceleration; }

float CaricaLineaDiForza::getCharge() { return charge; }

void CaricaLineaDiForza::updatePosition(float dt) {
  velocity.x += sommaForze.x * dt  ;
  position.x += velocity.x * dt* 1000 ;

  velocity.y += sommaForze.y * dt;
  position.y += velocity.y * dt*1000;
  std::cout << "Posizione x : " << position.x << std::endl;
  std::cout << "Velocita x : " << velocity.x << std::endl;
}

void dividiForzaPerMassa(vector2 &forza, float massa) {
  std::cout << "Divido per massa: " << massa << std::endl;
  forza.x = forza.x / massa;
  forza.y = forza.y / massa;
}

void CaricaLineaDiForza::addForce(vector2 forza) {
  std::cout << "Aggiunta forza x : " << forza.x << std::endl;
  forze.push_back(forza);
  std::cout << "Aggiunta accelerazione x : " << forza.x << std::endl;
}

void CaricaLineaDiForza::computeForces() {
  for (it = forze.begin(); it != forze.end(); it++) {
    sommaForze.x += it->x;
    sommaForze.y += it->y;
    std::cout << "Somma forze:" << sommaForze.x << std::endl;
  }

  dividiForzaPerMassa(sommaForze, mass);
//  acceleration.x += sommaForze.x;
//  acceleration.y += sommaForze.y;

  acceleration = sommaForze;
}

void CaricaLineaDiForza::render(SDL_Renderer *renderer) {
  DrawCircle(renderer, position.x, position.y, raggio);
  SDL_RenderDrawLine(renderer, position.x, position.y,position.x + acceleration.x, position.y + acceleration.y);
}

void CaricaLineaDiForza::emptyVectors() {
  forze.clear();
  sommaForze.x = 0;
  sommaForze.y = 0;
}

