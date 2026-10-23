#include "stdafx.h"
#include "Unit.h"
#include "Player.h"
#include "Game.h"
#include "UI.h"
#include "UnitManager.h"
#include "BatteryItem.h"

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

	if (ui == nullptr){ ui = FindGO<UI>("ui"); }

	//重力
	if (characterController.IsOnGround()) {
		position.y = 0.0f;
	}
	else {
		position.y -= 2.5f;
	}

	switch (status)
	{
	case Status::Normal:
		//元に戻す
		modelRender.SetScale(Vector3(1.0f, 1.0f, 1.0f));
		Direction(); //方向処理
		Move();      //移動処理
		break;
	case Status::SelfRepair:
		//少し大きくする
		modelRender.SetScale(Vector3(1.2f, 1.2f, 1.2f));
		
		SelfRepair();
		break;
	case Status::Stop:
		//機能停止
		//タイマー加算
		stopTimer += g_gameTime->GetFrameDeltaTime();

		if (stopTimer >= 60.0f)
		{
			//ドロップ
			BatteryItem* item = NewGO<BatteryItem>(0, "battery");
			item->SetPosition(position);
			item->SetRotation(rotation);
			//消滅
			auto manager = FindGO<UnitManager>("unitManager");
			manager->RemoveUnit(this);
			return;
		}

		break;
	case Status::Charging:
		break;
	case Status::Repairing:
		break;
	default:
		break;
	}

	PushUnit();  //ユニット押し出し処理
	PushPlayer();//プレイヤー押し出し処理

	//仮(不必要・テスト)
	if (GetAsyncKeyState('1') & 0x0001)
	{
		TakeDamage(10);
	}
	if (GetAsyncKeyState('2') & 0x0001)
	{
		status = Status::SelfRepair;
	}
	if (GetAsyncKeyState('3') & 0x0001)
	{
		battery = 0.0f;
	}
	
	//バッテリー切れ
	if (battery <= 0.0f)
	{
		battery = 0.0f;
		status = Status::Stop;
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
	//移動(電力消費)
	battery -= 0.4f * g_gameTime->GetFrameDeltaTime();
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
		//auto manager =
		//	FindGO<UnitManager>("unitManager");
		//manager->RemoveUnit(this);

		//自己修復状態へ
		status = Status::SelfRepair;
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
//自己修復
void Unit::SelfRepair()
{
	//一秒計測
	selfRepairTimer += g_gameTime->GetFrameDeltaTime();
	if (selfRepairTimer >= 1.0f)
	{
		selfRepairTimer = 0.0f;

		//HP回復
		hp += 5;

		//バッテリー消費
		battery -= 0.5f;
	}

	//バッテリー切れ
	if (battery <= 0.0f) {
		battery = 0.0f;
		status = Status::Stop;
		return;
	}
	//修復完了
	if (hp >= 100)
	{
		hp = 100;
		status = Status::Normal;
	}
}
//描画処理
void Unit::Render(RenderContext& rc)
{
	modelRender.Draw(rc); //ユニット
}