// SPDX-FileCopyrightText: 2023 Roman Koch <koch.roman@gmail.com>
// SPDX-License-Identifier: MIT
// SPDX-FileContributor: Roman Koch <koch.roman@gmail.com>
// SPDX-FileComment: Three-channel backlight implementation
// SPDX-FileType: SOURCE

#include <assert.h>
#include <stdio.h>

#include "backlight.hpp"
#include "backlight_color.hpp"
#include "board_assembly.hpp"
#include "smartled_color.hpp"
#include "smartled_type.hpp"

namespace
{
    engine::backlight::ChannelControl front_channel_control{
        [](const engine::backlight::Color &l, const engine::backlight::Color &r)
        {
            platform::board::assembly.frontlight.set_led_chain(l, r);
        }};

    engine::backlight::ChannelControl back_channel_control{
        [](const engine::backlight::Color &l, const engine::backlight::Color &r)
        {
            platform::board::assembly.backlight.set_led_chain(l, r);
        }};
}

namespace engine::backlight
{
    extern void initialize(void)
    {
        front_channel_control.initialize();
        back_channel_control.initialize();
    }

    extern void set_program(const PROGRAM _program, const CHANNEL _channel, const uint64_t _delay_ms)
    {
        assert(_channel != CHANNEL::UNDEFINED);

        if (_channel == CHANNEL::TOP_LIGHT)
        {
            front_channel_control.set_program(_program, _delay_ms);
        }
        else if (_channel == CHANNEL::BOTTOM_LIGHT)
        {
            back_channel_control.set_program(_program, _delay_ms);
        }
        else
        {
            assert(false && "Invalid channel specified");
        }
    }

    extern void set_program(const PROGRAM _program, const CHANNEL _channel, const Color &_left, const Color &_right, const uint64_t _delay_ms)
    {
        assert(_channel != CHANNEL::UNDEFINED);

        if (_channel == CHANNEL::TOP_LIGHT)
        {
            front_channel_control.set_left(_left);
            front_channel_control.set_right(_right);
            front_channel_control.set_program(_program, _delay_ms);
        }
        else if (_channel == CHANNEL::BOTTOM_LIGHT)
        {
            back_channel_control.set_left(_left);
            back_channel_control.set_right(_right);
            back_channel_control.set_program(_program, _delay_ms);
        }
        else
        {
            assert(false && "Invalid channel specified");
        }
    }

    extern void perform(void)
    {
        front_channel_control.perform();
        back_channel_control.perform();
    }
}
