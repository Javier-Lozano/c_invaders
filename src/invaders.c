#include "invaders.h"

///// Audio

#define SND_FREQ     (44100)
#define SND_FORMAT   (AUDIO_S16SYS)
#define SND_CHANNELS (1)
#define SND_SAMPLES  (1024)
#define SND_MAX_VOL  (100)

typedef struct Sound {
	Uint8 *buf;
	Uint32 len;
	Uint32 pos;
	char   vol;
	bool   play;
} Sound;

static const char * g_SoundSrc[SND_COUNT] = {
	"assets/06_cursor2.wav",     // Cursor
	"assets/59_confirm.wav",     // Confirm
	"assets/57_drop.wav",        // Cancel
	"assets/73_bowrelease1.wav", // Fire Player
	"assets/16_falling.wav",     // Fire Alien
	"assets/28_jingle.wav",      // Hit UFO
	"assets/69_explode.wav",     // Hit Player
	"assets/77_arrowbounce.wav", // Hit Miss
	"assets/78_arrowhit.wav",    // Hit Alien
	"assets/53_step2.wav",       // Alien Step 1
	"assets/52_step1.wav",       // Alien Step 2
	"assets/55_step4.wav",       // Alien Step 3
	"assets/54_step3.wav",       // Alien Step 4
	"assets/64_lose2.wav",       // Game Over
};

static SDL_AudioDeviceID g_AudioDevice;
static SDL_AudioSpec     g_AudioSpec;
static Sound             g_Sound[SND_COUNT];

void PlaySoundVol(SoundID id, char volume)
{
	if (id < 0 || id >= SND_COUNT)
		return;

	g_Sound[id].vol  = volume;
	g_Sound[id].pos  = 0;
	g_Sound[id].play = true;
}

void PlaySound(SoundID id)
{
	PlaySoundVol(id, 100);
}

static void audio_callback(void *userdata, Uint8* stream, int len)
{
	Sint16  mix[SND_SAMPLES] = {0};
	Sint16 *dst    = (Sint16*)stream;
	float   volume = (float)(*(char*)userdata) / (float)SND_MAX_VOL;

	SDL_memset(stream, 0, len);

	for(int i = 0; i < SND_COUNT; i++)
	{
		Sound  *snd = g_Sound+i;

		if (!snd->play)
			continue;

		Sint16 *src       = (Sint16*)(snd->buf + snd->pos);
		int     bytes     = snd->len - snd->pos;
		int     step      = (bytes < len) ? bytes : len;
		float   inner_vol = (float)snd->vol / (float)SND_MAX_VOL;

		for (int j = 0; j < (step/2); j++)
		{
			float sample = (float)src[j] * inner_vol;
			sample = CLAMP(sample, -32768, 32767);
			mix[j] += (Sint16)sample;
		}

		snd->pos += step;
		if (snd->pos >= snd->len)
			snd->play = false;
	}

	for (int i = 0; i < SND_SAMPLES; ++i)
	{
		float sample = mix[i] * volume;
		sample = CLAMP(sample, -32768, 32767);
		dst[i] = (Sint16)sample;
	}
}

///// Graphics

#define MAX_FRAMES (8)
#define GLYPH_SIZE  (8)

typedef struct {
	int frames[MAX_FRAMES];
	int length;
} Sequence;

static SDL_Texture *g_SpriteSheet;

static const unsigned int g_UTF8Map[12] = {
	0xC381, // Á
	0xC389, // É
	0xC38D, // Í
	0xC393, // Ó
	0xC39A, // Ú
	0xC391, // Ñ
	0xC3A1, // á
	0xC3A9, // é
	0xC3AD, // í
	0xC3B3, // ó
	0xC3BA, // ú
	0xC3B1  // ñ
};

