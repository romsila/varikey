#pragma once

#include "smartled_color.hpp"
#include "smartled_mapper.hpp"

namespace platform::hardware::smartled
{
    template <typename Mapper>
    struct Layer
    {
        enum class BlendMode
        {
            IGNORE = 0,
            ADD,
            SUBSTRACT,
            REPLACE,
            UNDEFINED = 0xff
        };
        struct Item
        {
            Color color;
            BlendMode mode;
            float opaque; // 0.0-1.0
        };

        static constexpr size_t MAX_LED_COUNT = Mapper::mapping.size();
        Item buffer[MAX_LED_COUNT];

        Layer() { clear(); }

        void clear(BlendMode mode = BlendMode::IGNORE)
        {
            for (auto &item : buffer)
            {
                item.color = Color{0, 0, 0};
                item.mode = mode;
                item.opaque = 1.0f;
            }
        }

        void set_pixel(size_t index, const Color &color, BlendMode mode = BlendMode::REPLACE, float opaque = 1.0f)
        {
            if (index >= MAX_LED_COUNT)
                return;
            buffer[index] = {color, mode, opaque};
        }

        void fade(float factor)
        {
            for (auto &item : buffer)
            {
                item.color.rgb.r = static_cast<uint8_t>(item.color.rgb.r * factor);
                item.color.rgb.g = static_cast<uint8_t>(item.color.rgb.g * factor);
                item.color.rgb.b = static_cast<uint8_t>(item.color.rgb.b * factor);
            }
        }
    };
}
