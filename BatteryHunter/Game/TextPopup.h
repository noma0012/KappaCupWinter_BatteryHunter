#pragma once

#include "Graphics/FontRender.h"

/// <summary>
/// ポップアップアニメーション種別
/// </summary>
enum class PopupAnim
{
    Rise,       // 上へ移動
    Bounce,     // 跳ねる
    Shake,      // 横揺れ
    ZoomOut     // 拡大縮小
};

/// <summary>
/// 描画レイヤー
/// </summary>
enum class PopupLayer
{
    World,      // ワールド座標
    Screen      // スクリーン座標
};

/// <summary>
/// 優先度
/// </summary>
enum class PopupPriority
{
    Low,
    Normal,
    High
};

/// <summary>
/// テキスト表示スタイル
/// </summary>
struct PopupStyle
{
    /// <summary>
    /// 文字色
    /// </summary>
    Vector4 color = g_vec4White;

    /// <summary>
    /// 初期サイズ
    /// </summary>
    float scale = 1.0f;

    /// <summary>
    /// 表示時間(秒)
    /// </summary>
    float lifeTime = 1.0f;

    /// <summary>
    /// 上昇速度
    /// </summary>
    float velocityY = 60.0f;

    /// <summary>
    /// アニメーション
    /// </summary>
    PopupAnim anim = PopupAnim::Rise;
};

/// <summary>
/// 汎用テキストポップアップ
///
/// ダメージ
/// 回復
/// EXP
/// GOLD
/// LEVEL UP
/// BOSS APPEAR
///
/// などの表示に利用する。
/// </summary>
class TextPopup : public IGameObject
{
public:

    /// <summary>
    /// 表示開始
    /// </summary>
    /// <param name="text">表示文字列</param>
    /// <param name="position">表示位置</param>
    /// <param name="style">スタイル</param>
    /// <param name="layer">描画レイヤー</param>
    /// <param name="priority">優先度</param>
    void Activate(
        const wchar_t* text,
        const Vector3& position,
        const PopupStyle& style,
        PopupLayer layer,
        PopupPriority priority
    );

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="deltaTime">経過時間</param>
    void Update(float deltaTime);

    /// <summary>
    /// 描画処理
    /// </summary>
    void Draw(RenderContext& rc);

    /// <summary>
    /// 使用中か？
    /// </summary>
    bool IsActive() const
    {
        return m_isActive;
    }

    /// <summary>
    /// 優先度取得
    /// </summary>
    PopupPriority GetPriority() const
    {
        return m_priority;
    }

private:

    //----------------------------------------
    // 描画
    //----------------------------------------

    /// フォント描画オブジェクト
    FontRender m_font;

    //----------------------------------------
    // 座標情報
    //----------------------------------------

    /// 現在座標
    Vector3 m_position = Vector3::Zero;

    /// 初期座標
    Vector3 m_startPosition = Vector3::Zero;

    //----------------------------------------
    // 設定
    //----------------------------------------

    /// 表示スタイル
    PopupStyle m_style;

    /// 描画レイヤー
    PopupLayer m_layer = PopupLayer::World;

    /// 優先度
    PopupPriority m_priority =
        PopupPriority::Normal;

    //----------------------------------------
    // 状態
    //----------------------------------------

    /// 経過時間
    float m_timer = 0.0f;

    /// 使用中フラグ
    bool m_isActive = false;
};