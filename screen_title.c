
const u8 g_TilemapC[] = { // 8x8
	   0,   0,0x73,0x70,0x70,0x74,   0,   0,
	   0,0x73,0x72,0x77,0x77,0x71,0x74,   0,
	0x73,0x72,0x73,0x70,0x70,0x74,0x71,0x74,
	0x70,0x75,0x70,0x7A,0x79,0x7A,   0,0x70,
	0x70,0x75,0x70,0x7C,0x7B,0x7C,   0,0x70,
	0x71,0x74,0x71,0x70,0x70,0x72,0x73,0x72,
	   0,0x71,0x74,0x78,0x78,0x73,0x72,   0,
	   0,   0,0x71,0x70,0x70,0x72,   0,   0,
};

const u8 g_TilemapInvaders[] = { // 24x5
	0x70,0x70,0x75,0x76,0x75,0x76,0x70,   0,0x70,0x7B,0x70,0x74,0x75,0x70,0x74,0x75,0x70,0x70,0x75,0x70,0x74,0x7B,0x70,0x74,
	0x75,0x76,0x75,0x70,0x73,0x76,0x70,   0,0x70,0x75,0x76,0x70,0x75,0x76,0x70,0x75,0x76,   0,0x75,0x76,0x70,0x75,0x76,0x79,
	0x75,0x76,0x75,0x70,0x70,0x76,0x70,0x77,0x70,0x75,0x70,0x70,0x75,0x76,0x70,0x75,0x70,0x76,0x75,0x70,0x74,0x79,0x70,0x74,
	0x75,0x76,0x75,0x76,0x71,0x76,0x75,0x70,0x76,0x75,0x76,0x70,0x75,0x76,0x70,0x75,0x76,   0,0x75,0x76,0x70,0x7B,   0,0x70,
	0x70,0x70,0x75,0x76,0x75,0x76,   0,0x70,   0,0x75,0x76,0x70,0x75,0x70,0x72,0x75,0x70,0x70,0x75,0x76,0x70,0x79,0x70,0x72,
};

void ScreenTitle(GameContext *ctx)
{
	SDL_Renderer *renderer = ctx->sdl.renderer;

	static u8 sel = 1;

	i8 down, up, ret;

	while(ctx->is_running && ctx->screen == SCREEN_TITLE)
	{
		// Events
		ProcessEvents(ctx);

		// Input
		up   = IsKeyPressed(SDLK_UP);
		down = IsKeyPressed(SDLK_DOWN);
		ret  = IsKeyPressed(SDLK_RETURN);

		// Selection
		if (up || down)
		{
			sel += down - up;
			sel = CLAMP_WRAP(sel, 1, 3); 
			PlaySound(SFX_CURSOR);
		}


		// PLAY
		if (sel == 1 && ret)
		{
			ctx->is_running = 0;
			PlaySound(SFX_SELECT);
		}
		// CONFIGURE
		if (sel == 2 && ret)
		{
			ctx->screen = SCREEN_CONFIG;
			PlaySound(SFX_SELECT);
		}
		// Exit
		if ((sel == 3 && ret) || IsKeyPressed(SDLK_ESCAPE))
			ctx->is_running = 0;

		// Prepare Rendering
		PrepareTarget(&ctx->sdl);

		// Tilemap C
		for (int i = 0; i < 64; i++)
		{
			int x = ((i % 8) * SPRITE_SIZE) + 88;
			int y = ((i / 8) * SPRITE_SIZE) + 64;
			DrawSpriteColor(renderer, g_TilemapC[i], x, y, (i < 32) ? 0x00FF00FF: 0x00FFFFFF);
		}
		// Tilemap Invaders
		for (int i = 0; i < 120; i++)
		{
			int x = ((i % 24) * SPRITE_SIZE) + 24;
			int y = ((i / 24) * SPRITE_SIZE) + 144;
			DrawSpriteColor(renderer, g_TilemapInvaders[i], x, y, (i < 48) ? 0xFF00FFFF : 0xFFFF00FF);
		}

		DrawText(renderer, "PLAYER", 16, 8, 0x00FFFFFF);
		DrawText(renderer, "%d", 16, 24, 0xFFFFFFFF, 0);
		DrawText(renderer, "HI-Score", 88, 8, 0xFFFF00FF);
		DrawText(renderer, "%d", 88, 24, 0xFFFF00FF, ctx->settings.hi_score);

		DrawText(renderer, "START",     104, 200, (sel == 1) ? 0xFFFF00FF : 0x808000FF);
		DrawText(renderer, "CONFIGURE", 104, 216, (sel == 2) ? 0xFFFF00FF : 0x808000FF);
		DrawText(renderer, "EXIT",      104, 232, (sel == 3) ? 0xFFFF00FF : 0x808000FF);

		DrawText(renderer, "CREDITS (2025)",  56, 256, 0xFFFF00FF);
		DrawText(renderer, "PROGRAMMING",     24, 272, 0xFF0000FF);
		DrawText(renderer, "HIGHWIND",       120, 272, 0x00FFFFFF);
		DrawText(renderer, "SFX FROM",        48, 288, 0xFF0000FF);
		DrawText(renderer, "SHIRU'S NESSFX", 120, 288, 0x00FFFFFF);

		RenderTarget(&ctx->sdl, &ctx->settings);
	}
}