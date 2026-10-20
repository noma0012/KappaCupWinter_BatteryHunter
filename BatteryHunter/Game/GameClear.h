#pragma once

//ゲームクリア。
class GameClear : public IGameObject
{
public:
    GameClear();
    ~GameClear();

    void Update();//更新処理
    void Render(RenderContext& rc);//描画処理

private:
    SpriteRender spriteRender;

};