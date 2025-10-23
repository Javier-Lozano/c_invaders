#include "SDL.h"

#define SOUND_COUNT (14)

const char * g_Sources[SOUND_COUNT] = {
	"06_cursor2.wav",     // Cursor
	"59_confirm.wav",     // Confirm
	"57_drop.wav",        // Cancel
	"73_bowrelease1.wav", // Fire Player
	"16_falling.wav",     // Fire Alien
	"28_jingle.wav",      // Hit UFO
	"69_explode.wav",     // Hit Player
	"77_arrowbounce.wav", // Hit Miss
	"78_arrowhit.wav",    // Hit Alien
	"52_step1.wav",       // Alien Step 1
	"53_step2.wav",       // Alien Step 2
	"54_step3.wav",       // Alien Step 3
	"55_step4.wav",       // Alien Step 4
	"64_lose2.wav",       // Game Over
};

const char * g_Names[SOUND_COUNT] = {
	"CURSOR",
	"CONFIRM",
	"CANCEL",
	"FIRE_PLAYER",
	"FIRE_ALIEN",
	"HIT_UFO",
	"HIT_PLAYER",
	"HIT_MISS",
	"HIT_ALIEN",
	"ALIEN_STEP_1",
	"ALIEN_STEP_2",
	"ALIEN_STEP_3",
	"ALIEN_STEP_4",
	"GAME_OVER"
};

// Target Spec
#define FREQ     (44100)
#define FORMAT   (AUDIO_S16)
#define CHANNELS (1)

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	SDL_AudioSpec spec[SOUND_COUNT];
	Uint8  *buffer[SOUND_COUNT];
	Uint32 length[SOUND_COUNT];
	//char   change[SOUND_COUNT] = {0};

	for(int i = 0; i < SOUND_COUNT; i++)
	{
		if (SDL_LoadWAV(g_Sources[i], &spec[i], &buffer[i], &length[i]) == NULL)
		{
			printf("Error: %s\n", SDL_GetError());
			return 1;
		}

		printf("Info:\n");
		printf("\t%s\n", g_Sources[i]);
		printf("\tFrequency: %d\n", spec[i].freq);
		printf("\tFormat: (%X)\n", spec[i].format);
		printf("\tChannels: %d\n", spec[i].channels);
		printf("\tSamples: %d\n", spec[i].samples);
	//	if (spec[i].freq != FREQ || spec[i].format != FORMAT || spec[i].channels != CHANNELS)
	//	{
	//		change[i] = 1;
	//		printf("\tConvert\n");
	//	}
	}

	FILE *file = fopen("sounds.c", "w");
	if (file == NULL)
		return 1;

	fprintf(file, "// sounds.c \n");
	fprintf(file, "// Sound effects by shiru8bit\n");
	fprintf(file, "// https://opengameart.org/content/nes-8-bit-sound-effects\n");
	fprintf(file, "// https://shiru.untergrund.net/index.shtml\n\n");

	fprintf(file, "// Spec\n");
	fprintf(file, "#define SOUND_COUNT    (%d)\n", SOUND_COUNT);
	fprintf(file, "#define SOUND_FREQ     (%d)\n", FREQ);
	fprintf(file, "#define SOUND_CHANNELS (%d)\n", CHANNELS);
	fprintf(file, "#define SOUND_BYTES    (%d)\n", (FORMAT & 0x00FF) / 8);
	fprintf(file, "#define SOUND_IS_FLOAT (%d)\n", (FORMAT >> 8) & 1);
	fprintf(file, "\n// All Sounds are packed toguether in a single buffer\n");
	fprintf(file, "static unsigned int g_SoundIndex[SOUND_COUNT][2] = {\n");
	int index = 0;
	for (int i = 0; i < SOUND_COUNT; i++)
	{
		fprintf(file, "\t{/*Index*/ %6d, /*Length*/ %6d}", index, length[i]);
		if (i != SOUND_COUNT -1)
		fprintf(file, ", // %s\n", g_Names[i]);
		else
		fprintf(file, "  // %s\n", g_Names[i]);
		index += length[i];
	}
	fprintf(file, "};\n\n");

	fprintf(file, "static unsigned char g_SoundBuffer[%d] = {\n", index);
	for (int i = 0; i < SOUND_COUNT; i++)
	{
		fprintf(file, "//\t%s\n\t", g_Names[i]);
		for (Uint32 x = 0; x < length[i]; x++)
		{
			Uint8* b = buffer[i];
			fprintf(file, "%d", b[x]);

			if (i != SOUND_COUNT -1 || x != length[i]-1)
				fprintf(file, ",");

			if (x == length[i]-1)
				fprintf(file, "\n");
		}
	}
	fprintf(file, "};\n");

	fclose(file);

	for(int i = 0; i < SOUND_COUNT; i++)
		SDL_FreeWAV(buffer[i]);

	return 0;
}
