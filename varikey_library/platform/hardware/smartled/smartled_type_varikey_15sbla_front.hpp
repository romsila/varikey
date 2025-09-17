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
    struct VarikeyFront15SblaFront : public SmartLed, public pulp::ComponentInterface

    {
        const float LUM_R = 1.400;
        const float LUM_G = 0.700;
        const float LUM_B = 0.400;

        const float LUM_R_FACTOR = 1.400;
        const float LUM_G_FACTOR = 0.700;
        const float LUM_B_FACTOR = 0.400;

        static const unsigned int PIO_PIN = 7;
        static const std::size_t LED_COUNT = 15;

        /*
            Note:
            The mapping is based on the physical layout of the LEDs on the Varikey 15SBLA.
            The order of the indices corresponds to the actual wiring of the LEDs.
        */
        using Mapper = smartled::LedMapper<
            smartled::INDEX::FIFTH,
            smartled::INDEX::SEVENTH,
            smartled::INDEX::SIXTH,
            smartled::INDEX::FOURTH,
            smartled::INDEX::THIRD,
            smartled::INDEX::SECOND,
            smartled::INDEX::FIRST,
            smartled::INDEX::EIGHTH,
            smartled::INDEX::TENTH,
            smartled::INDEX::THIRTEENTH,
            smartled::INDEX::NINTH,
            smartled::INDEX::TWELFTH,
            smartled::INDEX::FIFTEENTH,
            smartled::INDEX::ELEVENTH,
            smartled::INDEX::FOURTEENTH>;

        VarikeyFront15SblaFront() : SmartLed(0, PIO_PIN)
        {
            //layer.buffer[static_cast<int>(smartled::INDEX::FOURTEENTH)] = {Color{0xff, 0, 0}, Layer<Mapper>::BlendMode::REPLACE, 1.0f};
        }

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
            SmartLed::set_led_sequence<VarikeyFront15SblaFront::Mapper>(colors);
        }

        Layer<Mapper> layer;
    };
}
