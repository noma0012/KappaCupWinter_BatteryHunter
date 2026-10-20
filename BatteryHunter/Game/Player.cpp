#include "stdafx.h"
#include "Player.h"

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
//描画処理。
void Player::Render(RenderContext& rc)
{
	//ユニティちゃんを描画する。
	modelRender.Draw(rc);
}