#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <render.h>
#include <points.h>
#include <global.h>
#include <colfun.h>
#include <curve.h>

struct point3 lol(double t) {
    return (struct point3){t, 2+cos(100*t)/(1+t*t), sin(100*t)/(1+t*t)};
}

int main(int argc, char **argv) {
    FILE *fb;
    if (argc > 1) fb = open_framebuffer(argv[1]);
    else fb = open_framebuffer("/dev/fb0");
    render_func(fb, zero);
    uint32_t *buf = calloc(fbd[1]*fbd[0], 4);
    draw_3d_parametric(buf, lol, white, -1, 0.001, 1, 2);
    for (int i = 0; i < 50; ++i) render_buf(fb, buf);
    fclose(fb);
}
