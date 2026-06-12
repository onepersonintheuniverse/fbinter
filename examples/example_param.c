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
    char *cn = "/dev/dri/by-path/pci-0000:00:02.0-card";
    if (argc > 1) cn = argv[1];
    struct drm_state *s = open_drm(cn);
    render_func_drm(s->map, zero);
    uint32_t *buf = malloc(s->size);
    draw_parametric(buf, lol, white, 0, 0.001*M_PI, 2*M_PI, 4);
    render_buf_drm(s, buf);
    sleep(3);
    restore_drm(s);
    return 0;
}
