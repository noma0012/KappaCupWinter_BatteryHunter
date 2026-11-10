#include "stdafx.h"
#include "BatteryItem.h"
#include "Player.h"
#include "UI.h"

//バッテリーを管理するクラス
BatteryItem::BatteryItem() 
{

	modelRender.SetRaytracingWorld(false);

	//通常バッテリー読み込み
	modelRender.Init("Assets/modelData/Battery/normal_battery.tkm");
	//コアバッテリー読み込み
	//modelRender.Init("Assets/modelData/Battery/core_battery.tkm");

	player = FindGO<Player>("player");
	ui = FindGO<UI>("ui");
}

BatteryItem::~BatteryItem()
{

}
void BatteryItem::SetPosition(const Vector3& pos)
{
	position = pos;
	modelRender.SetPosition(position);
}
void BatteryItem::SetRotation(const Quaternion& rot)
{
	rotation = rot;
	modelRender.SetRotation(rotation);
}
void BatteryItem::Update()
{
	if (!m_isActive) { return; }
	if (player == nullptr) 
	{
		player = FindGO<Player>("player");
		return;
	}
	if (ui == nullptr)
	{
		ui = FindGO<UI>("ui");
		return;
	}
	//プレイヤーとの距離
	Vector3 diff = player->GetPosition() - position;
	float distanceSq = diff.LengthSq();
	float limit = radius + 30.0f;
	//取得判定
	if (distanceSq < limit * limit)
	{
		int Heal = 10;
		//回復
		player->AddBattery(Heal);
		ui->CreateHealPopup(
			Heal,
			player->GetPosition()
		);
		
		//非表示化
		SetActive(false);
		return;
	}

	modelRender.Update();
}
void BatteryItem::SetActive(bool flag)
{
	m_isActive = flag;
	if (!flag)
	{
		position = Vector3(0.0f, -10000.0f, 0.0f);
		modelRender.SetPosition(position);
		modelRender.Update();
	}
}
void BatteryItem::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}