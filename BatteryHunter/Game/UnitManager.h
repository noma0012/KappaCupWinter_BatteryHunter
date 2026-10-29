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
	/// <summary>
	/// 指定位置周辺のユニットを取得
	/// </summary>
	/// <param name="pos">検索中心座標</param>
	/// <param name="range">検索半径</param>
	/// <returns>範囲内のユニット一覧</returns>
	std::vector<Unit*>GetNearUnits(
		const Vector3& pos,
		float range
	);
private:
	//ユニット一覧
	std::vector<Unit*>m_units;
};

