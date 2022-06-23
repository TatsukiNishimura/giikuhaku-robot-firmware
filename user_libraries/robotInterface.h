#pragma once
#include "mbed.h"
#include <map>

namespace robohan
{
    struct Pose
    {
        float x;
        float y;
        float theta;
    };

    struct Twist
    {
        float vx;
        float vy;
        float vth;
    };

    enum class msgs
    {
        odometry = 0,
        twist = 1,
        command = 2,
        encoder = 3,
    };

    enum class dataType
    {
        Float = 0,
        Int = 1,
        Char = 2,
        Uint8 = 3,
        Bool = 4
    };

}