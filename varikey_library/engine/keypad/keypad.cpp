// SPDX-FileCopyrightText: 2023 Roman Koch <koch.roman@gmail.com>
// SPDX-License-Identifier: MIT
// SPDX-FileContributor: Roman Koch <koch.roman@gmail.com>
// SPDX-FileComment: keypad module
// SPDX-FileType: SOURCE

#include "keypad.hpp"
#include "backlight.hpp"
#include "display.hpp"
#include "engine_event_handler.hpp"
#include "hid_handler.hpp"
#include "hid_report.hpp"
#include "macros.hpp"
#include "param_serial_number.hpp"
#include "payload_keypad.hpp"
#include "revision.h"
#include "usb_descriptors.hpp"

namespace
{
    static engine::keypad::Mapping keypad_code_table;
}

namespace engine::keypad
{
    extern void switch_key(const key::Identifier _identifier, const key::State _state)
    {
        engine::handler::push_key_event(_identifier, _state);
        if (_state == key::State::RELEASE)
        {
            engine::keypad::perform(_identifier);
        }
    }

    extern void press_key(const key::Identifier _identifier)
    {
        switch_key(_identifier, key::State::PRESS);
    }

    extern void release_key(const key::Identifier _identifier)
    {
        switch_key(_identifier, key::State::RELEASE);
    }

    extern void set_mapping(const TABLE _table)
    {
        keypad_code_table.set_table(_table);
    }

    extern const TABLE get_mapping(void)
    {
        return keypad_code_table.get_table();
    }

    extern void enable_nums(const bool _enable)
    {
        static TABLE last_mode = keypad_code_table.get_table();
        if (_enable)
        {
            last_mode = keypad_code_table.get_table();
            keypad_code_table.set_table(TABLE::NUMBER);
        }
        else
        {
            keypad_code_table.set_table(last_mode);
        }
    }

    extern void enable_cups(const bool _enable)
    {
        enable_shift(_enable);
    }

    extern void enable_scroll(const bool _enable)
    {
    }

    extern void perform(const key::Identifier _identifier)
    {
        keypad_code_table.perform(_identifier);
    }

    extern void reset(void)
    {
        keypad_code_table.reset();
    }

    extern const uint8_t id2int(const key::Identifier _identifier)
    {
        return keypad_code_table.get_code(_identifier);
    }

    extern const key::Identifier int2id(const uint8_t _value)
    {
        if (_value >= 0x00 && _value <= 0x17)
        {
            return static_cast<const key::Identifier>(_value);
        }
        return key::Identifier::UNDEFINED;
    }
}
