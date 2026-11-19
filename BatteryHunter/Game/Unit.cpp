#include "stdafx.h"
#include "Unit.h"

#include "Player.h"
#include "UI.h"
#include "Generator.h"
#include "BatteryManager.h"

#pragma region コンストラクタ

Unit::Unit()
{

}

Unit::~Unit()
{

}

#pragma endregion

#pragma region 初期化

//初期設定
void Unit::SetUp(
	Type type,
	float x,
	float y,
	float z
)
{
	//種類設定
	this->type = type;

	//初期座標設定
	position = { x,y,z };

	//関連オブジェクト取得
	FindObjects();

	//モデル初期化
	InitModel();

	//性能設定
	InitParameter();

	//モデルへ位置反映
	modelRender.SetPosition(position);
	modelRender.Update();

	StopModel.SetPosition(position);
	StopModel.Update();
}

//モデル初期化
void Unit::InitModel()
{
	switch (type)
	{
	case Type::Basic:
		//通常モデル
		modelRender.Init("Assets/modelData/Unit/Unit_Normal.tkm");
		//停止モデル
		StopModel.Init("Assets/modelData/Unit/Stop_Unit_Normal.tkm");
		break;
	case Type::Charger:
		//充電ユニット専用モデル(0～5)
		batteryPack[0].Init("Assets/modelData/Unit/Unit_charge_pack_0.tkm");
		batteryPack[1].Init("Assets/modelData/Unit/Unit_charge_pack_1.tkm");
		batteryPack[2].Init("Assets/modelData/Unit/Unit_charge_pack_2.tkm");
		batteryPack[3].Init("Assets/modelData/Unit/Unit_charge_pack_3.tkm");
		batteryPack[4].Init("Assets/modelData/Unit/Unit_charge_pack_4.tkm");
		batteryPack[5].Init("Assets/modelData/Unit/Unit_charge_pack_5.tkm");
		break;
	case Type::Repair:
		//修理ユニット
		modelRender.Init("Assets/modelData/Unit/Unit_repair.tkm");
		StopModel.Init("Assets/modelData/Unit/Stop_Unit_repair.tkm");
		break;
	case Type::Boss:
		//ボスユニット
		modelRender.Init("Assets/modelData/Unit/Unit_Boss.tkm");
		StopModel.Init("Assets/modelData/Unit/Stop_Unit_Boss.tkm");
		break;
	}
}
//性能初期化
void Unit::InitParameter()
{
	//管理クラスなし
	if (unitManager == nullptr) { return; }

	//性能取得
	param = &unitManager->GetParam(type);

	//初期HP
	hp = param->hp;

	//初期バッテリー
	battery = param->battery;
}
//参照取得
void Unit::FindObjects()
{
	//プレイヤー
	if (player == nullptr) { player = FindGO<Player>("player"); }
	//UI
	if (ui == nullptr) { ui = FindGO<UI>("ui"); }
	//ユニット管理
	if (unitManager == nullptr) { unitManager = FindGO<UnitManager>("unitManager"); }
	//発電機
	if (generator == nullptr) { generator = FindGO<Generator>("generator"); }
}

#pragma endregion

#pragma region 更新
//更新処理
void Unit::Update()
{
	//参照再取得
	FindObjects();

	//重力処理
	UpdateGravity();

	//充電ユニット専用
	if (type == Type::Charger)
	{
		UpdateBatteryPack();
	}

	//状態更新
	switch (status)
	{
	case Status::Normal:
		UpdateNormal();
		break;
	case Status::SelfRepair:
		SelfRepair();
		break;
	case Status::Stop:
		UpdateStop();
		break;
	case Status::Charging:
		//発電機で充電
		ChargeGenerator();
		break;
	case Status::Repairing:
		break;
	}

	//押し出し処理
	PushPlayer();
	PushUnit();

	//モデル更新
	UpdateModel();
}
//通常状態更新
void Unit::UpdateNormal()
{
	//方向計算
	Direction();

	//攻撃判定
	if (distance <= param->attackRange)
	{
		Attack();
	}
	else
	{
		Move();
	}
	//バッテリー切れ
	if (battery <= 0.0f)
	{
		battery = 0.0f;

		status = Status::Stop;
	}
}

