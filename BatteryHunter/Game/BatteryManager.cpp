#include "stdafx.h"
#include "BatteryManager.h"
#include "BatteryItem.h"

BatteryManager::BatteryManager()
{
	for (int i = 0; i < POOL_SIZE; i++)
	{
		auto item = NewGO<BatteryItem>( 0,"batteryItem" );
		item->SetActive(false);
		m_pool.push_back(item);
	}
}
BatteryManager::~BatteryManager()
{

}
void BatteryManager::SpawnBattery(const Vector3& pos, const Quaternion& rot)
{
	for (auto item : m_pool)
	{
		if (item->IsActive())
		{
			continue;
		}
		item->SetPosition(pos);
		item->SetRotation(rot);
		item->SetActive(true);

		return;
	}
}