static const SDL_Rect g_Sprites[SPR_COUNT] = {
		{   0,  96, 16, 8 }, /* SPR_PLAYER */
		{  16,  96, 16, 8 }, /* SPR_ENEMY_A_1 */
		{  32,  96, 16, 8 }, /* SPR_ENEMY_A_2 */
		{  48,  96, 16, 8 }, /* SPR_ENEMY_B_1 */
		{  64,  96, 16, 8 }, /* SPR_ENEMY_B_2 */
		{  80,  96, 16, 8 }, /* SPR_ENEMY_C_1 */
		{  96,  96, 16, 8 }, /* SPR_ENEMY_C_2 */
		{ 112,  96, 16, 8 }, /* SPR_ENEMY_UFO */
		{   0, 104, 16, 8 }, /* SPR_PLAYER_DEAD_1 */
		{  16, 104, 16, 8 }, /* SPR_PLAYER_DEAD_2 */
		{  32, 104, 16, 8 }, /* SPR_PLAYER_DEAD_3 */
		{  48, 104, 16, 8 }, /* SPR_PLAYER_DEAD_4 */
		{  64, 104,  8, 8 }, /* SPR_HIT_1 */
		{  72, 104,  8, 8 }, /* SPR_HIT_2 */
		{  80, 104,  8, 8 }, /* SPR_HIT_3 */
		{  88, 104,  8, 8 }, /* SPR_HIT_4 */
		{  96, 104,  8, 8 }, /* SPR_BULLET_HIT_1 */
		{ 104, 104,  8, 8 }, /* SPR_BULLET_HIT_2 */
		{ 112, 104,  8, 8 }, /* SPR_BULLET_HIT_3 */
		{ 120, 104,  8, 8 }, /* SPR_BULLET_HIT_4 */
		{   0, 112,  8, 8 }, /* SPR_BULLET_PLAYER */
		{   8, 112,  8, 8 }, /* SPR_BULLET_A_1 */
		{  16, 112,  8, 8 }, /* SPR_BULLET_A_2 */
		{  24, 112,  8, 8 }, /* SPR_BULLET_A_3 */
		{  32, 112,  8, 8 }, /* SPR_BULLET_A_4 */
		{  40, 112,  8, 8 }, /* SPR_BULLET_B_1 */
		{  48, 112,  8, 8 }, /* SPR_BULLET_B_2 */
		{  56, 112,  8, 8 }, /* SPR_BULLET_B_3 */
		{  64, 112,  8, 8 }, /* SPR_BULLET_B_4 */
		{  72, 112,  8, 8 }, /* SPR_BULLET_C_1 */
		{  80, 112,  8, 8 }, /* SPR_BULLET_C_2 */
		{  88, 112,  8, 8 }, /* SPR_BULLET_C_3 */
		{  96, 112,  8, 8 }, /* SPR_BULLET_C_4 */
		{ 104, 112,  8, 8 }, /* SPR_SHIELD_1 */
		{ 112, 112,  8, 8 }, /* SPR_SHIELD_2 */
		{ 120, 112,  8, 8 }, /* SPR_SHIELD_3 */
		{   0, 120,  8, 8 }, /* SPR_TILE_1 */
		{   8, 120,  8, 8 }, /* SPR_TILE_2 */
		{  16, 120,  8, 8 }, /* SPR_TILE_3 */
		{  24, 120,  8, 8 }, /* SPR_TILE_4 */
		{  32, 120,  8, 8 }, /* SPR_TILE_5 */
		{  40, 120,  8, 8 }, /* SPR_TILE_6 */
		{  48, 120,  8, 8 }, /* SPR_TILE_7 */
		{  56, 120,  8, 8 }, /* SPR_TILE_8 */
		{  64, 120,  8, 8 }, /* SPR_TILE_9 */
		{  72, 120,  8, 8 }, /* SPR_TILE_10 */
		{  80, 120,  8, 8 }, /* SPR_TILE_11 */
		{  88, 120,  8, 8 }, /* SPR_TILE_12 */
		{  96, 120,  8, 8 }, /* SPR_TILE_13 */
		{ 104, 120,  8, 8 }  /* SPR_CURSOR */
};

