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
     * @brief Key identifier enumeration.
     *
     * This enumeration defines the identifiers for various keys on the keypad.
     * Each keypad driver should create events only with these identifiers.
     *
     * Note:
     * This table is not complete and will be **extended** in the future.
     * DEFINED KEY VALUES SHOULD'NT BE CHANGED!
     * Undefined keys should be set to `UNDEFINED.
     */
    enum class Identifier : uint8_t
    {
        KEY_01 = 0x00, /* SW1 */
        KEY_02 = 0x01, /* SW2 */
        KEY_03 = 0x02, /* SW3 */
        KEY_04 = 0x03, /* SW4 */
        KEY_05 = 0x04, /* SW5 */
        KEY_06 = 0x05, /* SW6 */
        KEY_07 = 0x06, /* SW7 */
        KEY_08 = 0x07, /* SW8 */
        KEY_09 = 0x08, /* SW9 */
        KEY_10 = 0x09, /* SW10 */
        KEY_11 = 0x0a, /* SW11 */
        KEY_12 = 0x0b, /* SW12 */
        KEY_13 = 0x0c, /* SW13 */
        KEY_14 = 0x0d, /* SW14 */
        KEY_15 = 0x0e, /* SW15 */
        KEY_70 = 0x7a, /* ROTARY1_LEFT */
        KEY_71 = 0x7b, /* ROTARY1_RIGHT */
        KEY_72 = 0x7c, /* ROTARY1_PUSH */
        KEY_73 = 0x7d, /* ROTARY2_LEFT */
        KEY_74 = 0x7e, /* ROTARY2_RIGHT */
        KEY_75 = 0x7f, /* ROTARY2_PUSH */
        KEY_80 = 0x80, /* JOYSTICK1_LEFT */
        KEY_81 = 0x81, /* JOYSTICK1_RIGHT */
        KEY_82 = 0x82, /* JOYSTICK1_UP */
        KEY_83 = 0x83, /* JOYSTICK1_DOWN */
        KEY_84 = 0x84, /* JOYSTICK2_LEFT */
        KEY_85 = 0x85, /* JOYSTICK2_RIGHT */
        KEY_86 = 0x86, /* JOYSTICK2_UP */
        KEY_87 = 0x87, /* JOYSTICK2_DOWN */
        KEY_88 = 0x8a, /* JOYSTICK1_PUSH */
        KEY_89 = 0x8b, /* JOYSTICK2_PUSH */
        UNDEFINED = 0xff,
    };
}
