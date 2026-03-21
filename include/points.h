#ifndef _KT_FBFUN_POINTS_H
#define _KT_FBFUN_POINTS_H
#include <stdint.h>

struct point3 { double x, y, z; };
struct point2 { double x, y; };
struct fbpos { int x, y; };

extern struct point2 project(struct point3 p); 

// i asked "kafan güzel mi?" to this function many times. may god have mercy on it
extern struct fbpos ptofbuf(struct point2 p, double xradius);

extern struct point2 fbuftop(struct fbpos fp, double xradius);

extern struct fbpos p3tofbuf(struct point3 p, double xradius);

extern double *getax(struct point3 *p, int axno);

extern void rotate(struct point3 *p, struct point3 *around, double angle, int ax1, int ax2);

extern void render_line_f(uint32_t *buf, int (*col)(int, int), struct fbpos p1, struct fbpos p2);
#endif
