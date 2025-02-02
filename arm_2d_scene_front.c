/*
 * Copyright (c) 2009-2024 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*============================ INCLUDES ======================================*/

#define __USER_SCENE_FRONT_IMPLEMENT__
#include "arm_2d_scene_front.h"

#include "arm_2d_scene_back.h"
#include "Virtual_TFT_Port.h"

#if defined(RTE_Acceleration_Arm_2D_Helper_PFB)

#include <stdlib.h>
#include <string.h>

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
#   pragma clang diagnostic ignored "-Wgnu-statement-expression"
#   pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#   pragma clang diagnostic ignored "-Wunused-function"
#   pragma clang diagnostic ignored "-Wmissing-declarations"
#   pragma clang diagnostic ignored "-Wimplicit-int-conversion" 
#elif __IS_COMPILER_ARM_COMPILER_5__
#   pragma diag_suppress 64,177
#elif __IS_COMPILER_IAR__
#   pragma diag_suppress=Pa089,Pe188,Pe177,Pe174
#elif __IS_COMPILER_GCC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wformat="
#   pragma GCC diagnostic ignored "-Wpedantic"
#   pragma GCC diagnostic ignored "-Wunused-function"
#   pragma GCC diagnostic ignored "-Wunused-variable"
#   pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
#endif

/*============================ MACROS ========================================*/

#if __GLCD_CFG_COLOUR_DEPTH__ == 8

#   define c_tileCMSISLogo          c_tileCMSISLogoGRAY8

#elif __GLCD_CFG_COLOUR_DEPTH__ == 16

#   define c_tileCMSISLogo          c_tileCMSISLogoRGB565

#elif __GLCD_CFG_COLOUR_DEPTH__ == 32

#   define c_tileCMSISLogo          c_tileCMSISLogoCCCA8888
#else
#   error Unsupported colour depth!
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
#undef this
#define this (*ptThis)

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

extern system_cfg_t SYSTEM_CFG;
extern const arm_2d_tile_t c_tileMonikaCCCA8888;
extern const arm_2d_tile_t c_tileEWLogoCCCA8888;
extern const arm_2d_tile_t c_tileArmLogoCCCA8888;

/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/

