// SPDX-FileCopyrightText: 2023 Roman Koch <koch.roman@gmail.com>
// SPDX-License-Identifier: MIT
// SPDX-FileContributor: Roman Koch <koch.roman@gmail.com>
// SPDX-FileComment: Three-channel backlight implementation
// SPDX-FileType: SOURCE

#include <assert.h>
#include <stdio.h>

#include "backlight_channel_control.hpp"
#include "backlight_color.hpp"
#include "board_assembly.hpp"
#include "smartled_color.hpp"

namespace engine::backlight
{
    void ChannelControl::initialize(void)
    {
        light_chain.status = INITIALIZED;
    }

    void ChannelControl::set_program(const PROGRAM _mode, const uint64_t _delay_ms)
    {
        assert(light_chain.status == INITIALIZED);
        light_chain.next_program = _mode;
        light_chain.next_delay_ms = _delay_ms;
    }

    void ChannelControl::perform(void)
    {
        assert(light_chain.status == INITIALIZED);

        if (light_chain.program != light_chain.next_program)
        {
            static uint64_t timestamp = platform::board::assembly.soc.get_stopwatch();
            const uint64_t us_diff = platform::board::assembly.soc.get_stopwatch() - timestamp;
            if (us_diff > light_chain.next_delay_ms * 1000)
            {
                light_chain.program = light_chain.next_program;
            }
        }

        switch (light_chain.program)
        {
        case PROGRAM::MEDIUM:
            perform_step(25);
            program_switch();
            break;
        case PROGRAM::SLOW:
            perform_step(50);
            program_switch();
            break;
        case PROGRAM::TURBO:
            perform_step(5);
            program_switch();
            break;
        case PROGRAM::ALERT:
            if (light_chain.right.current == engine::backlight::ALERT_COLOR)
            {
                set_left(engine::backlight::BLACK_COLOR);
                set_right(engine::backlight::BLACK_COLOR);
            }
            else
            {
                set_left(engine::backlight::ALERT_COLOR);
                set_right(engine::backlight::ALERT_COLOR);
            }
            perform_step(BLINK_ALERT);
            break;
        case PROGRAM::MOUNT:
            if (light_chain.right.current == Settings::MOUNTED_COLOR_RIGHT)
            {
                set_left(Settings::MOUNTED_COLOR_RIGHT);
                set_right(Settings::MOUNTED_COLOR_LEFT);
            }
            else
            {
                set_left(Settings::MOUNTED_COLOR_LEFT);
                set_right(Settings::MOUNTED_COLOR_RIGHT);
            }
            perform_step(BLINK_MOUNT);
            break;
        case PROGRAM::SUSPEND:
            if (light_chain.right.current == engine::backlight::SUSPEND_COLOR)
            {
                set_left(engine::backlight::BLACK_COLOR);
                set_right(engine::backlight::BLACK_COLOR);
            }
            else
            {
                set_left(engine::backlight::SUSPEND_COLOR);
                set_right(engine::backlight::SUSPEND_COLOR);
            }
            perform_step(BLINK_SUSPEND);
            break;
        case PROGRAM::CONST:
            if (!(light_chain.left.next == light_chain.left.current &&
                  light_chain.right.next == light_chain.right.current))
            {
                perform_step(10);
            }
            break;
        case PROGRAM::OFF:
            if (!(light_chain.left.next == light_chain.left.current &&
                  light_chain.right.next == light_chain.right.current))
            {
                set_left(engine::backlight::BLACK_COLOR);
                set_right(engine::backlight::BLACK_COLOR);
                perform_step(500);
            }
            break;
        default:
            break;
        }
    }

    void ChannelControl::set_left(const uint8_t r, const uint8_t g, const uint8_t b)
    {
        assert(light_chain.status == INITIALIZED);

        light_chain.left.next.rgb.r = r;
        light_chain.left.next.rgb.g = g;
        light_chain.left.next.rgb.b = b;
        light_chain.left.value.increment = 0;
    }

