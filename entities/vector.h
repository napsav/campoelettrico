#pragma once
#include <cmath>

struct vector2 {
  float x = 0;
  float y = 0;
  float modulo = 0;
  float xNormalized = 0;
  float yNormalized = 0;
  float intensita = 0;
};

vector2 distanza(vector2 a, vector2 b);
vector2 normalize(vector2 daNormalizzare);
