#pragma once

//プレイヤークラス。
class Player : public IGameObject
{
public:
	Player();
	~Player();
	//更新処理
	void Update();
	//その他更新処理
	void Move();         //移動処理
	void Rotation();     //回転処理
	void ManageState();  //ステート管理
	void PlayAnimation();//アニメーションの再生
	void Render(RenderContext& rc);//描画処理
	Vector3 GetPosition() const { return position; }//プレイヤー座標取得
	//メンバ変数。
	ModelRender modelRender;	//モデルレンダ―
	Vector3 position;			//座標
	enum EnAnimationClip {		//アニメーション
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_run,
		enAnimationClip_Jump,
		enAnimationClip_Num,
	};
	AnimationClip animationClips[enAnimationClip_Num];  //アニメーションクリップ。
	CharacterController characterController;            //キャラクターコントローラー。
	Vector3 moveSpeed;		                            //移動速度
	Quaternion rotation;                                //クォータニオン
	int playerState = 0;                                //プレイヤーのステート(状態)を表す変数。
};