void Unit::UpdateStop()
{

}

//重力処理
void Unit::UpdateGravity()
{
	//接地中
	if (characterController.IsOnGround())
	{
		position.y = 0.0f;
	}
	else
	{
		//落下
		position.y -= 5.0f;
	}
}
//モデル更新
void Unit::UpdateModel()
{
	//通常モデル
	modelRender.SetPosition(position);
	modelRender.SetRotation(rotation);

	//停止モデル
	StopModel.SetPosition(position);
	StopModel.SetRotation(rotation);

	//モデル更新
	modelRender.Update();
	StopModel.Update();

	//充電ユニット
	if (type == Type::Charger)
	{
		batteryPack[batteryLevel].SetPosition(position);
		batteryPack[batteryLevel].SetRotation(rotation);
		batteryPack[batteryLevel].Update();
	}
}

#pragma endregion

#pragma region 移動

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
		//自身の電力残量不足
		if (battery <= 20.0f)
		{
			Escape();
		}
		else
		{
			//対象検索
			{
				searchTimer += g_gameTime->GetFrameDeltaTime();

				if (searchTimer >= 0.5f)
				{

					SearchChargeTarget();
					searchTimer = 0.0f;
				}
			}

			//対象あり
			if (target != nullptr)
			{
				Charge();
			}
			else
			{
				Patrol();
			}
		}
		break;
	case Type::Repair:
		Patrol();
		break;
	}
}

//移動共通処理
void Unit::MoveDirection(const Vector3& dir)
{
	//停止
	if (dir.LengthSq() <= 0.0001f) { return; }
	//移動予定位置
	Vector3 nextPos = position + dir * param->moveSpeed / 60.0;

	//発電機台座めり込み防止
	{
		if (generator != nullptr)
		{
			Vector3 diff = nextPos - generator->GetPosition();
			const float generatorRadius = 110.0f + radius;
			float distSq = diff.LengthSq();
			if (distSq <= generatorRadius * generatorRadius)
			{
				//発電機台座範囲
				float dist = sqrtf(distSq);
				if (dist > 0.0001f)
				{
					diff /= dist;
					nextPos = generator->GetPosition() + diff * generatorRadius;
				}
			}
		}
	}

	//マップ外補正
	{
		const float mapLimit = 3950.0f - radius;
		//x座標補正
		if (nextPos.x > mapLimit)
		{
			nextPos.x = mapLimit;
		}
		else if (nextPos.x < -mapLimit)
		{
			nextPos.x = -mapLimit;
		}
		//z座標補正
		if (nextPos.z > mapLimit)
		{
			nextPos.z = mapLimit;
		}
		else if (nextPos.z < -mapLimit)
		{
			nextPos.z = -mapLimit;
		}
	}
	//ユニットめり込み防止
	{
		if (unitManager)
		{
			auto nearUnits =
				unitManager->GetNearUnits(
					nextPos,
					80.0f
				);

			for (auto other : nearUnits)
			{
				if (other == this)
				{
					continue;
				}

				Vector3 diff =
					nextPos - other->GetPosition();

				float limit =
					radius +
					other->GetRadius();

				float distSq =
					diff.LengthSq();

				if (distSq < limit * limit)
				{
					float dist =
						sqrtf(distSq);

					if (dist > 0.0001f)
					{
						diff /= dist;

						nextPos =
							other->GetPosition() +
							diff * limit;
					}
				}
			}
		}
	}
	//移動
	position = nextPos;

	//向き変更
	rotation.SetRotationYFromDirectionXZ(dir);
	//電力消費
	battery -= param->moveBatteryCost * g_gameTime->GetFrameDeltaTime();
}

