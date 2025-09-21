#ifndef _KT_FBFUN_3D_H
#define _KT_FBFUN_3D_H
#include <stdlib.h>
#include "points.h"
struct wframe {
    int vc, ec;
    struct point3 *v;
    int *e1, *e2;
};

void render_wframe(uint32_t *buf, int (*col)(int, int), struct wframe wf, double xradius) {
    struct fbpos *f = calloc(wf.vc, sizeof(struct fbpos));
    for (int i = 0; i < wf.vc; ++i) f[i] = p3tofbuf(wf.v[i], xradius);
    for (int i = 0; i < wf.ec; ++i) render_line_f(buf, col, f[wf.e1[i]], f[wf.e2[i]]);
    free(f);
}

void rotate_wframe(struct wframe *wf, struct point3 *around, double angle, int ax1, int ax2) {
    for (int i = 0; i < wf->vc; ++i) rotate(&wf->v[i], around, angle, ax1, ax2);
}
#endif
