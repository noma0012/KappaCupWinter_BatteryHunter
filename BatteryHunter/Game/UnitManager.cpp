#include "stdafx.h"
#include "UnitManager.h"
#include "Unit.h"

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
