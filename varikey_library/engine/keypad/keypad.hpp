// SPDX-FileCopyrightText: 2023 Roman Koch <koch.roman@gmail.com>
// SPDX-License-Identifier: MIT
// SPDX-FileContributor: Roman Koch <koch.roman@gmail.com>
// SPDX-FileComment: keypad module
// SPDX-FileType: SOURCE

#pragma once

#include "keypad_key_state.hpp"
#include "keypad_mapping.hpp"
#include "keypad_modifiers.hpp"

namespace engine::keypad
{
    extern void switch_key(const key::Identifier, const key::State);
    extern void press_key(const key::Identifier);
    extern void release_key(const key::Identifier);

    extern void set_mapping(const TABLE);
    extern const TABLE get_mapping(void);

    extern void enable_nums(const bool);
    extern void enable_cups(const bool);
    extern void enable_scroll(const bool);

    extern void perform(const key::Identifier);
    extern void reset(void);

    extern const uint8_t id2int(const key::Identifier);
    extern const key::Identifier int2id(const uint8_t);
}
