#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"
#include "Renderer.h"

StartScene::StartScene()
{
	StartScene::start();
}

StartScene::~StartScene()
= default;

void StartScene::draw()
{
	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 0, 0, 0, 255);
}

void StartScene::update()
{
	updateDisplayList();
}

void StartScene::clean()
{
	removeAllChildren();
}

void StartScene::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance().quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance().changeSceneState(PLAY_SCENE);
	}

	if (EventManager::Instance().getMouseButton(LEFT))
	{
		m_pPlayer->setAnimationState(PLAYER_CLOSE_ATTACK);
	}

}

void StartScene::start()
{
	const SDL_Color blue = { 0, 0, 255, 255 };
	const SDL_Color red = { 255,0,0,255 };
	const SDL_Color white = { 255,255,255,255 };

	/*m_pStartLabel = new Label("START SCENE", "Consolas", 80, blue, glm::vec2(400.0f, 40.0f));
	m_pStartLabel->setParent(this);
	addChild(m_pStartLabel);*/

	m_pStudentName = new Label("Sean Peoples", "Consolas", 80, red, glm::vec2(400.0f, 40.0f));
	m_pStudentName->setParent(this);
	addChild(m_pStudentName);

	m_pStudentID = new Label("ID #: 101332736", "Consolas", 40, white, glm::vec2(400.0f, 100.f));
	m_pStudentID->setParent(this);
	addChild(m_pStudentID);

	m_pInstructionsLabel = new Label("Press 1 to Play", "Consolas", 40, blue, glm::vec2(400.0f, 200.0f));
	m_pInstructionsLabel->setParent(this);
	addChild(m_pInstructionsLabel);


	m_pPlayer = new Player();
	m_pPlayer->getTransform()->position = glm::vec2(400.0f, 300.0f);
	addChild(m_pPlayer);

	// Start Button
	m_pStartButton = new Button();
	m_pStartButton->getTransform()->position = glm::vec2(400.0f, 400.0f);

	m_pStartButton->addEventListener(CLICK, [&]()-> void
		{
			m_pStartButton->setActive(false);
			TheGame::Instance().changeSceneState(PLAY_SCENE);
		});

	m_pStartButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pStartButton->setAlpha(128);
		});

	m_pStartButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pStartButton->setAlpha(255);
		});
	addChild(m_pStartButton);

	ImGuiWindowFrame::Instance().setDefaultGUIFunction();
}
