// SPDX-FileCopyrightText: 2024 Roman Koch <koch.roman@gmail.com>
// SPDX-License-Identifier: MIT
// SPDX-FileContributor: Roman Koch <koch.roman@gmail.com>
// SPDX-FileComment: keypad keycode module
// SPDX-FileType: SOURCE

#pragma once

#include <cstdint>
#include <cstdlib>
#include <map>
#include <vector>

#include "engine_variant.hpp"
#include "keypad_key_identifier.hpp"
#include "keypad_modifiers.hpp"
#include "keypad_table.hpp"

namespace engine::keypad
{
    struct Mapping
    {
        static const TABLE DEFAULT_MAPPING_TABLE = engine::Variant::DEFAULT_MAPPING_TABLE;
        static const uint16_t DEFAULT_CLICK_MS{engine::Variant::DEFAULT_CLICK_MS};
        static const uint16_t DEFAULT_PUSH_MS{engine::Variant::DEFAULT_PUSH_MS};

        void set_table(const TABLE _table);
        const TABLE get_table() const { return table; }

        const uint8_t get_code(const key::Identifier) const;

        void perform(const key::Identifier);
        void reset(void);

    private:
        TABLE table{DEFAULT_MAPPING_TABLE};

        static std::map<TABLE, code_table_t> mapping_tables;

        keypad::code_table_t &get_table_content(void) const;
    };

    template <typename T, typename U>
    U transfer_state(const T _state)
    {
        switch (_state)
        {
        case T::CLICK:
            return U::CLICK;
        case T::PUSH:
            return U::PUSH;
        case T::PRESS:
            return U::PRESS;
        case T::RELEASE:
            return U::RELEASE;
        default:
            return U::UNDEFINED;
        }
        return U::UNDEFINED;
    };
}
