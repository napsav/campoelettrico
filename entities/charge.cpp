#include "charge.h"

#include <SDL2/SDL.h>

#include <iostream>
#include <vector>

#include "../draw.h"
#include "settings.h"
#include "sorgente.h"
int raggio = 4;
Carica::Carica(float xpos, float ypos, float carica) {
  charge = carica;

  position.x = xpos;
  position.y = ypos;
  std::cout << "COSTRUTTORE carica con posizione x " << position.x
            << " e posizione y " << position.y << " - CARICA: " << charge
            << std::endl;
}

vector2 Carica::getVelocity() { return velocity; }

vector2 Carica::getPosition() { return position; }

vector2 Carica::getAcceleration() { return acceleration; }

float Carica::getCharge() { return charge; }

void Carica::updatePosition(float dt) {
  velocity.x = sommaForze.x * dt  ;
  position.x += (velocity.x * dt) * scala;

  velocity.y = sommaForze.y * dt;
  position.y += (velocity.y * dt) * scala;
  std::cout << "Posizione x : " << position.x << std::endl;
  std::cout << "Velocita x : " << velocity.x << std::endl;
}

void dividiForzaPerMassa(vector2 &forza, float massa) {
  std::cout << "Divido per massa: " << massa << std::endl;
  forza.x = forza.x / massa;
  forza.y = forza.y / massa;
}

void Carica::addForce(vector2 forza) {
  std::cout << "Aggiunta forza x : " << forza.x << std::endl;
  forze.push_back(forza);
  std::cout << "Aggiunta accelerazione x : " << forza.x << std::endl;
}

void Carica::computeForces() {
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

void Carica::render(SDL_Renderer *renderer) {
  DrawCircle(renderer, position.x, position.y, raggio);
  SDL_RenderDrawLine(renderer, position.x, position.y,position.x + acceleration.x, position.y + acceleration.y);
}

void Carica::emptyVectors() {
  forze.clear();
  sommaForze.x = 0;
  sommaForze.y = 0;
}

