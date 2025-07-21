// SPDX-FileCopyrightText: 2023 Roman Koch <koch.roman@gmail.com>
// SPDX-License-Identifier: MIT
// SPDX-FileContributor: Roman Koch <koch.roman@gmail.com>
// SPDX-FileComment: Hardware smartled varikey functionality
// SPDX-FileType: SOURCE

#pragma once

#include "smartled.hpp"
#include "smartled_mapper.hpp"

namespace platform::hardware::smartled
{
    struct VarikeyPrototype : public SmartLed
    {
        const float LUM_R = 1.400;
        const float LUM_G = 0.700;
        const float LUM_B = 0.400;

        const float LUM_R_FACTOR = 1.400;
        const float LUM_G_FACTOR = 0.700;
        const float LUM_B_FACTOR = 0.400;

        static const unsigned int PIO_PIN = 22;
        static const std::size_t LED_COUNT = 3;

        using Mapper = smartled::LedMapper<
            smartled::INDEX::FIRST,
            smartled::INDEX::SECOND,
            smartled::INDEX::THIRD>;

        VarikeyPrototype() : SmartLed(0, PIO_PIN) {}
    };
}
