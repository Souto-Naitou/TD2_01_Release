#pragma once

namespace DefaultSettings
{
    constexpr unsigned int kGameScreenWidth = 1200i32;
    constexpr unsigned int kGameScreenHeight = 800ui32;
    constexpr unsigned int kScreenWidth = 1200i32;
    constexpr unsigned int kScreenHeight = 800ui32;

    constexpr unsigned int kGameScenePosX = kScreenWidth - kGameScreenWidth;
    constexpr unsigned int kGameScenePosY = kScreenHeight - kGameScreenHeight;
}