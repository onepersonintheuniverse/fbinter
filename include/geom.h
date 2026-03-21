#ifndef __KT_FBFUN_GEOM_H
#define __KT_FBFUN_GEOM_H
#include <render.h>
#include <stdint.h>
#include <points.h>
extern void draw_disc(uint32_t *buf, int (*col)(int, int), struct point2 center, double radius, double xradius);
#endif
