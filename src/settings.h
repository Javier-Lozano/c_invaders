#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <stdbool.h>

struct SDL_Window;

typedef struct Settings {
	int  highscore[10];
	char names[10][10];
	char scale;
	char angle;
	bool vsync;
	bool fullscreen;
} Settings;

void LoadSAVEDAT (Settings *settings);
void WriteSAVEDAT(Settings *settings);
void ModScreen(struct SDL_Window *window, Settings *s, char s_value, char a_value);

#endif // SETTINGS_H_

