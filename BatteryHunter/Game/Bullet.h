#pragma once
#include "Player.h"

class Bullet : public IGameObject
{
public:
	Bullet();
	~Bullet();
	void Update();//更新処理
	void Render(RenderContext& rc);//描画処理
	///━━━━━━━━━━━━━━
	/// 発射
	///
	/// pos  : 発射位置
	/// dir  : 発射方向
	/// type : 弾の種類
	/// 
	/// この弾自身を初期化する関数
	///━━━━━━━━━━━━━━
	void Fire(
		const Vector3& pos,
		const Vector3& dir,
		Player::BulletType type
	);
	//有効判定
	bool IsActive() const 
	{
		return m_isActive;
	}
	//無効化
	void Deactivate() 
	{
		m_isActive = false;
	}
	//座標取得
	const Vector3& GetPosition()const 
	{
		return m_position;
	}
private:
	ModelRender m_modelRender; //モデル
	Vector3 m_position;        //座標
	Vector3 m_direction;       //移動方向
	float m_speed = 0.0f;      //移動速度
	float m_lifeTime = 0.0f;   //生存時間(消えるまでの時間)
	bool m_isActive = false;   //有効判定
	//弾種類
	Player::BulletType m_bulletType;
};

