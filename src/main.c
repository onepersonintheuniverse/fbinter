#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <stdint.h>
#include "render.h"
#include "colfun.h"

// splitmix64 algorithm from https://xorshift.di.unimi.it/splitmix64.c
static uint64_t x;

uint64_t next() {
    uint64_t z = (x += 0x9e3779b97f4a7c15);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
    z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
    return z ^ (z >> 31);
}

const int white = (1<<24)-1;
struct point3 { double x, y, z; };
struct point2 { double x, y; };
struct fbpos { int x, y; };

struct point2 project(struct point3 p) {
    struct point2 res = {p.x/p.y, p.z/p.y};
    return res;
}

// i asked "kafan güzel mi?" to this function many times. may god have mercy on it
struct fbpos ptofbuf(struct point2 p, double xradius) {
    double center_y = (1+fbd[0])/2, center_x = (1+fbd[1])/2;
    struct point2 centered = {p.x*fbd[1]/(2.0*xradius)+center_x, center_y-p.y*fbd[1]/(2.0*xradius)};
    struct fbpos res = {(int)centered.x, (int)centered.y};
    return res;
}

struct fbpos p3tofbuf(struct point3 p, double xradius) {
    return ptofbuf(project(p), xradius);
}

double *getax(struct point3 *p, int axno) {
    switch (axno) {
        case 1: return &(p->x);
        case 2: return &(p->y);
        case 3: return &(p->z);
        default: return NULL;
    }
}

void rotate(struct point3 *p, struct point3 *around, double angle, int ax1, int ax2) {
    double q1 = *getax(p, ax1)-*getax(around, ax1), q2 = *getax(p, ax2)-*getax(around, ax2), q3 = *getax(p, 6-ax1-ax2)-*getax(around, 6-ax1-ax2);
    double r1 = q1*cos(angle)+q2*sin(angle), r2 = -q1*sin(angle)+q2*cos(angle);
    *getax(p, ax1) = r1+*getax(around, ax1);
    *getax(p, ax2) = r2+*getax(around, ax2);
    *getax(p, 6-ax1-ax2) = q3+*getax(around, 6-ax1-ax2);
}

int main(int argc, char **argv) {
    x = time(NULL);
    FILE *fb = fopen(argv[1], "w"), *fbdims = fopen("/sys/class/graphics/fb0/virtual_size", "r");
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
    struct fbpos f[8]; for (int i = 0; i < 8; ++i) f[i] = p3tofbuf(v[i], 2);
    int e1[12] = {0, 2, 4, 6, 0, 1, 4, 5, 0, 1, 2, 3};
    int e2[12] = {1, 3, 5, 7, 2, 3, 6, 7, 4, 5, 6, 7};
    struct timeval t1, t2;
    double avg_us = 0;
    double dt = 0.01;
    double spd = 1;
    for (int _ = 0; _ < 1000; ++_) {
        gettimeofday(&t1, NULL);
        for (int i = 0; i < 12; ++i) render_line(fb, 0, f[e1[i]].x, f[e1[i]].y, f[e2[i]].x, f[e2[i]].y);
        for (int i = 0; i < 8; ++i) f[i] = p3tofbuf(v[i], 2);
        double rx = next()/(double)UINT64_MAX, ry = next()/(double)UINT64_MAX, rz = next()/(double)UINT64_MAX;
        xdir *= rx < thresh ? -1 : 1, ydir *= ry < thresh ? -1 : 1, zdir *= rz < thresh ? -1 : 1;
        for (int i = 0; i < 12; ++i) render_line(fb, white, f[e1[i]].x, f[e1[i]].y, f[e2[i]].x, f[e2[i]].y);
        fflush(fb);
        for (int i = 0; i < 8; ++i) {
            rotate(v+i, &cube_center, xdir*spd*dt, 1, 2);
            rotate(v+i, &cube_center, ydir*spd*dt, 1, 3);
            rotate(v+i, &cube_center, zdir*spd*dt, 2, 3);
        }
        gettimeofday(&t2, NULL);
        dt = t2.tv_sec-t1.tv_sec+(t2.tv_usec-t1.tv_usec)*1e-6;
        avg_us += (1000000L*(t2.tv_sec-t1.tv_sec)+t2.tv_usec-t1.tv_usec)/1e3;
    }
    printf("%lf fps avg rendering cube", 1e6/avg_us);
    return 0;
}
