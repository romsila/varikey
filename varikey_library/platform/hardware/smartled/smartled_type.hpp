// SPDX-FileCopyrightText: 2023 Roman Koch <koch.roman@gmail.com>
// SPDX-License-Identifier: MIT
// SPDX-FileContributor: Roman Koch <koch.roman@gmail.com>
// SPDX-FileComment: Hardware smartled type functionality
// SPDX-FileType: SOURCE

#pragma once

#include "platform_defines.hpp"
#include "revision.h"
#include "smartled_type_gmc.hpp"
#include "smartled_type_varikey.hpp"
#include "smartled_type_varikey_15sbla_back.hpp"
#include "smartled_type_varikey_15sbla_front.hpp"

namespace platform::hardware
{
    namespace smartled::variant
    {
        template <platform::defines::IDENTIFIER>
        struct Entity
        {
            using Frontlight = platform::Undefined;
        };

        template <>
        struct Entity<platform::defines::IDENTIFIER::VARIKEY_1_0>
        {
            using Frontlight = VarikeyPrototype;
            using Backlight = platform::Undefined;
        };

        template <>
        struct Entity<platform::defines::IDENTIFIER::VARIKEY_2_3>
        {
            using Frontlight = VarikeyFront15SblaFront;
            using Backlight = VarikeyBack15SblaBack;
        };

        template <>
        struct Entity<platform::defines::IDENTIFIER::GMCI_1_0>
        {
            using Frontlight = Gmci;
            using Backlight = platform::Undefined;
        };
    }
    using Frontlight = smartled::variant::Entity<platform::defines::IDENTIFIER(identity::hardware::IDENTIFIER)>::Frontlight;
    using Backlight = smartled::variant::Entity<platform::defines::IDENTIFIER(identity::hardware::IDENTIFIER)>::Backlight;
}
