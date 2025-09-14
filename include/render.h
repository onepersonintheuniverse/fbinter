#ifndef _KT_FBFUN_RENDER_H
#define _KT_FBFUN_RENDER_H
#include <time.h>

typedef _Bool bool;

int fbd[2];

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
 * @param flush If true, `fflush`
 */
void render_line(FILE * restrict fb, int col, int x1, int y1, int x2, int y2) {
    fseek(fb, 4*(x1+fbd[1]*y1), SEEK_SET); // fseek to first pixel
    int py = y1; // previous y value
    int xdir = x1 > x2 ? -1 : x1 == x2 ? 0 : 1; // x direction
    putw(col, fb);
    if (xdir == 0) { // draw vertical line
        int ydir = y1 > y2 ? -1 : y1 == y2 ? 0 : 1; // y direction
        for (int y = y1; y != y2; y += ydir) {
            putw(col, fb);
            fseek(fb, 4*ydir*fbd[1]-4, SEEK_CUR); // go 1 line down
        }
    }
    for (int x = x1+xdir; x != x2+xdir; x += xdir) {
        int y = ((y2-y1)*(x-x1))/(x2-x1)+y1; // x -> y function
        fseek(fb, 4*xdir-4, SEEK_CUR); // move to correct pixel
        int ydir = py > y ? -1 : py == y ? 0 : 1; // y direction 2: electric boogaloo
        for (int _ = py+ydir; _ != y+ydir; _ += ydir) { putw(col, fb); fseek(fb, 4*ydir*fbd[1]-4, SEEK_CUR); } // vertical line if necessary
        putw(col, fb); // last pixel
        py = y;
    }
}

void render_func(FILE * restrict fb, int (*f)(int, int)) {
    fseek(fb, 0, SEEK_SET);
    for (int i = 0; i < fbd[0]; ++i) {
        for (int j = 0; j < fbd[1]; ++j) {
            putw(f(i, j), fb);
        }
    }
    fflush(fb);
}

#endif
