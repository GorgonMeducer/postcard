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
#ifndef __POSTCARD_H__
#   define __POSTCARD_H__

#include <stdint.h>
#include <stdbool.h>
#include <arm_2d.h>

#ifdef   __cplusplus
extern "C" {
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

enum {
    FOOTNOTE_ALIGN_DEFAULT = 0,
    FOOTNOTE_ALIGN_RIGHT,
    FOOTNOTE_ALIGN_LEFT,
    FOOTNOTE_ALIGN_MIDDLE
};

typedef struct system_cfg_t {
    struct {
        char *pchInputPicturePath;
        char *pchLogoPath;
        char *pchStoryPath;
        char *pchEventName;
        uint16_t bUseA4                 : 1;
        uint16_t bValid                 : 1;
        uint16_t bDryRun                : 1;
        uint16_t bNoBackgroundColour    : 1;
        uint16_t bOneSideMode           : 1;
        uint16_t bAutoScaling           : 1;
        uint16_t bHideDefaultLogo       : 1;
        uint16_t                        : 1;
        uint16_t u2FootnoteLogoAlign    : 2;
        uint16_t                        : 6;
        
        float fScale;
        int16_t iFootnoteHeight;
        int16_t iStoryBoardMargin;
        int16_t iFootnoteBoxHeight;
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
        arm_2d_tile_t tTile;
        arm_2d_tile_t tMaskTile;
    } Logo;

    struct {
        char chFrontFileName[64];
        char chBackFileName[64];
        char chCombinedFileName[64];
    } Output;
} system_cfg_t;


/*============================ GLOBAL VARIABLES ==============================*/
extern system_cfg_t SYSTEM_CFG;
/*============================ PROTOTYPES ====================================*/


#ifdef   __cplusplus
}
#endif

#endif

