#pragma once

class RotateBoardManager
{
public: /// 公開メソッド
    RotateBoardManager(const RotateBoardManager&) = delete;
    RotateBoardManager& operator=(const RotateBoardManager&) = delete;
    RotateBoardManager(RotateBoardManager&&) = delete;
    RotateBoardManager& operator=(RotateBoardManager&&) = delete;

    static RotateBoardManager* GetInstance()
    {
        static RotateBoardManager instance;
        return &instance;
    }

    void SetLeaveFlag(bool _flag) { leaveFlag_ = _flag; }
    void SetIsAttack(bool _flag) { isAttack_ = _flag; }


    bool GetLeaveFlag() const { return leaveFlag_; }
    bool GetIsAttack() const { return isAttack_; }


private: /// 非公開データ
    bool leaveFlag_ = false;
    bool isAttack_ = false;

private: /// 非公開メソッド
    RotateBoardManager() = default;
    ~RotateBoardManager() = default;
}