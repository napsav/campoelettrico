#pragma once
#include <cmath>
#include "point.h"

struct vector2 {
  vector2 operator+(const vector2 &other) {
    return vector2{x + other.x, y + other.y};
  }
  float x = 0;
  float y = 0;
  float modulo = 0;
  float xNormalized = 0;
  float yNormalized = 0;

  operator Point() {
    return Point{x,y};
  }
};

vector2 distanza(vector2 a, vector2 b);
vector2 normalize(vector2 daNormalizzare);
