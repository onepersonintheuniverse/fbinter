#ifndef _KT_FBFUN_POINTS_H
#define _KT_FBFUN_POINTS_H
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "global.h"
#include "render.h"

struct point3 { double x, y, z; };
struct point2 { double x, y; };
struct fbpos { int x, y; };

struct point2 project(struct point3 p) {
    struct point2 res = {p.x/p.y, p.z/p.y};
    return res;
}

// i asked "kafan güzel mi?" to this function many times. may god have mercy on it
struct fbpos ptofbuf(struct point2 p, double xradius) {
    double center_y = (1+fbd[0])/2.0, center_x = (1+fbd[1])/2.0;
    struct point2 centered = {p.x*fbd[1]/(2.0*xradius)+center_x, center_y-p.y*fbd[1]/(2.0*xradius)};
    struct fbpos res = {(int)centered.x, (int)centered.y};
    return res;
}

struct fbpos p3tofbuf(struct point3 p, double xradius) {
    return ptofbuf(project(p), xradius);
}

double *getax(struct point3 *p, int axno) {
    switch (axno) {
        case 1: return &(p->x);
        case 2: return &(p->y);
        case 3: return &(p->z);
        default: return NULL;
    }
}

void rotate(struct point3 *p, struct point3 *around, double angle, int ax1, int ax2) {
    double q1 = *getax(p, ax1)-*getax(around, ax1), q2 = *getax(p, ax2)-*getax(around, ax2), q3 = *getax(p, 6-ax1-ax2)-*getax(around, 6-ax1-ax2);
    double r1 = q1*cos(angle)+q2*sin(angle), r2 = -q1*sin(angle)+q2*cos(angle);
    *getax(p, ax1) = r1+*getax(around, ax1);
    *getax(p, ax2) = r2+*getax(around, ax2);
    *getax(p, 6-ax1-ax2) = q3+*getax(around, 6-ax1-ax2);
}

void render_line_f(uint32_t *buf, int (*col)(int, int), struct fbpos p1, struct fbpos p2) {
    render_line(buf, col, p1.x, p1.y, p2.x, p2.y);
}
#endif
