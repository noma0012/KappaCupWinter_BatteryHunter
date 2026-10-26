#include "stdafx.h"
#include "BatteryItem.h"
#include "Player.h"
#include "UI.h"

//バッテリーを管理するクラス
BatteryItem::BatteryItem() 
{
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
	if (player == nullptr) 
	{
		player = FindGO<Player>("plaeyr");
		return;
	}
	//プレイヤーとの距離
	Vector3 diff = player->GetPosition() - position;
	float distanceSq = diff.LengthSq();
	float limit = radius + 30.0f;
	//取得判定
	if (distanceSq < limit + limit)
	{
		int Heal = 10;
		//回復
		player->AddBattery(Heal);
		ui->CreateHealPopup(
			Heal,
			player->GetPosition()
		);
		DeleteGO(this);
		return;
	}

	modelRender.Update();
}
void BatteryItem::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}