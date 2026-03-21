#ifndef _KT_FBFUN_3D_H
#define _KT_FBFUN_3D_H
#include <points.h>
struct wframe {
    int nvertices, nedges;
    struct point3 *vertices;
    int *edge_from, *edge_to;
};

extern void render_wframe(uint32_t *buf, int (*col)(int, int), struct wframe wf, double xradius);

extern void rotate_wframe(struct wframe *wf, struct point3 *around, double angle, int ax1, int ax2);
#endif
