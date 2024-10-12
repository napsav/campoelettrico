// <one line to give the program's name and a brief idea of what it does.>
// SPDX-FileCopyrightText: 2022 <copyright holder> <email>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "canvas.h"
#include "../../settings.h"
#include "../../macros.h"
#include "../../draw.h"
#include <SDL2/SDL.h>
#include <iostream>


Canvas::Canvas ( SDL_Renderer *renderer, Point pos )
{
    this->renderer = renderer;
    scroll = pos;
    startScroll = pos;
}

void Canvas::renderCanvas ( float x, float y )
{
    if ( scrolling ) {
        scroll.x += ( x - startScroll.x ) / scale;
        scroll.y += ( y - startScroll.y ) / scale;
        startScroll.x = x;
        startScroll.y = y;
    }

    if ( Settings::drawGrid ) {
        SDL_SetRenderDrawColor ( renderer, COLORE ( Settings::gridColor ) );
        for ( float x = fmod ( scroll.x, Settings::gridSize ); x < Settings::Settings::SCREEN_WIDTH; x += Settings::gridSize ) {
            SDL_RenderDrawLine ( renderer, x*scale, 0, x*scale, Settings::SCREEN_HEIGHT );
        }
        for ( float x = fmod ( scroll.y, Settings::gridSize ); x < Settings::SCREEN_HEIGHT; x += Settings::gridSize ) {
            SDL_RenderDrawLine ( renderer, 0, x*scale, Settings::SCREEN_WIDTH, x*scale );
        }
    }
    SDL_SetRenderDrawColor ( renderer, COLORE ( red ) );
    xAxis = scroll.x*scale;
    SDL_RenderDrawLine ( renderer, xAxis, 0, xAxis, Settings::SCREEN_HEIGHT );
    SDL_SetRenderDrawColor ( renderer, COLORE ( green ) );
    yAxis = scroll.y*scale;
    SDL_RenderDrawLine ( renderer, 0, yAxis, Settings::SCREEN_WIDTH, yAxis );

    SDL_SetRenderDrawColor ( renderer, COLORE ( yellow ) );
    if ( unscaledAxes ) {
        SDL_RenderDrawLine ( renderer, scroll.x, 0, scroll.x, Settings::SCREEN_HEIGHT );
        SDL_RenderDrawLine ( renderer, 0, scroll.y, Settings::SCREEN_WIDTH, scroll.y );
    }

    sides = pointBuffer.size();
    for ( unsigned i = 0; i + 1 < sides; i++ ) {
        drawLine ( pointBuffer[i], pointBuffer[i+1] );
    }
    if ( sides > 0 ) {
        if ( snap ) {
            drawLine (
            pointBuffer[sides-1], {
                round ( ( x- ( scroll.x*scale ) ) / ( Settings::gridSize*scale ) ) *Settings::gridSize,
                round ( ( y- ( scroll.y*scale ) ) / ( Settings::gridSize*scale ) ) *Settings::gridSize
            }
            );
        } else {
            drawScreenLine ( {
                ( pointBuffer[sides-1].x + scroll.x ) *scale,
                ( pointBuffer[sides-1].y + scroll.y ) *scale
            },
            {x,y}
                           );
        }

    }

    SDL_SetRenderDrawColor ( renderer, COLORE ( red ) );
    for ( auto p: pointBuffer ) {
        drawPoint ( p );
    }

    SDL_SetRenderDrawColor ( renderer, COLORE ( blue ) );
    for ( auto polygon: polygons ) {
        polygon.render ( renderer, scale, scroll );
    }

}

void Canvas::drawLine ( Point a, Point b )
{
    SDL_RenderDrawLine ( renderer, ( a.x + scroll.x ) *scale, ( a.y+scroll.y ) *scale, ( b.x+scroll.x ) *scale, ( b.y+scroll.y ) *scale );
};

void Canvas::drawPoint ( Point a )
{
    SDL_RenderDrawPoint ( renderer, ( a.x+scroll.x ) *scale, ( a.y+scroll.y ) *scale );
};

