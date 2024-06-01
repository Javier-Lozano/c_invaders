#ifndef _INPUT_H_
#define _INPUT_H_

/***** Macros *****/

#define CHECK_PRESSED(key)  (g_Input.key_state[key] & INPUT_PRESSED )
#define CHECK_RELEASED(key) (g_Input.key_state[key] & INPUT_RELEASED)
#define CHECK_HELD(key)     (g_Input.key_state[key] & INPUT_HELD    )

/***** Enums *****/

enum { // Input Mapping
	INPUT_UP,
	INPUT_DOWN,
	INPUT_LEFT,
	INPUT_RIGHT,
	INPUT_FIRE,
	INPUT_CANCEL,
	INPUT_COUNT
};

enum { // Input State
	INPUT_PRESSED  = 1,
	INPUT_RELEASED = 2,
	INPUT_HELD     = 4
};

/***** Types *****/

typedef struct {
	SDL_Scancode key_map[INPUT_COUNT];
	Uint8        key_state[INPUT_COUNT];
	Uint8        anykey;
} Input;

/***** Functions *****/

void Input_UpdateInput();

#endif // _INPUT_H_
