#include "stdafx.h"
#include "Unit.h"
#include "Player.h"
#include "Game.h"
#include "UI.h"
#include "UnitManager.h"

//ユニットを管理するクラス
//初期設定
void Unit::SetUp(Type type, float x, float y, float z) {
	this->type = type;
	position = { x,y,z };
	///━━━━━━━━━━━
	///作成物(オブジェクト)は
	///ボックスのみで構成する
	///━━━━━━━━━━━
	switch (type)
	{
	//通常ユニット
	case Type::Basic:
		//基本ユニット読み込み
		modelRender.Init("Assets/modelData/Unit/Unit_Enemy.tkm");
		break;
	case Type::Charger:
		//充電ユニット読み込み
		modelRender.Init("Assets/modelData/Unit/Unit_Enemy.tkm");
		break;
	//特殊ユニット
	case Type::Repair:
		//修理ユニット読み込み
		modelRender.Init("Assets/modelData/Unit/Unit_Enemy.tkm");
		break;
	//ボスユニット
	case Type::Boss:
		//ボスユニット読み込み
		modelRender.Init("Assets/modelData/Unit/Unit_Enemy_Boss.tkm");
		break;
	}

	modelRender.SetPosition(position);//座標設定
	modelRender.Update();             //更新処理
	//当たり判定
	physicsStaticObject.CreateFromModel(
		modelRender.GetModel(),
		modelRender.GetModel().GetWorldMatrix()
	);
}
Unit::Unit() {
	player = FindGO<Player>("player");
	ui = FindGO<UI>("ui");
}
Unit::~Unit() {

}
//更新処理
void Unit::Update() {

	if (ui == nullptr)
	{
		ui = FindGO<UI>("ui");
	}

	Direction(); //方向処理
	Move();      //移動処理
	PushUnit();  //ユニット押し出し処理
	PushPlayer();//プレイヤー押し出し処理

	if (GetAsyncKeyState('1') & 0x0001)
	{
		TakeDamage(10);
	}

	//回転更新
	modelRender.SetRotation(rotation);
	//位置更新
	modelRender.SetPosition(position);
	//当たり判定の追従
	physicsStaticObject.Release();
	physicsStaticObject.CreateFromModel(
		modelRender.GetModel(),
		modelRender.GetModel().GetWorldMatrix()
	);
	//モデル更新
	modelRender.Update();
}
//方向処理
void Unit::Direction() {
	//方向計算
	direction = player->GetPosition() - position;

	distance = direction.Length();
	direction.Normalize();

	//方向変更
	rotation.SetRotationYFromDirectionXZ(direction);
}
//移動処理
void Unit::Move() {

	//目標へ向かう
	//ここの>を<に変えて数字の部分を
	//索敵範囲のやつに変えたら出来る
	if (distance >50.0f) {
		position += direction * 4.0f;
	}
	//重力
	if (characterController.IsOnGround()) {
		position.y = 0.0f;
	}
	else {
		position.y -= 2.5f;
	}
}
//ユニット押し出し処理
void Unit::PushUnit() {
	//全ユニットをチェック
	auto unitManager = FindGO<UnitManager>("unitManager");
	for (auto other : unitManager->GetUnits()) {
		if (other == this) { continue; }//自分自身は除外
		//相手との距離ベクトル
		Vector3 diff = position - other->position;
		//当たり判定半径の合計
		float limit = radius + other->radius;
		//距離の二乗
		float distSq = diff.LengthSq();
		//半径以内なら接触
		if (distSq < limit * limit) {
			//実際の距離
			float dist = sqrtf(distSq);
			//ゼロ除算防止
			if (dist > 0.01f) {
				//方向ベクトルを正規化
				diff.Normalize();
				//めり込んだ量
				float push = (limit - dist) * 0.5f;
				//お互い半分ずつ押し出す
				position += diff * push;
				other->position -= diff * push;
			}
		}
	}
}
//プレイヤー押し出し処理
void Unit::PushPlayer() {
	//プレイヤーが存在しない場合は終了
	if (player == nullptr) { return; }
	//プレイヤーとの距離ベクトル
	Vector3 diff = position - player->GetPosition();
	//接触判定距離
	float limit = radius + playerRadius;
	//距離の二乗
	float distSq = diff.LengthSq();
	//接触しているか判定
	if (distSq < limit * limit) {
		//実際の距離
		float dist = sqrtf(distSq);
		//ゼロ除算防止
		if (dist > 0.01f) {
			//方向を正規化
			diff.Normalize();
			//めり込み量
			float push = limit - dist;
			//ユニットのみ押し出す
			position += diff * push;
			// お互い半分ずつ押し出す(強くする場合0.5fを0.8fへ・弱くする場合は0.5fを0.2fへ)
			//position += diff * (push * 0.5f);//プレイヤー押し出し量
			//player->position -= diff * (push * 0.5f);//ユニット押し出し量
		}
	}
}
//ダメージ処理
void Unit::TakeDamage(int damage)
{
	hp -= damage;
	if (hp <= 0)
	{
		hp = 0;
		auto manager =
			FindGO<UnitManager>("unitManager");
		manager->RemoveUnit(this);
		return;
	}
	Vector3 popupPos = position;

	//頭上へ表示
	popupPos.y += 100.0f;

	ui->CreateDamagePopup(
		damage,
		popupPos
	);
}
//描画処理
void Unit::Render(RenderContext& rc)
{
	modelRender.Draw(rc); //ユニット
}