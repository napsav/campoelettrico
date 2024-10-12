#pragma once
#include "entities/vector.h"
#include "ui/components/canvas.h"

extern int lunghezza;
extern int raggio;
extern float maxCarica;
extern int coloreBase;
extern float massa;
extern int raggioCarica;
extern int scala;
extern float coloreSfondo[4];
extern float coloreSorgente[4];
extern float coloreCarica[4];
extern unsigned int SCREEN_WIDTH;
extern unsigned int SCREEN_HEIGHT;
extern float salto;
extern float costanteColoumb;
extern int maxStep;
extern float densitaLinee;
extern float coloreBaseRGB[4];
extern float coloreGrigliaSecondario[4];
extern float coloreGrigliaPrimario[4];
extern float coloreLinee[4];
extern bool abilitaLog;
extern bool sorgentiColoreSegno;
extern bool darkMode;
extern float coloreGrCariche[4];
extern bool drawGraficoCariche;
extern int densita;
extern bool drawCampoVettoriale;
extern bool drawSorgenti;
extern bool drawGrid;
extern bool drawLineeDiCampo;
extern float caricaDiProva;
extern vector2 caricaNuova;
extern vector2 sorgenteNuova;
extern float caricaSorgenteNuova;

const float red[4] = {1.0,0,0,1};
const float green[4] = {0,1.0,0,1};
const float blue[4] = {0,0,1,1};
const float yellow[4] = {1,1,0,1};
const float cyan[4] = {0,1,1,1};

class Settings
{
public:
    static unsigned int gridSize;
    static bool drawGrid;
    static int SCREEN_HEIGHT;
    static int SCREEN_WIDTH;
    static float backgroundColor[4];
    static float gridColor[4];
    static float SCALE_STEP;
    static Canvas canvas;

private:
    Settings();
};
