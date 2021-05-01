#pragma once
#include "vector.h"
#include <SDL2/SDL.h>
#include <vector>

class Carica {
public:
Carica(float xpos, float ypos, float carica);
vector2 getPosition();
vector2 getVelocity();
void computeForces();
vector2 getAcceleration();
void move(float dt);
void render(SDL_Renderer *renderer);
void setAcceleration(vector2 accelerazione);

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
