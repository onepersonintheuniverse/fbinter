#ifndef __KT_FBFUN_CURVE_H
#define __KT_FBFUN_CURVE_H
#include <stdint.h>
#include "points.h"

void draw_parametric(uint32_t *buf, struct point2 (*pfun)(double), int (*col)(int, int), double start, double step, double stop, double xradius) {
    for (double t = start; t < stop; t += step) {
        struct point2 x1 = pfun(t), x2 = pfun(t+step);
        struct fbpos fp1 = ptofbuf(x1, xradius), fp2 = ptofbuf(x2, xradius);
        render_line_f(buf, col, fp1, fp2);
    }
}

void draw_3d_parametric(uint32_t *buf, struct point3 (*pfun)(double), int (*col)(int, int), double start, double step, double stop, double xradius) {
    for (double t = start; t < stop; t += step) {
        struct point3 x1 = pfun(t), x2 = pfun(t+step);
        struct fbpos fp1 = p3tofbuf(x1, xradius), fp2 = p3tofbuf(x2, xradius);
        render_line_f(buf, col, fp1, fp2);
    }
}


#endif
