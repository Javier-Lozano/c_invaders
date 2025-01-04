
void ScreenConfig(GameContext *ctx)
{
	SDL_Renderer *renderer = ctx->sdl.renderer;
	Settings     *settings = &ctx->settings;

	u8 sel = 1;
	i8 up, down, left, right, ret;

	while(ctx->is_running && ctx->screen == SCREEN_CONFIG)
	{
		// Events
		ProcessEvents(ctx);

		// Input
		left  = IsKeyPressed(SDLK_LEFT);
		right = IsKeyPressed(SDLK_RIGHT);
		up    = IsKeyPressed(SDLK_UP);
		down  = IsKeyPressed(SDLK_DOWN);
		ret   = IsKeyPressed(SDLK_RETURN);

		// Selection
		if (up || down)
		{
			PlaySound(SFX_CURSOR);
			sel += down - up;
			sel = CLAMP_WRAP(sel, 1, 5);
		}
		if (left || right || ret)
			PlaySound(SFX_SELECT);

		// Fullscreen
		if (sel == 1 && (left || right || ret))
		{
			settings->fullscreen = !settings->fullscreen;
			SDL_SetWindowFullscreen(ctx->sdl.window, settings->fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
		}

		// V-Sync
		if (sel == 2 && (left || right || ret))
		{
			settings->vsync = !settings->vsync;
			SDL_RenderSetVSync(ctx->sdl.renderer, settings->vsync);
		}

		// Show FPS
		if (sel == 3 && (left || right || ret))
			settings->show_fps = !settings->show_fps;

		// Scale
		if (sel == 4)
		{
			if (settings->fullscreen)
			{
				sel += (down - up);
			}
			else if (right || left)
			{
				settings->scale += (right - left);
				SetTargetScale(ctx, ctx->settings.scale);
			}
		}

		// Volume
		if (sel == 5 && (left || right))
		{
			settings->volume += (right - left) * 5;
			settings->volume = CLAMP(settings->volume, 0, 100);
			Mix_MasterVolume(settings->volume);
		}

		// Exit
		if (IsKeyPressed(SDLK_ESCAPE))
		{
			PlaySound(SFX_SELECT);
			ctx->screen = SCREEN_TITLE;

			// Save Settings
			SaveFile(&ctx->settings);
		}

		PrepareTarget(&ctx->sdl);

		DrawText(renderer, "CONFIGURE SETTINGS", 16, 16, 0xFFFF00FF);
		DrawText(renderer, "FULLSCREEN",  32,  32, (sel == 1) ? 0xFFFF00FF : 0x808040FF);
		DrawText(renderer, "%s",         128,  32, (sel == 1) ? 0xFF0000FF : 0x804040FF, settings->fullscreen ? "YES" : "NO");
		DrawText(renderer, "V-SYNC",      32,  48, (sel == 2) ? 0xFFFF00FF : 0x808040FF);
		DrawText(renderer, "%s",         128,  48, (sel == 2) ? 0xFF0000FF : 0x804040FF, settings->vsync ? "YES" : "NO");
		DrawText(renderer, "SHOW FPS",    32,  64, (sel == 3) ? 0xFFFF00FF : 0x808040FF);
		DrawText(renderer, "%s",         128,  64, (sel == 3) ? 0xFF0000FF : 0x804040FF, settings->show_fps ? "YES" : "NO");
			if (!settings->fullscreen)
			{
				DrawText(renderer, "SCALE",  32,  80, (sel == 4) ? 0xFFFF00FF : 0x808040FF);
				DrawText(renderer, "%d",    128,  80, (sel == 4) ? 0xFF0000FF : 0x804040FF, settings->scale);
			}
			else
			{
				DrawText(renderer, "SCALE",  32,  80, 0x404040FF);
				DrawText(renderer, "%d",    128,  80, 0x404040FF, settings->scale);
			}
		DrawText(renderer, "VOLUME",      32,  96, (sel == 5) ? 0xFFFF00FF : 0x808040FF);
		DrawText(renderer, "%d",         128,  96, (sel == 5) ? 0xFF0000FF : 0x804040FF, settings->volume);
		DrawText(renderer, "PRESS [",      16, 304, 0xFFFF00FF);
		DrawText(renderer, "ESC",          80, 304, 0xFF0000FF);
		DrawText(renderer, "] TO RETURN", 112, 304, 0xFFFF00FF);

		RenderTarget(&ctx->sdl, settings);
	}
}
