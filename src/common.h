#ifndef COMMON_H_
#define COMMON_H_

#include <stdbool.h>
#include <string.h>
#include "SDL.h"
#include "game.h"

#define WINDOW_W (240)
#define WINDOW_H (320)
#define FIXED_DT (1.0f/60.0f)

#define ASSERT(assert, msg) \
	do { \
		if (!(bool)(assert)) { \
			printf("\033[1;91m ASSERT:\033[0m '"#assert"' failed. '%s' at line %d\033.\n", __FILE__, __LINE__); \
			printf("\033[1;93mMESSAGE:\033[0m '%s'\n", msg); \
			exit(EXIT_FAILURE); \
		} \
	} while(0)

#define LERP(a, b, t) (((1 - t) * a) + t * b)
#define WRAP(val, min, max) ((val < min) ? max : ((val > max) ? min : val))
#define CLAMP(val, min, max) ((val < min) ? min : ((val > max) ? max : val))
#define MIN(v1, v2) (v1 < v2 ? v1 : v2)
#define MAX(v1, v2) (v1 > v2 ? v1 : v2)

#endif // COMMON_H_

