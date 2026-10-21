#pragma once
#include "Bullet.h"
//弾管理クラス
class BulletManager:public IGameObject
{
public:
	//最大弾数
	static const int MAX_BULLET = 300;
public:
	BulletManager();
	~BulletManager();
	void Update();
	void Render(RenderContext& rc);
	///━━━━━━━━━━━━━━━
	/// 発射
	/// 
	/// 空いている弾を探して
	/// 発射する関数
	///━━━━━━━━━━━━━━━
	void Fire(
		const Vector3& pos,
		const Vector3& dir,
		Player::BulletType type
	);
private:
	//弾配列
	Bullet m_bullets[MAX_BULLET];
	//発射間隔タイマー
	float m_shotTimer = 0.0f;
};

