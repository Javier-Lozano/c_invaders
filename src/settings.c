#include "SDL.h"
#include "settings.h"
#include "macros.h"

static const Settings g_DefaultSettings = {
	.highscore = {57300, 37500, 30000, 25000, 20000, 19500, 19000, 15000, 10000, 5000},
	.names = {
		"B. SABATH ",
		"DIO       ",
		"B.O.C.    ",
		"K. CRIMSON",
		"D. PURPLE ",
		"SAXON     ",
		"RHAPSODY  ",
		"PAGAN ALTR",
		"CANDLEMASS",
		"METALLICA ",
	},
	.scale = 2,
	.angle = 0,
	.vsync = false,
	.fullscreen = false
};

void LoadSAVEDAT(Settings *s)
{
	bool error_read = 0;
	bool error_eof  = 0; 

	FILE *file = fopen("SAVE.DAT", "rb");
	if (file == NULL)
		goto DEFAULT_FILE;

	fread(s, sizeof(Settings), 1, file);
	error_read = ferror(file);
	error_eof  = feof(file);
	fclose(file);

	if (error_read || error_eof)
		goto DEFAULT_FILE;

#ifdef DEBUG
	printf("'SAVE.DAT' loaded.\n");
	printf("Scale:   %d\n", s->scale);
	printf("Angle:   %d\n", s->angle);
	printf("Vsync:   %d\n", s->vsync);
	printf("Fullscr: %d\n", s->fullscreen);
#endif

	return;

DEFAULT_FILE:
	printf("Creating 'SAVE.DAT'\n");
	*s = g_DefaultSettings;
	WriteSAVEDAT(s);
	return;
}

void WriteSAVEDAT(Settings *s)
{
	FILE *file = fopen("SAVE.DAT", "wb");
	if (file == NULL)
	{
		printf("Error: Couldn't write to 'SAVE.DAT'\n");
		return;
	}

	fwrite(s, sizeof(Settings), 1, file);
	fclose(file);

#ifdef DEBUG
	printf("'SAVE.DAT' saved.\n");
	printf("Scale:   %d\n", s->scale);
	printf("Angle:   %d\n", s->angle);
	printf("Vsync:   %d\n", s->vsync);
	printf("Fullscr: %d\n", s->fullscreen);
#endif

	return;
}

void ModScreen(SDL_Window *window, Settings *s, char s_value, char a_value)
{
	SDL_DisplayMode display;
	int display_index = SDL_GetWindowDisplayIndex(window);
	if (SDL_GetCurrentDisplayMode(display_index, &display) != 0)
	{
		printf("%s\n", SDL_GetError());
		return;
	}

	// Rotate Screen
	if (a_value != 0)
		s->angle = WRAP(s->angle + a_value, 0, 3);

	bool is_vertical = (s->angle % 2) == 0;
	int w = is_vertical ? WINDOW_W : WINDOW_H;
	int h = is_vertical ? WINDOW_H : WINDOW_W;

	// Scale Screen
	if (s_value != 0 || a_value != 0)
		s->scale = CLAMP(s->scale + s_value, 1, MIN(display.h / h, display.w / w));

	if (!s->fullscreen)
	{
		SDL_SetWindowMinimumSize(window, w * s->scale, h * s->scale);
		SDL_SetWindowSize(window, w * s->scale, h * s->scale);
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
}
