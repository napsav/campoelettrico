#include "charge.h"
#include <SDL2/SDL.h>
#include <vector>

Carica::Carica(float xpos, float ypos, float carica) {
charge = carica;

position.x = xpos;
position.y = ypos;
}

vector2 Carica::getVelocity() {
	return velocity;
}

vector2 Carica::getPosition() {
	return position;
}


vector2 Carica::getAcceleration() {
	return acceleration;
}

void Carica::move(float dt) {
	velocity.x = acceleration.x * dt;
	position.x = velocity.x * dt;

	velocity.y = acceleration.y * dt;
	position.y = velocity.y * dt;
}


void Carica::setAcceleration(vector2 accelerazione) {
	acceleration = accelerazione;
}
