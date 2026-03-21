#include <drm_plug.h>
#include <drm.h>
#include <drm_mode.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

/**
 * @brief something somewhere fricked up and now the program must
 * @brief cease all functions immediately
 */
void uh_oh(const char *m) { perror(m); exit(1); }

/**
 * @brief pick a CRTC connection
 */
void pick_crtc(struct drm_state *s) {
    drmModeRes *res = drmModeGetResources(s->fd);
    if (!res) uh_oh("drmModeGetResources");
    drmModeConnector *best = NULL;
    drmModeConnector *fallback = NULL;
    for (int i = 0; i < res->count_connectors; i++) {
        drmModeConnector *con = drmModeGetConnector(s->fd, res->connectors[i]);
        if (!con) continue;
        if (con->connection != DRM_MODE_CONNECTED || con->count_modes == 0) {
            drmModeFreeConnector(con);
            continue;
        }

        if (!fallback) fallback = con;
        if (con->connector_type == DRM_MODE_CONNECTOR_DisplayPort) {
            best = con;
            break;
        }
        if (con != fallback) drmModeFreeConnector(con);
    }
    drmModeConnector *con = best ? best : fallback;
    if (!con) { fprintf(stderr, "no connected connector\n"); exit(1); }
    s->conn_id = con->connector_id;
    s->mode = con->modes[0];
    if (!con) {
        fprintf(stderr, "no connected connector\n");
        exit(1);
    }
    drmModeEncoder *enc = NULL;
    if (con->encoder_id) enc = drmModeGetEncoder(s->fd, con->encoder_id);
    if (enc && enc->crtc_id) s->crtc_id = enc->crtc_id;
    else if (res->count_crtcs < 1) {
        fprintf(stderr, "no CRTCs available");
        exit(1);
    }
    else s->crtc_id = res->crtcs[0];
    if (enc) drmModeFreeEncoder(enc);
    drmModeFreeConnector(con);
    drmModeFreeResources(res);
}

void dumbfb(struct drm_state *s) {
    struct drm_mode_create_dumb creq = {0};
    creq.width = s->mode.hdisplay;
    creq.height = s->mode.vdisplay;
    creq.bpp = 32;
    if (drmIoctl(s->fd, DRM_IOCTL_MODE_CREATE_DUMB, &creq) < 0) uh_oh("DRM_IOCTL_MODE_CREATE_DUMB");
    s->handle = creq.handle;
    s->pitch = creq.pitch;
    s->size = creq.size;
    uint32_t handles[4] = {s->handle, 0, 0, 0}, pitches[4] = {s->pitch, 0, 0, 0}, offsets[4] = {0, 0, 0, 0};
    const uint32_t XR24 = '42RX';
    if (drmModeAddFB2(s->fd, s->mode.hdisplay, s->mode.vdisplay, XR24, handles, pitches, offsets, &s->fb_id, 0) != 0) {
        if (drmModeAddFB(s->fd, s->mode.hdisplay, s->mode.vdisplay, 24, 32, s->pitch, s->handle, &s->fb_id) != 0) {
            uh_oh("drmModeAddFB/2");
        }
    }
    struct drm_mode_map_dumb mreq = {0};
    mreq.handle = s->handle;
    if (drmIoctl(s->fd, DRM_IOCTL_MODE_MAP_DUMB, &mreq) < 0) uh_oh("DRM_IOCTL_MODE_MAP_DUMB");
    s->map = mmap(NULL, s->size, PROT_READ|PROT_WRITE, MAP_SHARED, s->fd, mreq.offset);
    if (s->map == MAP_FAILED) uh_oh("mmap");
    memset(s->map, 0, s->size);
}

void modeset(struct drm_state *s) {
  if (drmModeSetCrtc(s->fd, s->crtc_id, s->fb_id, 0, 0, &s->conn_id, 1, &s->mode) != 0)
    uh_oh("drmModeSetCrtc");
}

