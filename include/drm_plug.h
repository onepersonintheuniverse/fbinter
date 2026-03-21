#ifndef _KT_FBFUN_INTERNAL_DRM_PLUG_H
#define _KT_FBFUN_INTERNAL_DRM_PLUG_H
#include <drm.h>
#include <drm_mode.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

struct drm_state {
    int fd;
    drmModeModeInfo mode;
    drmModeCrtc *pv_crtc;
    uint32_t width, height;
    uint32_t pitch;
    uint64_t size;
    uint32_t fb_id;
    uint32_t handle;
    uint32_t conn_id;
    uint32_t crtc_id;
    void *map;
};

/**
 * @brief something somewhere fricked up and now the program must
 * @brief cease all functions immediately
 */
extern void uh_oh(const char *m);

/**
 * @brief pick a CRTC connection
 */
extern void pick_crtc(struct drm_state *s);

extern void dumbfb(struct drm_state *s);

extern void modeset(struct drm_state *s);
#endif
