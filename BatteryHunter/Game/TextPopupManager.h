#pragma once

#include "TextPopup.h"

/// <summary>
/// テキストポップアップ管理クラス
///
/// 固定長配列 + リングバッファ方式
///
/// ・new/deleteを使用しない
/// ・生成コストが一定
/// ・大量表示に強い
/// </summary>
class TextPopupManager : public IGameObject
{
public:

    //----------------------------------------
    // 定数
    //----------------------------------------

    /// 最大ポップアップ数
    static const int MAX_POPUP = 100;

public:

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="deltaTime">経過時間</param>
    void Update(float deltaTime);

    /// <summary>
    /// 描画処理
    /// </summary>
    void Draw(RenderContext& rc);

    //----------------------------------------
    // 汎用生成
    //----------------------------------------

    /// <summary>
    /// ポップアップ生成
    /// </summary>
    /// <param name="text">表示文字列</param>
    /// <param name="position">表示位置</param>
    /// <param name="style">表示スタイル</param>
    /// <param name="layer">描画レイヤー</param>
    /// <param name="priority">優先度</param>
    void Create(
        const wchar_t* text,
        const Vector3& position,
        const PopupStyle& style,
        PopupLayer layer,
        PopupPriority priority
    );

    //----------------------------------------
    // 専用生成関数
    //----------------------------------------

    /// <summary>
    /// ダメージ表示
    /// </summary>
    void CreateDamage(
        int damage,
        const Vector3& position
    );

    /// <summary>
    /// クリティカル表示
    /// </summary>
    void CreateCritical(
        int damage,
        const Vector3& position
    );

    /// <summary>
    /// 回復表示
    /// </summary>
    void CreateHeal(
        int heal,
        const Vector3& position
    );

    /// <summary>
    /// 経験値取得表示
    /// </summary>
    void CreateExp(
        int exp,
        const Vector3& position
    );

    /// <summary>
    /// ゴールド取得表示
    /// </summary>
    void CreateGold(
        int gold,
        const Vector3& position
    );

    /// <summary>
    /// レベルアップ表示
    /// </summary>
    void CreateLevelUp(
        const Vector3& position
    );

    /// <summary>
    /// Wave開始表示
    /// </summary>
    void CreateWaveStart();

    /// <summary>
    /// ボス出現表示
    /// </summary>
    void CreateBossAppear();

private:

    //----------------------------------------
    // ポップアッププール
    //----------------------------------------

    /// 固定長配列
    TextPopup m_popups[MAX_POPUP];

    //----------------------------------------
    // リングバッファ用インデックス
    //----------------------------------------

    /// 次に使用する要素番号
    int m_nextIndex = 0;
};