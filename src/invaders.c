#include "invaders.h"

#define DEBUG_GRID 1
#define DEBUG_SAVE_LOG 0

///// Input

static char g_KeyState[SDL_NUM_SCANCODES];
static bool g_AnyKeyPressed;

void InputUpdate()
{
	const Uint8 *keyboard = SDL_GetKeyboardState(NULL);

	g_AnyKeyPressed = false;

	for(int i = 0; i < SDL_NUM_SCANCODES; i++)
	{
		char state = 0;
		if (keyboard[i] > 0)
		{
			state = STATE_DOWN;
			if ((g_KeyState[i] & STATE_DOWN) == 0)
			{
				state |= STATE_PRESSED;
				g_AnyKeyPressed = true;
			}
		}
		else if ((g_KeyState[i] & STATE_DOWN) == STATE_DOWN)
		{
			state = STATE_RELEASED;
		}

		g_KeyState[i] = state;
	}
}

void InputClear()
{
	g_AnyKeyPressed = false;

	for(int i = 0; i < SDL_NUM_SCANCODES; i++)
		g_KeyState[i] &= STATE_DOWN;
}

bool CheckState(SDL_Scancode scancode, ButtonState state)
{
	return (g_KeyState[scancode] & state) == state;
}

bool IsAnyKeyPressed()
{
	return g_AnyKeyPressed;
}

bool IsKeyDown(SDL_Keycode key)
{
	return CheckState(SDL_GetScancodeFromKey(key), STATE_DOWN);
}

bool IsKeyPressed(SDL_Keycode key)
{
	return CheckState(SDL_GetScancodeFromKey(key), STATE_PRESSED);
}

bool IsKeyReleased(SDL_Keycode key)
{
	return CheckState(SDL_GetScancodeFromKey(key), STATE_RELEASED);
}

///// Transition

static TransState g_TransState;
static double     g_TransOffset;
static double     g_TransTimer;
static int        g_TransStage;

void StartTransition(int stage)
{
	g_TransState  = TRANS_INIT;
	g_TransOffset = 0.0f;
	g_TransTimer  = 0.0f;
	g_TransStage  = stage;
}

static void update_transition(SDL_Renderer *renderer, double dt)
{
	const float speed  = dt * WINDOW_H;
	const float mid    = WINDOW_H / 2;
	const bool  draw_stage = g_TransStage > 0;

	SDL_FRect rects[2] = {
		{0,        0, WINDOW_W, 0},
		{0, WINDOW_H, WINDOW_W, 0}
	};

	switch (g_TransState)
	{
		case TRANS_NONE:
			return;
		case TRANS_INIT:
			g_TransOffset += speed;

			if (g_TransOffset > mid)
			{
				g_TransOffset = mid;
				g_TransState  = TRANS_WAIT; 
			}
			break;
		case TRANS_WAIT:
			g_TransTimer += dt;

			if (g_TransTimer > 2)
				g_TransState = TRANS_END; 

			break;
		case TRANS_END:
			g_TransOffset -= speed;

			if (g_TransOffset < 0)
				g_TransState = TRANS_NONE; 
			break;
	}

	rects[0].h =  g_TransOffset;
	rects[1].h = -g_TransOffset;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRectsF(renderer, rects, 2);

	if (draw_stage)
		DrawText(renderer, "^1STAGE^7 %d", 84, g_TransOffset-8, g_TransStage);
}

TransState GetTransitionState()
{
	return g_TransState;
}

///// Settings