//巡回
void Unit::Patrol()
{
	//タイマー加算
	patrolTimer += g_gameTime->GetFrameDeltaTime();
	//巡回地点未設定または到着した場合
	Vector3 dir = patrolTarget - position;
	if (patrolTimer >= 5.0f || dir.LengthSq() <= 100.0f)
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

//発電機へ移動
void Unit::Escape()
{
	if (generator == nullptr) { return; }

	Vector3 generatorPos = generator->GetPosition();

	Vector3 dir = generatorPos - position;

	float dist = dir.Length();

	//到着
	if (dist <= 150.0f)
	{
		status = Status::Charging;
		return;
	}

	dir.Normalize();

	MoveDirection(dir);
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
#pragma endregion

#pragma region 戦闘

//攻撃処理
void Unit::Attack()
{
	//プレイヤーが存在しない
	if (player == nullptr) { return; }
	//基本ユニット以外の場合
	if (type != Type::Basic) { return; }
	//プレイヤーへ向く
	rotation.SetRotationYFromDirectionXZ(direction);
	//攻撃タイマー加算
	attackTimer += g_gameTime->GetFrameDeltaTime();
	//攻撃間隔チェック
	if (attackTimer < param->attackInterval) { return; }
	//タイマーリセット
	attackTimer = 0.0f;
	//プレイヤーへダメージ
	player->Damage(param->attackPower);
	//攻撃時バッテリー消費
	battery -= param->attackBatteryCost;

}

#pragma endregion

#pragma region 状態

//ダメージ処理
void Unit::TakeDamage(int damage)
{
	hp -= damage;
	if (hp <= 0)
	{
		hp = 0;

		//機能停止状態
		if (status == Status::Stop){ return;}

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

#pragma endregion

#pragma region 充電

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
	else if (battery > 0.0f) { level = 1; }
	else { level = 0; }
	//変更なし
	if (batteryLevel == level) { return; }
	batteryLevel = level;
}

//充電対象検索
void Unit::SearchChargeTarget()
{
	//初期化
	target = nullptr;
	//管理クラスなし
	if (unitManager == nullptr) { return; }
	float nearestDistSq = FLT_MAX;
	//全ユニット検索
	for (auto unit : unitManager->GetUnits())
	{
		//自分自身の除外
		if (unit == this) { continue; }
		//停止ユニットまたは電力残量20%以下
		if (unit->GetStatus() == Status::Stop || unit->GetBattery() <= 20.0f)
		{
			Vector3 diff = unit->GetPosition() - position;
			float distSq = diff.LengthSq();
			if (distSq < nearestDistSq)
			{
				nearestDistSq = distSq;
				target = unit;
			}
			continue;
		}
	}
}

//充電処理
void Unit::Charge()
{
	//対象なし
	if (target == nullptr) { return; }
	//方向計算
	Vector3 dir = target->GetPosition() - position;
	float dist = dir.Length();
	//充電範囲外
	if (dist > 150.0f)
	{
		dir.Normalize();
		MoveDirection(dir);

		return;
	}
	//充電量
	float chargeAmount = 10.0f * g_gameTime->GetFrameDeltaTime();
	//対象へ充電
	target->battery += chargeAmount;
	//自身の電力消費
	battery -= chargeAmount;
	//対象バッテリー上限
	if (target->battery > 100.0f)
	{
		target->battery = 100.0f;
	}
	//自身の下限
	if (battery < 0.0f)
	{
		battery = 0.0f;
	}
	//機能停止ユニットの再起動
	if (target->GetStatus() == Status::Stop && target->battery >= 100.0f)
	{
		target->status = Status::Normal;
		target->stopTimer = 0.0f;
	}
	//充電完了
	if (target->GetBattery() >= 100.0f)
	{
		target = nullptr;
	}

}

//発電機充電処理
void Unit::ChargeGenerator()
{
	//充電
	battery += 10.0f * g_gameTime->GetFrameDeltaTime();

	//満タン
	if (battery >= 100.0f)
	{
		battery = 100.0f;
		//通常状態へ戻る
		status = Status::Normal;
	}
}

#pragma endregion

#pragma region 修理


#pragma endregion

#pragma region 描画

//描画処理
void Unit::Render(RenderContext& rc)
{
	//充電ユニット以外
	if (type != Type::Charger)
	{
		//機能停止モデル
		if (status == Status::Stop)
		{
			StopModel.Draw(rc);
		}
		//通常モデル
		else
		{
			modelRender.Draw(rc);
		}
	}
	//充電ユニットのみ描画
	else
	{
		batteryPack[batteryLevel].Draw(rc);
	}
}

#pragma endregion