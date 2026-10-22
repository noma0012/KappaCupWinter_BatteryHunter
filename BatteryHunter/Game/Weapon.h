#pragma once
#include "AttackData.h"
/// <summary>
/// 武器クラス
/// 
/// プレイヤーやユニットが所持する武器
/// 
/// ・攻撃情報
/// ・発射間隔
/// ・攻撃タイマー
/// を管理する
/// </summary>
class Weapon
{
public:
	Weapon()
	{
	}
	//更新
	void Update(float deltaTime) 
	{
		if (attackTimer > 0.0f)
		{
			attackTimer -= deltaTime;
		}
	}
	//攻撃可能判定
	bool CanAttack()const
	{
		return attackTimer <= 0.0f;
	}
	//攻撃開始
	void StartAttack()
	{
		attackTimer = attackData.coolTime;
	}
public:
	//攻撃情報
	AttackData attackData;
	//攻撃タイマー
	float attackTimer = 0.0f;
};

