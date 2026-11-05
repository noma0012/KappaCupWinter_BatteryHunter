#include "stdafx.h"
#include "Unit.h"
#include "Player.h"
#include "Game.h"
#include "UI.h"
#include "UnitManager.h"
#include "BatteryManager.h"

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
		//素手読み込み
		gearModel.Init("Assets/modelData/Unit/Fist.tkm");
		subGearModel.Init("Assets/modelData/Unit/Fist.tkm");
		//機能停止時の素手読み込み
		stopGearModel.Init("Assets/modelData/Unit/Stop_Fist.tkm");
		stopSubGearModel.Init("Assets/modelData/Unit/Stop_Fist.tkm");
		hasGear = true;
		hasSubGear = true;
		break;
	case Type::Charger:
		//充電ユニット読み込み
		modelRender.Init("Assets/modelData/Unit/Unit_Enemy.tkm");
		//充電バックパック(0～5)
		batteryPack[0].Init("Assets/modelData/Battery/charge_pack_0.tkm");
		batteryPack[1].Init("Assets/modelData/Battery/charge_pack_1.tkm");
		batteryPack[2].Init("Assets/modelData/Battery/charge_pack_2.tkm");
		batteryPack[3].Init("Assets/modelData/Battery/charge_pack_3.tkm");
		batteryPack[4].Init("Assets/modelData/Battery/charge_pack_4.tkm");
		batteryPack[5].Init("Assets/modelData/Battery/charge_pack_5.tkm");
		hasGear = true;
		break;
		//特殊ユニット
	case Type::Repair:
		//修理ユニット読み込み
		modelRender.Init("Assets/modelData/Unit/Unit_Enemy.tkm");
		//電動ドリル
		gearModel.Init("Assets/modelData/Battery/repair_tool_drill.tkm");
		//スパナ
		subGearModel.Init("Assets/modelData/Battery/repair_tool_spanner.tkm");
		hasGear = true;
		hasSubGear = true;
		break;
		//ボスユニット
	case Type::Boss:
		//ボスユニット読み込み
		modelRender.Init("Assets/modelData/Unit/Unit_Enemy_Boss.tkm");
		break;
	}
	StopModel.Init("Assets/modelData/Unit/Unit_Enemy_Stop.tkm");
	modelRender.SetPosition(position);//座標設定
	modelRender.Update();             //更新処理

	player = FindGO<Player>("player");
	ui = FindGO<UI>("ui");
	unitManager = FindGO<UnitManager>("unitManager");
	//性能取得
	if (unitManager)
	{
		param = &unitManager->GetParam(type);

		hp = param->hp;
		battery = param->battery;
	}
}
Unit::Unit()
{

}
Unit::~Unit()
{

}
//更新処理
void Unit::Update() {
	//再取得
	{
		if (player == nullptr) { player = FindGO<Player>("player"); }
		if (ui == nullptr) { ui = FindGO<UI>("ui"); }
		if (unitManager == nullptr) { unitManager = FindGO<UnitManager>("unitManager"); }
		if (param == nullptr && unitManager != nullptr) { param = &unitManager->GetParam(type); }
	}
	//重力
	if (characterController.IsOnGround()) { position.y = 0.0f; }
	else { position.y -= 2.5f; }

	UpdateBatteryPack(); //バックパック更新処理

	switch (status)
	{
	case Status::Normal:
		//通常
		Direction(); //方向処理
		//攻撃範囲内の場合
		if (distance <= param->attackRange)
		{
			Attack();//攻撃処理
		}
		else
		{
			Move(); //移動処理
		}
		
		break;
	case Status::SelfRepair:
		//自己修復
		SelfRepair();
		break;
	case Status::Stop:
		//機能停止
		//タイマー加算
		stopTimer += g_gameTime->GetFrameDeltaTime();

		if (stopTimer >= 60.0f)
		{
			//ドロップ
			auto batteryManager = FindGO<BatteryManager>("batteryManager");
			if (batteryManager)
			{
				batteryManager->SpawnBattery(position, rotation);
			}
			//消滅
			if (unitManager) {
				unitManager->RemoveUnit(this);
			}
			return;
		}

		break;
	case Status::Charging:
		//充電中
		break;
	case Status::Repairing:
		//修理中
		break;
	default:
		break;
	}
	
	PushUnit();  //ユニット押し出し処理
	PushPlayer();//プレイヤー押し出し処理
	UpdateGear();//装備位置更新処理
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

	//通常モデル
	modelRender.SetRotation(rotation);//回転更新
	modelRender.SetPosition(position);//位置更新
	//機能停止モデル
	StopModel.SetRotation(rotation);
	StopModel.SetPosition(position);
	//モデル更新
	modelRender.Update();
	gearModel.Update();
	subGearModel.Update();

	StopModel.Update();
	stopGearModel.Update();
	stopSubGearModel.Update();

}
//方向処理
void Unit::Direction() {
	if (player == nullptr) { return; }
	//方向計算
	direction = player->GetPosition() - position;

	distance = direction.Length();
	direction.Normalize();

	//方向変更
	//rotation.SetRotationYFromDirectionXZ(direction);
}
//移動処理
void Unit::Move() {

	if (param == nullptr) { return; }

	switch (type)
	{
	case Type::Basic:
		if (distance <= param->searchRange)
		{
			Chase();
		}
		else
		{
			Patrol();
		}
		break;
	case Type::Charger:
		if (battery <= 20.0f)
		{
			Escape();
		}
		else
		{
			Patrol();
		}
		break;
	case Type::Repair:
		Patrol();
		break;
	}
}
//ユニット押し出し処理
void Unit::PushUnit()
{
	//ユニット管理クラスが存在しない場合
	if (unitManager == nullptr) { return; }

	//近くのユニットのみ取得( 例：検索範囲 = 200.0f )
	auto nearUnits = unitManager->GetNearUnits(position, 200.0f);
	//近距離ユニットのみ処理
	for (auto other : nearUnits)
	{
		//自分自身は除外
		if (other == this) { continue; }
		//相手との距離ベクトル
		Vector3 diff = position - other->position;
		//接触距離
		float limit = radius + other->radius;
		//距離の二乗
		float distSq = diff.LengthSq();
		//接触判定
		if (distSq < limit * limit)
		{
			//ゼロ除算防止
			if (distSq > 0.0001f)
			{
				float dist = sqrtf(distSq);
				//方向を正規化
				diff /= dist;
				//押し出し量
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
//移動共通処理
void Unit::MoveDirection(const Vector3& dir)
{
	//停止
	if (dir.LengthSq() <= 0.0001f) { return; }
	//移動
	position += dir * param->moveSpeed;
	//向き変更
	rotation.SetRotationYFromDirectionXZ(dir);
	//電力消費
	battery -= param->moveBatteryCost * g_gameTime->GetFrameDeltaTime();
}
//プレイヤー追跡
void Unit::Chase()
{
	//攻撃範囲
	if (distance <= param->attackRange)
	{
		return;
	}
	//プレイヤーへ向く
	rotation.SetRotationYFromDirectionXZ(direction);

	MoveDirection(direction);
}
//発電機へ退避
void Unit::Escape()
{
	Vector3 generatorPos = { 0.0f,0.0f,0.0f };
	Vector3 dir = generatorPos - position;
	if (dir.LengthSq() <= 0.0001f) { return; }
	dir.Normalize();
	MoveDirection(dir);
}
//巡回
void Unit::Patrol()
{
	//タイマー加算
	patrolTimer += g_gameTime->GetFrameDeltaTime();
	//巡回地点未設定または到着した場合
	Vector3 dir = patrolTarget - position;
	if (patrolTimer >= 5.0f|| dir.LengthSq() <= 100.0f)
	{
		//タイマーリセット
		patrolTimer = 0.0f;
		//新しい巡回地点を決定
		patrolTarget.x = position.x + (rand() % 400 - 200);

		patrolTarget.y = 0.0f;

		patrolTarget.z = position.z + (rand() % 400 - 200);

		//方向再計算
		dir = patrolTarget - position;
	}
	//移動方向
	if (dir.LengthSq() <= 0.0001f) { return; }
	dir.Normalize();
	//共通移動処理
	MoveDirection(dir);
}
//攻撃処理
void Unit::Attack()
{
	//プレイヤーが存在しない
	if (player == nullptr) { return; }
	//プレイヤーへ向く
	rotation.SetRotationYFromDirectionXZ(direction);
	//攻撃タイマー加算
	attackTimer += g_gameTime->GetFrameDeltaTime();
	//攻撃間隔チェック
	if (attackTimer < param->attackInterval) { return; }
	//タイマーリセット
	attackTimer = 0.0f;
	//プレイヤーへダメージ
	player->Damage( param->attackPower );
	//攻撃時バッテリー消費
	battery -= param->attackBatteryCost;

}
//装備位置更新処理
void Unit::UpdateGear()
{
	//装備位置設定
	switch (type)
	{
	case Type::Basic:
	{
		//右拳
		Vector3 rightPos = position;
		Vector3 rightHand = { 40.0f,20.0f,30.0f };
		rotation.Apply(rightHand);
		rightPos += rightHand;

		//左拳
		Vector3 leftPos = position;
		Vector3 leftHand = { -40.0f,20.0f,30.0f };
		rotation.Apply(leftHand);
		leftPos += leftHand;

		if (status != Status::Stop)
		{
			//通常時の素手
			gearModel.SetPosition(rightPos);
			gearModel.SetRotation(rotation);
			subGearModel.SetPosition(leftPos);
			subGearModel.SetRotation(rotation);
		}
		else
		{
			//機能停止時の素手
			stopGearModel.SetPosition(rightPos);
			stopGearModel.SetRotation(rotation);
			stopSubGearModel.SetPosition(leftPos);
			stopSubGearModel.SetRotation(rotation);
		}
	}
	break;
	case Type::Charger:
	{
		//バックパック
		Vector3 gearPos = position;
		Vector3 back = { 0.0f,0.0f,-0.4f };
		rotation.Apply(back);
		gearPos += back;
		for (int i = 0; i < 6; i++)
		{
			batteryPack[i].SetPosition(gearPos);
			batteryPack[i].SetRotation(rotation);
			batteryPack[i].Update();
		}
	}
	break;
	case Type::Repair:
	{
		//ドリル(右手)
		Vector3 drillPos = position;
		Vector3 right = { 40.0f,20.0f,30.0f };
		rotation.Apply(right);
		drillPos += right;
		gearModel.SetPosition(drillPos);
		gearModel.SetRotation(rotation);
		//スパナ(左手)
		Vector3 wrenchPos = position;
		Vector3 left = { -40.0f,20.0f,30.0f };
		rotation.Apply(left);
		wrenchPos += left;
		subGearModel.SetPosition(wrenchPos);
		subGearModel.SetRotation(rotation);
	}
	break;
	case Type::Boss:
		break;
	}
}
//バックパック更新処理
void Unit::UpdateBatteryPack()
{
	//充電ユニット以外の場合
	if (type != Type::Charger) { return; }
	int level = 0;

	//残量判定
	     if (battery >= 80.0f) { level = 5; }
	else if (battery >= 60.0f) { level = 4; }
	else if (battery >= 40.0f) { level = 3; }
	else if (battery >= 20.0f) { level = 2; }
	else if (battery >   0.0f) { level = 1; }
	else                       { level = 0; }
	//変更なし
	if (batteryLevel == level) { return; }
	batteryLevel = level;
}
//描画処理
void Unit::Render(RenderContext& rc)
{
	if (status == Status::Stop){ StopModel.  Draw(rc); } //機能停止ユニット
	else                       { modelRender.Draw(rc); } //ユニット
	//充電ユニットのみ
	if (type == Type::Charger) { batteryPack[batteryLevel].Draw(rc); }
	else
	{
		if (type == Type::Basic&&status==Status::Stop)
		{
			//機能停止時の装備
			stopGearModel.Draw(rc);
			stopSubGearModel.Draw(rc);
		}
		else
		{
			if (hasGear) { gearModel.Draw(rc); } //装備
			if (hasSubGear) { subGearModel.Draw(rc); } //サブ装備
		}
	}
}