static const Sequence g_Sequences[SEQ_COUNT] = {
	/* SEQ_PLAYER_DEAD */
	{
		.frames = { SPR_PLAYER_DEAD_1, SPR_PLAYER_DEAD_2, SPR_PLAYER_DEAD_3, SPR_PLAYER_DEAD_4 },
		.length = 4
	},
	/* SEQ_HIT */
	{
		.frames = { SPR_HIT_1, SPR_HIT_2, SPR_HIT_3, SPR_HIT_4, -1 },
		.length = 5
	},
	/* SEQ_BULLET_HIT */
	{
		.frames = { SPR_BULLET_HIT_1, SPR_BULLET_HIT_2, SPR_BULLET_HIT_3, SPR_BULLET_HIT_4, -1},
		.length = 4
	},
	/* SEQ_BULLET_A */
	{
		.frames = { SPR_BULLET_A_1, SPR_BULLET_A_2, SPR_BULLET_A_3, SPR_BULLET_A_4 },
		.length = 4
	},
	/* SEQ_BULLET_B */
	{
		.frames = { SPR_BULLET_B_1, SPR_BULLET_B_2, SPR_BULLET_B_3, SPR_BULLET_B_4 },
		.length = 4
	},
	/* SEQ_BULLET_C */
	{
		.frames = { SPR_BULLET_C_1, SPR_BULLET_C_2, SPR_BULLET_C_3, SPR_BULLET_C_4 },
		.length = 4
	},
};

void SetGraphicsColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SDL_SetTextureColorMod(g_SpriteSheet, r, g, b);
	SDL_SetTextureAlphaMod(g_SpriteSheet, a);
}

void DrawTile(SDL_Renderer *renderer, int tile, int x, int y)
{
	if (tile < 0 || tile > 255)
		return;

	SDL_Rect dst = {x, y, GLYPH_SIZE, GLYPH_SIZE};
	SDL_Rect src = {
		src.x = (tile % 16) * GLYPH_SIZE,
		src.y = (tile / 16) * GLYPH_SIZE,
		GLYPH_SIZE,
		GLYPH_SIZE
	};

	SDL_RenderCopy(renderer, g_SpriteSheet, &src, &dst);
}

void DrawTileRGBA(SDL_Renderer *renderer, int tile, int x, int y, unsigned int rgba)
{
	SetGraphicsColor(
			(rgba & 0xFF000000) >> 24,
			(rgba & 0x00FF0000) >> 16,
			(rgba & 0x0000FF00) >> 8,
			(rgba & 0x000000FF));
	DrawTile(renderer, tile, x, y);
}

void DrawSprite(SDL_Renderer *renderer, SpriteID sprite, int x, int y)
{
	if (sprite < 0 || sprite > SPR_COUNT-1)
		return;

	SDL_Rect dst = { x, y, g_Sprites[sprite].w, g_Sprites[sprite].h };
	SDL_RenderCopy(renderer, g_SpriteSheet, &g_Sprites[sprite], &dst);
}

void DrawSpriteRGBA(SDL_Renderer *renderer, SpriteID sprite, int x, int y, unsigned int rgba)
{
	SetGraphicsColor(
			(rgba & 0xFF000000) >> 24,
			(rgba & 0x00FF0000) >> 16,
			(rgba & 0x0000FF00) >> 8,
			(rgba & 0x000000FF));
	DrawSprite(renderer, sprite, x, y);
}

int DrawChar(SDL_Renderer *renderer, unsigned int c, int x, int y)
{
	if (c < 0x80)
	{
		DrawTile(renderer, c, x, y);
		return 1;
	}
	else
	{
		for(int i = 0; i < 12; i++)
		{
			if (c == g_UTF8Map[i])
			{
				DrawTile(renderer, 128+i, x, y);
				return 1;
			}
		}
	}
	return 0;
}

int DrawCharRGBA(SDL_Renderer *renderer, unsigned int c, int x, int y, unsigned int rgba)
{
	SetGraphicsColor(
			(rgba & 0xFF000000) >> 24,
			(rgba & 0x00FF0000) >> 16,
			(rgba & 0x0000FF00) >> 8,
			(rgba & 0x000000FF));

	return DrawChar(renderer, c, x, y);
}

