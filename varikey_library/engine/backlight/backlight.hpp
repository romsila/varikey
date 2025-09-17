// SPDX-FileCopyrightText: 2023 Roman Koch <koch.roman@gmail.com>
// SPDX-License-Identifier: MIT
// SPDX-FileContributor: Roman Koch <koch.roman@gmail.com>
// SPDX-FileComment: Three-channel backlight interface
// SPDX-FileType: SOURCE

#pragma once

#include <stdint.h>

#include "backlight_channel.hpp"
#include "backlight_channel_control.hpp"
#include "backlight_program.hpp"
#include "backlight_settings.hpp"
#include "smartled_color.hpp"

namespace engine::backlight
{
    using Color = platform::hardware::smartled::Color;

    extern void initialize(void);
    extern void set_program(const PROGRAM, const CHANNEL, const uint64_t _delay_ms);
    extern void set_program(const PROGRAM, const CHANNEL, const Color &, const Color &, const uint64_t _delay_ms);
    extern void perform(void);
}
