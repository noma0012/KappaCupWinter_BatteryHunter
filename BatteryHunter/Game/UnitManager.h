#pragma once
class Unit;

/// <summary>
/// ユニット性能データ
/// </summary>
struct UnitParam
{
	//ステータス
	int hp;
	float battery;
	///索敵範囲
	float searchRange;
	///攻撃
	float attackRange;
	float attackInterval;
	int attackPower;
	///移動速度
	float moveSpeed;
	//消費
	float moveBatteryCost;
	float attackBatteryCost;
	float selfRepairBatteryCost;

};

enum class Type { //種類
	//通常ユニット
	Basic,     //基本ユニット
	Charger,   //充電ユニット
	//特殊ユニット
	Repair,    //修理ユニット
	//ボスユニット
	Boss,      //ボス
	Max
};
//状態
enum class Status {
	Normal,     //通常
	SelfRepair, //自己修復中
	Stop,       //機能停止
	Charging,   //充電中
	Repairing,  //修理中
};

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
	//全ユニット取得
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
	/// <summary>
	/// ユニット性能取得
	/// </summary>
	const UnitParam& GetParam(
		Type type
	)const;
private:
	//ユニット一覧
	std::vector<Unit*>m_units;
	//種別パラメーター
	UnitParam m_unitParam[(int)Type::Max];
};

