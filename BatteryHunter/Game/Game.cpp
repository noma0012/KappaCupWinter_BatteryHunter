#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "GameClear.h"
#include "Player.h"
#include "UI.h"
#include "Unit.h"
#include "BulletManager.h"
#include "UnitManager.h"

//ゲームを管理するクラス
bool Game::Start()
{
	///━━━━━━━━━━━━━━━━━━━━━
	/// ROM作成時の注意点
	/// DebugにするとFPSが表示される
	/// ReleaseにするとFPS表示が消える
	/// ゲームをROMにする場合
	/// Ctrl+Shift+B
	/// (ビルド→ソリューションビルド)を必ず押す
	///  押さなかった場合
	/// 変更したコードが反映されない
	///━━━━━━━━━━━━━━━━━━━━━
	///━━━━━━━━━━━━━━━━━━━━━
	/// ボタン配置について
	/// Xboxのボタン配置
	///━━━━━━━━━━━━━━━━━━━━━
	

	//ステージオブジェクト作成
	background_Ins = NewGO<BackGround>(0, "background");
	//カメラオブジェクト作成
	camera_Ins = NewGO<GameCamera>(0, "gamecamera");
	//プレイヤーオブジェクト作成
	player_Ins = NewGO<Player>(0, "player");

	//ユニット管理クラス作成
	unitManager_Ins = NewGO<UnitManager>(0, "unitManager");
	//ユニット作成
	for (int i = 0; i < 5; i++) {
		unitManager_Ins->SpawnUnit(Type::Basic, 1000.0f, 0.0f, 1000.0f);
	}


	//弾管理オブジェクト作成
	NewGO<BulletManager>(0, "bulletManager");

	//画面UI作成(一番下に配置)
	ui_Ins = NewGO<UI>(0, "ui");
	return true;
}
Game::Game() {

}
Game::~Game() {
	DeleteGO(background_Ins);
	DeleteGO(camera_Ins);
	DeleteGO(player_Ins);
	DeleteGO(ui_Ins);
	DeleteGO(unitManager_Ins);
	
}
void Game::Update()
{
	//ゲーム終了条件
	if (ui_Ins->Finished){
		//ゲームクリアのオブジェクトをつくる。
		gameclear_Ins = NewGO<GameClear>(0, "GameClear");

		//自身を削除(Game)
		DeleteGO(this);
	}
}
void Game::Render(RenderContext& rc)
{

}