    void ChannelControl::set_right(const uint8_t r, const uint8_t g, const uint8_t b)
    {
        assert(light_chain.status == INITIALIZED);

        light_chain.right.next.rgb.r = r;
        light_chain.right.next.rgb.g = g;
        light_chain.right.next.rgb.b = b;
        light_chain.right.value.increment = 0;
    }

    void ChannelControl::morph_left(const uint8_t r, const uint8_t g, const uint8_t b)
    {
        assert(light_chain.status == INITIALIZED);

        light_chain.left.next.rgb.r = r;
        light_chain.left.next.rgb.g = g;
        light_chain.left.next.rgb.b = b;
        light_chain.left.value.increment = 1;
    }

    void ChannelControl::morph_right(const uint8_t r, const uint8_t g, const uint8_t b)
    {
        assert(light_chain.status == INITIALIZED);

        light_chain.right.next.rgb.r = r;
        light_chain.right.next.rgb.g = g;
        light_chain.right.next.rgb.b = b;
        light_chain.right.value.increment = 1;
    }

    void ChannelControl::perform_step(const int _delay)
    {
        static uint64_t timestamp = platform::board::assembly.soc.get_stopwatch();
        const uint64_t current_time = platform::board::assembly.soc.get_stopwatch();
        if ((current_time - timestamp) < static_cast<uint64_t>(_delay) * 1000)
        {
            return;
        }
        timestamp = current_time;

        const int color_inc = 1;

        bool left_changed = false;
        if (light_chain.left.next != light_chain.left.current)
        {
            left_changed = true;
            if (light_chain.left.value.increment == 0)
            {
                light_chain.left.current.value[0] = light_chain.left.next.value[0];
                light_chain.left.current.value[1] = light_chain.left.next.value[1];
                light_chain.left.current.value[2] = light_chain.left.next.value[2];
            }
            else
            {
                if (light_chain.left.current.rgb.r < light_chain.left.next.rgb.r)
                {
                    light_chain.left.current.rgb.r = (light_chain.left.current.rgb.r < light_chain.left.next.rgb.r) ? light_chain.left.current.rgb.r + color_inc : light_chain.left.next.rgb.r;
                }
                else if (light_chain.left.current.rgb.r > light_chain.left.next.rgb.r)
                {
                    light_chain.left.current.rgb.r = (light_chain.left.current.rgb.r > light_chain.left.next.rgb.r) ? light_chain.left.current.rgb.r - color_inc : light_chain.left.next.rgb.r;
                }
                else
                {
                    light_chain.left.current.rgb.r = light_chain.left.next.rgb.r;
                }

                if (light_chain.left.current.rgb.g < light_chain.left.next.rgb.g)
                {
                    light_chain.left.current.rgb.g = (light_chain.left.current.rgb.g < light_chain.left.next.rgb.g) ? light_chain.left.current.rgb.g + color_inc : light_chain.left.next.rgb.g;
                }
                else if (light_chain.left.current.rgb.g > light_chain.left.next.rgb.g)
                {
                    light_chain.left.current.rgb.g = (light_chain.left.current.rgb.g > light_chain.left.next.rgb.g) ? light_chain.left.current.rgb.g - color_inc : light_chain.left.next.rgb.g;
                }
                else
                {
                    light_chain.left.current.rgb.g = light_chain.left.next.rgb.g;
                }

                if (light_chain.left.current.rgb.b < light_chain.left.next.rgb.b)
                {
                    light_chain.left.current.rgb.b = (light_chain.left.current.rgb.b < light_chain.left.next.rgb.b) ? light_chain.left.current.rgb.b + color_inc : light_chain.left.next.rgb.b;
                }
                else if (light_chain.left.current.rgb.b > light_chain.left.next.rgb.b)
                {
                    light_chain.left.current.rgb.b = (light_chain.left.current.rgb.b > light_chain.left.next.rgb.b) ? light_chain.left.current.rgb.b - color_inc : light_chain.left.next.rgb.b;
                }
                else
                {
                    light_chain.left.current.rgb.b = light_chain.left.next.rgb.b;
                }

                if (light_chain.left.current.rgb.r == light_chain.left.next.rgb.r &&
                    light_chain.left.current.rgb.g == light_chain.left.next.rgb.g &&
                    light_chain.left.current.rgb.b == light_chain.left.next.rgb.b)
                {
                    light_chain.left.value.increment = 0;
                }
            }
        }

        bool right_changed = false;
        if (light_chain.right.next != light_chain.right.current)
        {
            right_changed = true;
            if (light_chain.right.value.increment == 0)
            {
                light_chain.right.current.value[0] = light_chain.right.next.value[0];
                light_chain.right.current.value[1] = light_chain.right.next.value[1];
                light_chain.right.current.value[2] = light_chain.right.next.value[2];
            }
            else
            {
                if (light_chain.right.current.rgb.r < light_chain.right.next.rgb.r)
                {
                    light_chain.right.current.rgb.r = (light_chain.right.current.rgb.r < light_chain.right.next.rgb.r) ? light_chain.right.current.rgb.r + color_inc : light_chain.right.next.rgb.r;
                }
                else if (light_chain.right.current.rgb.r > light_chain.right.next.rgb.r)
                {
                    light_chain.right.current.rgb.r = (light_chain.right.current.rgb.r > light_chain.right.next.rgb.r) ? light_chain.right.current.rgb.r - color_inc : light_chain.right.next.rgb.r;
                }
                else
                {
                    light_chain.right.current.rgb.r = light_chain.right.next.rgb.r;
                }

                if (light_chain.right.current.rgb.g < light_chain.right.next.rgb.g)
                {
                    light_chain.right.current.rgb.g = (light_chain.right.current.rgb.g < light_chain.right.next.rgb.g) ? light_chain.right.current.rgb.g + color_inc : light_chain.right.next.rgb.g;
                }
                else if (light_chain.right.current.rgb.g > light_chain.right.next.rgb.g)
                {
                    light_chain.right.current.rgb.g = (light_chain.right.current.rgb.g > light_chain.right.next.rgb.g) ? light_chain.right.current.rgb.g - color_inc : light_chain.right.next.rgb.g;
                }
                else
                {
                    light_chain.right.current.rgb.g = light_chain.right.next.rgb.g;
                }

                if (light_chain.right.current.rgb.b < light_chain.right.next.rgb.b)
                {
                    light_chain.right.current.rgb.b = (light_chain.right.current.rgb.b < light_chain.right.next.rgb.b) ? light_chain.right.current.rgb.b + color_inc : light_chain.right.next.rgb.b;
                }
                else if (light_chain.right.current.rgb.b > light_chain.right.next.rgb.b)
                {
                    light_chain.right.current.rgb.b = (light_chain.right.current.rgb.b > light_chain.right.next.rgb.b) ? light_chain.right.current.rgb.b - color_inc : light_chain.right.next.rgb.b;
                }
                else
                {
                    light_chain.right.current.rgb.b = light_chain.right.next.rgb.b;
                }

                if (light_chain.right.current.rgb.r == light_chain.right.next.rgb.r &&
                    light_chain.right.current.rgb.g == light_chain.right.next.rgb.g &&
                    light_chain.right.current.rgb.b == light_chain.right.next.rgb.b)
                {
                    light_chain.right.value.increment = 0;
                }
            }
        }

        if (left_changed || right_changed)
        {
            set_smartled_func(light_chain.left.current, light_chain.right.current);
        }
    }

    void ChannelControl::program_switch()
    {
        static int program_id = 0;
        const int program_size = sizeof(program) / sizeof(program_t);

        if (light_chain.left.next == light_chain.left.current && light_chain.right.next == light_chain.right.current)
        {
            morph_left(program[program_id].left);
            morph_right(program[program_id].right);

            program_id = ((program_id + 1) < program_size) ? program_id + 1 : 0;
        }
    }
}
