#include "stdafx.h"
#include "Bullet.h"
#include "Player.h"
#include "UnitManager.h"

//弾を管理するクラス
Bullet::Bullet() 
{
	//弾読み込み
	m_modelRender.Init("Assets/modelData/Unit/Unit_Enemy.tkm");
}
Bullet::~Bullet() 
{

}
//発射
void Bullet::Fire(
	const Vector3& pos,
	const Vector3& dir,
	Player::BulletType type
) 
{
	m_position = pos;        //初期座標
	//発射方向
	m_direction = dir;       
	m_direction.Normalize();
	m_bulletType = type;     //弾種類
	m_speed = 1500.0f;       //移動速度
	m_lifeTime = 3.0f;       //生存時間
	m_isActive = true;       //使用開始
	//座標設定
	m_modelRender.SetPosition(m_position);
}
//更新管理
void Bullet::Update() 
{
	//無効の場合
	if (!m_isActive) { return; }
	//移動
	m_position +=
		m_direction *
		m_speed *
		g_gameTime->GetFrameDeltaTime();
	//生存時間(寿命減少)
	m_lifeTime -=
		g_gameTime->GetFrameDeltaTime();
	//寿命切れ
	if (m_lifeTime <= 0.0f)
	{
		m_modelRender.RemoveFromRaytracingWorld();
		m_isActive = false;
		return;
	}

	auto unitManager = FindGO<UnitManager>("unitManager");

	if (unitManager)
	{
		for (auto unit : unitManager->GetUnits())
		{
			Vector3 diff =
				unit->GetPosition() - m_position;
			float limit =
				unit->GetRadius() + m_radius;
			if (diff.LengthSq() < limit * limit)
			{
				unit->TakeDamage(10);
				m_modelRender.RemoveFromRaytracingWorld();
				m_isActive = false;
				return;
			}
		}
	}
	//座標更新
	m_modelRender.SetPosition(m_position);
	//モデル更新
	m_modelRender.Update();
}
//描画処理
void Bullet::Render(RenderContext& rc)
{
	//無効の場合
	if (!m_isActive) { return; }
	m_modelRender.Draw(rc);
}