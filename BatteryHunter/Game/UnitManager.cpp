#include "stdafx.h"
#include "UnitManager.h"
#include "Unit.h"
//ユニット管理クラス
UnitManager::UnitManager()
{
	///━━━━━━━
	/// 通常ユニット
	///━━━━━━━
	
	//基本ユニット
	m_unitParam[(int)Type::Basic] = {
		100,    //HP(耐久力)
		100.0f, //バッテリー
		2000.0f,//索敵範囲
		100.0f,  //攻撃範囲
		3.0f,   //攻撃間隔
		5,      //攻撃力
		4.0f,   //移動速度
		0.4f,   //移動消費
		5.0f,   //攻撃消費
		0.5f    //自己修復消費
	};
	//充電ユニット
	m_unitParam[(int)Type::Charger] = {
		80,     //HP(耐久力)
		100.0f, //バッテリー
		2000.0f,//索敵範囲
		0.0f,   //攻撃範囲
		0.0f,   //攻撃間隔
		0,      //攻撃力
		3.0f,   //移動速度
		0.2f,   //移動消費
		0.0f,   //攻撃消費
		0.3f    //自己修復消費
	};

	///━━━━━━━
	/// 特殊ユニット
	///━━━━━━━

	//修理ユニット
	m_unitParam[(int)Type::Repair] = {
		120,    //HP(耐久力)
		100.0f, //バッテリー
		2000.0f,//索敵範囲
		0.0f,   //攻撃範囲
		0.0f,   //攻撃間隔
		0,      //攻撃力
		3.0f,   //移動速度
		0.2f,   //移動消費
		0.0f,   //攻撃消費
		0.1f    //自己修復消費
	};

	///━━━━━━━
	/// ボスユニット
	///━━━━━━━
	
	//ボスユニット
	m_unitParam[(int)Type::Boss] = {
		2000,    //HP(耐久力)
		1000.0f, //バッテリー
		5000.0f, //索敵範囲
		1000.0f, //攻撃範囲
		15.0f,   //攻撃間隔
		50,      //攻撃力
		0.0f,    //移動速度
		0.0f,    //移動消費
		1.0f,    //攻撃消費
		0.5f     //自己修復消費
	};

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
	Type type,float x,float y,float z)
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
	const Vector3& pos,float range)
{
	//検索結果
	std::vector<Unit*> result;
	//二乗距離
	float rangeSq = range * range;
	//全ユニット確認
	for (auto unit : m_units)
	{
		//中心点との距離
		Vector3 diff = unit->GetPosition() - pos;
		float distSq = diff.LengthSq();
		//範囲内なら登録
		if (distSq <= rangeSq)
		{
			result.push_back(unit);
		}
	}
	return result;
}
//種類別パラメーター取得
const UnitParam& UnitManager::GetParam(Type type)const
{
	return m_unitParam[(int)type];
}
