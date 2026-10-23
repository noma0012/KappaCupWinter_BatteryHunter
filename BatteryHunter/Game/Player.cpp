#include "stdafx.h"
#include "Player.h"
#include "BulletManager.h"

Player::Player()
{
	//アニメーションクリップをロードする。
	animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	animationClips[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
	animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	animationClips[enAnimationClip_run ].Load("Assets/animData/run.tka");
	animationClips[enAnimationClip_run ].SetLoopFlag(true);
	animationClips[enAnimationClip_Jump].Load("Assets/animData/jump.tka");
	animationClips[enAnimationClip_Jump].SetLoopFlag(false);
	//ユニティちゃんのモデルを読み込む。
	modelRender.Init("Assets/modelData/unityChan.tkm", animationClips, enAnimationClip_Num, enModelUpAxisY);

	//キャラコンを初期化
	characterController.Init(25.0f, 75.0f, position);

	//初期化
	m_playerBattery = 100;
	m_playerBatteryMax = 100;

	m_hasCoreBattery = false;
	m_isEscape = false;
	m_isDead = false;

}

Player::~Player()
{

}

//更新処理。
void Player::Update()
{
	Move();               //移動処理
	Rotation();           //回転処理
	ManageState();	      //ステート管理
	PlayAnimation();      //アニメーションの再生
	Shoot();              //射撃処理
	//バッテリー切れ
	if (m_playerBattery <= 0) {
		m_playerBattery = 0;
		m_isDead = true;
	}
	if (m_shotTimer > 0.0f)
	{
		m_shotTimer -= 1.0f / 60.0f;
	}
	//弾切り替え
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		NextBullet();
	}
	modelRender.Update(); //絵描きさんの更新処理
}
//移動処理
void Player::Move()
{
	//xzの移動速度を0.0fにする。
	moveSpeed.x = 0.0f;
	moveSpeed.z = 0.0f;

	//左スティックの入力量を取得。
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//カメラの前方向と右方向のベクトルを持ってくる。
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y方向には移動させない。
	forward.y = 0.0f;
	right.y = 0.0f;

	//左スティックの入力量と120.0fを乗算。
	right *= stickL.x * 300.0f;
	forward *= stickL.y * 300.0f;

	//移動速度にスティックの入力量を加算する。
	moveSpeed += right + forward;

	//スティック入力と地面についていたら
	if ((fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f) && characterController.IsOnGround()) {
		//Bボタンが押されたらダッシュが発動する
		if (g_pad[0]->IsPress(enButtonB)) { moveSpeed.x *= 2.0f; moveSpeed.z *= 2.0f; }
	}

	//地面に付いていたら。
	if (characterController.IsOnGround())
	{
		//重力を無くす。
		moveSpeed.y = 0.0f;
		//Aボタンが押されたら。
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//ジャンプさせる。
			moveSpeed.y = 240.0f;
		}
	}
	//地面に付いていなかったら。
	else
	{
		//重力を発生させる。
		moveSpeed.y -= 5.0f;
	}

	//キャラクターコントローラーを使って座標を移動させる。
	position = characterController.Execute(moveSpeed, 1.0f / 60.0f);

	//絵描きさんに座標を教える。
	modelRender.SetPosition(position);
}
//回転処理
void Player::Rotation()
{
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	{
		//キャラクターの方向を変える。
		rotation.SetRotationYFromDirectionXZ(moveSpeed);
		//絵描きさんに回転を教える。
		modelRender.SetRotation(rotation);
	}
}
//ステート管理。
void Player::ManageState()
{
	//地面に付いていなかったら。
	if (characterController.IsOnGround() == false)
	{
		//ステートを1(ジャンプ中)にする。
		playerState = 1;
		//ここでManageStateの処理を終わらせる。
		return;
	}

	//地面に付いていたら。
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	{
		//ステートを3(走り)にする
		if (g_pad[0]->IsPress(enButtonB)) {
			playerState = 3;
		}
		//ステートを2(歩き)にする。
		else {
			playerState = 2;
		}
	}
	//xとzの移動速度が無かったら(スティックの入力が無かったら)。
	else
	{
		//ステートを0(待機)にする。
		playerState = 0;
	}
}
//アニメーション再生
void Player::PlayAnimation()
{
	switch (playerState) {
	case 0:modelRender.PlayAnimation(enAnimationClip_Idle);break; //0(待機)
	case 1:modelRender.PlayAnimation(enAnimationClip_Jump);break; //1(ジャンプ中)
	case 2:modelRender.PlayAnimation(enAnimationClip_Walk);break; //2(歩き)
	case 3:modelRender.PlayAnimation(enAnimationClip_run); break;}//3(走り)
}
//バッテリー回復
void Player::AddBattery(int value) {
	m_playerBattery += value;
	//回復量がMaxより大きい場合
	if (m_playerBattery > m_playerBatteryMax) {
		m_playerBattery = m_playerBatteryMax;
	}
}
//バッテリー消費
void Player::ConsumeBattery(int value) {
	m_playerBattery -= value;
	//バッテリーが0以下になったら0に調整する
	if (m_playerBattery < 0) {
		m_playerBattery = 0;
	}
}
//バッテリ切れ判定
bool Player::IsBatteryEmpty()const 
{
	return m_playerBattery <= 0;
}
//ダメージ
void Player::Damage(int value) 
{
	ConsumeBattery(value);
}
//コアバッテリー取得
void Player::GetCoreBattery() 
{
	//取得してない場合終了
	if (m_hasCoreBattery) { return; }
	m_hasCoreBattery = true;
	//最大容量増加
	m_playerBatteryMax = 200;
	//全回復
	m_playerBattery = m_playerBatteryMax;
}
//弾切り替え
void Player::NextBullet() 
{
	switch (m_bulletType)
	{
	case BulletType::Normal:
		m_bulletType = BulletType::EMP;
		break;
	case BulletType::EMP:
		m_bulletType = BulletType::Drain;
		break;
	case BulletType::Drain:
		m_bulletType = BulletType::Normal;
		break;
	}
}
//射撃
void Player::Shoot()
{
	if (!g_pad[0]->IsPress(enButtonB)) { return; }
	if (m_shotTimer > 0.0f) { return; }
	BulletManager* bulletManager =
		FindGO<BulletManager>("bulletManager");
	if (bulletManager == nullptr) { return; }
	bulletManager->Fire(
		position,
		GetForward(),
		m_bulletType
	);
	m_shotTimer = m_shotInterval;
	switch (m_bulletType)
	{
	case Player::BulletType::Normal:
		ConsumeBattery(0.1);
		break;
	case Player::BulletType::EMP:
		ConsumeBattery(0.5);
		break;
	case Player::BulletType::Drain:
		ConsumeBattery(0.3);
		break;
	}
}
//前方向取得
Vector3 Player::GetForward() const
{
	Vector3 forward = { 0.0f,0.0f,1.0f };
	rotation.Apply(forward);
	return forward;
}
//バッテリー回復処理
void Player::AddBattery(float value)
{
	m_playerBattery += value;
	if (m_playerBattery > m_playerBatteryMax)
	{
		m_playerBattery = m_playerBatteryMax;
	}
}
//描画処理。
void Player::Render(RenderContext& rc)
{
	//ユニティちゃんを描画する。
	modelRender.Draw(rc);
}