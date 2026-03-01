#include <render.h>
#include <global.h>
#include <colfun.h>

int gradient(int i, int j) {
    return ((j*255/fbd[1])<<16) | ((i*255/fbd[0])<<8);
}

int main(int argc, char **argv) {
    char *cn = "/dev/dri/card1";
    if (argc > 1) cn = argv[1];
    struct drm_state s = *open_drm(cn);
    render_func_drm(s.map, gradient);
    sleep(3);
    return 0;
}

