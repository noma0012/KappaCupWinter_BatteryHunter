#pragma once

class Player;

class BatteryItem:public IGameObject
{
public:
	BatteryItem();
	~BatteryItem();

	void SetPosition(const Vector3& pos);
	void SetRotation(const Quaternion& rot);
private:
	void Update();
	void Render(RenderContext& rc);

	ModelRender modelRender;
	Quaternion rotation;

	Vector3 position;
	float radius = 30.0f;

	Player* player = nullptr;
};

