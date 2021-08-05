#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "PathNode.h"
#include "Renderer.h"
#include "Tile.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
}


void PlayScene::update()
{
	updateDisplayList();
	/*m_mousePosition = EventManager::Instance().getMousePosition();
	m_pEnemy->setTargetPosition(m_mousePosition);*/

	m_CheckEnemyLOS(m_pPlayer);
	m_CheckDetection(m_pPlayer);

	if (m_pEnemy->hasDetection())
	{
		m_pEnemy->setTargetPosition(m_pPlayer->getTransform()->position);
		m_pEnemy->setAnimationState(ENEMY_WALK_DOWN);
		m_pEnemy->Seek();
	}

	// Set three conditions of decision tree here.
	decisionTree->getLOSNode()->setLOS(decisionTree->getAgent()->hasLOS()); // Or use m_pShip if you want to be lazy/wrong.
	decisionTree->getRadiusNode()->setIsWithinRadius(
		Util::distance(m_pEnemy->getTransform()->position, m_pPlayer->getTransform()->position) <= 400.0f);
	decisionTree->getCloseCombatNode()->setIsWithinCombatRange(
		Util::distance(m_pEnemy->getTransform()->position, m_pPlayer->getTransform()->position) <= 60.0f);


	for (int i = 0; i < m_Obstacles.size(); i++)
	{
		if (CollisionManager::circleCircleCheck(m_pPlayer->m_CollisionRadius, m_Obstacles[i]->m_CollisionRadius) && m_playerFacingRight == false)
		{
			m_pPlayer->getTransform()->position = glm::vec2(m_pPlayer->getTransform()->position.x + 3.0f, m_pPlayer->getTransform()->position.y);
			
		}
		else if (CollisionManager::circleCircleCheck(m_pPlayer->m_CollisionRadius, m_Obstacles[i]->m_CollisionRadius) && m_playerFacingRight == true)
		{
			m_pPlayer->getTransform()->position = glm::vec2(m_pPlayer->getTransform()->position.x - 3.0f, m_pPlayer->getTransform()->position.y);
		}
		if (CollisionManager::circleCircleCheck(m_pPlayer->m_CollisionRadius, m_Obstacles[i]->m_CollisionRadius) && m_playerFacingDown == false)
		{
			m_pPlayer->getTransform()->position = glm::vec2(m_pPlayer->getTransform()->position.x, m_pPlayer->getTransform()->position.y + 3.0f);
		}
		else if (CollisionManager::circleCircleCheck(m_pPlayer->m_CollisionRadius, m_Obstacles[i]->m_CollisionRadius) && m_playerFacingDown == true)
		{
			m_pPlayer->getTransform()->position = glm::vec2(m_pPlayer->getTransform()->position.x, m_pPlayer->getTransform()->position.y - 3.0f);
		}
	}
}

