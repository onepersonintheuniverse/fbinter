#ifndef _KT_FBFUN_RENDER_H
#define _KT_FBFUN_RENDER_H
#include <time.h>
#include "global.h"

int distance(int x1, int y1, int x2, int y2) {
    return (x2-x1)+fbd[1]*(y2-y1); // pixel distance for fseeking
}

/**
 * @brief Render a line to `fb` like it is a framebuffer. Units are in pixels.
 *
 * @param fb The framebuffer.
 * @param col The color in hex.
 * @param x1 The x coordinate (abscissa) of first point.
 * @param y1 The y coordinate (oordinate) of first point.
 * @param x2 The x coordinate (abscissa) of second point.
 * @param y2 The y coordinate (oordinate) of second point.
 */

void render_buf(FILE *fb, uint32_t *buf) {
    fseek(fb, 0, SEEK_SET);
    for (int y = 0; y < fbd[0]; ++y) for (int x = 0; x < fbd[1]; ++x) putw(buf[x+fbd[1]*y], fb);
}

int check_bounds(int x, int y) {
    return 0 <= y && y < fbd[0] && 0 <= x && x < fbd[1];
}

void render_line(uint32_t *buf, int (*col)(int, int), int x1, int y1, int x2, int y2) {
    int py = y1; // previous y value
    int xdir = x1 > x2 ? -1 : x1 == x2 ? 0 : 1; // x direction
    if (check_bounds(x1, y1)) buf[fbd[1]*y1+x1] = col(x1, y1);
    if (xdir == 0) { // draw vertical line
        int ydir = y1 > y2 ? -1 : y1 == y2 ? 0 : 1; // y direction
        for (int y = y1+ydir; y != y2+ydir; y += ydir) if (check_bounds(x1, y)) {
            buf[fbd[1]*y+x1] = col(x1, y);
        }
    }
    for (int x = x1+xdir; x != x2+xdir; x += xdir) {
        int y = ((y2-y1)*(x-x1))/(x2-x1)+y1; // x -> y function
        int ydir = py > y ? -1 : py == y ? 0 : 1; // y direction 2: electric boogaloo
        for (int _ = py+ydir; _ != y+ydir; _ += ydir) if (check_bounds(x, y)) {
            buf[fbd[1]*_+x] = col(x, _);
        } // vertical line if necessary
        if (check_bounds(x, y)) buf[fbd[1]*y+x] = col(x, y); // last pixel
        py = y;
    }
}

void render_func(FILE *fb, int (*f)(int, int)) {
    fseek(fb, 0, SEEK_SET);
    for (int i = 0; i < fbd[0]; ++i) {
        for (int j = 0; j < fbd[1]; ++j) {
            putw(f(i, j), fb);
        }
    }
    fflush(fb);
}

#endif
