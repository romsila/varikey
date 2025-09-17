// SPDX-FileCopyrightText: 2023 Roman Koch <koch.roman@gmail.com>
// SPDX-License-Identifier: MIT
// SPDX-FileContributor: Roman Koch <koch.roman@gmail.com>
// SPDX-FileComment: Hardware smartled varikey 15sbla functionality
// SPDX-FileType: SOURCE

#pragma once

#include "component_interface.hpp"
#include "smartled.hpp"
#include "smartled_layer.hpp"
#include "smartled_mapper.hpp"

namespace platform::hardware::smartled
{
    struct VarikeyBack15SblaBack : public SmartLed, public pulp::ComponentInterface
    {
        const float LUM_R = 1.400;
        const float LUM_G = 0.700;
        const float LUM_B = 0.400;

        const float LUM_R_FACTOR = 1.400;
        const float LUM_G_FACTOR = 0.700;
        const float LUM_B_FACTOR = 0.400;

        static const unsigned int PIO_PIN = 6;
        static const std::size_t LED_COUNT = 5;
        using Mapper = smartled::LedMapper<
            smartled::INDEX::FIRST,
            smartled::INDEX::SECOND,
            smartled::INDEX::THIRD,
            smartled::INDEX::FOURTH,
            smartled::INDEX::FIFTH>;

        VarikeyBack15SblaBack() : SmartLed(1, PIO_PIN) {}

        virtual void initialize() override
        {
            SmartLed::initialize<Mapper>();
        }

        virtual void shutdown() override
        {
            SmartLed::shutdown<Mapper>();
        }

        void set_led_chain(const Color &_left, const Color &_right)
        {
            SmartLed::set_led_chain<Mapper>(_left, _right, layer);
        }

        void set_led_sequence(const Color (&colors)[Mapper::mapping.size()])
        {
            SmartLed::set_led_sequence<VarikeyBack15SblaBack::Mapper>(colors);
        }

        Layer<Mapper> layer;
    };
}
