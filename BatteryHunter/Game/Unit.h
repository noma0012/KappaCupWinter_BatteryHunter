#pragma once
#include "Weapon.h"

using namespace std;

class Player;
class UI;

enum class Type { //種類
	//通常ユニット
	Basic,     //基本ユニット
	Charger,   //充電ユニット
	//特殊ユニット
	Repair,    //修理ユニット
	//ボスユニット
	Boss,      //ボス
	Max
};
//状態
enum class Status { 
	Normal,     //通常
	SelfRepair, //自己修復中
	Stop,       //機能停止
	Charging,   //充電中
	Repairing   //修理中
};

class Unit : public IGameObject
{
public:
	Type type;//種類

	Unit();
	~Unit();

	void SetUp(Type type, float x, float y, float z);//初期設定
	void TakeDamage(int damage);
	float GetRadius()const { return radius; }//方向取得
	Vector3 GetPosition()const { return position; }//座標取得
	Status GetStatus()const { return status; }//状態取得
private:
	void Update();                  //更新処理
	void Render(RenderContext& rc); //描画処理
	void Direction();               //方向処理
	void Move();                    //移動処理
	void PushUnit();  //ユニット押し出し処理
	void PushPlayer();//プレイヤー押し出し処理
	void SelfRepair();//自己修復
	//Vector3
	Vector3 position;      //位置(ユニット)
	Vector3 direction;     //方向(Vector3)
	//float
	float moveSpeed;            //移動速度
	float distance;             //方向(float)
	float radius = 30.0f;       //当たり判定の半径
	float playerRadius = 30.0f; //プレイヤーの当たり判定半径
	//その他
	ModelRender modelRender;    //ユニット
	ModelRender StopModel;      //機能停止ユニット
	Quaternion rotation;        //ユニット回転
	PhysicsStaticObject physicsStaticObject;
	CharacterController characterController;

	Weapon weapon;
	int hp = 100;
	float battery = 100.0f;//バッテリー残量
	float selfRepairTimer = 0.0f;//自己修復タイマー
	float stopTimer = 0.0f;//機能停止タイマー
	Status status = Status::Normal;//状態
	

	Player* player = nullptr;
	UI* ui;
	Unit* target = nullptr;
};

