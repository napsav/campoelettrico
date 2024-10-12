#include "settings.h"
#include "entities/vector.h"
/*
float coloreGrigliaPrimario[4] = {0.559, 0.559, 0.559, 1};
float coloreGrigliaSecondario[4] = {0.784, 0.784, 0.784, 1};
float coloreSfondo[4] = {1, 1, 1, 1};
float coloreCarica[4] = {1, 0, 0, 1};
float coloreSorgente[4] = {0, 0, 0, 1};
float coloreGrCariche[4] = {0, 0, 0, 1};
float coloreLinee[4] = {0, 0.647, 1, 1};
float coloreBaseRGB[4] = {0.710, 0.710, 0.710, 1};
int coloreBase = 0xFF;
*/

float coloreGrigliaPrimario[4] = {0.667, 0.667, 0.667, 1};
float coloreGrigliaSecondario[4] = {0.4, 0.4, 0.4, 1};
float coloreSfondo[4] = {0, 0, 0, 1};
float coloreCarica[4] = {1, 0, 0, 1};
float coloreSorgente[4] = {1, 1, 1, 1};
float coloreGrCariche[4] = {1, 1, 1, 1};
float coloreLinee[4] = {0, 0.647, 1, 1};
float coloreBaseRGB[4] = {0.284, 0.284, 0.284, 1};
int coloreBase = 0x46;


float costanteColoumb = 8.987551792314e9;
unsigned int SCREEN_HEIGHT = 720;
unsigned int SCREEN_WIDTH = 1280;
vector2 caricaNuova = {226.97501, 300};
vector2 sorgenteNuova = {20, 20};
float massa = 1e4;
float densitaLinee = 1e8;
int scala = 1000;
int densita = 16;
int lunghezza = 10;
int raggio = 10;
float maxCarica = 0.05;
bool drawGrid = true;
bool drawCampoVettoriale = true;
bool sorgentiColoreSegno = false;
bool drawLineeDiCampo = true;
bool drawSorgenti = true;
bool drawGraficoCariche = true;
bool darkMode = true;
bool abilitaLog = false;
float caricaDiProva = 1.602176634e-19;
float caricaSorgenteNuova = 1.5e-9;

unsigned int Settings::gridSize = 10;
bool Settings::drawGrid = true;

int Settings::SCREEN_HEIGHT = 720;
int Settings::SCREEN_WIDTH = 1280;
float Settings::backgroundColor[4] = {0, 0, 0, 1};
float Settings::gridColor[4] = {0.2, 0.2, 0.2, 1};
float Settings::SCALE_STEP = 0.20;
