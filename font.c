#include "font.h"

//int main()
//{
//	// Variables
//	SDL_Window *window;
//	SDL_Renderer *renderer;
//	SDL_Surface *surface;
//	SDL_Texture *texture;
//	SDL_Event event;
//	BitmapFont *font;
//	int printed = 0;
//
//	// Init SDL
//	SDL_Init(SDL_INIT_VIDEO);
//	window = SDL_CreateWindow("Font", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN );
//	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
//	
//	// Load Texture
//	surface = SDL_LoadBMP("../resources/font.bmp");
//	texture = SDL_CreateTextureFromSurface(renderer, surface);
//	SDL_FreeSurface(surface);
//	SDL_Rect r = {0};
//	SDL_QueryTexture(texture, NULL, NULL, &r.w, &r.h);
//
//	// Create Font
//	font = FontCreate(texture, " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_ÁÉÍÓÚÑ♠♥♦♣", 8, 8);
//	SDL_Color red = {0xFF, 0, 0, 0xFF};
//	SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
//
//	int loop = 1;
//
//	while(loop)
//	{
//		while(SDL_PollEvent(&event))
//		{
//			if(event.type ==SDL_QUIT)
//			{
//				loop = 0;
//			}
//		}
//
//		// Prepare
//		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
//		SDL_RenderClear(renderer);
//
//		// Render Texture
//		SDL_RenderCopy(renderer, texture, NULL, &r);
//
//		// Print Font
//		FontPrint(renderer, font, NULL, "ABC 123 Ñ\nÁÉÍÓÚ♠♥♦♣%d", 0, r.h, 2, NULL, 99);
//		FontPrint(renderer, font, NULL, "REDred1", 0, r.h + 32, 2, &red);
//		printed = FontPrint(renderer, font, NULL, "WHITEwhite2", 0, r.h + 48, 2, &white);
//
//		SDL_SetRenderDrawColor(renderer, 0, 0, 0xFF, 0xFF);
//		SDL_RenderDrawLine(renderer, 0, 8, 640, 8);
//		SDL_RenderDrawLine(renderer, 0, 16, 640, 16);
//		SDL_RenderDrawLine(renderer, 0, 24, 640, 24);
//		SDL_RenderDrawLine(renderer, 0, 32, 640, 32);
//		SDL_RenderDrawLine(renderer, 0, 40, 640, 40);
//		SDL_RenderDrawLine(renderer, 0, 48, 640, 48);
//		SDL_RenderDrawLine(renderer, 0, 56, 640, 56);
//		SDL_RenderDrawLine(renderer, 0, 64, 640, 64);
//		SDL_RenderDrawLine(renderer, 0, 72, 640, 72);
//		SDL_RenderDrawLine(renderer, 0, 80, 640, 80);
//
//		// Present
//		SDL_RenderPresent(renderer);
//	}
//
//	printf("Printed Chars: %d\n", printed);
//
//	//for(int i = 0; i < font->mapSize; i++)
//	//{
//	//	printf("Map[i]: %X\n", font->map[i]);
//	//}
//
//	// Clean Up
//	FontDestroy(font);
//	SDL_DestroyTexture(texture);
//	SDL_DestroyWindow(window);
//	SDL_DestroyRenderer(renderer);
//	SDL_Quit();
//}

///// Helper Functions

static size_t utf8_strlen(const char *str);
static Uint32* map_string_to_uint32(const char *str);
static int DrawGlyph(SDL_Renderer *renderer, BitmapFont *font, Uint32 v, int x, int y);

/////

BitmapFont *FontCreate(SDL_Texture *texture, const char *char_map, int glyph_w, int glyph_h)
{
	// Variables
	BitmapFont *font = NULL;
	int map_len;
	int cols, rows;

	// Check texture
	if (texture == NULL)
	{
		fprintf(stderr, "Error: Couldn't create Font. Invaild Texture.\n");
		return NULL;
	}

	// Get rows and columns
	SDL_QueryTexture(texture, NULL, NULL, &cols, &rows);
	cols /= glyph_w;
	rows /= glyph_h;

	// Get char_map length
	map_len = utf8_strlen(char_map);

	// Check if the ammount of glyphs are less than the map length
	if ((cols * rows) < map_len)
	{
		fprintf(stderr, "Error: Texture is too small.\n");
		return NULL;
	}

	// Fill font
	font = (BitmapFont *)malloc(sizeof(BitmapFont));
	font->map = map_string_to_uint32(char_map);
	font->mapSize = map_len;
	font->glyphSize = (SDL_Point) {glyph_w, glyph_h};
	font->texture = texture;

	return font;
}

void FontDestroy(BitmapFont *font)
{
	if (font != NULL)
	{
		free(font->map);
		free(font);
	}
}

