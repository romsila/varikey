// SPDX-FileCopyrightText: 2023 Roman Koch <koch.roman@gmail.com>
// SPDX-License-Identifier: MIT
// SPDX-FileContributor: Roman Koch <koch.roman@gmail.com>
// SPDX-FileComment: Hardware smartled ws2812 functionality
// SPDX-FileType: SOURCE

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "macros.hpp"
#include "smartled.hpp"
#include "smartled_type.hpp"

namespace platform::hardware::smartled
{
    void SmartLed::put_pixel(uint8_t r, uint8_t g, uint8_t b)
    {
        assert(state_machine != UNDEFINED_INDEX && "State machine index must be defined.");

        uint32_t color = ((uint32_t)(r) << 8) |
                         ((uint32_t)(g) << 16) |
                         (uint32_t)(b);
        pio_sm_put_blocking(pio0, state_machine, color << 8u);
    }
}
