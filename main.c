/****************************************************************************
*  Copyright 2023 Gorgon Meducer (Email:embedded_zhuoran@hotmail.com)       *
*                                                                           *
*  Licensed under the Apache License, Version 2.0 (the "License");          *
*  you may not use this file except in compliance with the License.         *
*  You may obtain a copy of the License at                                  *
*                                                                           *
*     http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                           *
*  Unless required by applicable law or agreed to in writing, software      *
*  distributed under the License is distributed on an "AS IS" BASIS,        *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
*  See the License for the specific language governing permissions and      *
*  limitations under the License.                                           *
*                                                                           *
****************************************************************************/

/*============================ INCLUDES ======================================*/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "Virtual_TFT_Port.h"
#include "arm_2d_helper.h"
#include "arm_2d_scenes.h"
#include "arm_2d_disp_adapters.h"

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-prototypes"
#   pragma clang diagnostic ignored "-Wunused-variable"
#   pragma clang diagnostic ignored "-Wunused-parameter"
#   pragma clang diagnostic ignored "-Wgnu-statement-expression"
#elif __IS_COMPILER_ARM_COMPILER_5__
#elif __IS_COMPILER_GCC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wformat="
#   pragma GCC diagnostic ignored "-Wpedantic"
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef volatile struct system_cfg_t {
    struct {
        const char *pchInputPicturePath;
        const char *pchStoryPath;
        bool bValid;
    } Input;
} system_cfg_t;

/*============================ GLOBAL VARIABLES ==============================*/

system_cfg_t SYSTEM_CFG = {0};

/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/

bool file_exists(const char *path, const char *pchMode) 
{
    FILE *fp = fopen(path, pchMode);
    if (fp) {
        fclose(fp);
        return true;
    }
    return false;
}

static
arm_2d_err_t process_args(int argc, char* argv[])
{

    bool bInputIsValid = true;
    for (int n = 0; n < argc; n++) {
        //printf("[%s]\r\n", argv[n]);

        if (0 == n) {
            continue;
        }

        if (0 == strncmp(argv[n], "-p", 2)) {
            n++;
            if (n >= argc) {
                bInputIsValid = false;
                break;
            }

            if (!file_exists(argv[n], "r")) {
                printf("ERROR: Cannot find the input picture from \"%s\".", argv[n]);
                bInputIsValid = false;
                break;
            }

            SYSTEM_CFG.Input.pchInputPicturePath = argv[n];
            continue;
        }

        if (0 == strncmp(argv[n], "-t", 2)) {
            n++;
            if (n >= argc) {
                bInputIsValid = false;
                break;
            }

            if (!file_exists(argv[n], "r")) {
                printf("ERROR: Cannot find the input story from \"%s\".", argv[n]);
                bInputIsValid = false;
                break;
            }

            SYSTEM_CFG.Input.pchStoryPath = argv[n];
            continue;
        }
    }

    SYSTEM_CFG.Input.bValid = bInputIsValid;
    if (!SYSTEM_CFG.Input.bValid) {
        return ARM_2D_ERR_INVALID_PARAM;
    }

    return ARM_2D_ERR_NONE;
}

/*----------------------------------------------------------------------------
  Main function
 *----------------------------------------------------------------------------*/

int app_2d_main_thread (void *argument)
{
    while (1) {
        if (arm_fsm_rt_cpl == disp_adapter0_task()) {
            VT_sdl_flush(1);
        }
    }

    return 0;
}

int main(int argc, char* argv[])
{
    if (ARM_2D_ERR_NONE != process_args(argc, argv)) {
        return -1;
    }

    VT_init();

    arm_irq_safe {
        arm_2d_init();
    }

    disp_adapter0_init();

    SDL_CreateThread(app_2d_main_thread, "arm-2d thread", NULL);

    while (1) {
        VT_sdl_refresh_task();
        if(VT_is_request_quit()){
            break;
        }
    }

    VT_deinit();
    return 0;
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif


