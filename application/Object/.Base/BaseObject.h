#pragma once
#include <Vector2.h>
#include <string>

class BaseObject
{
public:

    /// <summary>
    /// 更新
    /// </summary>
    virtual void Initialize() {};

    /// <summary>
    /// 更新
    /// </summary>
    virtual void Update() {};

    /// <summary>
    /// 描画
    /// </summary>
    virtual void Draw() {};

    /// <summary>
    /// デバッグ用ウィンドウ
    /// </summary>
    virtual void DebugWindow();

    /// <summary>
    /// ワールド座標を取得
    /// </summary>
    /// <returns></returns>
    inline  Vector2     GetWorldPosition()  const
    {
        return position_;
    }

    /// <summary>
    /// ワールド座標を設定
    /// </summary>
    /// <param name="_position"></param>
    inline  void        SetWorldPosition(const Vector2& _position)
    {
        position_ = _position;
    }

    /// <summary>
    /// HPを取得
    /// </summary>
    /// <returns></returns>
    inline  uint32_t    GetHP()             const
    {
        return hp_;
    }

    /// <summary>
    /// 死亡フラグを取得
    /// </summary>
    /// <returns></returns>
    inline  bool        GetIsDead()         const
    {
        return isDead_;
    }

protected:

    std::string     objectID_       = {};       // オブジェクト識別子
    Vector2         position_       = {};       // ワールド座標
    Vector2         velocity_       = {};
    Vector2         acceleration_   = {};
    bool            isDead_         = false;    // 死亡フラグ
    int32_t         hp_             = 0u;       // HP
};