#include "vector.h"

vector2 normalize(vector2 daNormalizzare) {
  vector2 normalizzato;
  float x = daNormalizzare.x;
  float y = daNormalizzare.y;
  float modulo = sqrt((x * x) + (y * y));
  normalizzato.x = daNormalizzare.x;
  normalizzato.y = daNormalizzare.y;
  if (modulo != 0) {
    normalizzato.xNormalized = x / modulo;
    normalizzato.yNormalized = y / modulo;
  } else {
    normalizzato.xNormalized = 0;
    normalizzato.yNormalized = 0;
  }

  normalizzato.modulo = modulo;
  return normalizzato;
}

vector2 distanza(vector2 a, vector2 b) {
  vector2 distanza;
  distanza.x = -a.x + b.x;
  distanza.y = -a.y + b.y;
  return normalize(distanza);
}
