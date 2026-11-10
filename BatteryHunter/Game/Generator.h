#pragma once
class Generator : public IGameObject
{
public:
	Generator();
	~Generator();
	void Update();
	void Render(RenderContext& rc);

	ModelRender modelRender;
	PhysicsStaticObject physicsStaticObject;

	bool Startup = false;//起動判定
};

