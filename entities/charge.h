#pragma once
#include <SDL2/SDL.h>

#include <vector>

#include "vector.h"
#include "settings.h"

class Carica {
 public:
  Carica(float xpos, float ypos, float carica, float massa);
  vector2 getPosition();
  vector2 getVelocity();
  void computeForces();
  vector2 getAcceleration();
  void updatePosition(float dt);
  void render(SDL_Renderer *renderer);
  void addForce(vector2 forza);
  void emptyVectors();
  float getCharge();

 private:
  float charge;
  float mass;
  vector2 velocity;
  vector2 acceleration;
  vector2 position;
  vector2 sommaForze;
  std::vector<vector2>::iterator it;
  std::vector<vector2> forze;
};
