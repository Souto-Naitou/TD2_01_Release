#pragma once

#include "Object/.Base/BaseObject.h"
#include "Collision/Collider/Collider.h"
#include <Vector2.h>
#include <Vector4.h>

class Enemy : public BaseObject
{
public:

    enum class BounceTarget
    {
        Enemy,
        Player,
        RotateBoard,
    };

    enum class State
    {
        Normal,
        Suck
    };


    Enemy() = default;
    ~Enemy();

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="idx">数多のEnemy内で一意の数字 (リストのサイズを渡すと楽)</param>
    void                            Initialize(std::string _idx);


    /// <summary>
    /// 更新
    /// </summary>
    void                            Update();


    /// <summary>
    /// 描画
    /// </summary>
    void                            Draw();


public: // Getter ===============

    /// <summary>
    /// コライダーを取得
    /// </summary>
    /// <returns>コライダーのポインタ</returns>
    inline      const Collider*     GetCollider() const
    {
        return &collider_;
    }


    /// <summary>
    /// プレイヤーによる攻撃を受けた後かどうか
    /// </summary>
    /// <returns>フラグ</returns>
    inline      const bool          GetIsBounce() const
    {
        return isBounce_;
    }


    /// <summary>
    /// すでに巣壁とぶつかったかどうか
    /// </summary>
    /// <returns>フラグ</returns>
    inline      const bool          GetIsCollidedNest() const
    {
        return isCollidedNest;
    }

public: // Setter ===============

    /// <summary>
    /// ターゲットにする座標を指定
    /// </summary>
    /// <param name="_target">座標</param>
    inline      void                SetTargetPosition(const Vector2& _target)
    {
        positionTarget_ = _target;
    }


    /// <summary>
    /// ラグ軽減を行うか
    /// </summary>
    /// <param name="_flag">フラグ</param>
    void                            SetEnableLighter(bool _flag);


    /// <summary>
    /// 反発力を設定
    /// </summary>
    /// <param name="_target">誰とあたったときか</param>
    /// <param name="_power">反発力</param>
    static      void                SetBouncePower(BounceTarget _target, float _power);


    /// <summary>
    /// 死亡フラグを設定
    /// </summary>
    /// <param name="_flag">フラグ</param>
    void                            SetIsDead(bool _flag) { isDead_ = _flag; }


    /// <summary>
    /// すでに巣壁とぶつかったかどうか
    /// </summary>
    /// <returns>フラグ</returns>
    inline      void                SetIsCollidedNest(bool _flag) { isCollidedNest = _flag; }


    /// <summary>
    /// ステートを変更
    /// </summary>
    /// <param name="_state">ステート</param>
    inline      void                SetState(State _state) { state_ = _state; }


    /// <summary>
    /// ステートを変更
    /// </summary>
    /// <param name="_state">ステート</param>
    /// <param name="_val">値</param>
                void                SetState(State _state, float _val);



public: // Others ===============

    /// <summary>
    /// 衝突時コールバック関数
    /// </summary>
    /// <param name="_other">衝突した相手のコライダー</param>
    void                            OnCollision(const Collider* _other);


    /// <summary>
    /// 衝突時コールバック関数
    /// </summary>
    /// <param name="_other">衝突</param>
    void                            OnCollisionTrigger(const Collider* _other);


    void                            RunSetMask();


private: /// 非公開データ
    Collider                        collider_                   = {};           // コライダー
    Vector2                         vertices_[3]                = {};           // 三角形の頂点

    std::string                     idx_                        = {};           // index (Enemy内の一意の数字)

    float                           suctionPower_               = 0.0f;
    State                           state_                      = State::Normal;
    Vector4                         color_                      = {};           // 色
    Vector2                         positionTarget_             = {};           // ターゲットのベクトル (原点中心)
    Vector2                         distanceToTarget            = {};           // ターゲットまでのベクトル
    Vector2                         ellipseAB_                  = {};           // 楕円を構成するAとB
    Vector2                         velocity_move               = {};           // 移動用（苦肉の策）
    float                           rotation_                   = 0.0f;         // 回転角
    float                           radius_                     = 0.0f;         // 半径
    float                           moveSpeed_                  = 0.0f;         // 移動スピード
    float                           moveSpeed_sucked_           = 0.0f;         // 吸い込み時に加算するスピード(最大値)
    float                           collisionCooldown_          = 0.0f;         // 衝突判定クールダウン
    const float                     collisionCooldownDuration_  = 0.5f;         // 衝突判定クールダウン感覚

    float                           bouncePowerMax_             = 14.0f;        // 衝突時のぶっとび速度
    bool                            isBounce_                   = false;        // 跳ね返りフラグ
    bool                            isCollidedNest              = false;        // ネストに攻撃したかどうか

    bool                            hasCollided_                = false;        // 衝突フラグ
    int32_t                         outScreenPadding_           = 0ui32;        // スクリーン外判定を行う範囲の余白
    bool                            isOutOfScreen_              = false;        // スクリーン外かどうか

    /// すたてぃっくなめんば～
    static  float                   bouncePower_enemy_;                         // Enemy同士の反発力
    static  float                   bouncePower_rotateBoard_;                   // RotateBoardとの反発力
    static  float                   bouncePower_nestWall_;                      // NestWallとの反発力

    uint32_t                        hitWallSH_                      = 0xFFFFFFFF;   // ヒットサウンドのSH
    uint32_t                        hitRotateBoardSH_               = 0xFFFFFFFF;   // ヒットサウンドのSH


private: /// シングルトン インスタンスのポインタ
    CollisionManager*               pCollisionManager_;

private: /// 非公開メソッド
    void DebugWindow();
};