#pragma once

#include <algorithm>
#undef max
#undef min

class CEasing
{
public:
    // tの値をそのまま返します(等速直線運動)
    static float Linear(float t, float maxT = 1.0f)
    {
        t /= maxT;

        t = std::clamp(t, 0.0f, 1.0f);

        return t;
    }

    // https://easings.net/ja#easeInQuad
    static float EaseInQuad(float t, float maxT = 1.0f)
    {
        t /= maxT;

        t = std::clamp(t, 0.0f, 1.0f);

        return t * t;
    }

    // https://easings.net/ja#easeOutQuad
    static float EaseOutQuad(float t, float maxT = 1.0f)
    {
        t /= maxT;

        t = std::clamp(t, 0.0f, 1.0f);

        return t * (2 - t);
    }

    // https://easings.net/ja#easeInOutQuad
    static float EaseInOutQuad(float t, float maxT = 1.0f)
    {
        t /= maxT;

        t = std::clamp(t, 0.0f, 1.0f);

        if (t < 0.5f)
        {
            return 2 * t * t;
        }
        else
        {
            return -1 + (4 - 2 * t) * t;
        }
    }

    // https://easings.net/ja#easeInCubic
    static float EaseInCubic(float t, float maxT = 1.0f)
    {
        t /= maxT;

        t = std::clamp(t, 0.0f, 1.0f);

        return t * t * t;
    }

    // https://easings.net/ja#easeOutCubic
    static float EaseOutCubic(float t, float maxT = 1.0f)
    {
        t /= maxT;

        t = std::clamp(t, 0.0f, 1.0f);

        return --t * t * t + 1;
    }

    static float EaseOutQuint(float t, float maxT = 1.0f)
    {
        t /= maxT;

        t = std::clamp(t, 0.0f, 1.0f);

        return 1 - powf(1 - t, 5);
    }

    // https://easings.net/ja#easeInOutCubic
    static float EaseInOutCubic(float t, float maxT = 1.0f)
    {
        t /= maxT;

        t = std::clamp(t, 0.0f, 1.0f);

        if (t < 0.5f)
        {
            return 4 * t * t * t;
        }
        else
        {
            return (t - 1) * (2 * (t - 1)) * (2 * (t - 1)) + 1;
        }
    }

    // https://easings.net/ja#easeOutBack
    static float EaseOutBack(float t, float maxT = 1.0f, float s = 1.70158f)
    {
        t /= maxT;
        t = std::clamp(t, 0.0f, 1.0f);
        return --t * t * ((s + 1) * t + s) + 1;
    }

    // https://easings.net/ja#easeOutBounce
    static float EaseOutBounce(float t, float maxT = 1.0f)
    {
        t /= maxT;
        t = std::clamp(t, 0.0f, 1.0f);
        const float n1 = 7.5625;
        const float d1 = 2.75;

        if (t < 1 / d1)
        {
            return n1 * t * t;
        }
        else if (t < 2 / d1) {
            return n1 * (t -= 1.5 / d1) * t + 0.75;
        }
        else if (t < 2.5 / d1) {
            return n1 * (t -= 2.25 / d1) * t + 0.9375;
        }
        else
        {
            return n1 * (t -= 2.625 / d1) * t + 0.984375;
        }
    }
};