static int draw_text(SDL_Renderer *renderer, const char *text, bool formatted, int *x, int *y, va_list args)
{
	int start_x = *x;
	int count   = 0;

	for(int i = 0; text[i]; i++)
	{
		unsigned char c = (unsigned char)text[i];

		if (c == ' ')
		{
			*x += GLYPH_SIZE;
			count++;
			continue;
		}
		else if (c == '\n')
		{
			*x = start_x;
			*y += GLYPH_SIZE;
			count++;
			continue;
		}
		else if (formatted && c == '%')
		{
			char format[8] = {0};
			char temp[256] = {0};
			char f = text[i+1];

			//TODO: Implement floating point precision format specifier

			switch(f)
			{
				case '\0':
					continue;
				case 'd':
				case 'x':
				case 'X':
				case 'c':
				case 's':
					snprintf(format, 8, "%%%c", f);
					vsnprintf(temp, 256, format, args);
					count += draw_text(renderer, temp, false, x, y, args);
					i++;
					continue;
				case 'f':
					vsnprintf(temp, 256, "%.2f", args);
					count += draw_text(renderer, temp, false, x, y, args);
					i++;
					continue;
				default:
					break;
			}
		}
		else if (c == '^')
		{
			unsigned char r = 0, g = 0, b = 0;
			char *endptr = NULL;
			int code = strtol(&text[i+1], &endptr, 10);

			if (code > 0 && code <= 8)
			{
				r = (code & 1) ? 255 : 0;
				g = (code & 2) ? 255 : 0;
				b = (code & 4) ? 255 : 0;
				SDL_SetTextureColorMod(g_SpriteSheet, r, g, b);

				int offset = (endptr - (text + i)) - 1;
				i += offset;
				continue;
			}
		}

		if (c < 0x80)
		{
			DrawTile(renderer, c, *x, *y);
			*x += GLYPH_SIZE;
			count++;
		}
		else if (c > 0xC1 && c < 0xF5)
		{
			const unsigned char *str = (unsigned char *)text;
			unsigned int utf8 = (unsigned int)c;

			int index = i;

			while((str[++index] & 0xC0) == 0x80)
				utf8 = (utf8 << 8) | (unsigned int)str[index];

			i = index - 1;

			for(int u = 0; u < 12; u++)
			{
				if (utf8 == g_UTF8Map[u])
				{
					DrawTile(renderer, 128+u, *x, *y);
					*x += GLYPH_SIZE;
					count++;
					break;
				}
			}
		}
	}

	return count;
}

int DrawText(SDL_Renderer *renderer, const char *str, int x, int y, ...)
{
	int count;
	va_list args;

	va_start(args, y);
	count = draw_text(renderer, str, true, &x, &y, args);
	va_end(args);

	return count;
}

int DrawTextRGBA(SDL_Renderer *renderer, const char *str, int x, int y, unsigned int rgba, ...)
{
	int count;
	va_list args;

	SetGraphicsColor(
			(rgba >> 24) & 0xFF,
			(rgba >> 16) & 0xFF,
			(rgba >> 8 ) & 0xFF,
			(rgba      ) & 0xFF);

//	SetGraphicsColor(
//			(rgba & 0xFF000000) >> 24,
//			(rgba & 0x00FF0000) >> 16,
//			(rgba & 0x0000FF00) >> 8,
//			(rgba & 0x000000FF));

	va_start(args, rgba);
	count = draw_text(renderer, str, true, &x, &y, args);
	va_end(args);

	return count;
}

void InitAnimation(Animation *anim, SequenceID seq_id, float seconds)
{
	anim->seq_id   = seq_id;
	anim->pivot    = 0;
	anim->duration = seconds;
	anim->timer    = 0.0f;
}

void PlayAnimation(SDL_Renderer *renderer, Animation *anim, float dt, int x, int y)
{
	if (anim->seq_id < 0 || anim->seq_id > SEQ_COUNT-1)
		return;

	const Sequence *seq = &g_Sequences[anim->seq_id];
	const float step = anim->duration / seq->length;

	anim->timer += dt;
	if (anim->timer >= step)
	{
		anim->timer -= step;
		anim->pivot++;

		if (anim->pivot == seq->length)
			anim->pivot = 0;
	}

	DrawSprite(renderer, seq->frames[anim->pivot], x, y);
}

