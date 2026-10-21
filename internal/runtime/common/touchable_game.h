#ifndef TOUCHABLE_GAME_H
#define TOUCHABLE_GAME_H
#include <stdint.h>
/* Each game supplies these hooks. They run on a guest thread. */
void touchable_game_install(void);
int touchable_game_host_call(int number, uint64_t *registers);
#endif
