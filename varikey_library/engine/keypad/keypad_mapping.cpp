// SPDX-FileCopyrightText: 2024 Roman Koch <koch.roman@gmail.com>
// SPDX-License-Identifier: MIT
// SPDX-FileContributor: Roman Koch <koch.roman@gmail.com>
// SPDX-FileComment: keypad keycode module
// SPDX-FileType: SOURCE

#include <cassert>

#include "tusb.h"

#include "keypad_key_code.hpp"
#include "keypad_mapping.hpp"
#include "keypad_modifiers.hpp"
#include "param_mapping.hpp"

namespace engine::keypad
{
    static code_table_t custom_value = {
        {key::Code::SINGLE, 0, 0, {0xff}},
        {key::Code::SINGLE, 0, 0, {0xff}},
        {key::Code::SINGLE, 0, 0, {0xff}},
        {key::Code::SINGLE, 0, 0, {0xff}},
        {key::Code::SINGLE, 0, 0, {0xff}},

        {key::Code::SINGLE, 0, 0, {0xff}},
        {key::Code::SINGLE, 0, 0, {0xff}},
        {key::Code::SINGLE, 0, 0, {0xff}},
        {key::Code::SINGLE, 0, 0, {0xff}},
        {key::Code::SINGLE, 0, 0, {0xff}},

        {key::Code::SINGLE, 0, 0, {0xff}},
        {key::Code::SINGLE, 0, 0, {0xff}},
        {key::Code::SINGLE, 0, 0, {0xff}},

        {key::Code::SINGLE, 0, 0, {0xff}},
        {key::Code::SINGLE, 0, 0, {0xff}},
        {key::Code::SINGLE, 0, 0, {0xff}},

        {key::Code::SINGLE, 0, 0, {0xff}},
        {key::Code::SINGLE, 0, 0, {0xff}},
        {key::Code::SINGLE, 0, 0, {0xff}},
        {key::Code::SINGLE, 0, 0, {0xff}},

        {key::Code::SINGLE, 0, 0, {0xff}},
        {key::Code::SINGLE, 0, 0, {0xff}},
        {key::Code::SINGLE, 0, 0, {0xff}},
        {key::Code::SINGLE, 0, 0, {0xff}},
    };

    static const code_table_t embedded_box = {
        {key::Code::SINGLE, 0, 0, {HID_KEY_F5}},          /* SW1 */
        {key::Code::SINGLE, 0, 0, {HID_KEY_F4}},          /* SW2 */
        {key::Code::SINGLE, 0, 0, {HID_KEY_F3}},          /* SW3 */
        {key::Code::SINGLE, 0, 0, {HID_KEY_F2}},          /* SW4 */
        {key::Code::SINGLE, 0, 0, {HID_KEY_F1}},          /* SW5 */
        {key::Code::SINGLE, 0, 0, {HID_KEY_HELP}},        /* SW6 */
        {key::Code::SINGLE, 0, 0, {HID_KEY_ESCAPE}},      /* SW7 */
        {key::Code::SINGLE, 0, 0, {HID_KEY_INSERT}},      /* SW8 */
        {key::Code::SINGLE, 0, 0, {HID_KEY_ARROW_LEFT}},  /* SW9 */
        {key::Code::SINGLE, 0, 0, {HID_KEY_ARROW_UP}},    /* SW10 */
        {key::Code::SINGLE, 0, 0, {HID_KEY_ARROW_DOWN}},  /* SW11 */
        {key::Code::SINGLE, 0, 0, {HID_KEY_ARROW_RIGHT}}, /* SW12 */
        {key::Code::SINGLE, 0, 0, {HID_KEY_SPACE}},       /* SW13 */
        {key::Code::SINGLE, 0, 0, {HID_KEY_ENTER}},       /* SW14 */
        {key::Code::SINGLE, 0, 0, {HID_KEY_HOME}},        /* SW15 */
    };