void PlayScene::clean()
{
	removeAllChildren();
	// Clean up DecisionTree because it's not a DisplayObject.
	decisionTree->clean();
	delete decisionTree; 
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();
	bool colliding = false;
	//for(int i = 0; i < m_Obstacles.size(); i ++)
	//{
	//	if(CollisionManager::circleCircleCheck(m_pPlayer->m_CollisionRadius, m_Obstacles[i]->m_CollisionRadius))
	//	{
	//		colliding = true;
	//	}
	//}
	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A) && (m_pPlayer->getTransform()->position.x > 0) && colliding == false)
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_pPlayer->SetAccelX(-3.0);
			//m_pPlayer->getTransform()->position.x = m_pPlayer->getTransform()->position.x - 2.0f;
			m_playerFacingRight = false;
			std::cout << "------------------------" << std::endl;
			std::cout << decisionTree->MakeDecision() << std::endl;
			std::cout << "------------------------\n" << std::endl;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D) &&(m_pPlayer->getTransform()->position.x < Config::SCREEN_WIDTH) && colliding == false)
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_pPlayer->SetAccelX(3.0);
			//m_pPlayer->getTransform()->position.x = m_pPlayer->getTransform()->position.x + 2.0f;
			m_playerFacingRight = true;
			std::cout << "------------------------" << std::endl;
			std::cout << decisionTree->MakeDecision() << std::endl;
			std::cout << "------------------------\n" << std::endl;
		}
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W) && (m_pPlayer->getTransform()->position.y > 0) && colliding == false)
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_UP);
			m_pPlayer->SetAccelY(-3.0);
			//m_pPlayer->getTransform()->position.y = m_pPlayer->getTransform()->position.y - 2.0f;
			m_playerFacingDown = false;
			std::cout << "------------------------" << std::endl;
			std::cout << decisionTree->MakeDecision() << std::endl;
			std::cout << "------------------------\n" << std::endl;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_S) && (m_pPlayer->getTransform()->position.y < Config::SCREEN_HEIGHT) && colliding == false)
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_DOWN);
			m_pPlayer->SetAccelY(3.0);
			//m_pPlayer->getTransform()->position.y = m_pPlayer->getTransform()->position.y + 2.0f;
			m_playerFacingDown = true;
			std::cout << "------------------------" << std::endl;
			std::cout << decisionTree->MakeDecision() << std::endl;
			std::cout << "------------------------\n" << std::endl;
		}
	}

	if(EventManager::Instance().getMouseButton(RIGHT))
	{
		Bullet* bill = new Bullet();
		m_mousePosition = EventManager::Instance().getMousePosition();
		//bill->getTransform()->position = m_mousePosition;
		if (m_mousePosition.y > m_pPlayer->getTransform()->position.y)
		{
			bill->setAnimationState(BULLET_DOWN);
			bill->getTransform()->position = glm::vec2( m_pPlayer->getTransform()->position.x, m_pPlayer->getTransform()->position.y + 30.0f);
			bill->SetAccelY(2.0f);
			
		}
		else if(m_mousePosition.y < m_pPlayer->getTransform()->position.y)
		{

			bill->setAnimationState(BULLET_UP);
			bill->getTransform()->position = glm::vec2(m_pPlayer->getTransform()->position.x, m_pPlayer->getTransform()->position.y - 30.0f);
			bill->SetAccelY(-2.0f);
		}
		else if (m_mousePosition.x > m_pPlayer->getTransform()->position.x)
		{
			bill->getTransform()->position = glm::vec2(m_pPlayer->getTransform()->position.x + 30.0f, m_pPlayer->getTransform()->position.y);
			bill->SetAccelX(2.0f);
		}
		
		else if (m_mousePosition.x <= m_pPlayer->getTransform()->position.x)
		{
			bill->getTransform()->position.x = m_pPlayer->getTransform()->position.x - 30.0f;
			bill->SetAccelX(-2.0f);
		}
		
		addChild(bill);
		m_pBullets.push_back(bill);
		bill = nullptr;
	}
	
	if (EventManager::Instance().getMouseButton(LEFT))
	{
		m_pPlayer->setAnimationState(PLAYER_CLOSE_ATTACK);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance().quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_H))
	{
		
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	// Add Background
	m_pBackground = new Background();
	m_pBackground->getTransform()->position = glm::vec2(0.0f, 0.0f);
	addChild(m_pBackground);

	m_buildNodeGrid();
	m_pPatrolPath.push_back(m_getTile(10, 5));
	m_pPatrolPath.push_back(m_getTile(1, 5));
	
	//m_buildTileGrid();

	//////Very Long way of feeding in all obstacles... Need a better way to do this
	////All for groups of Impassable objects
	/*for (int i = 0; i < 6; i++)
	{
		Obstacle* obs = new Obstacle();
		obs->getTransform()->position = m_getTile(i + 1, 11)->getTransform()->position;
		obs->setGridPosition(i + 1, 11);
		m_Obstacles.push_back(obs);
		addChild(obs);
	}*/
	//for (int i = 0; i < 5; i++)
	//{
	//	Obstacle* obs = new Obstacle();
	//	obs->getTransform()->position = m_getTile(i + 1, 12)->getTransform()->position + offset;
	//	obs->setGridPosition(i + 1, 12);
	//	m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//	m_Obstacles.push_back(obs);
	//	addChild(obs);
	//}
	//for(int i = 0; i < 6; i ++)
	//{
	//	Obstacle* obs = new Obstacle();
	//	obs->getTransform()->position = m_getTile(i, 10)->getTransform()->position + offset;
	//	obs->setGridPosition(i , 10);
	//	m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//	m_Obstacles.push_back(obs);
	//	addChild(obs);
	//}
	//for (int i = 0; i < 6; i++)
	//{
	//	Obstacle* obs = new Obstacle();
	//	obs->getTransform()->position = m_getTile(i, 9)->getTransform()->position + offset;
	//	obs->setGridPosition(i, 9);
	//	m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//	m_Obstacles.push_back(obs);
	//	addChild(obs);
	//}
	//for (int i = 1; i < 5; i++)
	//{
	//	Obstacle* obs = new Obstacle();
	//	obs->getTransform()->position = m_getTile(i, 8)->getTransform()->position + offset;
	//	obs->setGridPosition(i, 8);
	//	m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//	m_Obstacles.push_back(obs);
	//	addChild(obs);
	//}
	//for (int i = 1; i < 5; i++)
	//{
	//	Obstacle* obs = new Obstacle();
	//	obs->getTransform()->position = m_getTile(i, 8)->getTransform()->position + offset;
	//	obs->setGridPosition(i, 8);
	//	m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//	m_Obstacles.push_back(obs);
	//	addChild(obs);
	//}
	//for (int i = 1; i < 5; i++)
	//{
	//	Obstacle* obs = new Obstacle();
	//	obs->getTransform()->position = m_getTile(i, 7)->getTransform()->position + offset;
	//	obs->setGridPosition(i, 7);
	//	m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//	m_Obstacles.push_back(obs);
	//	addChild(obs);
	//}
	//for (int i = 3; i < 5; i++)
	//{
	//	Obstacle* obs = new Obstacle();
	//	obs->getTransform()->position = m_getTile(i, 6)->getTransform()->position + offset;
	//	obs->setGridPosition(i, 6);
	//	m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//	m_Obstacles.push_back(obs);
	//	addChild(obs);
	//}
	//for(int i = 5; i < 9; i ++)
	//{
	//	Obstacle* obs = new Obstacle();
	//	obs->getTransform()->position = m_getTile(i, 0)->getTransform()->position + offset;
	//	obs->setGridPosition(i, 0);
	//	m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//	m_Obstacles.push_back(obs);
	//	addChild(obs);
	//}
	//for (int i = 5; i < 9; i++)
	//{
	//	Obstacle* obs = new Obstacle();
	//	obs->getTransform()->position = m_getTile(i, 1)->getTransform()->position + offset;
	//	obs->setGridPosition(i, 1);
	//	m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//	m_Obstacles.push_back(obs);
	//	addChild(obs);
	//}
	//for (int i = 5; i < 9; i++)
	//{
	//	Obstacle* obs = new Obstacle();
	//	obs->getTransform()->position = m_getTile(i, 2)->getTransform()->position + offset;
	//	obs->setGridPosition(i, 2);
	//	m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//	m_Obstacles.push_back(obs);
	//	addChild(obs);
	//}
	//for (int i = 6; i < 8; i++)
	//{
	//	Obstacle* obs = new Obstacle();
	//	obs->getTransform()->position = m_getTile(i, 3)->getTransform()->position + offset;
	//	obs->setGridPosition(i, 3);
	//	m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//	m_Obstacles.push_back(obs);
	//	addChild(obs);
	//}
	//for (int i = 6; i < 8; i++)
	//{
	//	Obstacle* obs = new Obstacle();
	//	obs->getTransform()->position = m_getTile(i, 4)->getTransform()->position + offset;
	//	obs->setGridPosition(i, 4);
	//	m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//	m_Obstacles.push_back(obs);
	//	addChild(obs);
	//}
	//for (int i = 16; i < 18; i++)
	//{
	//	Obstacle* obs = new Obstacle();
	//	obs->getTransform()->position = m_getTile(i, 6)->getTransform()->position + offset;
	//	obs->setGridPosition(i, 6);
	//	m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//	m_Obstacles.push_back(obs);
	//	addChild(obs);
	//}
	//for (int i = 14; i < 18; i++)
	//{
	//	Obstacle* obs = new Obstacle();
	//	obs->getTransform()->position = m_getTile(i, 5)->getTransform()->position + offset;
	//	obs->setGridPosition(i, 5);
	//	m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//	m_Obstacles.push_back(obs);
	//	addChild(obs);
	//}
	//for (int i = 14; i < 18; i++)
	//{
	//	Obstacle* obs = new Obstacle();
	//	obs->getTransform()->position = m_getTile(i, 4)->getTransform()->position + offset;
	//	obs->setGridPosition(i, 4);
	//	m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//	m_Obstacles.push_back(obs);
	//	addChild(obs);
	//}
	//for (int i = 13; i < 17; i++)
	//{
	//	Obstacle* obs = new Obstacle();
	//	obs->getTransform()->position = m_getTile(i, 3)->getTransform()->position + offset;
	//	obs->setGridPosition(i, 3);
	//	m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//	m_Obstacles.push_back(obs);
	//	addChild(obs);
	//}
	//for (int i = 13; i < 19; i++)
	//{
	//	Obstacle* obs = new Obstacle();
	//	obs->getTransform()->position = m_getTile(i, 2)->getTransform()->position + offset;
	//	obs->setGridPosition(i, 2);
	//	m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//	m_Obstacles.push_back(obs);
	//	addChild(obs);
	//}
	//for (int i = 13; i < 19; i++)
	//{
	//	Obstacle* obs = new Obstacle();
	//	obs->getTransform()->position = m_getTile(i, 1)->getTransform()->position + offset;
	//	obs->setGridPosition(i, 1);
	//	m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//	m_Obstacles.push_back(obs);
	//	addChild(obs);
	//}
	//for (int i = 15; i < 17; i++)
	//{
	//	Obstacle* obs = new Obstacle();
	//	obs->getTransform()->position = m_getTile(i, 0)->getTransform()->position + offset;
	//	obs->setGridPosition(i, 0);
	//	m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//	m_Obstacles.push_back(obs);
	//	addChild(obs);
	//}
	//for (int i = 0; i < 2; i++)
	//{
	//	Obstacle* obs = new Obstacle();
	//	obs->getTransform()->position = m_getTile(i, 0)->getTransform()->position + offset;
	//	obs->setGridPosition(i, 0);
	//	m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//	m_Obstacles.push_back(obs);
	//	addChild(obs);
	//}
	//for (int i = 0; i < 2; i++)
	//{
	//	Obstacle* obs = new Obstacle();
	//	obs->getTransform()->position = m_getTile(i, 1)->getTransform()->position + offset;
	//	obs->setGridPosition(i, 1);
	//	m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//	m_Obstacles.push_back(obs);
	//	addChild(obs);
	//}
	//// Single objects
	//Obstacle* obs = new Obstacle();
	//obs->getTransform()->position = m_getTile(0, 3)->getTransform()->position + offset;
	//obs->setGridPosition(0, 3);
	//m_getTile(obs->getGridPosition())->setTileStatus(IMPASSABLE);
	//m_Obstacles.push_back(obs);
	//addChild(obs);
	//
	//
	//Obstacle* obsShrub = new Obstacle();
	//obsShrub->getTransform()->position = m_getTile(6, 13)->getTransform()->position + offset;
	//obsShrub->setGridPosition(6, 13);
	//m_getTile(obsShrub->getGridPosition())->setTileStatus(IMPASSABLE);
	//m_Obstacles.push_back(obsShrub);
	//addChild(obsShrub);

	//obsShrub = nullptr;
	//delete obsShrub;

	//obs = nullptr;
	//delete obs;

	//for (auto obstacle : m_Obstacles)
	//	obstacle->setVisible(true);

	//for(auto obstacle: m_Obstacles)
	//{
	//	addChild(obstacle);
	//}

	// add the Player to the scene
	m_pPlayer = new Player();
	m_pPlayer->getTransform()->position = glm::vec2(700.0f, 500.0f);
	addChild(m_pPlayer);
	m_playerFacingRight = true;
	m_playerFacingDown = true;

	// add Enemy to the scene
	m_pEnemy = new Enemy();
	m_pEnemy->getTransform()->position = glm::vec2(100.0f, 220.0f);
	m_pEnemy->setTargetPosition(m_pPatrolPath[0]->getTransform()->position);
	addChild(m_pEnemy);
	
	decisionTree = new DecisionTree();
	decisionTree->setAgent(m_pEnemy);
	decisionTree->Display();

	std::cout << "------------------------" << std::endl;
	std::cout << decisionTree->MakeDecision() << std::endl;
	std::cout << "------------------------\n" << std::endl;


	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function()
{
	// TODO:
	// Toggle Visibility for the StarShip and the Target
		
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 - M2021 - A3", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static bool gridVisible = true;
	if (ImGui::Checkbox("Toggle Grid", &gridVisible))
	{
		m_bToggleGrid(gridVisible);
	}
	
	// allow ship rotation
	static int angle;
	if (ImGui::SliderInt("Enemy Direction", &angle, -360, 360))
	{
		m_pEnemy->setCurrentHeading(angle);

		std::cout << "------------------------" << std::endl;
		std::cout << decisionTree->MakeDecision() << std::endl;
		std::cout << "------------------------\n" << std::endl;
		std::cout << "Current Direction: (" << m_pEnemy->getCurrentDirection().x<< ", "<<m_pEnemy->getCurrentDirection().y <<")"<<std::endl;
		std::cout << "Current Heading: " << m_pEnemy->getCurrentHeading() << std::endl;
	}

	ImGui::Separator();

	static int shipPosition[] = { m_pEnemy->getTransform()->position.x, m_pEnemy->getTransform()->position.y };
	if (ImGui::SliderInt2("Enemy Position", shipPosition, 0, 800))
	{
		m_pEnemy->getTransform()->position.x = shipPosition[0];
		m_pEnemy->getTransform()->position.y = shipPosition[1];

		std::cout << "------------------------" << std::endl;
		std::cout << decisionTree->MakeDecision() << std::endl;
		std::cout << "------------------------\n" << std::endl;
	}
	static int targetPosition[] = { m_pPlayer->getTransform()->position.x, m_pPlayer->getTransform()->position.y };
	if (ImGui::SliderInt2("Player Position", targetPosition, 0, 800))
	{
		m_pPlayer->getTransform()->position.x = targetPosition[0];
		m_pPlayer->getTransform()->position.y = targetPosition[1];

		std::cout << "------------------------" << std::endl;
		std::cout << decisionTree->MakeDecision() << std::endl;
		std::cout << "------------------------\n" << std::endl;
	}

	ImGui::Separator();
	
	ImGui::End();
}

void PlayScene::m_CheckEnemyLOS(DisplayObject* target_object)
{
	m_pEnemy->setHasLOS(false);
	// if ship to target distance is less than or equal to LOS Distance
	auto ShipToTargetDistance = Util::getClosestEdge(m_pEnemy->getTransform()->position, target_object );
	if (ShipToTargetDistance <= m_pEnemy->getLOSDistance())
	{
		std::vector<DisplayObject*> contactList;
		for (auto object : getDisplayList())
		{
			if ((object->getType() != m_pBackground->getType())&& 
				(object->getType() != m_pEnemy->getType()) && (object->getType() != target_object->getType()) && (object->getType() !=m_pPathNodes[0]->getType()))
			{
				// check if object is farther than than the target
				auto ShipToObjectDistance = Util::getClosestEdge(m_pEnemy->getTransform()->position, object);
				if (ShipToObjectDistance <= ShipToTargetDistance)
					contactList.push_back(object);
			}
		}
		auto hasLOS = CollisionManager::LOSCheck(m_pEnemy->getTransform()->position,
			m_pEnemy->getTransform()->position + (m_pEnemy->getCurrentDirection() * m_pEnemy->getLOSDistance()),
			contactList, target_object);
		m_pEnemy->setHasLOS(hasLOS);
	}
}

void PlayScene::m_CheckDetection(DisplayObject* target_object)
{
	m_pEnemy->setHasDetection(false);
	auto EnemyToTargetDistance = Util::distance(m_pEnemy->getTransform()->position, target_object->getTransform()->position);
	if(EnemyToTargetDistance <= m_pEnemy->getDetectionRadius())
	{
		m_pEnemy->setHasDetection(true);
	}
}

void PlayScene::m_buildNodeGrid()
{
	auto tileSize = Config::TILE_SIZE;

	// add path_nodes to the Grid
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			PathNode* path_node = new PathNode();
			path_node->getTransform()->position = glm::vec2(
				(col * tileSize) + tileSize * 0.5f, (row * tileSize) + tileSize * 0.5f);
			addChild(path_node);
			m_pPathNodes.push_back(path_node);
		}
	}
}

void PlayScene::m_buildTileGrid()
{
	auto tileSize = Config::TILE_SIZE; // alias for Tile size

	// add tiles to the grid
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			Tile* tile = new Tile(); // create a new empty tile
			tile->getTransform()->position = glm::vec2(col * tileSize, row * tileSize); // world position
			tile->setGridPosition(col, row);
			addChild(tile);
			tile->addLabels();
			tile->setEnabled(false);
			m_pTileGrid.push_back(tile);
		}
	}
}

void PlayScene::m_bToggleGrid(bool state)
{
	for(auto path_node :m_pPathNodes)
	{
		path_node->setVisible(state);
	}
}

PathNode* PlayScene::m_getTile(const int col, const int row)
{
	return m_pPathNodes[(row * Config::COL_NUM) + col];
}

PathNode* PlayScene::m_getTile(glm::vec2 grid_position)
{
	const auto col = grid_position.x;
	const auto row = grid_position.y;

	return m_pPathNodes[(row * Config::COL_NUM) + col];
}
