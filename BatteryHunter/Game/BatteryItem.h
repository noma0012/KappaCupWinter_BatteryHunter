#pragma once

class Player;
class UI;

class BatteryItem:public IGameObject
{
public:
	BatteryItem();
	~BatteryItem();

	void SetPosition(const Vector3& pos);
	void SetRotation(const Quaternion& rot);
	void SetActive(bool flag);
	bool IsActive() const { return m_isActive; }
private:
	void Update();
	void Render(RenderContext& rc);

	ModelRender modelRender;

	Vector3 position;
	Quaternion rotation;
	
	float radius = 30.0f;

	bool m_isActive = false;

	Player* player = nullptr;
	UI* ui = nullptr;
};

