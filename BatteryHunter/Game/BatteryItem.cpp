#include "stdafx.h"
#include "BatteryItem.h"
#include "Player.h"

//バッテリーを管理するクラス
BatteryItem::BatteryItem() 
{
	//modelRender.Init("Assets/modelData/Battery/NormalBattery.tkm");
	modelRender.Init("Assets/modelData/Unit/Unit_Enemy.tkm");

	player = FindGO<Player>("player");
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
		//回復
		player->AddBattery(10);
		DeleteGO(this);
		return;
	}

	modelRender.Update();
}
void BatteryItem::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}