#pragma once

//攻撃種類
enum class AttackType
{
	None,     //なし
	Melee,    //素手・近接
	Bullet,   //通常弾
	EMP,      //EMP弾
	Drain,    //ドレイン弾
	Laser,    //レーザー
	Explosion //爆発
};
///━━━━━━━━━━━━━━━━━━
/// 攻撃情報
/// 
/// ダメージ処理に必要な情報をまとめる
///━━━━━━━━━━━━━━━━━━
struct AttackData
{
public:
	//攻撃種類
	AttackType type = AttackType::None;
	//ダメージ量
	int damage = 0;
	//攻撃距離(近接攻撃用)
	float range = 0.0f;
	//弾速度(弾・レーザー用)
	float speed = 0.0f;
	//発射間隔
	float coolTime = 0.0f;
	//バッテリー消費量
	int batteryCost = 0;
	//EMP停止時間
	float stunTime = 0.0f;
	//ドレイン回復量
	int drainValue = 0;
};

