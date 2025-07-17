// SPDX-FileCopyrightText: 2024 Roman Koch <koch.roman@gmail.com>
// SPDX-License-Identifier: MIT
// SPDX-FileContributor: Roman Koch <koch.roman@gmail.com>
// SPDX-FileComment: Keymatrix chip keymatrix handler adp5585 functionality
// SPDX-FileType: SOURCE

#include "keymatrix_handler_adp5585.hpp"
#include "keypad.hpp"

namespace platform::hardware
{
    struct keycode_item_t
    {
        engine::keypad::key::State state;
        TRIGGER trigger;
    };

    keycode_item_t stack[4];

    void KeymatrixHandlerAdp5585::push_wheel_turn(const TRIGGER _trigger, const engine::keypad::key::State _state)
    {
        if (_trigger != stack[3].trigger)
        {
            for (uint8_t stack_index = 0; stack_index < 3; ++stack_index)
            {
                stack[stack_index].state = stack[stack_index + 1].state;
                stack[stack_index].trigger = stack[stack_index + 1].trigger;
            }
        }

        stack[3].trigger = _trigger;
        stack[3].state = _state;

        auto cleanup_stack = [&]()
        {
            stack[0].state = engine::keypad::key::State::UNDEFINED;
            stack[1].state = engine::keypad::key::State::UNDEFINED;
            stack[2].state = engine::keypad::key::State::UNDEFINED;
            stack[3].state = engine::keypad::key::State::UNDEFINED;
            stack[0].trigger = TRIGGER::UNDEFINED;
            stack[1].trigger = TRIGGER::UNDEFINED;
            stack[2].trigger = TRIGGER::UNDEFINED;
            stack[3].trigger = TRIGGER::UNDEFINED;
        };

        auto check_state = [&]()
        {
            return (stack[0].state == engine::keypad::key::State::PRESS &&
                    stack[1].state == engine::keypad::key::State::PRESS &&
                    stack[2].state == engine::keypad::key::State::RELEASE &&
                    stack[3].state == engine::keypad::key::State::RELEASE)
                       ? true
                       : false;
        };

        if (stack[0].trigger == TRIGGER::UP &&
            stack[1].trigger == TRIGGER::DN &&
            stack[2].trigger == TRIGGER::UP &&
            stack[3].trigger == TRIGGER::DN &&
            check_state())
        {
            cleanup_stack();
            engine::keypad::switch_key(engine::keypad::key::Identifier::KEY_70, engine::keypad::key::State::PRESS);
            engine::keypad::switch_key(engine::keypad::key::Identifier::KEY_70, engine::keypad::key::State::RELEASE);
        }
        else if (stack[0].trigger == TRIGGER::DN &&
                 stack[1].trigger == TRIGGER::UP &&
                 stack[2].trigger == TRIGGER::DN &&
                 stack[3].trigger == TRIGGER::UP &&
                 check_state())
        {
            cleanup_stack();
            engine::keypad::switch_key(engine::keypad::key::Identifier::KEY_71, engine::keypad::key::State::PRESS);
            engine::keypad::switch_key(engine::keypad::key::Identifier::KEY_71, engine::keypad::key::State::RELEASE);
        }
    }
}
