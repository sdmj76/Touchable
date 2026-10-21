/* Rhythm Heaven touch hints: which buttons the left half of the phone screen presses. */
#ifndef RH_TOUCH_H
#define RH_TOUCH_H
#include <stdint.h>

typedef struct { int layout, first, second, extra; } rh_touch_hint;   /* ss_touch.h layout, buttons or D-pad direction mask, SS_TOUCH_SHOW_* bits */
typedef struct { int active; unsigned keys; } rh_touch_state;  /* zero-initialize */

/* Feed one guest path the game opened. Returns 1 and fills *out when the hint changes. */
int rh_touch_path(rh_touch_state *state, const char *path, rh_touch_hint *out);
/* Returns 1 when the path is a graph archive of a new screen, so a load or a fade is in progress.
   Call it before rh_touch_path. The scene archives that a remix opens during play return 0. */
int rh_touch_loading(const rh_touch_state *state, const char *path);
#endif
