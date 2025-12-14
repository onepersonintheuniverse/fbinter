#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include "render.h"
#include "colfun.h"
#include "global.h"
#include "points.h"
#include "3d.h"

// splitmix64 algorithm from https://xorshift.di.unimi.it/splitmix64.c
static uint64_t x;

uint64_t next() {
    uint64_t z = (x += 0x9e3779b97f4a7c15);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
    z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
    return z ^ (z >> 31);
}

int main(int argc, char **argv) {
    x = time(NULL);
    FILE *fb;
    if (argc > 1) fb = fopen(argv[1], "w");
    else fb = fopen("/dev/fb0", "w");
    FILE *fbdims = fopen("/sys/class/graphics/fb0/virtual_size", "r");
    fscanf(fbdims, "%d,%d", fbd+1, fbd);
    fclose(fbdims);
    render_func(fb, zero);
    struct point3 v[8], cube_center = {0, 3, 0};
    for (int i = 0; i < 8; ++i) {
        int a = i&1, b = (i>>1)&1, c = i>>2;
        struct point3 p = {(double)(2*a-1), 3+(double)(2*b-1), (double)(2*c-1)};
        v[i] = p;
    }
    double xdir = 1, ydir = 1, zdir = 1, thresh = 0.01;
    int e1[12] = {0, 2, 4, 6, 0, 1, 4, 5, 0, 1, 2, 3};
    int e2[12] = {1, 3, 5, 7, 2, 3, 6, 7, 4, 5, 6, 7};
    struct wframe wf = {8, 12, v, e1, e2};
    struct timeval t1, t2;
    double dt = 0.01, avg_s = 0;
    double spd = 1;
    uint32_t *buf;
    for (int _ = 0; _ < 1000; ++_) {
        gettimeofday(&t1, NULL);
        buf = calloc(fbd[0]*fbd[1], 4);
        double rx = next()/(double)UINT64_MAX, ry = next()/(double)UINT64_MAX, rz = next()/(double)UINT64_MAX;
        xdir *= rx < thresh ? -1 : 1, ydir *= ry < thresh ? -1 : 1, zdir *= rz < thresh ? -1 : 1;
        render_wframe(buf, white, wf, 2.0);
        render_buf(fb, buf);
        fflush(fb);
        free(buf);
        rotate_wframe(&wf, &cube_center, xdir*spd*dt, 2, 3);
        rotate_wframe(&wf, &cube_center, ydir*spd*dt, 1, 3);
        rotate_wframe(&wf, &cube_center, zdir*spd*dt, 1, 2);
        gettimeofday(&t2, NULL);
        avg_s += dt = t2.tv_sec-t1.tv_sec+(t2.tv_usec-t1.tv_usec)*1e-6;
    }
    avg_s /= 1000.0;
    printf("%.3f fps avg [%.3f ms/f]", 1/avg_s, 1000*avg_s);
    return 0;
}
