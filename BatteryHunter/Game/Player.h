#pragma once

//プレイヤークラス。
class Player : public IGameObject
{
public:
	//弾種類
	enum class BulletType {
		Normal, //通常弾
		EMP,    //EMP弾
		Drain   //ドレイン弾
	};
	//アニメーション
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_run,
		enAnimationClip_Jump,
		enAnimationClip_Num,
	};
	Player();
	~Player();
	//メイン処理
	void Update();
	void Render(RenderContext& rc);//描画処理
	//更新処理
	void Move();         //移動処理
	void Rotation();     //回転処理
	void ManageState();  //ステート管理
	void PlayAnimation();//アニメーションの再生
	//バッテリー
	//回復
	void AddBattery(int value);
	//消費
	void ConsumeBattery(int value);
	//空判定
	bool IsBatteryEnpty() const;
	//ダメージ
	void Damage(int value);
	//コアバッテリー取得
	//効果　最大容量100→200
	void GetCoreBattery();
	//コアバッテリー所持判定
	bool HasCoreBattery() const {
		return m_hasCoreBattery;
	}
	//脱出判定設定
	void SetEscape(bool flag) {
		m_isEscape = flag;
	}
	//脱出状態取得
	bool IsEscape() const {
		return m_isEscape;
	}
	//弾丸切り替え
	void NextBullet();
	//現在の弾取得
	BulletType GetBulletType()const {
		return m_bulletType;
	}
	//失敗・死亡判定
	bool IsDead() const {
		return m_isDead;
	}
	//現在バッテリー取得
	int GetBattery()const {
		return m_playerBattery;
	}
	//最大バッテリー取得
	int GetBatteryMax() const {
		return m_playerBatteryMax;
	}
	//プレイヤー座標取得
	Vector3 GetPosition() const { return position; }
private:
	//モデル
	ModelRender modelRender;

	Vector3 position;			//座標
	Vector3 moveSpeed;          //移動速度
	Quaternion rotation;        //回転


	AnimationClip animationClips[enAnimationClip_Num];  //アニメーションクリップ。
	CharacterController characterController;            //キャラクターコントローラー。
		                            
	int playerState = 0;                                //プレイヤーのステート(状態)を表す変数。
	//バッテリー
	int m_playerBattery = 100;     //バッテリー残量
	int m_playerBatteryMax = 100;  //バッテリー容量
	//条件
	bool m_hasCoreBattery = false; //コア所持
	bool m_isEscape = false;       //脱出済み
	bool m_isDead = false;         //失敗判定・死亡
	bool m_isTimeOver = false;     //時間切れ　コア取得後制限時間を超えた場合に使用
	//現在装備中の弾
	BulletType m_bulletType = BulletType::Normal;
	//射撃
	void Shoot();
	//発射間隔
	float m_shotInterval = 0.15f;
	//発射タイマー
	float m_shotTimer = 0.0f;
};
