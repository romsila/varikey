// SPDX-FileCopyrightText: 2023 Roman Koch <koch.roman@gmail.com>
// SPDX-License-Identifier: MIT
// SPDX-FileContributor: Roman Koch <koch.roman@gmail.com>
// SPDX-FileComment: Registry implementation for parameter management
// SPDX-FileType: SOURCE

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "checksum.hpp"
#include "chunk.h"
#include "cmd_control.hpp"
#include "cmd_reset_msg.hpp"
#include "macros.hpp"
#include "param_backlight.hpp"
#include "param_features.hpp"
#include "param_keypad.hpp"
#include "param_maintainer.hpp"
#include "param_position.hpp"
#include "param_serial_number.hpp"
#include "param_user.hpp"
#include "parameter.hpp"
#include "random.hpp"
#include "registry.hpp"
#include "varikey.hpp"

#define _FORMAT_PARAMETER_REGISTRY

namespace registry
{

    typedef enum
    {
        UNDEFINED,
        READY,
    } status_t;

    static status_t status = UNDEFINED;

    static void node_config_init(void);

    extern void initialize(void)
    {

#if defined(FORMAT_PARAMETER_REGISTRY)

        parameter::param_format();
        parameter::param_backup_create();
#endif

        if (parameter::param_check() == FAILURE)
        {
            parameter::param_format();
            engine::hci::cmd::control::reset_indication(engine::hci::cmd::reset::RESULT::PARAMETER_MISSED);
        }

        node_config_init();
        if (is_ready() == SUCCESS)
        {
            if (parameter::param_backup_check() == FAILURE)
            {
                parameter::param_backup_create();
                engine::hci::cmd::control::reset_indication(engine::hci::cmd::reset::RESULT::BACKUP_CREATED);
            }
        }
        else
        {
            engine::hci::cmd::control::reset_indication(engine::hci::cmd::reset::RESULT::CRITICAL_ERROR);
            if (parameter::param_backup_restore() != SUCCESS)
            {
                parameter::param_backup_create();
                engine::hci::cmd::control::reset_indication(engine::hci::cmd::reset::RESULT::BACKUP_CREATED);
            }
        }
        // debug_out_mem();
    }

    static void node_config_init(void)
    {
        assert(status == UNDEFINED); // state should be undefined

        if (parameter::param_backlight_load() == FAILURE)
        {
            return;
        }

        if (parameter::param_display_load() == FAILURE)
        {
            return;
        }

        if (parameter::param_features_load() == FAILURE)
        {
            return;
        }

        if (parameter::param_keypad_load() == FAILURE)
        {
            return;
        }

        if (parameter::param_maintainer_load() == FAILURE)
        {
            return;
        }

        if (parameter::param_mapping_load() == FAILURE)
        {
            return;
        }

        if (parameter::param_position_load() == FAILURE)
        {
            return;
        }

        // Serial number: previously a FAILURE aborted entire initialization.
        // We now attempt to auto-heal if invalid (all 0x00 or INVALID_VALUE) and continue.
        if (parameter::param_serial_number_load() == FAILURE)
        {
            // Attempt regeneration
            parameter::serial_number::g_register.initialize();
            // Persist regenerated serial; ignore store failure (will retry next boot)
            parameter::param_serial_number_store();
        }
        else
        {
            // Additional runtime validation: check if all bytes are zero -> treat as invalid and regenerate
            bool all_zero = true;
            for (size_t i = 0; i < parameter::serial_number::SIZE; ++i)
            {
                if (parameter::serial_number::g_register.value[i] != 0x00)
                {
                    all_zero = false;
                    break;
                }
            }
            if (all_zero)
            {
                parameter::serial_number::g_register.initialize();
                parameter::param_serial_number_store();
            }
        }

        srand(*(uint16_t *)parameter::serial_number::g_register.value);

        status = READY;
    }

    extern void shutdown(void)
    {
        assert(status == READY); // state should be initialized

        status = UNDEFINED;
    }

    extern result_t is_ready(void)
    {
        if (status == READY)
        {
            return SUCCESS;
        }
        return FAILURE;
    }
}