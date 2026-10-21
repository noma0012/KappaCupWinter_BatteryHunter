#include "stdafx.h"
#include "BulletManager.h"

BulletManager::BulletManager()
{

}
BulletManager::~BulletManager()
{

}
//更新処理
void BulletManager::Update()
{
	//発射タイマー更新
	if (m_shotTimer > 0.0f)
	{
		m_shotTimer -=
			g_gameTime->GetFrameDeltaTime();
	}
	//弾更新
	for (int i = 0; i < MAX_BULLET; i++) 
	{
		if (m_bullets[i].IsActive())
		{
			m_bullets[i].Update();
		}
	}
}
//描画処理
void BulletManager::Render(RenderContext& rc)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (m_bullets[i].IsActive())
		{
			m_bullets[i].Render(rc);
		}
	}
}
//発射
bool BulletManager::Fire(
	const Vector3& pos,
	const Vector3& dir,
	Player::BulletType type
)
{
	//発射間隔中(クールタイム)
	if(m_shotTimer > 0.0f)
	{
		return false;
	}
	//弾種ごとの間隔
	switch (type)
	{
	case Player::BulletType::Normal:
		m_shotTimer = 0.15;
		break;
	case Player::BulletType::EMP:
		m_shotTimer = 1.0f;
		break;
	case Player::BulletType::Drain:
		m_shotTimer = 0.5f;
		break;
	}
	//空きを探す
	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (!m_bullets[i].IsActive())
		{
			m_bullets[i].Fire(
				pos,
				dir,
				type
			);
			return true;
		}
	}
	return false;
}