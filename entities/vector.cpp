#include "vector.h"

vector2 normalize(vector2 daNormalizzare) {
  vector2 normalizzato;
  float x = daNormalizzare.x;
  float y = daNormalizzare.y;
  float modulo = sqrt((x * x) + (y * y));
  normalizzato.xNormalized = x / modulo;
  normalizzato.yNormalized = y / modulo;
  normalizzato.modulo = modulo;
  normalizzato.intensita = daNormalizzare.intensita;
  return normalizzato;
}

vector2 distanza(vector2 a, vector2 b) {
  vector2 distanza;
  distanza.x = -a.x + b.x;
  distanza.y = -a.y + b.y;
  return normalize(distanza);
}