    static const code_table_t number_value = {
        {key::Code::SINGLE, 0, 0, {HID_KEY_0}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_1}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_2}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_3}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_4}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_5}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_6}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_7}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_8}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_9}},
        {key::Code::ROTATION, 0, 0, {HID_KEY_0, HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_4, HID_KEY_5, HID_KEY_6, HID_KEY_7, HID_KEY_8, HID_KEY_9, HID_KEY_PERIOD}},
        {key::Code::ROTATION, 0, 0, {HID_KEY_0, HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_4, HID_KEY_5, HID_KEY_6, HID_KEY_7, HID_KEY_8, HID_KEY_9, HID_KEY_PERIOD}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_BACKSPACE}},
    };

    static const code_table_t telephon_value = {
        {key::Code::ROTATION, 0, 0, {HID_KEY_1, HID_KEY_COMMA, HID_KEY_PERIOD}},                                                                                                                                                                                                                                                                                                                                                                   /* SW1 */
        {key::Code::ROTATION, 0, 0, {HID_KEY_2, HID_KEY_A, HID_KEY_B, HID_KEY_C}},                                                                                                                                                                                                                                                                                                                                                                 /* SW2 */
        {key::Code::ROTATION, 0, 0, {HID_KEY_3, HID_KEY_D, HID_KEY_E, HID_KEY_F}},                                                                                                                                                                                                                                                                                                                                                                 /* SW3 */
        {key::Code::ROTATION, 0, 0, {HID_KEY_4, HID_KEY_G, HID_KEY_H, HID_KEY_I}},                                                                                                                                                                                                                                                                                                                                                                 /* SW4 */
        {key::Code::ROTATION, 0, 0, {HID_KEY_5, HID_KEY_J, HID_KEY_K, HID_KEY_L}},                                                                                                                                                                                                                                                                                                                                                                 /* SW5 */
        {key::Code::ROTATION, 0, 0, {HID_KEY_6, HID_KEY_M, HID_KEY_N, HID_KEY_O}},                                                                                                                                                                                                                                                                                                                                                                 /* SW6 */
        {key::Code::ROTATION, 0, 0, {HID_KEY_7, HID_KEY_P, HID_KEY_Q, HID_KEY_R, HID_KEY_S}},                                                                                                                                                                                                                                                                                                                                                      /* SW7 */
        {key::Code::ROTATION, 0, 0, {HID_KEY_8, HID_KEY_T, HID_KEY_U, HID_KEY_V}},                                                                                                                                                                                                                                                                                                                                                                 /* SW8 */
        {key::Code::ROTATION, 0, 0, {HID_KEY_9, HID_KEY_W, HID_KEY_X, HID_KEY_Y, HID_KEY_Z}},                                                                                                                                                                                                                                                                                                                                                      /* SW9 */
        {key::Code::ROTATION, 0, 0, {HID_KEY_0, HID_KEY_SPACE, HID_KEY_CAPS_LOCK}},                                                                                                                                                                                                                                                                                                                                                                /* SW10 */
        {key::Code::ROTATION, 0, 0, {HID_KEY_0, HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_4, HID_KEY_5, HID_KEY_6, HID_KEY_7, HID_KEY_8, HID_KEY_9, HID_KEY_A, HID_KEY_B, HID_KEY_C, HID_KEY_D, HID_KEY_E, HID_KEY_F, HID_KEY_G, HID_KEY_H, HID_KEY_I, HID_KEY_J, HID_KEY_K, HID_KEY_L, HID_KEY_M, HID_KEY_N, HID_KEY_O, HID_KEY_P, HID_KEY_Q, HID_KEY_R, HID_KEY_S, HID_KEY_T, HID_KEY_U, HID_KEY_V, HID_KEY_W, HID_KEY_X, HID_KEY_Y, HID_KEY_Z}}, /* SW11 */
        {key::Code::ROTATION, 0, 0, {HID_KEY_0, HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_4, HID_KEY_5, HID_KEY_6, HID_KEY_7, HID_KEY_8, HID_KEY_9, HID_KEY_A, HID_KEY_B, HID_KEY_C, HID_KEY_D, HID_KEY_E, HID_KEY_F, HID_KEY_G, HID_KEY_H, HID_KEY_I, HID_KEY_J, HID_KEY_K, HID_KEY_L, HID_KEY_M, HID_KEY_N, HID_KEY_O, HID_KEY_P, HID_KEY_Q, HID_KEY_R, HID_KEY_S, HID_KEY_T, HID_KEY_U, HID_KEY_V, HID_KEY_W, HID_KEY_X, HID_KEY_Y, HID_KEY_Z}}, /* SW12 */
        {key::Code::ROTATION, 0, 0, {HID_KEY_BACKSPACE}},                                                                                                                                                                                                                                                                                                                                                                                          /* SW13 */
    };

    static const code_table_t functional_value = {
        {key::Code::SINGLE, 0, 0, {HID_KEY_F1}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_F2}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_F3}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_F4}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_F5}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_F6}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_F7}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_F8}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_F9}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_F10}},
        {key::Code::ROTATION, 0, 0, {HID_KEY_F1, HID_KEY_F2, HID_KEY_F3, HID_KEY_F4, HID_KEY_F5, HID_KEY_F6, HID_KEY_F7, HID_KEY_F8, HID_KEY_F9, HID_KEY_F10}},
        {key::Code::ROTATION, 0, 0, {HID_KEY_F1, HID_KEY_F2, HID_KEY_F3, HID_KEY_F4, HID_KEY_F5, HID_KEY_F6, HID_KEY_F7, HID_KEY_F8, HID_KEY_F9, HID_KEY_F10}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_ESCAPE}},
    };

    static const code_table_t multimedia_value = {
        {key::Code::SINGLE, 0, 0, {HID_KEY_ESCAPE}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_HELP}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_CUT}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_COPY}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_PASTE}},

        {key::Code::SINGLE, 0, 0, {HID_KEY_PAUSE}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_STOP}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_CLEAR}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_CANCEL}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_FIND}},

        {key::Code::SINGLE, 0, 0, {HID_KEY_VOLUME_UP}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_VOLUME_DOWN}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_MUTE}},
    };

    static const code_table_t navigation_value = {
        {key::Code::SINGLE, 0, 0, {HID_KEY_ESCAPE}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_HOME}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_END}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_INSERT}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_DELETE}},

        {key::Code::SINGLE, 0, 0, {HID_KEY_ARROW_UP}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_ARROW_DOWN}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_ARROW_LEFT}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_ARROW_RIGHT}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_SELECT}},

        {key::Code::SINGLE, 0, 0, {HID_KEY_PAGE_UP}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_PAGE_DOWN}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_ENTER}},

        {key::Code::SINGLE, 0, 0, {HID_KEY_PAGE_UP}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_PAGE_DOWN}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_ENTER}},

        {key::Code::SINGLE, 0, 0, {HID_KEY_ARROW_DOWN}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_ARROW_LEFT}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_ARROW_RIGHT}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_ARROW_UP}},

        {key::Code::SINGLE, 0, 0, {HID_KEY_ARROW_DOWN}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_ARROW_LEFT}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_ARROW_RIGHT}},
        {key::Code::SINGLE, 0, 0, {HID_KEY_ARROW_UP}},
    };

    std::map<TABLE, code_table_t> Mapping::mapping_tables = {
        {TABLE::CUSTOM, custom_value},
        {TABLE::EMBEDDED_BOX, embedded_box},
        {TABLE::FUNCTIONAL, functional_value},
        {TABLE::MULTIMEDIA, multimedia_value},
        {TABLE::NAVIGATION, navigation_value},
        {TABLE::NUMBER, number_value},
        {TABLE::TELEFON, telephon_value},
    };

    void Mapping::set_table(const TABLE _table)
    {
        table = _table;
        if (table == TABLE::CUSTOM)
        {
            size_t parameter_index = 0;
            for (auto &i : custom_value)
            {
                i.modifier.value = registry::parameter::mapping::g_register.value[parameter_index].modifier;
                i.value[0] = registry::parameter::mapping::g_register.value[parameter_index].code;

                if (parameter_index < registry::parameter::mapping::SIZE)
                    parameter_index++;
                else
                    break;
            }
        }
    }

    keypad::code_table_t &Mapping::get_table_content() const
    {
        auto it = mapping_tables.find(table);
        if (it == mapping_tables.end())
        {
            assert(false && "Mapping table not found");
            static keypad::code_table_t dummy_table;
            return dummy_table;
        }
        return it->second;
    }

    const uint8_t Mapping::get_code(const key::Identifier _identifier) const
    {
        const keypad::code_table_t &current_table = static_cast<const keypad::code_table_t>(get_table_content());
        const uint8_t key_index = static_cast<const uint8_t>(_identifier);

        const key::Code &key_item = current_table[key_index];

        return key_item.value[key_item.cursor];
    }

    void Mapping::perform(const key::Identifier _identifier)
    {
        keypad::code_table_t &current_table = get_table_content();
        const uint8_t key_index = static_cast<const uint8_t>(_identifier);

        if (key_index < current_table.size())
        {
            if (key_index == (uint8_t)key::Identifier::KEY_70 ||
                key_index == (uint8_t)key::Identifier::KEY_73)
            {
                key::Code &up_key = current_table[key_index];
                if (up_key.mode == engine::keypad::key::Code::MODE::ROTATION)
                {
                    up_key.cursor = (up_key.cursor < up_key.value.size() - 1) ? up_key.cursor + 1 : 0;
                }

                key::Code &dn_key = current_table[key_index + 1];
                if (dn_key.mode == engine::keypad::key::Code::MODE::ROTATION)
                {
                    dn_key.cursor = (dn_key.cursor < dn_key.value.size() - 1) ? dn_key.cursor + 1 : 0;
                }
            }
            else if (key_index == (uint8_t)key::Identifier::KEY_71 ||
                     key_index == (uint8_t)key::Identifier::KEY_74)
            {
                key::Code &up_key = current_table[key_index - 1];
                if (up_key.mode == engine::keypad::key::Code::MODE::ROTATION)
                {
                    up_key.cursor = (up_key.cursor > 0) ? up_key.cursor - 1 : up_key.value.size() - 1;
                }

                key::Code &dn_key = current_table[key_index];
                if (dn_key.mode == engine::keypad::key::Code::MODE::ROTATION)
                {
                    dn_key.cursor = (dn_key.cursor > 0) ? dn_key.cursor - 1 : dn_key.value.size() - 1;
                }
            }
            else
            {
                key::Code &key_item = current_table[key_index];
                if (key_item.mode == engine::keypad::key::Code::MODE::ROTATION)
                {
                    key_item.cursor = (key_item.cursor < key_item.value.size() - 1) ? key_item.cursor + 1 : 0;
                }
                else
                {
                    key_item.cursor = 0;
                }
            }
        }
    }

    void Mapping::reset(void)
    {
        auto reset_cursor = [this](engine::keypad::code_table_t &_table)
        {
            for (key::Code &i : _table)
            {
                i.cursor = 0;
            }
        };

        for (auto &pair : mapping_tables)
        {
            reset_cursor(pair.second);
        }
    }
}
