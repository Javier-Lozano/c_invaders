#ifndef UTILS_H_
#define UTILS_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define ASSERT(assert, msg) \
	do { \
		if ( (bool)(assert) == false ) { \
			fprintf(stderr, "ASSERT: '"#assert"' failed. File: %s at line %d.\n\t-> '%s'\n", __FILE__, __LINE__, msg); \
			exit(EXIT_FAILURE); \
		} \
	} while(0) \

#define CLAMP(val, min, max)      (val < min) ? min : ((val > max) ? max : val)
#define CLAMP_WRAP(val, min, max) (val < min) ? max : ((val > max) ? min : val)
#define MIN(a, b) a < b ? a : b
#define MAX(a, b) a > b ? a : b

#endif // UTILS_H_
