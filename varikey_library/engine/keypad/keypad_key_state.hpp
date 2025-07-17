// SPDX-FileCopyrightText: 2025 Roman Koch <koch.roman@gmail.com>
// SPDX-License-Identifier: MIT
// SPDX-FileContributor: Roman Koch <koch.roman@gmail.com>
// SPDX-FileComment: keypad keycode module
// SPDX-FileType: SOURCE

#pragma once

#include <cstdint>

namespace engine::keypad::key
{
    /**
     * @brief Key state enumeration.
     * 
     * This enumeration defines the various states a key can be in.
     */
    enum class State : uint8_t
    {
        CLICK = 0x00,
        PRESS = 0x01,
        PUSH = 0x02,
        RELEASE = 0x03,

        UNDEFINED = 0xff,
    };
}
