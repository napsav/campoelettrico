// Scrolling and zooming canvas
// SPDX-FileCopyrightText: 2022
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef CANVAS_H
#define CANVAS_H
#pragma once
#include "../imgui.h"
#include <SDL2/SDL.h>
#include "../../entities/point.h"
#include "../../entities/polygon.h"
#include <vector>


class Canvas
{
public:
    Canvas(SDL_Renderer *renderer, Point pos);

    void renderCanvas(float x, float y );

    void zoom(int32_t wheel, float x, float y);

    void handleEvents(SDL_Event &e, float x, float y);

    void setOrigin ( Point pos );

    Point getOrigin () {return scroll;};

    void drawLine(Point a, Point b);
    void drawPoint(Point a);

    void drawScreenLine(Point a, Point b);
    void drawScreenPoint(Point a);
    void drawCircle(Point a, uint32_t radius);

    float scale = 1.0;
    bool unscaledAxes = false;
    bool snap = true;
private:
    std::vector<Polygon> polygons;
    std::vector<Point> pointBuffer;
    unsigned int sides = 0;

    void completePolygon();

    float prevScale = 1.0;

    float xAxis = 0;
    float yAxis = 0;

    bool drawingPolygon = false;
    bool spacePressed = false;
    bool scrolling = false;
    Point scroll;
    Point startScroll;
    SDL_Renderer *renderer;
};

#endif // CANVAS_H
