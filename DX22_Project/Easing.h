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

        return t * (2.0f - t);
    }

    // https://easings.net/ja#easeInOutQuad
    static float EaseInOutQuad(float t, float maxT = 1.0f)
    {
        t /= maxT;

        t = std::clamp(t, 0.0f, 1.0f);

        if (t < 0.5f)
        {
            return 2.0f * t * t;
        }
        else
        {
            return -1.0f + (4.0f - 2.0f * t) * t;
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

        return --t * t * t + 1.0f;
    }

    static float EaseOutQuint(float t, float maxT = 1.0f)
    {
        t /= maxT;

        t = std::clamp(t, 0.0f, 1.0f);

        return 1.0f - powf(1.0f - t, 5.0f);
    }

    // https://easings.net/ja#easeInOutCubic
    static float EaseInOutCubic(float t, float maxT = 1.0f)
    {
        t /= maxT;

        t = std::clamp(t, 0.0f, 1.0f);

        if (t < 0.5f)
        {
            return 4.0f * t * t * t;
        }
        else
        {
            return (t - 1.0f) * (2.0f * (t - 1.0f)) * (2.0f * (t - 1.0f)) + 1.0f;
        }
    }

    // https://easings.net/ja#easeInBack
    static float EaseInBack(float t, float maxT = 1.0f, float s = 1.70158f)
    {
        t /= maxT;
        t = std::clamp(t, 0.0f, 1.0f);
        const float c3 = s + 1;
        return c3 * t * t * t - s * t * t;
    }

    // https://easings.net/ja#easeOutBack
    static float EaseOutBack(float t, float maxT = 1.0f, float s = 1.70158f)
    {
        t /= maxT;
        t = std::clamp(t, 0.0f, 1.0f);
        return --t * t * ((s + 1.0f) * t + s) + 1.0f;
    }

    // https://easings.net/ja#easeOutBounce
    static float EaseOutBounce(float t, float maxT = 1.0f)
    {
        t /= maxT;
        t = std::clamp(t, 0.0f, 1.0f);
        const float n1 = 7.5625f;
        const float d1 = 2.75f;

        if (t < 1 / d1)
        {
            return n1 * t * t;
        }
        else if (t < 2 / d1) {
            return n1 * (t -= 1.5f / d1) * t + 0.75f;
        }
        else if (t < 2.5 / d1) {
            return n1 * (t -= 2.25f / d1) * t + 0.9375f;
        }
        else
        {
            return n1 * (t -= 2.625f / d1) * t + 0.984375f;
        }
    }
};
