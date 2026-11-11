#include "stdafx.h"
#include "UI.h"
#include "Player.h"
#include "BackGround.h"
#include "Game.h"
#include "Gauge.h"
#include "Generator.h"

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
    font[PlayerBattery].SetPosition({ -550.0f,-380.0f,0.0f });//プレイヤー電力残量
    //表示する色を設定する。
    font[Time].SetColor(g_vec4White);

    //プレイヤーバッテリー設定
    Gauge[Player_Gauge].Init("Assets/sprite/White.DDS", 300.0f, 30.0f);
    Gauge[Player_Gauge].SetPosition({ -900.0f,-400.0f,0.0f });
    Gauge[Player_Gauge].SetType(Gauge::PlayerBattery);

    //ボスバッテリー仮設定
    Gauge[Boss_Gauge].Init("Assets/sprite/White.DDS", 500.0f, 40.0f);
    Gauge[Boss_Gauge].SetPosition({ -235.0f,450.0f,0.0f });
    Gauge[Boss_Gauge].SetType(Gauge::BossBattery);

    //ライン仮設定
    Line[0].SetPosition({ -250.0f,470.0f,0.0f });
    Line[1].SetPosition({    0.0f,470.0f,0.0f });
    Line[2].SetPosition({  250.0f,470.0f,0.0f });
    //時間装飾用
    frame[Time_frame].Init("Assets/sprite/Time_frame.DDS",200.0f,70.0f);
    frame[Time_frame].SetPosition({ 17.5f, 500.5f,0.0f });

    //プレイヤー電力ゲージ装飾用
    frame[Gauge_frame].Init("Assets/sprite/Gauge_frame.DDS", 340.0f, 200.0f);
    frame[Gauge_frame].SetPosition({ -750.0f,-400.0f,0.0f });

    //プレイヤー電力ゲージの背景装飾用
    frame[Background_frame].Init("Assets/sprite/Time_frame.DDS", 320.0f, 50.0f);
    frame[Background_frame].SetPosition({ -760.0f,-400.0f,0.0f });

    //発電機停止ゲージ
    Gauge[Generator_Gauge].Init("Assets/sprite/White.DDS", 500.0f, 40.0f);
    Gauge[Generator_Gauge].SetPosition({ -235.0f,400.0f,0.0f });
    Gauge[Generator_Gauge].SetType(Gauge::GeneratorStop);

    player = FindGO<Player>("player");
    game = FindGO<Game>("game");
    generator = FindGO<Generator>("generator");
}
UI::~UI()
{

}
void UI::SetGame(Game* g) { game = g; }
//更新管理
void UI::Update()
{
    if (generator == nullptr) { generator = FindGO<Generator>("generator"); }

    SetTime();//時間更新
    SetPlayerPos();//位置座標更新
    SetBattery();//バッテリー更新

    //------------------------
    // Popup更新
    //------------------------

    m_textPopupManager.Update(
        g_gameTime->GetFrameDeltaTime()
    );

    for (int i = 0; i < Max_frame; i++)
    {
        frame[i].Update();
    }
}
//時間管理
void UI::SetTime()
{
    remain = max(0,(int)duration<float>(steady_clock::now() - startTime).count());

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
    static float bossBattery = 100.0f;
    //減少速度
    bossBattery -= 0.01f;


    //サイズ設定
    Gauge[Player_Gauge].SetRate(player->GetBattery() / player->GetBatteryMax());
    Gauge[Boss_Gauge].SetRate(bossBattery / 100.0f);

    //電力残量表示
    swprintf_s(buffer[PlayerBattery], 32, L"%.1f", player->GetBattery());
    font[PlayerBattery].SetText(buffer[PlayerBattery]);

    if (generator)
    {
        Gauge[Generator_Gauge].SetRate(generator->GetStopRate());
    }

    //更新処理
    Gauge[Player_Gauge].Update();
    Gauge[Boss_Gauge].Update();
    if (generator && generator->IsPlayerInsideRange())
    {
        Gauge[Generator_Gauge].Update();
    }
}
//ダメージポップアップ生成管理
void UI::CreateDamagePopup(int damage,const Vector3& pos)
{
    m_textPopupManager.CreateDamage(
        damage,
        pos
    );
}
//回復ポップアップ生成管理
void UI::CreateHealPopup(int Heal, const Vector3& pos)
{
    m_textPopupManager.CreateHeal(
        Heal,
        pos
    );
}
//描画管理
void UI::Render(RenderContext& rc)
{
    frame[Time_frame].Draw(rc);
    frame[Background_frame].Draw(rc);

    for (int i = 0; i < MaxUI; i++)     {  font[i].Draw(rc); }

    Gauge[Player_Gauge].Draw(rc);
    Gauge[Boss_Gauge].Draw(rc);

    if (generator && generator->IsPlayerInsideRange())
    {
        Gauge[Generator_Gauge].Draw(rc);
    }

    frame[Gauge_frame].Draw(rc);

    for (int i = 0; i < 3; i++) { Line[i].Draw(rc); }

    // Popup描画
    m_textPopupManager.Draw(rc);
}