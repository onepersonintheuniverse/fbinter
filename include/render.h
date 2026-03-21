#ifndef _KT_FBFUN_RENDER_H
#define _KT_FBFUN_RENDER_H
#include <stdio.h>
#include <stdint.h>
#include <drm_util.h>
#include <global.h>

extern FILE *open_framebuffer(char *fb_dev);

extern struct drm_state *open_drm(char *card);

extern int distance(int x1, int y1, int x2, int y2);

/**
 * @brief Render a line to `fb` like it is a framebuffer. Units are in pixels.
 *
 * @param fb The framebuffer.
 * @param col The color in hex.
 * @param x1 The x coordinate (abscissa) of first point.
 * @param y1 The y coordinate (oordinate) of first point.
 * @param x2 The x coordinate (abscissa) of second point.
 * @param y2 The y coordinate (oordinate) of second point.
 */

extern void render_buf(FILE *fb, uint32_t *buf);

extern int check_bounds(int x, int y);

extern void render_line(uint32_t *buf, int (*col)(int, int), int x1, int y1, int x2, int y2);

extern void render_func(FILE *fb, int (*f)(int, int));

extern void render_func_drm(uint32_t *buf, int (*f)(int, int));
#endif
