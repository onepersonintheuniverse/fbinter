#ifndef __KT_FBFUN_GEOM_H
#define __KT_FBFUN_GEOM_H
#include <render.h>
#include <stdint.h>
#include <points.h>

void draw_disc(uint32_t *buf, int (*col)(int, int), struct point2 center, double radius, double xradius) {
    for (int y = 0; y < fbd[0]; ++y) for (int x = 0; x < fbd[1]; ++x) {
        struct point2 p = fbuftop((struct fbpos){x, y}, xradius);
        if ((p.x-center.x)*(p.x-center.x)+(p.y-center.y)*(p.y-center.y) <= radius*radius) buf[y*stride+x] = col(x, y);
    }
}

#endif