int FontPrint(SDL_Renderer *renderer, BitmapFont *font, const char *str, int x, int y, SDL_Color *color, ...)
{
	// Variables
	va_list list;
	char temp_str[TEMP_STR_LEN] = {0};

	const Uint8 *s;
	int printed = 0;
	Uint32 value = 0;
	SDL_Point position = {x, y};

	SDL_Color prev_color;

	// Check pointers
	if (renderer == NULL || font == NULL || str == NULL)
	{
		fprintf(stderr,"Error at %s: Renderer: %d | Font: %d | String: %d.\n", __func__, renderer != NULL, font != NULL, str != NULL);
		return 0;
	}
	
	// Check if a new color was specified
	if (color != NULL)
	{
		// Store previous color and alpha modulation and change to new
		SDL_GetTextureColorMod(font->texture, &prev_color.r, &prev_color.g, &prev_color.b);
		SDL_GetTextureAlphaMod(font->texture, &prev_color.a);

		SDL_SetTextureColorMod(font->texture, color->r, color->g, color->b);
		SDL_SetTextureAlphaMod(font->texture, color->a);
	}

	// Cast string to uint8 array
	s = (const Uint8*)str;

	// Start va_args
	va_start(list, color);

	// Process Text
	for(size_t i = 0; s[i]; i++)
	{
		// Check for space
		if (s[i] == ' ')
		{
			position.x += font->glyphSize.x;
			printed++;
			continue;
		}

		// Check for new line
		if (s[i] == '\n')
		{
			position.x = x;
			position.y += font->glyphSize.y;
			printed++;
			continue;
		}

		// Format
		if (s[i] == '%')
		{
			// Go to next index
			i++;

			// Break loop if NULL
			if (s[i] == '\0') { break; }

			switch(s[i])
			{
				// Integer (Decimal)
				case 'd':
					{
						SDL_itoa(va_arg(list, int), temp_str, 10);
						for(int j = 0; temp_str[j]; j++)
						{
							DrawGlyph(renderer, font, (Uint8)temp_str[j], position.x, position.y);
							position.x += font->glyphSize.x;
							printed++;
						}
						continue;
					}
			}
		}

		// Get cast char to Uint32
		value = (Uint32)s[i] & 0xFF;

		// Check if it's an UTF-8 value
		if (value > 0xC1 && value < 0xF5)
		{
			while ((s[++i] & 0xC0) == 0x80)
			{
				value = (value << 8) | s[i];
			}
			i--;
		}

		if(DrawGlyph(renderer, font, value, position.x, position.y))
		{
			position.x += font->glyphSize.x;
			printed++;
		}
	}

	// Swap back to previous modulation
	if (color != NULL)
	{
		SDL_SetTextureColorMod(font->texture, prev_color.r, prev_color.g, prev_color.b);
		SDL_SetTextureAlphaMod(font->texture, prev_color.a);
	}

	// End va_args
	va_end(list);

	return printed;
}

static size_t utf8_strlen(const char *str)
{
	const Uint8 *s;
	size_t len = 0;

	// Check pointer
	if (str == NULL)
	{
		fprintf(stderr, "Error at %s: Invalid string", __func__);
		return 0;
	}

	// Cast string to uint8 array
	s = (const Uint8*)str;

	// Count characters
	for(int i = 0; s[i]; i++)
	{
		if ((s[i] > 0xC1 && s[i] < 0xF5) || s[i] < 0x80) { len++; }
	}
	
	return len;
}

static Uint32* map_string_to_uint32(const char *str)
{
	const Uint8 *s;
	Uint32 *map = NULL;
	size_t map_len = 0;
	size_t map_index = 0;

	// Check pointer
	if (str == NULL)
	{
		fprintf(stderr, "Error at %s: Invalid string", __func__);
		return 0;
	}

	// Cast string to uint8 array
	s = (const Uint8*)str;

	// Get string length
	map_len = utf8_strlen(str);
	if (map_len == 0) { return NULL; }

	// Allocate memory for an uint32 array of the same size as string
	// uint32 is big enough to fit all UTF-8 compliant characters
	map = (Uint32*)calloc(map_len, sizeof(Uint32));

	// Store characters as an uint32 value
	for(int i = 0; s[i]; i++)
	{
		if (s[i] > 0xC1 && s[i] < 0xF5) // UTF-8 Initial Byte
		{
			map[map_index] = s[i];
			while ((s[++i] & 0xC0) == 0x80)
			{
				map[map_index] = (map[map_index] << 8) | s[i];
			}
			map_index++;
			i--;
		}
		else if (s[i] < 0x80)	// ASCII character
		{
			map[map_index] = s[i];
			map_index++;
		}
	}

	return map;
}

static int DrawGlyph(SDL_Renderer *renderer, BitmapFont *font, Uint32 v, int x, int y)
{
	for(int j = 0; j < font->mapSize; j++)
	{
		if (font->map[j] == v)
		{
			SDL_Rect src = {0, 0, font->glyphSize.x, font->glyphSize.y};
			SDL_Rect dst = {x, y, font->glyphSize.x, font->glyphSize.y};
			int max_cols;
			int col;
			int row;

			SDL_QueryTexture(font->texture, NULL, NULL, &max_cols, NULL);
			max_cols /= font->glyphSize.x;

			col = j % max_cols;
			row = j / max_cols;

			src.x = col * font->glyphSize.x;
			src.y = row * font->glyphSize.y;

			SDL_RenderCopy(renderer, font->texture, &src, &dst);

			return 1;
		}
	}
	return 0;
}
