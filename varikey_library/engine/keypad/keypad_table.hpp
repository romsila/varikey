// SPDX-FileCopyrightText: 2025 Roman Koch <koch.roman@gmail.com>
// SPDX-License-Identifier: MIT
// SPDX-FileContributor: Roman Koch <koch.roman@gmail.com>
// SPDX-FileComment: keypad keycode module
// SPDX-FileType: SOURCE

#pragma once

#include <cstdint>
#include <vector>

#include "keypad_key_code.hpp"

namespace engine::keypad
{
    enum class TABLE : uint8_t
    {
        CUSTOM = 0x05,
        EMBEDDED_BOX = 0x06,
        FUNCTIONAL = 0x01,
        MULTIMEDIA = 0x04,
        NAVIGATION = 0x02,
        NUMBER = 0x00,
        TELEFON = 0x03,
        UNDEFINED = 0xff,
    };
    typedef std::vector<key::Code> code_table_t;
}