void DrawSpriteSheet(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, g_SpriteSheet, NULL, &(SDL_Rect){0,0,128,128});
}

///// Input

static char g_KeyState[SDL_NUM_SCANCODES];
static bool g_AnyKeyPressed;

static void input_update()
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

///// Scene

static SceneID g_SceneID;
static bool    g_ChangeScene;
static Scene   g_Scene;

void SwitchScene(SceneID id)
{
	g_SceneID = id;
	g_ChangeScene = true;
}

///// Settings

static const Settings g_DefaultSettings = {
	.highscore = {
		57300, 37500, 30000, 25000, 20000, 19500, 19000, 15000, 10000, 5000
	},
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
	.volume = 50,
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

#if (0)
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

#if (0)
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

	if (input != 0)
		PlaySoundVol(SND_CURSOR, 20);
	if (left || right || accept)
		PlaySoundVol(SND_CONFIRM, 20);


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
			{
				ModScreen(window, settings, right-left, 0);
			}
			break;
		case 3:
			if (left || right)
			{
				ModScreen(window, settings, 0, right-left);
			}
			break;
		case 4:
			if (left || right)
			{
				settings->vsync = !settings->vsync;
				SDL_RenderSetVSync(renderer, settings->vsync);
			}
			break;
		case 5:
			if (left || right)
			{
				char val = (right-left) * 5;
				settings->volume = CLAMP(settings->volume + val, 0, 100);
			}
		case 6:
			if (accept)
			{
				*settings = g_DefaultSettings;
				SDL_SetWindowFullscreen(window, settings->fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
				SDL_RenderSetVSync(renderer, settings->vsync);
				ModScreen(window, settings, 0, 0);
			}
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
	DrawTextRGBA(renderer, "VOLUME             %d",     16, 128, g_SettingsSelect == 5 ? 0xFF0000FF : 0x808080FF, settings->volume);
	DrawTextRGBA(renderer, "RESET ALL"           ,      16, 240, g_SettingsSelect == 6 ? 0xFF0000FF : 0x808080FF);
}

///// Game

static Uint64 g_TimeCurr;
static Uint64 g_TimePrev;
static double g_ElapsedTime;
static double g_Accumulator;

void InitGame(GameContext *game, int argc, char *argv[])
{
	Uint32 flags_init   = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
	Uint32 flags_window = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
	Uint32 flags_render = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;

	SDL_Window   *window;
	SDL_Renderer *renderer;
	SDL_Texture  *fbuffer;

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

	int angle = game->settings.angle;
	int scale = game->settings.scale;
	int w = (angle % 2) == 0 ? WINDOW_W : WINDOW_H;
	int h = (angle % 2) == 0 ? WINDOW_H : WINDOW_W;

	// Init SDL
	//SDL_SetHint(SDL_HINT_AUDIODRIVER, "alsa");
	ASSERT(SDL_Init(flags_init) == 0, SDL_GetError());

	window = SDL_CreateWindow("C Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w * scale, h * scale, flags_window);
	ASSERT(window != NULL, SDL_GetError());

	renderer = SDL_CreateRenderer(window, -1, flags_render);
	ASSERT(renderer != NULL, SDL_GetError());

	fbuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_W, WINDOW_H);
	ASSERT(fbuffer != NULL, SDL_GetError());

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetWindowMinimumSize(window, w * scale, h * scale);

	// Sound
	// All sound effects are 44100hz, 16bit signed int, mono
	SDL_AudioSpec spec = {
		.freq     = SND_FREQ,
		.format   = SND_FORMAT,
		.channels = SND_CHANNELS,
		.callback = audio_callback,
		.samples  = SND_SAMPLES,
		.userdata = &game->settings.volume
	};

	g_AudioDevice = SDL_OpenAudioDevice(NULL, 0, &spec, &g_AudioSpec, 0);
	ASSERT(g_AudioDevice != 0, SDL_GetError());

	SDL_AudioSpec dummy;
	for (int i = 0; i < SND_COUNT; i++)
		ASSERT(SDL_LoadWAV(g_SoundSrc[i], &dummy, &g_Sound[i].buf, &g_Sound[i].len) != NULL, SDL_GetError());

	SDL_PauseAudioDevice(g_AudioDevice, 0);

	// Graphics
	SDL_Surface *surface = SDL_LoadBMP("assets/sprites.bmp");
	ASSERT(surface != NULL, SDL_GetError());

	SDL_SetColorKey(surface, true, SDL_MapRGBA(surface->format, 0, 0, 0, 0xFF));

	g_SpriteSheet = SDL_CreateTextureFromSurface(renderer, surface);
	ASSERT(g_SpriteSheet != NULL, SDL_GetError());
	SDL_SetTextureBlendMode(g_SpriteSheet, SDL_BLENDMODE_BLEND);

	SDL_FreeSurface(surface);

	// Scene
	//g_Scene = GetSceneTitle();
	g_Scene = GetScenePlay();

	// Time
	g_TimePrev = SDL_GetPerformanceCounter();

	// Game Context
	game->window     = window;
	game->renderer   = renderer;
	game->fbuffer    = fbuffer;
	game->framerate  = DEFAULT_FPS;
	game->is_running = true;
}

void CloseGame(GameContext *game)
{
	for (int i = 0; i < SND_COUNT; i++)
		SDL_FreeWAV(g_Sound[i].buf);

	SDL_DestroyTexture(game->fbuffer);
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);
	SDL_Quit();

	WriteSAVEDAT(&game->settings);
}

