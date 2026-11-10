#include "stdafx.h"
#include "Generator.h"

//発電機を管理するクラス
Generator::Generator()
{
	//発電機読み込み
	modelRender.Init("Assets/modelData/Battery/generator_on.tkm");
	//modelRender.Init("Assets/modelData/Battery/generator_off.tkm");
	modelRender.Update();
	physicsStaticObject.CreateFromModel(modelRender.GetModel(), modelRender.GetModel().GetWorldMatrix());
}
Generator::~Generator()
{

}
void Generator::Update()
{

}
void Generator::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}

