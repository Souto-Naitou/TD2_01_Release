#pragma once

#include "Draw2D.h"
#include "Collision/Collider/Collider.h"
#include "Collision/CollisionManager/CollisionManager.h"

#include "externals/easing/Easing.h"
#include <Vector2.h>
#include <Rect2.h>

#include <vector>
#include <array>
#include <memory>


/// <summary>
/// 回転板オブジェクト
/// </summary>
class RotateBoard : public BaseObject
{
public: /// 公開メソッド

    RotateBoard();
    ~RotateBoard();


    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize();


    /// <summary>
    /// マスク設定をする
    /// </summary>
    void RunSetMask();


    /// <summary>
    /// 更新
    /// </summary>
    void Update();


    /// <summary>
    /// 描画
    /// </summary>
    void Draw();


    /// <summary>
    /// 親の頂点を設定
    /// </summary>
    /// <param name="_vertices"></param>
    void SetVertices(const std::vector<Vector2>* _vertices);


    void OnCollision(const Collider* _other);


    const Vector2* GetVertices(int _idx) const
    {
        if (_idx == 1)
            return verticesCollider1_;
        else if (_idx == 2)
            return verticesCollider2_;

        return nullptr;
    }


    const bool GetIsCorner() const { return isCorner_; }

private: /// 非公開データ

    std::vector<Vector2>                        course_         = {};
    std::array<std::pair<uint32_t, Vector2>, 2> points_         = {};
    std::unique_ptr<Easing>                     pEasingEdgeMove = nullptr;

    Collider                collider1_              = {};       // コライダー
    Collider                collider2_              = {};
    Rect2                   rect_                   = {};       // レクト
    int32_t                 padding_                = 0;        // 隙間
    bool                    isDrawCourse_           = false;    // コースを描画するかどうか
    bool                    isDrawPoints_           = false;    // 頂点を描画するかどうか
    bool                    isCorner_               = false;
    float                   offset_                 = 0.0f;     // オフセット値
    float                   t1                      = 0.0f;     // time1
    float                   t2                      = 0.0f;     // time2
    Vector2                 verticesCollider2_[2]   = {};       // コライダーにわたす頂点ベクトル
    Vector2                 verticesCollider1_[2]   = {};       // コライダーにわたす頂点ベクトル

private: /// 他オブジェクトから借りるデータ
    CollisionManager*           pCollisionManager_  = nullptr;  // コリジョンマネージャ
    const std::vector<Vector2>* parentVertices_     = nullptr;  // 親の頂点
    Draw2D*                     pDraw2D_            = nullptr;  // 2D描画

private: /// 非公開メソッド
    void UpdateCourse();    // コースを更新
    void DrawPoints();      // 頂点を描画
    void DrawCourse();      // コースを描画
    void DrawBoardLine();   // 板のラインを描画

    /// <summary>
    /// デバッグ用ウィンドウ
    /// </summary>
    void DebugWindow();
};