void GameLoop(GameContext *game)
{
	const double fixed_dt = 1.0f / (double)game->framerate;

	// Timing
	g_TimeCurr = SDL_GetPerformanceCounter();
	g_ElapsedTime = (double)(g_TimeCurr - g_TimePrev) / (double)SDL_GetPerformanceFrequency();
	g_TimePrev = g_TimeCurr;
	g_Accumulator += g_ElapsedTime;

	// Clear Screen
	SDL_SetRenderDrawColor(game->renderer, 16, 16, 16, 255);
	SDL_RenderClear(game->renderer);
	SDL_SetRenderTarget(game->renderer, game->fbuffer);
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
	SDL_RenderClear(game->renderer);

	// Reset Spritesheet Color to White
	SetGraphicsColor(255, 255, 255, 255);

	// Events
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
							return;
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

	input_update();

	// Scene
	if (g_ChangeScene)
	{
		switch(g_SceneID)
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


	g_Scene.update(game, g_ElapsedTime);

	for(; g_Accumulator > fixed_dt; g_Accumulator -= fixed_dt)
	{
		g_Scene.fixed_update(game, fixed_dt);
		InputClear();
	}

	const double alpha = g_Accumulator / fixed_dt;

	g_Scene.draw(game, g_ElapsedTime, alpha);

	update_transition(game->renderer, g_ElapsedTime);

#if (1)
	SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 32);
	for(int x = 0; x < WINDOW_W; x +=8 )
		SDL_RenderDrawLine(game->renderer, x, 0, x, WINDOW_H);
	for(int y = 0; y < WINDOW_H; y +=8 )
		SDL_RenderDrawLine(game->renderer, 0, y, WINDOW_W, y);
	SDL_SetRenderDrawColor(game->renderer, 255, 0, 255, 64);
	SDL_RenderDrawLine(game->renderer, WINDOW_W/2, 0, WINDOW_W/2, WINDOW_H);
	SDL_RenderDrawLine(game->renderer, 0, WINDOW_H/2, WINDOW_W, WINDOW_H/2);

	//DrawTextRGBA(game->renderer, "VSync: %s", 0, 0, 0xFFFF0080, game->settings.vsync ? "YES" : "NO");
	//DrawTextRGBA(game->renderer, "Fullscreen: %s", 0, 8, 0xFFFF0080, game->settings.fullscreen ? "YES" : "NO");
	//DrawTextRGBA(game->renderer, "Rotation: %d°", 0, 16, 0xFFFF0080, game->settings.angle*90);
#endif

	// Present to Screen
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

