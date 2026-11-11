#pragma once
class Player;

//発電機状態
enum class GeneratorState
{
	Running,   //稼働中
	Stopped,   //停止中
	Restarting,//再起動中
};
class Generator : public IGameObject
{
public:
	Generator();
	~Generator();

	void Update();
	void Render(RenderContext& rc);

	//状態取得
	GeneratorState GetState()const
	{
		return state;
	}
	//停止判定
	bool IsStopped()const
	{
		return state == GeneratorState::Stopped;
	}
	//修理判定
	bool IsNeedRepair()const
	{
		return needRepair;
	}
	//座標取得
	Vector3 GetPosition()const
	{
		return position;
	}
	//停止ゲージ取得
	float GetStopGauge()const
	{
		return stopGauge;
	}
	//修理ユニットから呼ぶ
	void RestartGenerator();

	//停止率取得(0～1)
	float GetStopRate()const
	{
		return stopGauge / stopGaugeMax;
	}
	//プレイヤー範囲内判定取得
	bool IsPlayerInsideRange()const
	{
		return isPlayerInRange;
	}
private:
	//プレイヤー範囲取得
	bool IsPlayerInRange();

	//モデル
	ModelRender generatorONmodel;
	ModelRender generatorOFFmodel;

	PhysicsStaticObject physicsStaticObject;

	//座標
	Vector3 position = { 0.0f,0.0f,0.0f };

	//状態
	GeneratorState state = GeneratorState::Running;

	//停止ゲージ
	float stopGauge = 0.0f;
	float stopGaugeMax = 100.0f;

	//停止時間
	float stopTime = 10.0f;
	float stopTimeMax = 30.0f;

	//停止タイマー
	float stopTimer = 0.0f;

	//プレイヤー操作範囲
	float interactRange = 300.0f;

	//修理要求
	bool needRepair = false;

	//プレイヤー範囲内判定
	bool isPlayerInRange = false;

	Player* player = nullptr;

};

