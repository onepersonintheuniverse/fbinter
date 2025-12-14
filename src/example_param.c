#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "curve.h"
#include "points.h"
#include "render.h"
#include "colfun.h"

struct point2 lol(double t) {
    return (struct point2){cos(t)/cos(fmod(t+M_PI/5, 2*M_PI/5)-M_PI/5), sin(t)/cos(fmod(t+M_PI/5, 2*M_PI/5)-M_PI/5)};
}

int main(int argc, char **argv) {
    FILE *fb;
    if (argc > 1) fb = fopen(argv[1], "w");
    else fb = fopen("/dev/fb0", "w");
    FILE *fbdims = fopen("/sys/class/graphics/fb0/virtual_size", "r");
    fscanf(fbdims, "%d,%d", fbd+1, fbd);
    fclose(fbdims);
    render_func(fb, zero);
    uint32_t *buf = calloc(fbd[0]*fbd[1], 4);
    draw_parametric(buf, lol, white, 0, 0.001*M_PI, 2*M_PI, 4);
    for (int i = 0; i < 50; ++i) render_buf(fb, buf);
    fclose(fb);
    return 0;
}
