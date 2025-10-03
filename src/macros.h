#ifndef MACROS_H_
#define MACROS_H_

#define WINDOW_W  (240)
#define WINDOW_H  (320)
#define INNER_FPS (60)

#define ASSERT(assert, str) \
	do { \
		if (!(assert)) { \
			printf("\033[1;91m ASSERT:\033[0m '"#assert"' failed. '%s' at line %d\033.\n", __FILE__, __LINE__); \
			printf("\033[1;93mMESSAGE:\033[0m '%s'\n", str); \
			exit(EXIT_FAILURE); \
		} \
	} while(0)

#define LERP(a, b, t) (((1 - t) * a) + t * b)
#define WRAP(val, min, max) ((val < min) ? max : ((val > max) ? min : val))
#define CLAMP(val, min, max) ((val < min) ? min : ((val > max) ? max : val))
#define MIN(v1, v2) (v1 < v2 ? v1 : v2)
#define MAX(v1, v2) (v1 > v2 ? v1 : v2)

#endif // MACROS_H_

