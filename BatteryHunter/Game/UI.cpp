#include "stdafx.h"
#include "UI.h"
#include "Player.h"
#include "BackGround.h"
#include "Game.h"
#include "Gauge.h"
//画面表示を管理するクラス
UI::UI()
{
    ///━━━━━━━━━━
    ///画面サイズ1920×1080
    ///━━━━━━━━━━
    
    // タイマー開始
    startTime = steady_clock::now();
    
    //座標設定
    font[Time].SetPosition({ -44.0f, 520.0f,0.0f });       //時間
    font[PlayerPos].SetPosition({ -900.0f, 460.0f,0.0f }); //プレイヤー座標
    //表示する色を設定する。
    font[Time].SetColor(g_vec4White);

    //プレイヤーバッテリー仮設定
    Battery[B_Player].Init("Assets/sprite/White.DDS", 300.0f, 30.0f);
    Battery[B_Player].SetPosition({ -900.0f,-400.0f,0.0f });
    Battery[B_Player].SetType(Gauge::PlayerBattery);

    //ボスバッテリー仮設定
    Battery[B_Boss].Init("Assets/sprite/White.DDS", 500.0f, 40.0f);
    Battery[B_Boss].SetPosition({ -235.0f,450.0f,0.0f });
    Battery[B_Boss].SetType(Gauge::BossBattery);

    //ライン仮設定
    Line[0].SetPosition({ -250.0f,470.0f,0.0f });
    Line[1].SetPosition({    0.0f,470.0f,0.0f });
    Line[2].SetPosition({  250.0f,470.0f,0.0f });

    player = FindGO<Player>("player");
    game = FindGO<Game>("game");
}
UI::~UI()
{

}
void UI::SetGame(Game* g) { game = g; }
//更新管理
void UI::Update()
{
    SetTime();//時間更新
    SetPlayerPos();//位置座標更新
    SetBattery();//バッテリー更新

    //------------------------
    // Popup更新
    //------------------------

    m_textPopupManager.Update(
        g_gameTime->GetFrameDeltaTime()
    );

}
//時間管理
void UI::SetTime()
{
    remain = max(0,(int)duration<float>(steady_clock::now() - startTime).count());

    //------------------------
    // 5秒ごとに表示
    //------------------------

    static int oldTime = -1;

    if (remain != oldTime)
    {
        oldTime = remain;

        if (remain % 1 == 0)
        {
            m_textPopupManager.CreateDamage(
                100,
                Vector3(0.0f, 0.0f, 0.0f)
            );
        }
    }

    //一時間になったら終了状態にする
    if (remain == 3600 && !Finished) { Finished = true; }

    //表示更新
    swprintf_s(buffer[Time], 32, L"%02d:%02d",remain/60 ,remain%60);
    font[Time].SetText(buffer[Time]);
}
//位置座標管理
void UI::SetPlayerPos() {
    Vector3 pos = player->GetPosition();
    swprintf_s(buffer[PlayerPos], 32, L"X:%.1f Y:%.1f Z:%.1f", pos.x, pos.y, pos.z);
    font[PlayerPos].SetText(buffer[PlayerPos]);
}
//バッテリー管理
void UI::SetBattery() {
    //ボスバッテリーの区切り線
    for (int i = 0; i < 3; i++) { Line[i].SetText(L"|"); }

    //初期バッテリーサイズ
    static float playerBattery = 100.0f;
    static float bossBattery = 100.0f;
    //減少速度
    playerBattery -= 0.05f;
    bossBattery -= 0.01f;
    //サイズ設定
    Battery[B_Player].SetRate(playerBattery / 100.0f);
    Battery[B_Boss].SetRate(bossBattery / 100.0f);
    //更新処理
    Battery[B_Player].Update();
    Battery[B_Boss].Update();
}
//ダメージポップアップ生成管理
void UI::CreateDamagePopup(int damage,const Vector3& pos)
{
    m_textPopupManager.CreateDamage(
        damage,
        pos
    );
}
//描画管理
void UI::Render(RenderContext& rc)
{
    font[Time].Draw(rc);
    font[PlayerPos].Draw(rc);

    for (int i = 0; i < B_Max; i++) { Battery[i].Draw(rc); }
    for (int i = 0; i < 3; i++)     { Line[i].Draw(rc); }

    // Popup描画
    m_textPopupManager.Draw(rc);
}