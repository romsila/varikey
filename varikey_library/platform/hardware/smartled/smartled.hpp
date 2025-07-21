// SPDX-FileCopyrightText: 2023 Roman Koch <koch.roman@gmail.com>
// SPDX-License-Identifier: MIT
// SPDX-FileContributor: Roman Koch <koch.roman@gmail.com>
// SPDX-FileComment: Hardware smartled ws2812 functionality
// SPDX-FileType: SOURCE

#pragma once

#include <cstdint>
#include <limits>

#include <hardware/clocks.h>
#include <hardware/pio.h>
#include <pico/assert.h>
#include <pico/stdlib.h>
#include <pico/time.h>

#include "smartled_color.hpp"
#include "ws2812.pio.h"

namespace platform::hardware::smartled
{
    struct SmartLed
    {
        /**
         * @brief The index of the state machine in the PIO.
         *
         * Values: 0 or 1, depending on the PIO configuration.
         */
        static constexpr int UNDEFINED_INDEX = std::numeric_limits<int>::max();

        SmartLed() = delete;
        SmartLed(const SmartLed &) = delete;
        SmartLed &operator=(const SmartLed &) = delete;
        SmartLed(SmartLed &&) = delete;
        SmartLed &operator=(SmartLed &&) = delete;
        explicit SmartLed(const int sm_index, const int pio_pin) : state_machine(sm_index), pio_pin(pio_pin) {}

        template <typename Mapper>
        void initialize()
        {
            PIO pio = pio0;
            uint offset = pio_add_program(pio, &ws2812_program);
            ws2812_program_init(pio, state_machine, offset, pio_pin, 800000, false);
            pio_sm_clear_fifos(pio, state_machine);

            static const size_t LED_COUNT = Mapper::mapping.size();
            const Color colors[LED_COUNT] = {0};
            set_led_sequence<Mapper>(colors);
            sleep_ms(500);
        }

        template <typename Mapper>
        void shutdown()
        {
            static const size_t LED_COUNT = Mapper::mapping.size();
            const Color colors[LED_COUNT] = {0};
            set_led_sequence<Mapper>(colors);
            sleep_ms(500);
        }

        void put_pixel(uint8_t r, uint8_t g, uint8_t b);

        template <typename Mapper>
        void set_smartled(const Color &_left, const Color &_right)
        {
            constexpr float brightness_back = 0.25f;

            constexpr size_t led_count = Mapper::mapping.size();
            static_assert(led_count >= 2, "Need at least two LEDs.");

            Color led_colors[led_count];

            for (size_t i = 0; i < led_count; ++i)
            {
                float t = static_cast<float>(i) / (led_count - 1); // t ∈ [0, 1]

                led_colors[i].rgb.r = static_cast<uint8_t>((_left.rgb.r + t * (_right.rgb.r - _left.rgb.r)) * brightness_back);
                led_colors[i].rgb.g = static_cast<uint8_t>((_left.rgb.g + t * (_right.rgb.g - _left.rgb.g)) * brightness_back);
                led_colors[i].rgb.b = static_cast<uint8_t>((_left.rgb.b + t * (_right.rgb.b - _left.rgb.b)) * brightness_back);
            }
            set_led_sequence<Mapper>(led_colors);
        }

        template <typename Mapper>
        void set_led_sequence(const Color (&colors)[Mapper::mapping.size()])
        {
            // extends template with:  std::size_t N
            // static_assert(N == Mapper::mapping.size(), "Color array size does not match LED mapper size.");

            for (std::size_t i = 0; i < Mapper::mapping.size(); ++i)
            {
                const std::size_t index = static_cast<std::size_t>(Mapper::mapping[i]);
                const auto &color = colors[index];
                put_pixel(color.rgb.r, color.rgb.g, color.rgb.b);
            }
        }

    private:
        int state_machine{UNDEFINED_INDEX};
        int pio_pin{0};
    };
}
