#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "GameOverScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	BaseScene* newScene = nullptr;

	if (sceneName == "title") {
		newScene = new TitleScene();
	}
	else if (sceneName == "game") {
		newScene = new GameScene();
	}
	else if (sceneName == "clear") {
		newScene = new ClearScene();
	}
	else if (sceneName == "gameover") {
		newScene = new GameOverScene();
	}

	return newScene;
}
