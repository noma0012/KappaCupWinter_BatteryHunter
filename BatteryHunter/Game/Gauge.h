#pragma once

/// <summary>
/// 汎用ゲージクラス。
/// HP、バッテリー、シールドなどを表示するための共通クラス。
/// 
/// 機能
/// ・残量に応じて横幅を変更
/// ・残量に応じて色を変更
/// ・低残量時の点滅
/// ・0%時は灰色化
/// 
/// 使用例
/// Gauge m_batteryGauge;
/// Gauge m_hpGauge;
/// </summary>
class Gauge : public IGameObject
{
public:
    //ゲージの種類
    enum Type
    {
        Battery,       // バッテリー
        PlayerBattery, //プレイヤー専用バッテリー
        BossBattery,   //ボス専用バッテリー
        HP,            // HP
        Shield         // シールド
    };

public:

    Gauge();
    ~Gauge();

    /// <summary>
    /// 初期化・初期設定
    /// 白色画像推奨
    /// </summary>
    /// <param name="filePath">画像パス</param>
    /// <param name="width">初期横幅</param>
    /// <param name="height">初期高さ</param>
    void Init( const char* filePath,float width,float height);
    void Update();                           //更新処理
    void Draw(RenderContext& rc);            //描画処理
    void SetPosition( const Vector3& pos );  //座標設定
    /// <summary>
    /// 残量設定
    /// 
    /// 0.0f = 0%
    /// 1.0f = 100%
    /// </summary>
    void SetRate( float rate );

    //ゲージの種類設定
    void SetType(Type type);
    //現在の残量取得
    float GetRate() const{ return m_rate; }

private:
    //バッテリー用色取得
    Vector4 CalcBatteryColor();
    // スプライト描画
    SpriteRender m_sprite;
    // 残量(0～1)
    float m_rate = 1.0f;
    // 点滅用タイマー
    float m_flashTimer = 0.0f;
    // 種類
    Type m_type = Battery;
};