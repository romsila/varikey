// SPDX-FileCopyrightText: 2025 Roman Koch <koch.roman@gmail.com>
// SPDX-License-Identifier: MIT
// SPDX-FileContributor: Roman Koch <koch.roman@gmail.com>
// SPDX-FileComment: keypad keycode module
// SPDX-FileType: SOURCE

#pragma once

#include <vector>

#include "keypad_modifiers.hpp"

namespace engine::keypad::key
{
    /**
     * @brief Keycode structure
     *
     * This structure represents a keycode with its mode, cursor position,
     */
    struct Code
    {
        enum MODE
        {
            ROTATION,
            SEQUENCE,
            SINGLE,
        } mode;                 /// mode: hot to handle the keycode
        size_t cursor;          /// current position in the sequence
        MODIFIER modifier;      /// current modifier state
        std::vector<int> value; /// keycode sequence
    };
}
