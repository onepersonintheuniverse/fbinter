#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <math.h>
#include "render.h"
#include "colfun.h"
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

struct point4 {
    double x, y, z, w;
};

struct wframe4 {
    int vc, ec;
    struct point4 *v;
    int *e1, *e2;
};

double *getax4(struct point4 *p, int axno) {
    switch (axno) {
        case 1: return &(p->x);
        case 2: return &(p->y);
        case 3: return &(p->z);
        case 4: return &(p->w);
        default: return NULL;
    }
}

void rotate4(struct point4 *p, struct point4 *around, double angle, int ax1, int ax2) {
    struct point4 q = {p->x - around->x, p->y - around->y, p->z - around->z, p->w - around->w};
    double px1 = *getax4(&q, ax1), px2 = *getax4(&q, ax2);
    *getax4(p, ax1) = px1*cos(angle)+px2*sin(angle)+*getax4(around, ax1);
    *getax4(p, ax2) = -px1*sin(angle)+px2*cos(angle)+*getax4(around, ax2);
}

void rotate_wframe4(struct wframe4 *wf, struct point4 *around, double angle, int ax1, int ax2) {
    for (int i = 0; i < wf->vc; ++i) rotate4(&(wf->v[i]), around, angle, ax1, ax2);
}

struct point3 project4(struct point4 *p, struct point3 *to) {
    struct point3 q = {p->x/p->w + to->x, p->y/p->w + to->y, p->z/p->w + to->z};
    return q;
}

struct wframe project4_wframe(struct wframe4 *wf, struct point3 *to) {
    struct point3 *v = calloc(wf->vc, sizeof(struct point3));
    for (int i = 0; i < wf->vc; ++i) v[i] = project4(&wf->v[i], to);
    struct wframe wf3 = {wf->vc, wf->ec, v, wf->e1, wf->e2};
    return wf3;
}

int main(int argc, char **argv) {
    x = time(NULL);
    char *cn = "/dev/dri/card1";
    if (argc > 1) cn = argv[1];
    struct drm_state s = *open_drm(cn);
    struct point4 v[16], hyper_center = {0, 0, 0, 3};
    for (int i = 0; i < 16; ++i) {
        int a = i&1, b = (i>>1)&1, c = (i>>2)&1, d = (i>>3)&1;
        struct point4 p = {(double)(2*a-1), (double)(2*b-1), (double)(2*c-1), 3+(double)(2*d-1)};
        v[i] = p;
    }
    int e1[32] = {0, 2, 4, 6, 8, 10, 12, 14, 0, 1, 4, 5, 8, 9, 12, 13, 0, 1, 2, 3, 8, 9, 10, 11, 0, 1, 2, 3, 4, 5, 6, 7};
    int e2[32] = {1, 3, 5, 7, 9, 11, 13, 15, 2, 3, 6, 7, 10, 11, 14, 15, 4, 5, 6, 7, 12, 13, 14, 15, 8, 9, 10, 11, 12, 13, 14, 15};
    struct wframe4 wf = {16, 32, v, e1, e2};
    struct timeval t1, t2;
    uint64_t frames = 0;
    double dt = 0.01, avg_s = 0;
    double spd = 1;
    uint32_t *buf = malloc(s.size);
    struct timespec rq = {0, 5000000}, rm;
    struct wframe wf3;
    rotate_wframe4(&wf, &hyper_center, next()/(double)UINT64_MAX, 1, 4);
    rotate_wframe4(&wf, &hyper_center, next()/(double)UINT64_MAX, 2, 4);
    rotate_wframe4(&wf, &hyper_center, next()/(double)UINT64_MAX, 3, 4);
    while (avg_s < 10) {
        gettimeofday(&t1, NULL);
        render_func_drm(buf, zero);
        wf3 = project4_wframe(&wf, &(struct point3){0.0, 3.0, 0.0});
        render_wframe(buf, white, wf3, 2.0);
        free(wf3.vertices);
        memcpy(s.map, buf, s.size);
        rotate_wframe4(&wf, &hyper_center, spd*dt, 1, 4);
        rotate_wframe4(&wf, &hyper_center, spd*dt, 2, 4);
        rotate_wframe4(&wf, &hyper_center, spd*dt, 3, 4);
        // nanosleep(&rq, &rm);
        gettimeofday(&t2, NULL);
        avg_s += dt = t2.tv_sec-t1.tv_sec+(t2.tv_usec-t1.tv_usec)*1e-6;
        ++frames;
    }
    free(buf);
    avg_s /= frames;
    restore_drm(&s);
    printf("%.3f fps avg [%.3f ms/f]", 1/avg_s, 1000*avg_s);
    return 0;
}
