#include "stdafx.h"
#include "BackGround.h"
//ステージを管理するクラス
BackGround::BackGround()
{
	//コメントアウトする。
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	
	//ステージ(仮)を読み込む
	modelRender.Init("Assets/modelData/ground.tkm");
	modelRender.Update();
	physicsStaticObject.CreateFromModel(modelRender.GetModel(), modelRender.GetModel().GetWorldMatrix());
}

BackGround::~BackGround()
{

}

void BackGround::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}