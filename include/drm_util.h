#ifndef _KT_FBFUN_DRM_H
#define _KT_FBFUN_DRM_H
#include <xf86drmMode.h>
#include <drm_plug.h>
#include <fcntl.h>
extern void make_drm(struct drm_state *s, char *card);
extern void restore_drm(struct drm_state *s);
#endif