static const Settings g_DefaultSettings = {
	.highscore = {57300, 37500, 30000, 25000, 20000, 19500, 19000, 15000, 10000, 5000},
	.names = {
		"B. SABBATH",
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


void LoadSAVEDAT(Settings *settings)
{
	bool error_read = 0;
	bool error_eof  = 0; 

	FILE *file = fopen("SAVE.DAT", "rb");
	if (file == NULL)
		goto DEFAULT_FILE;

	fread(settings, sizeof(Settings), 1, file);
	error_read = ferror(file);
	error_eof  = feof(file);
	fclose(file);

	if (error_read || error_eof)
		goto DEFAULT_FILE;

#if defined DEBUG && DEBUG_SAVE_LOG
	printf("'SAVE.DAT' loaded.\n");
	printf("Scale:   %d\n", settings->scale);
	printf("Angle:   %d\n", settings->angle);
	printf("Vsync:   %d\n", settings->vsync);
	printf("Fullscr: %d\n", settings->fullscreen);
#endif

	return;

DEFAULT_FILE:
	printf("Creating 'SAVE.DAT'\n");
	*settings = g_DefaultSettings;
	WriteSAVEDAT(settings);
	return;
}

void WriteSAVEDAT(Settings *settings)
{
	FILE *file = fopen("SAVE.DAT", "wb");
	if (file == NULL)
	{
		printf("Error: Couldn't write to 'SAVE.DAT'\n");
		return;
	}

	fwrite(settings, sizeof(Settings), 1, file);
	fclose(file);

#if defined DEBUG && DEBUG_SAVE_LOG
	printf("'SAVE.DAT' saved.\n");
	printf("Scale:   %d\n", settings->scale);
	printf("Angle:   %d\n", settings->angle);
	printf("Vsync:   %d\n", settings->vsync);
	printf("Fullscr: %d\n", settings->fullscreen);
#endif

	return;
}

void ModScreen(struct SDL_Window *window, Settings *s, char s_value, char a_value)
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

static char g_SettingsSelect = 1;

void UpdateSettings(SDL_Window *window, SDL_Renderer *renderer, Settings *settings)
{
	char input  = IsKeyPressed(SDLK_DOWN) - IsKeyPressed(SDLK_UP);
	char left   = IsKeyPressed(SDLK_LEFT);
	char right  = IsKeyPressed(SDLK_RIGHT);
	bool accept = IsKeyPressed(SDLK_RETURN);
	g_SettingsSelect = WRAP(g_SettingsSelect + input, 1, 6);

	switch(g_SettingsSelect)
	{
		case 1:
			if (left || right || accept)
			{
				settings->fullscreen = !settings->fullscreen;
				SDL_SetWindowFullscreen(window, settings->fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
				ModScreen(window, settings, 0, 0);
			}
			break;
		case 2:
			if (left || right)
				ModScreen(window, settings, right-left, 0);
			break;
		case 3:
			if (left || right)
				ModScreen(window, settings, 0, right-left);
			break;
		case 4:
			settings->vsync = !settings->vsync;
			SDL_RenderSetVSync(renderer, settings->vsync);
			break;
		case 5:
			// TODO: Volume Control
		case 6:
			break;
	}
}

void DrawSettings(SDL_Renderer *renderer, Settings *settings)
{
	DrawText(renderer, "^2C",       112, 16);
	DrawText(renderer, "^5INVADERS", 88, 32);
	DrawText(renderer, "^3SETTINGS", 88, 48);

	DrawTextRGBA(renderer, "FULLSCREEN         %s",     16,  64, g_SettingsSelect == 1 ? 0xFF0000FF : 0x808080FF, settings->fullscreen ? "YES" : "NO");
	DrawTextRGBA(renderer, "SCALING            %d",     16,  80, g_SettingsSelect == 2 ? 0xFF0000FF : 0x808080FF, settings->scale);
	DrawTextRGBA(renderer, "ORIENTATION        %d DEG", 16,  96, g_SettingsSelect == 3 ? 0xFF0000FF : 0x808080FF, settings->angle * 90);
	DrawTextRGBA(renderer, "VSYNC              %s",     16, 112, g_SettingsSelect == 4 ? 0xFF0000FF : 0x808080FF, settings->vsync ? "YES" : "NO");
	DrawTextRGBA(renderer, "VOLUME             %d",     16, 128, g_SettingsSelect == 5 ? 0xFF0000FF : 0x808080FF, 0);
	DrawTextRGBA(renderer, "RESET ALL"           ,      16, 240, g_SettingsSelect == 6 ? 0xFF0000FF : 0x808080FF);
}

///// Scene

static SceneID g_TargetScene;
static bool    g_ChangeScene;
static Scene   g_Scene;

void SwitchScene(SceneID id)
{
	g_TargetScene = id;
	g_ChangeScene = true;
}

///// GameContext

static Uint64 g_TimePrev;
static Uint64 g_TimeCurr;
static double g_ElapsedTime;
static double g_Accumulator;

void InitInvaders(GameContext *game, int argc, char *argv[])
{
	Uint32 flags_init   = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
	Uint32 flags_window = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
	Uint32 flags_render = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
	//Uint32 flags_render = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC;

	LoadSAVEDAT(&game->settings);

	// Args
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "vsync") == 0)
		{
			game->settings.vsync = true;
			SDL_Log("VSync Enabled\n");
		}
		else if (strcmp(argv[i], "fullscreen") == 0)
		{
			game->settings.fullscreen = true;
			SDL_Log("Fullscreen Enabled\n");
		}
	}

	// Apply Settings
	if (game->settings.scale < 1)
		game->settings.scale = 1;

	if (game->settings.angle < 0 || game->settings.angle > 3)
		game->settings.angle = 0;

	if (game->settings.vsync)
		flags_render |= SDL_RENDERER_PRESENTVSYNC;

	if (game->settings.fullscreen)
		flags_window |= SDL_WINDOW_FULLSCREEN_DESKTOP;

	// Init SDL
	int w = (game->settings.angle % 2) == 0 ? WINDOW_W : WINDOW_H;
	int h = (game->settings.angle % 2) == 0 ? WINDOW_H : WINDOW_W;

	SDL_SetHint(SDL_HINT_AUDIODRIVER, "alsa");
	ASSERT(SDL_Init(flags_init) == 0, SDL_GetError());

	game->window = SDL_CreateWindow("C Invaders",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			w * game->settings.scale,
			h * game->settings.scale,
			flags_window);
	ASSERT(game->window != NULL, SDL_GetError());

	game->renderer = SDL_CreateRenderer(game->window, -1, flags_render);
	ASSERT(game->renderer != NULL, SDL_GetError());

	game->fbuffer = SDL_CreateTexture(game->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_W, WINDOW_H);
	ASSERT(game->fbuffer != NULL, SDL_GetError());

	SDL_SetRenderDrawBlendMode(game->renderer, SDL_BLENDMODE_BLEND);
	SDL_SetWindowMinimumSize(game->window,
			w * game->settings.scale,
			h * game->settings.scale);

	// Graphics
	ASSERT(InitGraphics(game->renderer), SDL_GetError());

	// 
	//SwitchScene(SCENE_PLAY);
	g_Scene = GetScenePlay();

	g_TimePrev = SDL_GetPerformanceCounter();

	game->framerate  = DEFAULT_FPS;
	game->is_running = true;
}

