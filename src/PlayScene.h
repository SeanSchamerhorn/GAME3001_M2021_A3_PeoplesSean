#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Button.h"
#include "Label.h"
#include "Obstacle.h"
#include "ship.h"
#include "Background.h"
#include "Bullet.h"
#include "Target.h"
#include "DecisionTree.h"
#include "Enemy.h"
#include "PathNode.h"
#include "Player.h"
#include "Tile.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

	//void moveStarShip() const;
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	glm::vec2 m_mousePosition;
	bool m_isGridEnabled;

	// Game Objects
	
	Enemy* m_pEnemy;
	Player* m_pPlayer;

	
	std::vector<Bullet*> m_pBullets;

	Background* m_pBackground;

	bool m_playerFacingRight;
	bool m_playerFacingDown;
	
	// Vectors
	std::vector<PathNode*> m_pPatrolPath;
	std::vector<PathNode*> m_pPathNodes;
	std::vector<Tile*> m_pTileGrid;
	std::vector<Obstacle*> m_Obstacles;

	void m_buildTileGrid();
	void m_buildNodeGrid();
	
	void m_CheckEnemyLOS(DisplayObject* target_object);
	void m_CheckDetection(DisplayObject* target_object);
	
	// UI Controls
	bool m_gridVisible;
	void m_bToggleGrid(bool state);

	// Decision Tree
	DecisionTree* decisionTree;

	// convenience functions
	PathNode* m_getTile(int col, int row);
	PathNode* m_getTile(glm::vec2 grid_position);
};

#endif /* defined (__PLAY_SCENE__) */