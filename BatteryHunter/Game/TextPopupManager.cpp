#include "stdafx.h"
#include "TextPopupManager.h"

/// <summary>
/// ポップアップ生成
/// </summary>
/// <param name="text">表示文字列</param>
/// <param name="position">表示位置</param>
/// <param name="style">表示スタイル</param>
/// <param name="layer">描画レイヤー</param>
/// <param name="priority">優先度</param>
void TextPopupManager::Create(
    const wchar_t* text,
    const Vector3& position,
    const PopupStyle& style,
    PopupLayer layer,
    PopupPriority priority
)
{
    //----------------------------------------
    // 現在のスロットへ生成
    //----------------------------------------

    m_popups[m_nextIndex].Activate(
        text,
        position,
        style,
        layer,
        priority
    );

    //----------------------------------------
    // リングバッファ更新
    //----------------------------------------

    m_nextIndex++;

    //----------------------------------------
    // 配列末尾なら先頭へ戻る
    //----------------------------------------

    if (m_nextIndex >= MAX_POPUP)
    {
        m_nextIndex = 0;
    }
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="deltaTime">経過時間</param>
void TextPopupManager::Update(
    float deltaTime
)
{
    //----------------------------------------
    // 全Popup更新
    //----------------------------------------

    for (int i = 0; i < MAX_POPUP; i++)
    {
        m_popups[i].Update(
            deltaTime
        );
    }
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="rc">レンダーコンテキスト</param>
void TextPopupManager::Draw(
    RenderContext& rc
)
{
    //----------------------------------------
    // 全Popup描画
    //----------------------------------------

    for (int i = 0; i < MAX_POPUP; i++)
    {
        m_popups[i].Draw(rc);
    }
}

/// <summary>
/// ダメージ表示
/// </summary>
/// <param name="damage">ダメージ量</param>
/// <param name="position">表示位置</param>
void TextPopupManager::CreateDamage(
    int damage,
    const Vector3& position
)
{
    wchar_t text[64];

    swprintf_s(
        text,
        L"-%d",
        damage
    );

    PopupStyle style;

    style.color = Vector4(
        1.0f,
        1.0f,
        1.0f,
        1.0f
    );

    style.scale = 1.0f;

    style.lifeTime = 1.0f;

    style.velocityY = 80.0f;

    style.anim = PopupAnim::Rise;

    Create(
        text,
        position,
        style,
        PopupLayer::World,
        PopupPriority::Low
    );
}

/// <summary>
/// クリティカル表示
/// </summary>
/// <param name="damage">ダメージ量</param>
/// <param name="position">表示位置</param>
void TextPopupManager::CreateCritical(
    int damage,
    const Vector3& position
)
{
    wchar_t text[64];

    swprintf_s(
        text,
        L"CRITICAL! %d",
        damage
    );

    PopupStyle style;

    style.color = Vector4(
        1.0f,
        0.0f,
        0.0f,
        1.0f
    );

    style.scale = 2.0f;

    style.lifeTime = 1.5f;

    style.velocityY = 120.0f;

    style.anim = PopupAnim::Bounce;

    Create(
        text,
        position,
        style,
        PopupLayer::World,
        PopupPriority::High
    );
}

/// <summary>
/// 回復表示
/// </summary>
/// <param name="heal">回復量</param>
/// <param name="position">表示位置</param>
void TextPopupManager::CreateHeal(
    int heal,
    const Vector3& position
)
{
    wchar_t text[64];

    swprintf_s(
        text,
        L"+%d",
        heal
    );

    PopupStyle style;

    style.color = Vector4(
        0.0f,
        0.0f,
        1.0f,
        1.0f
    );

    style.scale = 1.2f;

    style.lifeTime = 1.2f;

    style.velocityY = 60.0f;

    style.anim = PopupAnim::Rise;

    Create(
        text,
        position,
        style,
        PopupLayer::World,
        PopupPriority::Normal
    );
}

/// <summary>
/// EXP獲得表示
/// </summary>
/// <param name="exp">取得経験値</param>
/// <param name="position">表示位置</param>
void TextPopupManager::CreateExp(
    int exp,
    const Vector3& position
)
{
    wchar_t text[64];

    swprintf_s(
        text,
        L"EXP +%d",
        exp
    );

    PopupStyle style;

    style.color = Vector4(
        0.0f,
        1.0f,
        1.0f,
        1.0f
    );

    style.scale = 1.0f;

    style.lifeTime = 1.5f;

    style.velocityY = 50.0f;

    style.anim = PopupAnim::Rise;

    Create(
        text,
        position,
        style,
        PopupLayer::World,
        PopupPriority::Normal
    );
}

/// <summary>
/// ゴールド獲得表示
/// </summary>
/// <param name="gold">取得ゴールド</param>
/// <param name="position">表示位置</param>
void TextPopupManager::CreateGold(
    int gold,
    const Vector3& position
)
{
    wchar_t text[64];

    swprintf_s(
        text,
        L"GOLD +%d",
        gold
    );

    PopupStyle style;

    style.color = Vector4(
        1.0f,
        1.0f,
        0.0f,
        1.0f
    );

    style.scale = 1.0f;

    style.lifeTime = 1.5f;

    style.velocityY = 50.0f;

    style.anim = PopupAnim::Rise;

    Create(
        text,
        position,
        style,
        PopupLayer::World,
        PopupPriority::Normal
    );
}
/// <summary>
/// レベルアップ表示
/// </summary>
/// <param name="position">表示位置</param>
void TextPopupManager::CreateLevelUp(
    const Vector3& position
)
{
    PopupStyle style;

    //----------------------------------------
    // レベルアップ用スタイル
    //----------------------------------------

    style.color = Vector4(
        1.0f,
        1.0f,
        0.0f,
        1.0f
    );

    style.scale = 3.0f;

    style.lifeTime = 2.0f;

    style.velocityY = 30.0f;

    style.anim = PopupAnim::ZoomOut;

    Create(
        L"LEVEL UP!",
        position,
        style,
        PopupLayer::Screen,
        PopupPriority::High
    );
}

/// <summary>
/// Wave開始表示
/// </summary>
void TextPopupManager::CreateWaveStart()
{
    PopupStyle style;

    //----------------------------------------
    // Wave開始用スタイル
    //----------------------------------------

    style.color = Vector4(
        1.0f,
        1.0f,
        0.0f,
        1.0f
    );

    style.scale = 4.0f;

    style.lifeTime = 3.0f;

    style.velocityY = 20.0f;

    style.anim = PopupAnim::ZoomOut;

    //----------------------------------------
    // 画面中央表示
    //----------------------------------------

    Create(
        L"WAVE START",
        Vector3(
            640.0f,
            360.0f,
            0.0f
        ),
        style,
        PopupLayer::Screen,
        PopupPriority::High
    );
}

/// <summary>
/// ボス出現表示
/// </summary>
void TextPopupManager::CreateBossAppear()
{
    PopupStyle style;

    //----------------------------------------
    // ボス出現用スタイル
    //----------------------------------------

    style.color = Vector4(
        1.0f,
        0.0f,
        0.0f,
        1.0f
    );

    style.scale = 5.0f;

    style.lifeTime = 4.0f;

    style.velocityY = 10.0f;

    style.anim = PopupAnim::Shake;

    //----------------------------------------
    // 画面中央表示
    //----------------------------------------

    Create(
        L"BOSS APPEAR",
        Vector3(
            640.0f,
            360.0f,
            0.0f
        ),
        style,
        PopupLayer::Screen,
        PopupPriority::High
    );
}