void EndInvaders(GameContext *game)
{
	WriteSAVEDAT(&game->settings);
	CloseGraphics();
	SDL_DestroyTexture(game->fbuffer);
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);
	SDL_Quit();
}

static void events(GameContext *game)
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	bool shift = state[SDL_SCANCODE_RSHIFT] || state[SDL_SCANCODE_LSHIFT];
	bool alt   = state[SDL_SCANCODE_RALT]   || state[SDL_SCANCODE_LALT];

	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				game->is_running = false;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					case SDLK_v:
						game->settings.vsync = !game->settings.vsync;
						SDL_RenderSetVSync(game->renderer, game->settings.vsync);
						break;
					case SDLK_RETURN:
						if (alt)
						{
							game->settings.fullscreen = !game->settings.fullscreen;
							SDL_SetWindowFullscreen(game->window, game->settings.fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
							ModScreen(game->window, &game->settings, 0, 0);
						}
						break;
					case SDLK_PAGEUP:
						if (shift)
							ModScreen(game->window, &game->settings, 0, 1);
						else
							ModScreen(game->window, &game->settings, 1, 0);
						break;
					case SDLK_PAGEDOWN:
						if (shift)
							ModScreen(game->window, &game->settings, 0, -1);
						else
							ModScreen(game->window, &game->settings, -1, 0);
						break;
				}
				break;
		}
	}

	InputUpdate();
}

