#include "stdafx.h"
#include "Generator.h"
#include "Player.h"

//発電機を管理するクラス
Generator::Generator()
{
	//発電機ONモデル読み込み
	generatorONmodel.Init("Assets/modelData/Battery/generator_on.tkm");
	//発電機OFFモデル読み込み
	generatorOFFmodel.Init("Assets/modelData/Battery/generator_off.tkm");

	generatorONmodel.SetPosition(position);
	generatorOFFmodel.SetPosition(position);

	generatorONmodel.Update();
	generatorOFFmodel.Update();

	//物理
	physicsStaticObject.CreateFromModel(generatorONmodel.GetModel(), generatorONmodel.GetModel().GetWorldMatrix());

	player = FindGO<Player>("player");
}
Generator::~Generator()
{

}
//更新処理
void Generator::Update()
{
	if (player == nullptr) { player = FindGO<Player>("player"); }

	//範囲判定更新
	IsPlayerInRange();

	switch (state)
	{
	case GeneratorState::Running:
	{
		//発電機停止処理
		if (IsPlayerInRange())
		{
			stopGauge += 5.0f * g_gameTime->GetFrameDeltaTime();
		}
		else
		{
			//リセットなし
			stopGauge -= 2.0f * g_gameTime->GetFrameDeltaTime();
			if (stopGauge < 0.0f)
			{
				stopGauge = 0.0f;
			}
		}
		if (stopGauge >= stopGaugeMax)
		{
			stopGauge = stopGaugeMax;

			state = GeneratorState::Stopped;

			stopTimer = 0.0f;

			needRepair = true;
		}
	}
	break;
	case GeneratorState::Stopped:
	{
		//停止中
		stopTimer += g_gameTime->GetFrameDeltaTime();

		//時間経過で自動復旧させたい場合はここ
	}
	break;
	case GeneratorState::Restarting:
		break;
	default:
		break;
	}
	generatorONmodel.Update();
	generatorOFFmodel.Update();
}
//プレイヤー範囲取得
bool Generator::IsPlayerInRange()
{
	if (player == nullptr)
	{ isPlayerInRange = false; return false; }

	Vector3 diff = player->GetPosition() - position;
	isPlayerInRange = 
		diff.LengthSq() <=
		interactRange * interactRange;
	return isPlayerInRange;
}
//再起動
void Generator::RestartGenerator()
{
	state = GeneratorState::Running;

	stopGauge = 0.0f;
	stopTimer = 0.0f;

	needRepair = false;

	//停止時間増加
	stopTime += 5.0f;

	if (stopTime > stopTimeMax)
	{
		stopTime = stopTimeMax;
	}
}
//描画処理
void Generator::Render(RenderContext& rc)
{
	switch (state)
	{
	case GeneratorState::Running:
		generatorONmodel.Draw(rc);
		break;
	case GeneratorState::Stopped:
		generatorOFFmodel.Draw(rc);
		break;
	case GeneratorState::Restarting:
		generatorOFFmodel.Draw(rc);
		break;
	default:
		break;
	}
}