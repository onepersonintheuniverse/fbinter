#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <render.h>
#include <geom.h>

int red(int x, int y) { return 0xff0000; }
int green(int x, int y) { return 0x00ff00; }
int blue(int x, int y) { return 0x0000ff; }

int main(int argc, char **argv) {
    FILE *fb;
    if (argc > 1) fb = open_framebuffer(argv[1]);
    else fb = open_framebuffer("/dev/fb0");
    uint32_t *buf = calloc(fbd[0]*fbd[1], 4);
    draw_disc(buf, red, (struct point2){1.0, 1.0}, 1.5, 5.0);
    draw_disc(buf, green, (struct point2){-0.5, 0.3}, 0.8, 5.0);
    draw_disc(buf, blue, (struct point2){0.0, -0.9}, 1.3, 5.0);
    for (int i = 0; i < 50; ++i) render_buf(fb, buf);
    fclose(fb);
    return 0;
}
