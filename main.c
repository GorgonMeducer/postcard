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
#include "arm_2d_scene_front.h"

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
        bool bDryRun;
    } Input;

    struct {
        char *pchStory;
        size_t tSize;
    } Story;

    struct {
        arm_2d_tile_t tTile;
        arm_2d_tile_t tMaskTile;
    } Picture;

    struct {
        char chFrontFileName[64];
        char chBackFileName[64];
        char chCombinedFileName[64];
    } Output;
} system_cfg_t;

/*============================ GLOBAL VARIABLES ==============================*/

system_cfg_t SYSTEM_CFG = {
    .Output = {
        .chFrontFileName = {"front_XXXXXXXX"},
        .chBackFileName = {"back_XXXXXXXX"},
        .chCombinedFileName = {"combined_XXXXXXXX"},
    },
};

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

void run_os_command(const char *pchCommandLine) {
    static char s_chBuffer[4096];
    FILE *ptPipe = popen(pchCommandLine, "r");
    if (!ptPipe) {
        printf("Failed to execute command: [%s]\r\n", pchCommandLine);
        return;
    }

    while (fgets(s_chBuffer, sizeof(s_chBuffer), ptPipe) != NULL) {
        printf("%s", s_chBuffer);
    }

    pclose(ptPipe);
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

        if ( 0 == strncmp(argv[n], "--dryrun", 8)) {
            SYSTEM_CFG.Input.bDryRun = true;
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
    arm_2d_scene_front_init(&DISP0_ADAPTER);

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
    printf("\t-p [picture path]  Input picture (*.bmp)\r\n");
    printf("\t-t [text path]     Input text file.\r\n");
    printf("\t--A4, --a4         Use A4 papers rather than A5 papers for printing.\r\n");
    printf("\t--dryrun           Generate PDF and skip printing.\r\n");
    printf("\r\n");
}

static size_t load_story(const char *pchPath, char **ppchOutput)
{
    FILE *fp = fopen(pchPath, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    if (file_size == -1) {
        perror("Error getting file size");
        fclose(fp);
        return 0;
    }

    *ppchOutput = (char *)malloc((size_t)file_size + 1);
    if (*ppchOutput == NULL) {
        perror("Memory allocation failed");
        fclose(fp);
        return 0;
    }

    fseek(fp, 0, SEEK_SET);
    size_t bytes_read = fread(*ppchOutput, 1, (size_t)file_size, fp);
    (*ppchOutput)[bytes_read] = '\0';

    fclose(fp);
    return bytes_read;
}

void __ccc888_to_cccn888(uint8_t *pchSrc, int16_t iPitch, uint32_t *pwDes, int16_t iWidth, int16_t iHeight)
{
    for (int16_t y = 0; y < iHeight; y++) {
        uint8_t *pchLineSrc = pchSrc;
        uint32_t *pwLineDes = pwDes;

        for (int16_t x = 0; x < iWidth; x++) {
            ((uint8_t *)pwLineDes)[0] = *pchLineSrc++;
            ((uint8_t *)pwLineDes)[1] = *pchLineSrc++;
            ((uint8_t *)pwLineDes)[2] = *pchLineSrc++;
            ((uint8_t *)pwLineDes)[3] = 0xFF;

            pwLineDes++;
        }

        pchSrc += iPitch;
        pwDes += iWidth;
    }
}

static bool load_picture(const char *pchPath, arm_2d_tile_t *ptTile, arm_2d_tile_t *ptMask)
{
    memset(ptTile, 0, sizeof(arm_2d_tile_t));
    memset(ptMask, 0, sizeof(arm_2d_tile_t));

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
            case 24:
                __ccc888_to_cccn888(ptImage->pixels,
                                    ptImage->pitch, 
                                    ptTile->pwBuffer, 
                                    ptImage->w,
                                    ptImage->h);
                break;
            case 32:
                memcpy(ptTile->pwBuffer, ptImage->pixels, ptImage->h * ptImage->w * 4);
                break;
            default:
                return false;
        }

        ptMask->tRegion.tSize.iHeight = ptImage->h;
        ptMask->tRegion.tSize.iWidth = ptImage->w;
        ptMask->bIsRoot = true;
        ptMask->bHasEnforcedColour = true;
        ptMask->tInfo.tColourInfo.chScheme = ARM_2D_CHANNEL_8in32;
        ptMask->nAddress = ptTile->nAddress + 3;

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
                         &SYSTEM_CFG.Picture.tTile,
                         &SYSTEM_CFG.Picture.tMaskTile)) {
            printf("ERROR: failed to load bmp file, %s\n", SDL_GetError());
            ret = -1;
            break;
        };

        /* initialize temp files */
        do {
            mktemp(SYSTEM_CFG.Output.chFrontFileName);
            int32_t nStringLength = strlen(SYSTEM_CFG.Output.chFrontFileName);
            SYSTEM_CFG.Output.chFrontFileName[nStringLength++] = '.';
            SYSTEM_CFG.Output.chFrontFileName[nStringLength++] = 'b';
            SYSTEM_CFG.Output.chFrontFileName[nStringLength++] = 'm';
            SYSTEM_CFG.Output.chFrontFileName[nStringLength++] = 'p';
            SYSTEM_CFG.Output.chFrontFileName[nStringLength++] = '\0';
            
            mktemp(SYSTEM_CFG.Output.chBackFileName);
            nStringLength = strlen(SYSTEM_CFG.Output.chBackFileName);
            SYSTEM_CFG.Output.chBackFileName[nStringLength++] = '.';
            SYSTEM_CFG.Output.chBackFileName[nStringLength++] = 'b';
            SYSTEM_CFG.Output.chBackFileName[nStringLength++] = 'm';
            SYSTEM_CFG.Output.chBackFileName[nStringLength++] = 'p';
            SYSTEM_CFG.Output.chBackFileName[nStringLength++] = '\0';

            mktemp(SYSTEM_CFG.Output.chCombinedFileName);
            nStringLength = strlen(SYSTEM_CFG.Output.chCombinedFileName);
            SYSTEM_CFG.Output.chCombinedFileName[nStringLength++] = '.';
            SYSTEM_CFG.Output.chCombinedFileName[nStringLength++] = 'p';
            SYSTEM_CFG.Output.chCombinedFileName[nStringLength++] = 'd';
            SYSTEM_CFG.Output.chCombinedFileName[nStringLength++] = 'f';
            SYSTEM_CFG.Output.chCombinedFileName[nStringLength++] = '\0';
        } while(0);


        /* start arm-2d task */

        SDL_CreateThread(app_2d_main_thread, "arm-2d thread", NULL);
        while (1) {
            if(!VT_sdl_refresh_task()){
                break;
            }
        }

        /* generate PDF */
        do {
            static char s_chCommandLine[256] = {0};
            snprintf(  s_chCommandLine, 
                        dimof(s_chCommandLine), 
                        "convert %s %s %s",
                        SYSTEM_CFG.Output.chFrontFileName,
                        SYSTEM_CFG.Output.chBackFileName,
                        SYSTEM_CFG.Output.chCombinedFileName);

            run_os_command(s_chCommandLine);
        } while(0);

        /* printf PDF */
        do {
            static char s_chCommandLine[256] = {0};
            if (SYSTEM_CFG.Input.bUseA4) {
                snprintf(  s_chCommandLine, 
                            dimof(s_chCommandLine), 
                            "lp -o media=A4 -o sides=two-sided-short-edge -o ColorModel=Color -o orientation-requested=4 -o fit-to-page %s",
                            SYSTEM_CFG.Output.chCombinedFileName);
            } else {
                snprintf(  s_chCommandLine, 
                            dimof(s_chCommandLine), 
                            "lp -o media=A5 -o sides=two-sided-short-edge -o ColorModel=Color -o orientation-requested=4 -o fit-to-page %s",
                            SYSTEM_CFG.Output.chCombinedFileName);
            }
            if (SYSTEM_CFG.Input.bDryRun) {
                printf("\r\n\r\n It is dry-run mode. Please use the following command line to print: \r\n %s\r\n", s_chCommandLine);
            } else {
                run_os_command(s_chCommandLine);
            }
            
        } while(0);

        /* delete temporary files*/
        do {
            static char s_chCommandLine[256] = {0};
            snprintf(  s_chCommandLine, 
                        dimof(s_chCommandLine), 
                        "rm %s",
                        SYSTEM_CFG.Output.chFrontFileName);

            run_os_command(s_chCommandLine);

            memset(s_chCommandLine, 0, sizeof(s_chCommandLine));
            snprintf(  s_chCommandLine, 
                        dimof(s_chCommandLine), 
                        "rm %s",
                        SYSTEM_CFG.Output.chBackFileName);

            run_os_command(s_chCommandLine);

            if (!SYSTEM_CFG.Input.bDryRun) {
                memset(s_chCommandLine, 0, sizeof(s_chCommandLine));
                snprintf(  s_chCommandLine, 
                            dimof(s_chCommandLine), 
                            "rm %s",
                            SYSTEM_CFG.Output.chCombinedFileName);

                run_os_command(s_chCommandLine);
            }
        } while(0);

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


