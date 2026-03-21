#ifndef __KT_FBFUN_CURVE_H
#define __KT_FBFUN_CURVE_H
#include <stdint.h>
#include "points.h"
extern void draw_parametric(uint32_t *buf, struct point2 (*pfun)(double), int (*col)(int, int), double start, double step, double stop, double xradius);
extern void draw_3d_parametric(uint32_t *buf, struct point3 (*pfun)(double), int (*col)(int, int), double start, double step, double stop, double xradius);
#endif
