#include "charge.h"

#include <SDL2/SDL.h>

#include <iostream>
#include <vector>
#define LOGVECTOR(stringa, vec) std::cout << stringa << "\tX: "<< vec.x << "\tY: " << vec.y << std::endl;
#define LOGXY(x,y) std::cout << "Coppia\tX: "<< x << "\tY: " << y << std::endl;
#include "../draw.h"
#include "settings.h"
#include "sorgente.h"
int raggioCarica = 4;
Carica::Carica(float xpos, float ypos, float carica, float massa) {
    mass = massa;
  charge = carica;
  position.x = xpos;
  position.y = ypos;
  std::cout << "COSTRUTTORE carica\tX: " << position.x
            << "\tY: " << position.y << "\tCARICA: " << charge
            << std::endl;
}

vector2 Carica::getVelocity() { return velocity; }

vector2 Carica::getPosition() { return position; }

vector2 Carica::getAcceleration() { return acceleration; }

float Carica::getCharge() { return charge; }

void Carica::updatePosition(float dt) {
  velocity.x += sommaForze.x * dt  ;
  position.x += (velocity.x * dt) * scala;

  velocity.y += sommaForze.y * dt;
  position.y += (velocity.y * dt) * scala;
  LOGVECTOR("Posizione", position);
  LOGVECTOR("Velocità", velocity);
}

void dividiForzaPerMassa(vector2 &forza, float massa) {
  forza.x = forza.x / massa;
  forza.y = forza.y / massa;
}

void Carica::addForce(vector2 forza) {
  LOGVECTOR("Forza       ", forza);
  forze.push_back(forza);
  LOGVECTOR("Accelerazione", acceleration);
}

void Carica::computeForces() {
  for (it = forze.begin(); it != forze.end(); it++) {
    sommaForze.x += it->x;
    sommaForze.y += it->y;
    LOGVECTOR("Somma forze",sommaForze);
  }

  dividiForzaPerMassa(sommaForze, mass);
//  acceleration.x += sommaForze.x;
//  acceleration.y += sommaForze.y;

  acceleration = sommaForze;
}

void Carica::render(SDL_Renderer *renderer) {
  DrawCircle(renderer, position.x, position.y, raggioCarica);
  SDL_RenderDrawLine(renderer, position.x, position.y,position.x + acceleration.x, position.y + acceleration.y);
}

void Carica::emptyVectors() {
  forze.clear();
  sommaForze.x = 0;
  sommaForze.y = 0;
}

