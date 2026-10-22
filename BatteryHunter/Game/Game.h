#pragma once

#include "Level3DRender/LevelRender.h"
#include "Unit.h"
using namespace std;

class Player;
class BackGround;
class GameCamera;
class GameClear;
class UI;
class UnitManager;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void SetUnit(Type type, float x, float y, float z);
	void RemoveUnit(Unit* unit);
	
private:

	ModelRender m_modelRender;
	Vector3 m_pos;

	//インスタンス
	BackGround* background_Ins;
	GameCamera* camera_Ins;
	Player*     player_Ins;
	UI*         ui_Ins;
	GameClear* gameclear_Ins;
	UnitManager* unitManager_Ins;
};

