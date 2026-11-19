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
#include "BatteryManager.h"
#include "Generator.h"
#include "nature/SkyCube.h"
#include "graphics/RenderingEngine.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 空(スカイキューブ)の設定
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void Game::InitSky()
{
	//既存のスカイキューブを削除
	DeleteGO(m_skyCube);

	//新しいスカイキューブを生成
	m_skyCube = NewGO<SkyCube>(0,"skycube");

	//スカイキューブ自体の明るさ
	//1.0f = 標準
	//2.0f = 明るい
	m_skyCube->SetLuminance(0.5f);

	//空の種類を設定
	m_skyCube->SetType(enSkyCubeType_Night);

	//━━━━━━━━━━━━━━━━━━━━━━
	// 環境光(IBL)設定
	// 空のテクスチャを利用して
	// ステージ全体の明るさを決定する
	//━━━━━━━━━━━━━━━━━━━━━━
	//g_renderingEngine->SetAmbientByIBLTexture(
	//	m_skyCube->GetTextureFilePath(),
	//	0.2f	//環境光の強さ
	//);

	//━━━━━━━━━━━━━━━━━━━━━━
	// ディレクションライト無効化
	// 強い影や太陽光を消す
	//━━━━━━━━━━━━━━━━━━━━━━
	g_renderingEngine->SetDirectionLight(
		0,			 //ライト番号
		g_vec3Zero,  //方向
		g_vec3Zero   //色(0で無効)
	);
}

//ゲームを管理するクラス
bool Game::Start()
{
	//空生成
	InitSky();

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
	///━━━━━━━━━━━
	///作成物(オブジェクト)は
	///ボックスのみで構成する
	///━━━━━━━━━━━

	//ステージオブジェクト作成
	background_Ins = NewGO<BackGround>(0, "background");
	//カメラオブジェクト作成
	camera_Ins = NewGO<GameCamera>(0, "gamecamera");
	//プレイヤーオブジェクト作成
	player_Ins = NewGO<Player>(0, "player");

	//ユニット管理クラス作成
	unitManager_Ins = NewGO<UnitManager>(0, "unitManager");
	//ユニット作成
	for (int i = 0; i < 3; i++) {
		unitManager_Ins->SpawnUnit(Type::Basic, 1000.0f, 0.0f, 1000.0f);
		unitManager_Ins->SpawnUnit(Type::Charger, 1000.0f, 0.0f, -1000.0f);
		unitManager_Ins->SpawnUnit(Type::Repair, -1000.0f, 0.0f, 1000.0f);
	}

	//弾管理オブジェクト作成
	NewGO<BulletManager>(0, "bulletManager");
	//バッテリー管理オブジェクト作成
	NewGO<BatteryManager>(0, "batteryManager");
	//発電機オブジェクト作成
	generator_Ins = NewGO< Generator>(0, "generator");
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
	DeleteGO(generator_Ins);
	DeleteGO(m_skyCube);

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