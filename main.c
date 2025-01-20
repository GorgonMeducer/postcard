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

#include "__arm_2d_impl.h"

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
typedef struct system_cfg_t {
    struct {
        char *pchInputPicturePath;
        char *pchStoryPath;
        bool bUseA4;
        bool bValid;
    } Input;

    struct {
        char *pchStory;
        size_t tSize;
    } Story;

    struct {
        arm_2d_tile_t tTile;
    } Picture;

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

    if (argc <= 1) {
        return ARM_2D_ERR_MISSING_PARAM;
    }

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
                printf("ERROR: Cannot find the input story from the given text file \"%s\".", argv[n]);
                bInputIsValid = false;
                break;
            }

            SYSTEM_CFG.Input.pchStoryPath = argv[n];
            continue;
        }

        if (    (0 == strncmp(argv[n], "--A4", 4)) 
            ||  (0 == strncmp(argv[n], "--a4", 4))) {
            SYSTEM_CFG.Input.bUseA4 = true;
            continue;
        }

        if (    (0 == strncmp(argv[n], "--help", 6)) 
            ||  (0 == strncmp(argv[n], "-h", 2))) {
            bInputIsValid = false;
            return ARM_2D_ERR_MISSING_PARAM;
        }
    }

    SYSTEM_CFG.Input.bValid = bInputIsValid;
    if (!SYSTEM_CFG.Input.bValid) {
        return ARM_2D_ERR_INVALID_PARAM;
    } else if ((NULL == SYSTEM_CFG.Input.pchInputPicturePath)
            || (NULL == SYSTEM_CFG.Input.pchStoryPath)) {
        return ARM_2D_ERR_MISSING_PARAM;
    }

    return ARM_2D_ERR_NONE;
}

/*----------------------------------------------------------------------------
  Main function
 *----------------------------------------------------------------------------*/

int app_2d_main_thread (void *argument)
{
    while (1) {
        if (VT_is_request_quit()) {
            break;
        }
        if (arm_fsm_rt_cpl == disp_adapter0_task()) {
            VT_sdl_flush(1);
        }
    }
    return 0;
}

static void print_help(void)
{
    printf("The Postcard is a command line tool that generates and prints out a postcard with given input picture and a story text. \r\n");
    printf("\r\noptions:\r\n");
    printf("\t-h, --help         show this help message and exit\r\n");
    printf("\t-p [picture path]  Input picture (*.png)\r\n");
    printf("\t-t [text path]     Input text file.\r\n");
    printf("\t--A4, --a4         Use A4 papers rather than A5 papers for printing.\r\n");
    printf("\r\n");
}

static size_t load_story(const char *pchPath, char **ppchOutput)
{
    FILE *fp = fopen(pchPath, "r");
    do {
        if (NULL == fp) {
            break;
        }
        
        fseek(fp, 0, SEEK_END);
        fpos_t tPos = 0;
        if (0 != fgetpos(fp, &tPos)) {
            break;
        }
        
        *ppchOutput = malloc(tPos);
        if (NULL == *ppchOutput) {
            break;
        }

        fseek(fp, 0, SEEK_SET);

        return fread(*ppchOutput, 1, tPos, fp);
    } while(0);

    return 0;
}

static bool load_picture(const char *pchPath, arm_2d_tile_t *ptTile)
{
    memset(ptTile, 0, sizeof(arm_2d_tile_t));

    SDL_Surface *ptImage = SDL_LoadBMP(pchPath);
    do {
        if (!ptImage) {
            break;
        }
        ptTile->tRegion.tSize.iHeight = ptImage->h;
        ptTile->tRegion.tSize.iWidth = ptImage->w;
        ptTile->bIsRoot = true;
        ptTile->bHasEnforcedColour = true;
        ptTile->tInfo.tColourInfo.chScheme = ARM_2D_COLOUR_CCCA8888;

        ptTile->pwBuffer = malloc( ptImage->h * ptImage->w * 4);
        if (NULL == ptTile->pwBuffer) {
            break;
        }
        
        switch (ptImage->format->BitsPerPixel) {
            case 8:
                __arm_2d_impl_gray8_to_cccn888((uint8_t *)ptImage->pixels,
                                               ptImage->pitch,
                                               ptTile->pwBuffer,
                                               ptTile->tRegion.tSize.iWidth,
                                               &ptTile->tRegion.tSize);
                break;
            case 16:
                if (ptImage->pitch != ptTile->tRegion.tSize.iWidth * 2) {
                    return false;
                }
                __arm_2d_impl_rgb565_to_cccn888((uint16_t *)ptImage->pixels,
                                                ptTile->tRegion.tSize.iWidth,
                                                ptTile->pwBuffer,
                                                ptTile->tRegion.tSize.iWidth,
                                                &ptTile->tRegion.tSize);
                break;
            case 32:
                memcpy(ptTile->pwBuffer, ptImage->pixels, ptImage->h * ptImage->w * 4);
                break;
            default:
            case 24:
                return false;
        }

        SDL_FreeSurface(ptImage);

        return true;

    } while(0);

    return false;
}


int main(int argc, char* argv[])
{
    int ret = 0;

    arm_2d_err_t tResult = process_args(argc, argv);
    switch (tResult) {
        case ARM_2D_ERR_NONE:
            break;
        case ARM_2D_ERR_MISSING_PARAM:
            print_help();
            //fall-through;
        default:
            return -1;
            break;
    }

    VT_init();

    arm_irq_safe {
        arm_2d_init();
    }

    disp_adapter0_init();

    
    do {
        /* load story */
        SYSTEM_CFG.Story.tSize = load_story(SYSTEM_CFG.Input.pchStoryPath, 
                                            (char **)&SYSTEM_CFG.Story.pchStory);
        if (0 == SYSTEM_CFG.Story.tSize) {
            printf("ERROR: Cannot read the story or the story file is empty: %s\r\n", SYSTEM_CFG.Input.pchStoryPath);
            ret = -1;
            break;
        }

        /* load png */

        if (!load_picture(SYSTEM_CFG.Input.pchInputPicturePath,
                         &SYSTEM_CFG.Picture.tTile)) {
            printf("ERROR: failed to load bmp file, %s\n", SDL_GetError());
            ret = -1;
            break;
        };

        /* start arm-2d task */

        SDL_CreateThread(app_2d_main_thread, "arm-2d thread", NULL);
        while (1) {
            if(!VT_sdl_refresh_task()){
                break;
            }
        }

    } while(0);

    /* release resources */
    if (NULL != SYSTEM_CFG.Story.pchStory) {
        free(SYSTEM_CFG.Story.pchStory);
        SYSTEM_CFG.Story.pchStory = NULL;
        SYSTEM_CFG.Story.tSize = 0;
    }
    if (NULL != SYSTEM_CFG.Picture.tTile.pchBuffer) {
        free(SYSTEM_CFG.Picture.tTile.pchBuffer);
        SYSTEM_CFG.Picture.tTile.pchBuffer = NULL;
    }

    VT_deinit();
    return ret;
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif


