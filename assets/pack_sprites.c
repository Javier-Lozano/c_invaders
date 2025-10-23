#include "SDL.h"

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	SDL_Surface *surface = SDL_LoadBMP("sprites.bmp");
	if (surface == NULL)
	{
		printf("Error: %s\n", SDL_GetError());
		return 1;
	}

	printf("Info:\n");
	printf("\t%dx%d %d Bytes per pixel\n", surface->w, surface->h, surface->format->BytesPerPixel);
	printf("\tPitch: %d (Width x Bytes per Pixel)\n", surface->pitch);
	printf("\t%s\n", SDL_GetPixelFormatName(surface->format->format));
	printf("\tR-Mask: %08X\n", surface->format->Rmask);
	printf("\tG-Mask: %08X\n", surface->format->Gmask);
	printf("\tB-Mask: %08X\n", surface->format->Bmask);
	printf("\tA-Mask: %08X\n", surface->format->Amask);

	FILE *file = fopen("sprites.c", "w");
	if (file == NULL)
		return 1;

	fprintf(file, "// sprites.c\n\n");

	fprintf(file, "#define SPR_W (%d)\n", surface->w);
	fprintf(file, "#define SPR_H (%d)\n", surface->h);
	fprintf(file, "#define SPR_PITCH (%d)\n", surface->pitch);
	if (surface->format->BytesPerPixel == 1)
	{
		fprintf(file, "#define SPR_INDEXED\n");
		fprintf(file, "#define SPR_NCOLORS (%d)\n\n", surface->format->palette->ncolors);
		fprintf(file, "static unsigned char g_SprDataColors[SPR_NCOLORS][4] = {\n");
		for(int c = 0; c < surface->format->palette->ncolors; c++)
		{
			SDL_Color col = surface->format->palette->colors[c];
			fprintf(file, "\t{%3d,%3d,%3d,%3d", col.r, col.g, col.b, col.a);
			fprintf(file, "}");
			if (c != (surface->format->palette->ncolors-1))
			fprintf(file, ",\n");
		}
		fprintf(file, "\n};\n\n");
	}

	fprintf(file, "static unsigned char g_SprData[%d] = {\n", surface->pitch * surface->h);

	Uint8 *pixels = (Uint8*)surface->pixels;
	for(int y = 0; y < surface->h; y++)
	{
		fprintf(file, "\t");
		for(int x = 0; x < surface->w; x++)
		{
			int index = (y * surface->pitch) + (x % surface->w);
			fprintf(file, "%d", pixels[index]);
			if (y != surface->h - 1 || x != surface->w - 1)
				fprintf(file, ",");
		}
		fprintf(file, "\n");
	}

	fprintf(file, "};\n");

	fclose(file);

	SDL_FreeSurface(surface);
	return 0;
}
