#pragma once
#include "Weapon.h"
#include "UnitManager.h"

using namespace std;

class Player;
class UI;

class Unit : public IGameObject
{
public:
	Type type;     //種類
	Weapon weapon;

	Unit();
	~Unit();

	void SetUp(Type type, float x, float y, float z);//初期設定
	void TakeDamage(int damage);
	float GetRadius()const { return radius; }//方向取得
	Vector3 GetPosition()const { return position; }//座標取得
	Status GetStatus()const { return status; }//状態取得
	float GetBattery()const { return battery; }//バッテリー取得
private:
	void Update();                  //更新処理
	void Render(RenderContext& rc); //描画処理
	void Direction();               //方向処理
	void Move();                    //移動処理
	void PushUnit();      //ユニット押し出し処理
	void PushPlayer();    //プレイヤー押し出し処理
	void SelfRepair();    //自己修復
	void Patrol();        //巡回
	void Chase();         //プレイヤー追跡
	void Escape();        //発電機へ避難
	void MoveDirection( const Vector3& dir ); //移動共通処理
	void Attack();        //攻撃処理
	void UpdateGear();    //装備位置更新
	void UpdateBatteryPack(); //バックパック更新処理
	void SearchChargeTarget();//充電対象検索
	void Charge();//充電処理
	//Vector3
	Vector3 position;      //位置(ユニット)
	Vector3 direction;     //方向(Vector3)
	Vector3 patrolTarget;  //巡回地点
	//float
	float moveSpeed;            //移動速度
	float distance;             //方向(float)
	float radius = 30.0f;       //当たり判定の半径
	float playerRadius = 30.0f; //プレイヤーの当たり判定半径
	//その他
	ModelRender modelRender;    //ユニット
	ModelRender StopModel;      //機能停止ユニット
	ModelRender gearModel;      //メイン装備
	ModelRender subGearModel;   //サブ装備(修理ユニット用)
	ModelRender batteryPack[6]; //バックパック(充電ユニット用)

	ModelRender stopGearModel;    //機能停止時メイン装備(基本ユニット用)
	ModelRender stopSubGearModel; //機能停止時サブ装備(基本ユニット用)

	Quaternion rotation;        //ユニット回転
	CharacterController characterController;

	int hp = 100;
	int batteryLevel = 5;//現在表示レベル
	float battery = 100.0f;//バッテリー残量
	float selfRepairTimer = 0.0f;//自己修復タイマー
	float stopTimer = 0.0f;//機能停止タイマー
	Status status = Status::Normal;//状態
	float patrolTimer = 0.0f;
	float attackTimer = 0.0f;//攻撃間隔タイマー

	//装備の有無(false なし true あり)
	bool hasGear = false;
	bool hasSubGear = false;

	Player* player = nullptr;
	UI* ui;
	Unit* target = nullptr;
	UnitManager* unitManager = nullptr;
	const UnitParam* param = nullptr;
};

