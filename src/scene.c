#include "scene.h"
#include "game.h"
#include "input.h"
#include "macros.h"

static Scene   g_CurrentScene;
static SceneID g_SceneID;
static bool    g_ChangeScene;

void UpdateScene(struct GameContext *game)
{
	const double fixed_dt = 1.0f / INNER_FPS;

	if (g_ChangeScene)
	{
		switch(g_SceneID)
		{
			case SCENE_TITLE:
				g_CurrentScene = GetSceneTitle();
				break;
			case SCENE_PLAY:
				g_CurrentScene = GetScenePlay();
				break;
			default:
				g_CurrentScene = GetSceneTitle();
				break;
		}

		g_ChangeScene = false;
	}

	if (g_CurrentScene.is_starting)
	{
		g_CurrentScene.init(game);
		g_CurrentScene.is_starting = false;
	}

	g_CurrentScene.update(game);

	for(; game->accumulator > fixed_dt; game->accumulator -= fixed_dt)
	{
		g_CurrentScene.fixed_update(game);
		InputStateClear();
	}

	g_CurrentScene.draw(game);
}

void SetScene(SceneID id)
{
	g_ChangeScene = true;
	g_SceneID = id;
}