static void __on_scene_front_load(arm_2d_scene_t *ptScene)
{
    user_scene_front_t *ptThis = (user_scene_front_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}

static void __after_scene_front_switching(arm_2d_scene_t *ptScene)
{
    user_scene_front_t *ptThis = (user_scene_front_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}

static void __on_scene_front_depose(arm_2d_scene_t *ptScene)
{
    user_scene_front_t *ptThis = (user_scene_front_t *)ptScene;
    ARM_2D_UNUSED(ptThis);
    
    ptScene->ptPlayer = NULL;
    
    ARM_2D_OP_DEPOSE(this.tTransOP);

    arm_foreach(int64_t,this.lTimestamp, ptItem) {
        *ptItem = 0;
    }

    if (!this.bUserAllocated) {
        __arm_2d_free_scratch_memory(ARM_2D_MEM_TYPE_UNSPECIFIED, ptScene);
    }
}

/*----------------------------------------------------------------------------*
 * Scene front                                                                    *
 *----------------------------------------------------------------------------*/

static void __on_scene_front_background_start(arm_2d_scene_t *ptScene)
{
    user_scene_front_t *ptThis = (user_scene_front_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}

static void __on_scene_front_background_complete(arm_2d_scene_t *ptScene)
{
    user_scene_front_t *ptThis = (user_scene_front_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}


static void __on_scene_front_frame_start(arm_2d_scene_t *ptScene)
{
    user_scene_front_t *ptThis = (user_scene_front_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}

static void __on_scene_front_frame_complete(arm_2d_scene_t *ptScene)
{
    user_scene_front_t *ptThis = (user_scene_front_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

    if (!this.bFinishedDrawing) {
        this.bFinishedDrawing = true;
    } else {
        VT_save_screenshot(SYSTEM_CFG.Output.chFrontFileName);

        arm_2d_scene_player_switch_to_next_scene(
                                this.use_as__arm_2d_scene_t.ptPlayer);
    }
}

static void __before_scene_front_switching_out(arm_2d_scene_t *ptScene)
{
    user_scene_front_t *ptThis = (user_scene_front_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

    arm_2d_scene_back_init(this.use_as__arm_2d_scene_t.ptPlayer);
}

static
IMPL_PFB_ON_DRAW(__pfb_draw_scene_front_handler)
{
    ARM_2D_PARAM(pTarget);
    ARM_2D_PARAM(ptTile);
    ARM_2D_PARAM(bIsNewFrame);

    user_scene_front_t *ptThis = (user_scene_front_t *)pTarget;
    arm_2d_size_t tScreenSize = ptTile->tRegion.tSize;

    ARM_2D_UNUSED(tScreenSize);

    arm_2d_canvas(ptTile, __top_canvas) {
    /*-----------------------draw the foreground begin-----------------------*/
        arm_2d_dock(__top_canvas, 100 ) {

            arm_2d_layout(__dock_region) {
                
                __item_line_dock_vertical(c_tileArmLogoCCCA8888.tRegion.tSize.iHeight,
                                          (__dock_region.tSize.iWidth - 2000) / 2,
                                          (__dock_region.tSize.iWidth - 2000) / 2,
                                          0, 0) {
                    arm_2d_dock_left(__item_region, c_tileArmLogoCCCA8888.tRegion.tSize.iWidth) {
                        arm_2d_tile_copy_to_cccn888(&c_tileArmLogoCCCA8888,
                                                    ptTile,
                                                    &__left_region);

                    }
                }

                __item_line_dock_vertical() {

                    arm_2d_layout(__item_region, BOTTOM_UP) {

                        __item_line_dock_vertical(  c_tileEWLogoCCCA8888.tRegion.tSize.iHeight,
                                                    (__dock_region.tSize.iWidth - 2000) / 2,
                                                    (__dock_region.tSize.iWidth - 2000) / 2,
                                                    0, 0) {
                            arm_2d_dock_right(__item_region, c_tileEWLogoCCCA8888.tRegion.tSize.iWidth) {
                                arm_2d_tile_copy_to_cccn888(&c_tileEWLogoCCCA8888,
                                                            ptTile,
                                                            &__right_region);

                            }
                        }

                        __item_line_dock_vertical(  (__dock_region.tSize.iWidth - 2000) / 2,
                                                    (__dock_region.tSize.iWidth - 2000) / 2,
                                                    20, 20) {

                            //arm_2d_draw_box(ptTile, &__item_region, 4, GLCD_COLOR_BLACK, 32);

                            arm_2d_location_t tImageCentre = {
                                .iX = SYSTEM_CFG.Picture.tTile.tRegion.tSize.iWidth / 2,
                                .iY = SYSTEM_CFG.Picture.tTile.tRegion.tSize.iHeight / 2,
                            };

                            float fScaleW = (float)__item_region.tSize.iWidth / (float)SYSTEM_CFG.Picture.tTile.tRegion.tSize.iWidth;
                            float fScaleH = (float)__item_region.tSize.iHeight / (float)SYSTEM_CFG.Picture.tTile.tRegion.tSize.iHeight;
                            float fScale = MIN(fScaleW, fScaleH);

                            arm_2dp_tile_transform_with_src_mask_and_opacity(
                                &this.tTransOP,
                                &SYSTEM_CFG.Picture.tTile,
                                &SYSTEM_CFG.Picture.tMaskTile,
                                ptTile,
                                &__item_region,
                                tImageCentre,
                                0.0f,
                                fScale,
                                255);
                        }
                    }
                }
            }
        }

    /*-----------------------draw the foreground end  -----------------------*/
    }
    ARM_2D_OP_WAIT_ASYNC();

    return arm_fsm_rt_cpl;
}

ARM_NONNULL(1)
user_scene_front_t *__arm_2d_scene_front_init(   arm_2d_scene_player_t *ptDispAdapter, 
                                        user_scene_front_t *ptThis)
{
    bool bUserAllocated = false;
    assert(NULL != ptDispAdapter);

    if (NULL == ptThis) {
        ptThis = (user_scene_front_t *)
                    __arm_2d_allocate_scratch_memory(   sizeof(user_scene_front_t),
                                                        __alignof__(user_scene_front_t),
                                                        ARM_2D_MEM_TYPE_UNSPECIFIED);
        assert(NULL != ptThis);
        if (NULL == ptThis) {
            return NULL;
        }
    } else {
        bUserAllocated = true;
    }

    memset(ptThis, 0, sizeof(user_scene_front_t));

    *ptThis = (user_scene_front_t){
        .use_as__arm_2d_scene_t = {

            /* the canvas colour */
            .tCanvas = {GLCD_COLOR_WHITE}, 

            /* Please uncommon the callbacks if you need them
             */
            .fnOnLoad       = &__on_scene_front_load,
            .fnScene        = &__pfb_draw_scene_front_handler,
            //.fnAfterSwitch  = &__after_scene_front_switching,

            /* if you want to use predefined dirty region list, please uncomment the following code */
            //.ptDirtyRegion  = (arm_2d_region_list_item_t *)s_tDirtyRegions,
            

            //.fnOnBGStart    = &__on_scene_front_background_start,
            //.fnOnBGComplete = &__on_scene_front_background_complete,
            .fnOnFrameStart = &__on_scene_front_frame_start,
            .fnBeforeSwitchOut = &__before_scene_front_switching_out,
            .fnOnFrameCPL   = &__on_scene_front_frame_complete,
            .fnDepose       = &__on_scene_front_depose,

            .bUseDirtyRegionHelper = false,
        },
        .bUserAllocated = bUserAllocated,
    };

    /* ------------   initialize members of user_scene_front_t begin ---------------*/

    ARM_2D_OP_INIT(this.tTransOP);

    /* ------------   initialize members of user_scene_front_t end   ---------------*/

    arm_2d_scene_player_append_scenes(  ptDispAdapter, 
                                        &this.use_as__arm_2d_scene_t, 
                                        1);

    return ptThis;
}


#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

#endif


