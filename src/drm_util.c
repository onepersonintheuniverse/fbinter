#include <drm_util.h>
#include <xf86drmMode.h>
#include <drm_plug.h>
#include <fcntl.h>

void make_drm(struct drm_state *s, char *card) {
    s->fd = open(card, O_RDWR|O_CLOEXEC);
    if (s->fd < 0) uh_oh("can't open given card");
    pick_crtc(s);
    dumbfb(s);
    s->pv_crtc = drmModeGetCrtc(s->fd, s->crtc_id);
    if (!s->pv_crtc) uh_oh("drmModeGetCrtc");
    modeset(s);
}

void restore_drm(struct drm_state *s) {
    if (!s->pv_crtc) return;
    if (drmModeSetCrtc(s->fd, s->pv_crtc->crtc_id, s->pv_crtc->buffer_id, s->pv_crtc->x, s->pv_crtc->y, &s->conn_id, 1, &s->pv_crtc->mode) != 0) uh_oh("couldn't restore crtc");
    drmModeFreeCrtc(s->pv_crtc);
    s->pv_crtc = NULL;
}

