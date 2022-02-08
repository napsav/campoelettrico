#include "charge.h"

int raggioCarica = 8;
Carica::Carica(float xpos, float ypos, float carica, float massa) {
  mass = massa;
  charge = carica;
  position.x = xpos;
  position.y = ypos;
  if (abilitaLog) {
    std::cout << "COSTRUTTORE carica\tX: " << position.x
              << "\tY: " << position.y << "\tCARICA: " << charge
              << std::endl;
  }
}

vector2 Carica::getVelocity() { return velocity; }

vector2 Carica::getPosition() { return position; }

vector2 Carica::getAcceleration() { return acceleration; }

float Carica::getCharge() { return charge; }

void Carica::updatePosition(float dt) {
  velocity.x += sommaForze.x * dt;
  position.x += (velocity.x * dt) * scala;

  velocity.y += sommaForze.y * dt;
  position.y += (velocity.y * dt) * scala;
  if (abilitaLog) {
    LOGVECTOR("Posizione", position);
    LOGVECTOR("Velocità", velocity);
  }
}

void dividiForzaPerMassa(vector2 &forza, float massa) {
  forza.x = forza.x / massa;
  forza.y = forza.y / massa;
}

void Carica::addForce(vector2 forza) {
  if (abilitaLog)
    LOGVECTOR("Forza       ", forza);
  forze.push_back(forza);
}

void Carica::computeForces() {
  for (it = forze.begin(); it != forze.end(); it++) {
    sommaForze.x += it->x;
    sommaForze.y += it->y;
    if (abilitaLog)
      LOGVECTOR("Somma forze", sommaForze);
  }

  dividiForzaPerMassa(sommaForze, mass);
  //  acceleration.x += sommaForze.x;
  //  acceleration.y += sommaForze.y;

  acceleration = sommaForze;
}

void Carica::render(SDL_Renderer *renderer) {
  DrawCircle(renderer, position.x, position.y, raggioCarica);
  SDL_RenderDrawLine(renderer, position.x, position.y, position.x + acceleration.x, position.y + acceleration.y);
}

void Carica::emptyVectors() {
  forze.clear();
  sommaForze.x = 0;
  sommaForze.y = 0;
}
