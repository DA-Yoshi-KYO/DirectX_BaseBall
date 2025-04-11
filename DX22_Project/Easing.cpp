#include "Easing.h"
#include <algorithm>

float easeOutBack(float t, float tMax, float origin, float end, float overshoot)
{
    t = t / tMax;
    if (t > 1.0f)
    {
        return end;
    }
    t -= 1.0f;
    constexpr float fCalc1 = 7.5625f;
    constexpr float fCalc2 = 2.75f;
    float fValue = end - origin;

    return (t * t * ((overshoot + 1) * t + overshoot) + 1.0f) * fValue + origin;
}

float easeOutBounce(float t, float tMax, float origin, float end)
{
    t = t / tMax;
    if (t > 1.0f)
    {
        return end;
    }

    constexpr float fCalc1 = 7.5625f;
    constexpr float fCalc2 = 2.75f;
    float fValue = end - origin;

    if (t < 1 / fCalc2)
    {
        return (fCalc1 * t * t) * fValue + origin;
    }
    else if (t < 2 / fCalc2)
    {
        return (fCalc1 * (t -= 1.5 / fCalc2) * t + 0.75) * fValue + origin;
    }
    else if (t < 2.5 / fCalc2)
    {
        return (fCalc1 * (t -= 2.25 / fCalc2) * t + 0.9375) * fValue + origin;
    }
    else
    {
        return (fCalc1 * (t -= 2.625 / fCalc2) * t + 0.984375) * fValue + origin;
    }
}