void Canvas::drawScreenLine ( Point a, Point b )
{
    SDL_RenderDrawLine ( renderer,  a.x, a.y, b.x, b.y );
};

void Canvas::drawScreenPoint ( Point a )
{
    SDL_RenderDrawPoint ( renderer, a.x, a.y );
};

void Canvas::drawCircle ( Point a, uint32_t radius )
{
    DrawCircle ( renderer, ( a.x+scroll.x ) *scale, ( a.y+scroll.y ) *scale, radius*scale );
};

void Canvas::zoom ( int32_t wheel, float x, float y )
{
    prevScale = scale;
    if ( wheel > 0 ) {
        if ( scale >= 10.0f || scale + Settings::SCALE_STEP >= 10.0f ) {
            scale = 10.0f;
        } else {
            scale += Settings::SCALE_STEP;
        }
    } else {
        if ( scale <= 1 || scale - Settings::SCALE_STEP <= 1 ) {
            scale = 1;
        } else {
            scale -= Settings::SCALE_STEP;
        }
    }

    scroll.x += ( 1/scale ) *x - ( 1/prevScale ) *x;
    scroll.y += ( 1/scale ) *y - ( 1/prevScale ) *y;
}

void Canvas::completePolygon()
{
    drawingPolygon = false;
    unsigned int sides = pointBuffer.size();
    if ( sides > 2 ) {
        Point *polygonPoints = new Point[sides];
        for ( unsigned int i = 0; i < sides; i++ ) {
            polygonPoints[i] = pointBuffer[i];
        }
        polygons.push_back ( Polygon ( sides, polygonPoints ) );
    }

    pointBuffer.clear();
}

void Canvas::handleEvents ( SDL_Event &e, float x, float y )
{
    if ( e.type == SDL_MOUSEBUTTONDOWN ) {
        if ( e.button.button == SDL_BUTTON_LEFT ) {
            if ( spacePressed ) {
                scrolling = true;
                startScroll.x = x;
                startScroll.y = y;
            }/*
            else
            {
                drawingPolygon = true;
                if ( snap )
                {
                    pointBuffer.push_back (
                    {
                        round ( ( x- ( scroll.x*scale ) ) / ( Settings::gridSize*scale ) ) *Settings::gridSize,
                        round ( ( y- ( scroll.y*scale ) ) / ( Settings::gridSize*scale ) ) *Settings::gridSize
                    }
                    );
                }
                else
                {
                    pointBuffer.push_back (
                        (Point{x, y}/scale)-scroll
                    );
                }
            }*/

        } else if ( e.button.button == SDL_BUTTON_MIDDLE ) {
            scrolling = true;
            startScroll.x = x;
            startScroll.y = y;
        } else if ( e.button.button == SDL_BUTTON_RIGHT ) {
            //completePolygon();
        }
    } else if ( e.type == SDL_MOUSEBUTTONUP ) {
        if ( ( e.button.button == SDL_BUTTON_LEFT || e.button.button == SDL_BUTTON_MIDDLE ) && scrolling ) {
            scrolling = false;
            startScroll.x = 0;
            startScroll.y = 0;
        } else if ( e.button.button == SDL_BUTTON_RIGHT ) {

        }
    } else if ( e.type == SDL_MOUSEWHEEL ) {
        zoom ( e.wheel.y, x, y );
    } else  if ( e.type == SDL_KEYDOWN ) {
        switch ( e.key.keysym.sym ) {
        case SDLK_SPACE:
            spacePressed = true;
            break;
        case SDLK_RETURN:
            //completePolygon();
            break;
        }
    } else if ( e.type == SDL_KEYUP ) {
        switch ( e.key.keysym.sym ) {
        case SDLK_SPACE:
            spacePressed = false;
            break;
        default:
            break;
        }
    }
}

void Canvas::setOrigin ( Point pos )
{
    scroll.x = pos.x;
    scroll.y = pos.y;
}
