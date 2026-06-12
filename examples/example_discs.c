#include <stdint.h>
#include <stdlib.h>
#include <render.h>
#include <geom.h>

int red(int x, int y) { return 0xff0000; }
int green(int x, int y) { return 0x00ff00; }
int blue(int x, int y) { return 0x0000ff; }

int main(int argc, char **argv) {
    char *cn = "/dev/dri/by-path/pci-0000:00:02.0-card";
    if (argc > 1) cn = argv[1];
    struct drm_state *s = open_drm(cn);
    uint32_t *buf = malloc(s->size);
    draw_disc(buf, red, (struct point2){1.0, 1.0}, 1.5, 5.0);
    draw_disc(buf, green, (struct point2){-0.5, 0.3}, 0.8, 5.0);
    draw_disc(buf, blue, (struct point2){0.0, -0.9}, 1.3, 5.0);
    render_buf_drm(s, buf);
    sleep(3);
    free(buf);
    restore_drm(s);
    return 0;
}
