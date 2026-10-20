#include "stdafx.h"
#include "Gauge.h"
//ゲージを管理するクラス
Gauge::Gauge()
{

}

Gauge::~Gauge()
{

}


//初期化・初期設定
void Gauge::Init(
    const char* filePath,
    float width,
    float height
)
{
    m_sprite.Init(
        filePath,
        width,
        height
    );

    //------------------------------------------------
    // 左端を基準にする
    //
    // 残量が減ると
    // □□□□□□
    // ↓
    // □□□
    //
    // のように左から減る
    //------------------------------------------------
    m_sprite.SetPivot(
        Vector2(
            0.0f,
            0.5f
        )
    );
}

//座標設定
void Gauge::SetPosition( const Vector3& pos)
{
    m_sprite.SetPosition(pos);
}

//残量設定
void Gauge::SetRate(float rate)
{
    m_rate = rate;
    //------------------------------------------------
    // 0～1の範囲に制限
    //------------------------------------------------
    if (m_rate < 0.0f){ m_rate = 0.0f; }
    if (m_rate > 1.0f){ m_rate = 1.0f; }
}
//ゲージ種類設定
void Gauge::SetType(Type type){ m_type = type; }

/// <summary>
/// バッテリー色取得。
/// 
/// 色変化
/// 
/// 100～80% 濃い青
/// 80～60%  青
/// 60～40%  青緑
/// 40～25%  黄色
/// 25～15%  濃い黄色
/// 15～10%  オレンジ
/// 10～5%   濃いオレンジ
/// 5～1%    赤点滅
/// 0%       灰色
/// </summary>
Vector4 Gauge::CalcBatteryColor()
{
    //------------------------------------------------
    // 完全放電
    //------------------------------------------------
    if (m_rate <= 0.0f){ return Vector4(0.4f,0.4f,0.4f,1.0f); }
    //------------------------------------------------
    // 危険領域
    // 赤点滅
    //------------------------------------------------
    if (m_rate <= 0.05f)
    {
        float flash =
            (sinf(m_flashTimer) + 1.0f) * 0.5f;

        return Vector4(
            0.4f + flash * 0.6f,
            0.0f,
            0.0f,
            1.0f
        );
    }

    if (m_rate <= 0.10f)
    {
        return Vector4(
            1.0f,
            0.25f,
            0.0f,
            1.0f
        );
    }

    if (m_rate <= 0.15f)
    {
        return Vector4(
            1.0f,
            0.5f,
            0.0f,
            1.0f
        );
    }

    if (m_rate <= 0.25f)
    {
        return Vector4(
            1.0f,
            0.85f,
            0.0f,
            1.0f
        );
    }

    if (m_rate <= 0.40f)
    {
        return Vector4(
            1.0f,
            1.0f,
            0.0f,
            1.0f
        );
    }

    if (m_rate <= 0.60f)
    {
        return Vector4(
            0.0f,
            1.0f,
            0.5f,
            1.0f
        );
    }

    if (m_rate <= 0.80f)
    {
        return Vector4(
            0.0f,
            0.5f,
            1.0f,
            1.0f
        );
    }

    return Vector4(
        0.0f,
        0.3f,
        1.0f,
        1.0f
    );
}

//更新処理
void Gauge::Update()
{
    //------------------------------------------------
    // 点滅タイマー更新
    //------------------------------------------------
    m_flashTimer += 0.25f;

    Vector4 color =
        Vector4(
            1.0f,
            1.0f,
            1.0f,
            1.0f
        );

    //------------------------------------------------
    // 色決定
    //------------------------------------------------
    switch (m_type)
    {
    case Battery:
        color = CalcBatteryColor();
        break;
    case PlayerBattery:
        if (m_rate <= 0.0f) { color=Vector4(0.4f, 0.4f, 0.4f, 1.0f); }
        else{ color = Vector4(0.0f, 0.0f, 1.0f, 1.0f); }
        break;
    case BossBattery:
        if (m_rate <= 0.0f) { color=Vector4(0.4f, 0.4f, 0.4f, 1.0f); }
        else { color = Vector4(1.0f, 0.0f, 0.0f, 1.0f); }
        break;
    case HP:
        color = Vector4( 0.0f,1.0f, 0.0f, 1.0f);
        break;
    }

    m_sprite.SetMulColor(color);

    //------------------------------------------------
    // ゲージ長さ変更
    //------------------------------------------------

    float scaleX;

    //ゲージの長さが0.0f以下の場合
    if (m_rate <= 0.0f)
    {
        //ゲージのサイズを100%にする
        scaleX = 1.0f;
    }
    else
    {
        //ゲージのサイズを順次変更
        scaleX = m_rate;
    }

    m_sprite.SetScale(
        Vector3(
            scaleX,
            1.0f,
            1.0f
        )
    );

    m_sprite.Update();
}

//描画処理
void Gauge::Draw(RenderContext& rc)
{
    m_sprite.Draw(rc);
}