#ifndef _KT_FBFUN_DRM_H
#define _KT_FBFUN_DRM_H
#include <drm_plug.h>
#include <fcntl.h>

void make_drm(struct drm_state *s, char *card) {
    s->fd = open(card, O_RDWR|O_CLOEXEC);
    if (s->fd < 0) uh_oh("can't open given card");
    pick_crtc(s);
    dumbfb(s);
    modeset(s);
}

#endif
