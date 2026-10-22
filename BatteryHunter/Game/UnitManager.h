#pragma once
#include "Unit.h"

///━━━━━━━━━━
/// ユニット管理クラス
/// 
/// ・生成
/// ・削除
/// ・検索
/// ・管理
///━━━━━━━━━━
class UnitManager :public IGameObject
{
public:
	UnitManager();
	~UnitManager();

	void Update();
	void Render(RenderContext& rc);

	//ユニット生成
	Unit* SpawnUnit(
		Type type,
		float x,
		float y,
		float z
	);
	//ユニット削除
	void RemoveUnit(Unit* unit);
	//ユニット取得
	const std::vector<Unit*>& GetUnits()const
	{
		return m_units;
	}
private:
	//ユニット一覧
	std::vector<Unit*>m_units;
};