static void draw_to_screen(GameContext *game)
{
	int w, h;
	SDL_GetWindowSize(game->window, &w, &h);

	bool is_vertical = (game->settings.angle % 2) == 0;
	double angle = game->settings.angle * 90.0f;

	SDL_Rect rect = {
		.w = WINDOW_W * game->settings.scale,
		.h = WINDOW_H * game->settings.scale,
	};

	if (is_vertical)
	{
		rect.x = (w / 2) - (rect.w / 2);
		rect.y = (h / 2) - (rect.h / 2);
	}
	else
	{
		rect.x = (w / 2) - (rect.h / 2) + (40 * game->settings.scale);
		rect.y = (h / 2) - (rect.w / 2) - (40 * game->settings.scale);
	}

	SDL_SetRenderTarget(game->renderer, NULL);
	SDL_RenderCopyEx(game->renderer, game->fbuffer, NULL, &rect, angle, NULL, 0);

	SDL_RenderPresent(game->renderer);
}

void GameLoop(GameContext *game)
{
	const double fixed_dt = 1.0f / (double)game->framerate;

	// Clear Screen
	SDL_SetRenderDrawColor(game->renderer, 16, 16, 16, 255);
	SDL_RenderClear(game->renderer);
	SDL_SetRenderTarget(game->renderer, game->fbuffer);
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
	SDL_RenderClear(game->renderer);

	SetGraphicsColor(255, 255, 255, 255);

	// Events
	events(game);

	// Scene
	if (g_ChangeScene)
	{
		switch(g_TargetScene)
		{
			case SCENE_TITLE:
				g_Scene = GetSceneTitle();
				break;
			case SCENE_PLAY:
				g_Scene = GetScenePlay();
				break;
			default:
				g_Scene = GetSceneTitle();
				break;
		}

		g_ChangeScene = false;
	}

	if (g_Scene.is_starting)
	{
		g_Scene.init();
		g_Scene.is_starting = false;
	}

	// Timing
	g_TimeCurr = SDL_GetPerformanceCounter();
	g_ElapsedTime = (double)(g_TimeCurr - g_TimePrev) / (double)SDL_GetPerformanceFrequency();
	g_TimePrev = g_TimeCurr;
	g_Accumulator += g_ElapsedTime;


	g_Scene.update(game, g_ElapsedTime);

	for(; g_Accumulator > fixed_dt; g_Accumulator -= fixed_dt)
	{
		g_Scene.fixed_update(game, fixed_dt);
		InputClear();
	}

	const double alpha = g_Accumulator / fixed_dt;

	g_Scene.draw(game, g_ElapsedTime, alpha);

	update_transition(game->renderer, g_ElapsedTime);

#if defined DEBUG && DEBUG_GRID
	SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 32);
	for(int x = 0; x < WINDOW_W; x +=8 )
		SDL_RenderDrawLine(game->renderer, x, 0, x, WINDOW_H);
	for(int y = 0; y < WINDOW_H; y +=8 )
		SDL_RenderDrawLine(game->renderer, 0, y, WINDOW_W, y);
	SDL_SetRenderDrawColor(game->renderer, 255, 0, 255, 64);
		SDL_RenderDrawLine(game->renderer, WINDOW_W/2, 0, WINDOW_W/2, WINDOW_H);
		SDL_RenderDrawLine(game->renderer, 0, WINDOW_H/2, WINDOW_W, WINDOW_H/2);

		DrawTextRGBA(game->renderer, "VSync: %s", 0, 0, 0xFFFF0080, game->settings.vsync ? "YES" : "NO");
		DrawTextRGBA(game->renderer, "Fullscreen: %s", 0, 8, 0xFFFF0080, game->settings.fullscreen ? "YES" : "NO");
		DrawTextRGBA(game->renderer, "Rotation: %d°", 0, 16, 0xFFFF0080, game->settings.angle*90);
#endif

	// Present to Screen
	draw_to_screen(game);
}

