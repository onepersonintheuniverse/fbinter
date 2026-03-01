#ifndef _KT_FBFUN_3D_H
#define _KT_FBFUN_3D_H
#include <stdlib.h>
#include "points.h"
struct wframe {
    int nvertices, nedges;
    struct point3 *vertices;
    int *edge_from, *edge_to;
};

void render_wframe(uint32_t *buf, int (*col)(int, int), struct wframe wf, double xradius) {
    struct fbpos *f = calloc(wf.nvertices, sizeof(struct fbpos));
    for (int i = 0; i < wf.nvertices; ++i) f[i] = p3tofbuf(wf.vertices[i], xradius);
    for (int i = 0; i < wf.nedges; ++i) render_line_f(buf, col, f[wf.edge_from[i]], f[wf.edge_to[i]]);
    free(f);
}

void rotate_wframe(struct wframe *wf, struct point3 *around, double angle, int ax1, int ax2) {
    for (int i = 0; i < wf->nvertices; ++i) rotate(&wf->vertices[i], around, angle, ax1, ax2);
}
#endif
