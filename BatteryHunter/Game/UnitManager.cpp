#include "stdafx.h"
#include "UnitManager.h"
#include "Unit.h"
//ユニット管理クラス
UnitManager::UnitManager()
{

}
UnitManager::~UnitManager()
{

}
void UnitManager::Update()
{

}
void UnitManager::Render(RenderContext& rc)
{

}
//生成
Unit* UnitManager::SpawnUnit(
	Type type,
	float x,
	float y,
	float z
)
{
	Unit* unit =
		NewGO<Unit>(0, "unit");
	unit->SetUp(type, x, y, z);
	m_units.push_back(unit);

	return unit;
}
//削除
void UnitManager::RemoveUnit(Unit* unit)
{
	auto it = std::find(
		m_units.begin(),
		m_units.end(),
		unit
	);
	if (it != m_units.end())
	{
		DeleteGO(unit);
		m_units.erase(it);
	}
}
//近くのユニット取得
std::vector<Unit*> UnitManager::GetNearUnits(
	const Vector3& pos,
	float range
)
{
	//検索結果
	std::vector<Unit*> result;
	//二乗距離を使用して高速化
	float rangeSq = range * range;
	//全ユニットを走査(索敵)
	for (auto unit : m_units)
	{
		//検索中心との距離
		Vector3 diff = unit->GetPosition() - pos;
		//距離の二乗
		float distSq = diff.LengthSq();
		//範囲内なら登録
		if (distSq <= rangeSq)
		{
			result.push_back(unit);
		}
	}
	return result;
}
