#pragma once

class BatteryItem;

class BatteryManager :public IGameObject
{
public:
	BatteryManager();
	~BatteryManager();

	//バッテリー出現
	void SpawnBattery(
		const Vector3& pos,
		const Quaternion& rot
	);
private:
	std::vector<BatteryItem*>m_pool;
	static const int POOL_SIZE = 100;
};

