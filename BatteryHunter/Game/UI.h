#pragma once
#include "Gauge.h"
#include "TextPopupManager.h"
using namespace std::chrono;

class Player;
class Game;

class UI : public IGameObject
{
public:
    //UI種類
    enum UITopic {
        Time,      //時間
        PlayerPos, //プレイヤー位置座標
        PlayerBattery,//プレイヤー電力残量
        MaxUI
    };
    //UIゲージ用バッテリー
    enum UIBattery {
        B_Player,
        B_Boss,
        B_Max
    };
    UI();
    ~UI();
    //更新管理
    void Update();
    //その他更新管理
    void SetTime();
    void SetPlayerPos();
    void SetBattery();
    //その他
    void SetGame(Game* g);
    void Render(RenderContext& rc);//描画
    bool Finished;//終了状態

public:
    //ダメージ表示
    void CreateDamagePopup(int damage, const Vector3& pos);
    //回復表示
    void CreateHealPopup(int Heal, const Vector3& pos);
private:
    steady_clock::time_point startTime;//開始時間
    int remain;//現在の時間
    wchar_t buffer[MaxUI][32];// 表示用
    //フォント描画（既存のクラス）48px
    FontRender font[MaxUI];
    FontRender Line[3];
    //インスタンス
    Player* player;
    Game* game;

    Gauge Battery[B_Max];

    // ポップアップ管理
    TextPopupManager m_textPopupManager;
};
