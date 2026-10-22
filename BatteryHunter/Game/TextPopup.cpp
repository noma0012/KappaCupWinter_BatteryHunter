#include "stdafx.h"
#include "TextPopup.h"

/// <summary>
/// ポップアップ表示開始
/// </summary>
/// <param name="text">表示文字列</param>
/// <param name="position">表示位置</param>
/// <param name="style">表示スタイル</param>
/// <param name="layer">描画レイヤー</param>
/// <param name="priority">優先度</param>
void TextPopup::Activate(
    const wchar_t* text,
    const Vector3& position,
    const PopupStyle& style,
    PopupLayer layer,
    PopupPriority priority
)
{
    //----------------------------------------
    // 状態初期化
    //----------------------------------------

    m_isActive = true;

    m_timer = 0.0f;

    m_style = style;

    m_layer = layer;

    m_priority = priority;

    //----------------------------------------
    // 座標初期化
    //----------------------------------------

    m_position = position;

    m_startPosition = position;

    //----------------------------------------
    // 重なり防止のため少しランダムにずらす
    //----------------------------------------

    m_position.x += static_cast<float>(
        rand() % 40 - 20
        );

    m_position.y += static_cast<float>(
        rand() % 20
        );

    //----------------------------------------
    // フォント設定
    //----------------------------------------

    m_font.SetText(text);

    m_font.SetPosition(m_position);

    m_font.SetColor(m_style.color);

    m_font.SetScale(m_style.scale);

    //----------------------------------------
    // 見やすくするため影を付ける
    //----------------------------------------

    m_font.SetShadowParam(
        true,
        2.0f,
        Vector4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="deltaTime">経過時間</param>
void TextPopup::Update(float deltaTime)
{
    //----------------------------------------
    // 非アクティブは処理しない
    //----------------------------------------

    if (!m_isActive)
    {
        return;
    }

    //----------------------------------------
    // 経過時間更新
    //----------------------------------------

    m_timer += deltaTime;

    //----------------------------------------
    // 上昇移動
    //----------------------------------------

    m_position.y +=
        m_style.velocityY * deltaTime;

    //----------------------------------------
    // アニメーション処理
    //----------------------------------------

    switch (m_style.anim)
    {
    case PopupAnim::Rise:
    {
        // 上昇のみ
    }
    break;

    case PopupAnim::Bounce:
    {
        // 少し上下に跳ねる

        m_position.y +=
            sinf(m_timer * 15.0f) * 2.0f;
    }
    break;

    case PopupAnim::Shake:
    {
        // 左右へ揺らす

        m_position.x +=
            sinf(m_timer * 20.0f);
    }
    break;

    case PopupAnim::ZoomOut:
    {
        // 出現直後を大きく表示

        float scale =
            m_style.scale +
            (1.0f - m_timer);

        m_font.SetScale(scale);
    }
    break;
    }

    //----------------------------------------
    // フェードアウト
    //----------------------------------------

    float alpha =
        1.0f -
        (m_timer / m_style.lifeTime);

    if (alpha < 0.0f)
    {
        alpha = 0.0f;
    }

    Vector4 color =
        m_font.GetColor();

    color.w = alpha;

    m_font.SetColor(color);

    //----------------------------------------
    // 描画座標
    //----------------------------------------

    Vector3 drawPosition =
        m_position;

    //----------------------------------------
    // ワールド座標変換
    //----------------------------------------

    if (m_layer == PopupLayer::World)
    {
        Vector2 screenPos;

        g_camera3D->CalcScreenPositionFromWorldPosition(
            screenPos,
            m_position
        );
        drawPosition.x = screenPos.x;
        drawPosition.y = screenPos.y;
        drawPosition.z = 0.0f;

    }

    //----------------------------------------
    // 表示位置更新
    //----------------------------------------

    m_font.SetPosition(
        drawPosition
    );

    //----------------------------------------
    // 表示寿命終了
    //----------------------------------------

    if (m_timer >= m_style.lifeTime)
    {
        m_isActive = false;
    }
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="rc">レンダーコンテキスト</param>
void TextPopup::Draw(
    RenderContext& rc
)
{
    //----------------------------------------
    // 非アクティブなら描画しない
    //----------------------------------------

    if (!m_isActive)
    {
        return;
    }

    //----------------------------------------
    // 文字描画
    //----------------------------------------

    m_font.Draw(rc);
}