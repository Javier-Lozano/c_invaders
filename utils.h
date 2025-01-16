#ifndef UTILS_H_
#define UTILS_H_

#include "SDL.h"

#define ASSERT(assert) \
	do { \
		if (!(bool)(assert)) { \
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "\rASSERT: '"#assert"' failed. '%s' line '%d'.", __FILE__, __LINE__); \
			exit(EXIT_FAILURE); \
		} \
	} while(0) \

#define ASSERT_MSG(assert, msg) \
	do { \
		if (!(bool)(assert)) { \
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "\rASSERT: '"#assert"' failed. '%s' line '%d'.\nMESSAGE: '%s'", __FILE__, __LINE__, msg); \
			exit(EXIT_FAILURE); \
		} \
	} while(0) \


#define CLAMP(val, min, max)      (val < min) ? min : ((val > max) ? max : val)
#define CLAMP_WRAP(val, min, max) (val < min) ? max : ((val > max) ? min : val)
#define MIN(a, b) a < b ? a : b
#define MAX(a, b) a > b ? a : b

#endif // UTILS_H_
