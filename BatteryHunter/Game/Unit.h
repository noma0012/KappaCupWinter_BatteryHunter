#pragma once
#include "UnitManager.h"

using namespace std;

class Player;
class UI;
class Generator;

class Unit : public IGameObject
{
public:
	///--------------------
	///　 コンストラクト
	///--------------------
	Unit();
	~Unit();

	///--------------------
	///　　　 初期化
	///--------------------
	
	/// <summary>
	/// 初期設定
	/// </summary>
	/// <param name="type">ユニットの種類</param>
	/// <param name="x">X座標</param>
	/// <param name="y">Y座標</param>
	/// <param name="z">Z座標</param>
	void SetUp(
		Type type,
		float x,
		float y,
		float z
	);
	///--------------------
	///　　 ダメージ
	///--------------------
	
	/// <summary>
	/// ダメージ処理
	/// </summary>
	void TakeDamage(int damage);

	///--------------------
	///　　　 取得
	///--------------------

	/// <summary>
	/// 種類取得
	/// </summary>
	Type GetType() const
	{
		return type;
	}
	
	/// <summary>
	/// 座標取得
	/// </summary>
	Vector3 GetPosition() const
	{
		return position;
	}

	/// <summary>
	/// 半径取得
	/// </summary>
	float GetRadius() const
	{
		return radius;
	}

	/// <summary>
	/// 状態取得
	/// </summary>
	Status GetStatus() const
	{
		return status;
	}

	/// <summary>
	/// バッテリー取得
	/// </summary>
	float GetBattery() const
	{
		return battery;
	}
private:
#pragma region 初期化
	//モデル初期化
	void InitModel();

	//性能初期化
	void InitParameter();

	//参照取得
	void FindObjects();

#pragma endregion

#pragma region 更新

	//更新処理
	void Update();

	//描画処理
	void Render(RenderContext& rc);

	//通常状態更新
	void UpdateNormal();

	//停止状態更新
	void UpdateStop();

	//重力処理
	void UpdateGravity();

	//モデル更新
	void UpdateModel();

#pragma endregion

#pragma region 移動

	//方向計算
	void Direction();

	//移動処理
	void Move();

	//共通移動処理
	void MoveDirection(
		const Vector3& dir
	);

	//巡回
	void Patrol();

	//追跡
	void Chase();

	//退避
	void Escape();

	//プレイヤー押し出し
	void PushPlayer();

	//ユニット押し出し
	void PushUnit();
#pragma endregion

#pragma region 戦闘

	//攻撃処理
	void Attack();

#pragma endregion

#pragma region 状態

	//自己修復
	void SelfRepair();

#pragma endregion

#pragma region 充電

	//バックパック更新処理
	void UpdateBatteryPack();

	//充電対象検索
	void SearchChargeTarget();

	//充電処理
	void Charge();

	//発電機充電処理
	void ChargeGenerator();

#pragma endregion

private:
	///--------------------
	///　　　　種類
	///--------------------
	
	Type type;

	///--------------------
	///　　　　基本
	///--------------------
	
	Vector3 position;      //座標
	Vector3 direction;     //方向
	Vector3 patrolTarget;  //巡回地点

	Quaternion rotation;   //回転

	///--------------------
	///　　 当たり判定
	///--------------------
	
	float radius = 30.0f;       //ユニット半径
	float playerRadius = 30.0f; //プレイヤー半径

	///--------------------
	///　　 ステータス
	///--------------------
	
	//保有体力
	int hp = 100;
	//保有バッテリー
	float battery = 100.0f;
	//対象との距離
	float distance;
	//状態
	Status status = Status::Normal;
	//性能
	const UnitParam* param = nullptr;

	///--------------------
	///　　  タイマー
	///--------------------

	//巡回
	float patrolTimer = 0.0f;
	//攻撃
	float attackTimer = 0.0f;
	//自己修復
	float selfRepairTimer = 0.0f;
	//停止
	float stopTimer = 0.0f;
	//検索
	float searchTimer = 0.0f;

	///--------------------
	///　　   充電
	///--------------------

	//表示レベル
	int batteryLevel = 5;
	//充電対象
	Unit* target = nullptr;

	///--------------------
	///　　   モデル
	///--------------------

	//通常モデル
	ModelRender modelRender;
	//停止モデル
	ModelRender StopModel;
	//充電ユニット専用モデル
	ModelRender batteryPack[6];

	///--------------------
	///　　	　物理
	///--------------------

	CharacterController characterController;
	
	///--------------------
	///　　   参照
	///--------------------

	Player* player = nullptr;
	UI* ui;
	UnitManager* unitManager = nullptr;
	Generator* generator = nullptr;

};