#include <stdlib.h>
#include <stdint.h>
#include <render.h>
#include <points.h>
#include <global.h>
#include <colfun.h>
#include <math.h>
#include <curve.h>
#include <unistd.h>

struct point3 lol(double t) {
    return (struct point3){t, 2+cos(100*t)/(1+t*t), sin(100*t)/(1+t*t)};
}

int main(int argc, char **argv) {
    char *cn = "/dev/dri/by-path/pci-0000:00:02.0-card";
    if (argc > 1) cn = argv[1];
    struct drm_state *s = open_drm(cn);
    render_func_drm(s->map, zero);
    uint32_t *buf = malloc(s->size);
    draw_3d_parametric(buf, lol, white, -1, 0.001, 1, 2);
    render_buf_drm(s, buf);
    sleep(3);
    free(buf);
    restore_drm(s);
}
