
#pragma once
#include <SDL2/SDL.h>

#include "entities/vector.h"

#include <vector>

struct Point {
	Point(float xpos, float ypos) {
		x = xpos;
		y = ypos;
	}
	float x;
	float y;
};


class Graph {
	public:
		void render(SDL_Renderer *renderer);
		std::vector<Point> puntiDelGrafico;
};
