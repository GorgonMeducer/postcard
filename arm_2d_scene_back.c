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

#define __USER_SCENE_BACK_IMPLEMENT__
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

extern 
const arm_2d_tile_t c_tileOnArmLogoCCCA8888;

extern
const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_IBMPlexMono46_A8;

extern
const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_Calibri46_A8;

extern
const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_Lato64_A8;

extern
const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_CalibriBold46_A8;

extern
const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_BradleyHand64_A8;

/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/

static void __on_scene_back_load(arm_2d_scene_t *ptScene)
{
    user_scene_back_t *ptThis = (user_scene_back_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}

static void __after_scene_back_switching(arm_2d_scene_t *ptScene)
{
    user_scene_back_t *ptThis = (user_scene_back_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}

static void __on_scene_back_depose(arm_2d_scene_t *ptScene)
{
    user_scene_back_t *ptThis = (user_scene_back_t *)ptScene;
    ARM_2D_UNUSED(ptThis);
    
    ptScene->ptPlayer = NULL;
    
    arm_foreach(int64_t,this.lTimestamp, ptItem) {
        *ptItem = 0;
    }

    if (!this.bUserAllocated) {
        __arm_2d_free_scratch_memory(ARM_2D_MEM_TYPE_UNSPECIFIED, ptScene);
    }
}

/*----------------------------------------------------------------------------*
 * Scene back                                                                    *
 *----------------------------------------------------------------------------*/

static void __on_scene_back_background_start(arm_2d_scene_t *ptScene)
{
    user_scene_back_t *ptThis = (user_scene_back_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}

static void __on_scene_back_background_complete(arm_2d_scene_t *ptScene)
{
    user_scene_back_t *ptThis = (user_scene_back_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}


static void __on_scene_back_frame_start(arm_2d_scene_t *ptScene)
{
    user_scene_back_t *ptThis = (user_scene_back_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}

static void __on_scene_back_frame_complete(arm_2d_scene_t *ptScene)
{
    user_scene_back_t *ptThis = (user_scene_back_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

    if (!this.bFinishedDrawing) {
        this.bFinishedDrawing = true;
    } else {
        VT_save_screenshot(SYSTEM_CFG.Output.chBackFileName);

        VT_request_quit();
    }
}

static void __before_scene_back_switching_out(arm_2d_scene_t *ptScene)
{
    user_scene_back_t *ptThis = (user_scene_back_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}

static
IMPL_PFB_ON_DRAW(__pfb_draw_scene_back_handler)
{
    ARM_2D_PARAM(pTarget);
    ARM_2D_PARAM(ptTile);
    ARM_2D_PARAM(bIsNewFrame);

    user_scene_back_t *ptThis = (user_scene_back_t *)pTarget;
    arm_2d_size_t tScreenSize = ptTile->tRegion.tSize;

    ARM_2D_UNUSED(tScreenSize);

    arm_2d_canvas(ptTile, __top_canvas) {

        arm_2d_dock(__top_canvas, 200, 200, 100, 100) {

            //arm_2d_draw_box(ptTile, &__dock_region, 4, GLCD_COLOR_BLACK, 32);

            arm_2d_layout(__dock_region) {
                
                __item_line_dock_vertical(46) {
                    arm_lcd_text_set_target_framebuffer(ptTile);
                    
                    arm_lcd_text_set_draw_region(&__item_region);
                    arm_lcd_text_set_colour(GLCD_COLOR_BLACK, GLCD_COLOR_WHITE);
                    arm_lcd_text_set_font((arm_2d_font_t *)&ARM_2D_FONT_CalibriBold46_A8);
                    //arm_lcd_text_set_scale(1.1f);
                    arm_lcd_printf_label(ARM_2D_ALIGN_MIDDLE_LEFT, "March 2025, NürnbergMesse GmbH");
                    //arm_lcd_text_set_scale(1.0f);
                }

                __item_line_dock_vertical() {

                    arm_2d_layout(__item_region, BOTTOM_UP) {

                        /* draw footnote */
                        __item_line_dock_vertical(c_tileOnArmLogoCCCA8888.tRegion.tSize.iHeight) {

                            arm_lcd_text_set_target_framebuffer(ptTile);
                            arm_lcd_text_set_font((arm_2d_font_t *)&ARM_2D_FONT_Lato64_A8);
                            arm_lcd_text_set_scale(1.2f);
                            arm_2d_size_t tStringSize = 
                                arm_lcd_get_string_line_box(
                                                        "The Fugure is Built ", 
                                                        &ARM_2D_FONT_Lato64_A8);

                            arm_2d_dock_horizontal( __item_region, 
                                                        c_tileOnArmLogoCCCA8888.tRegion.tSize.iWidth 
                                                    +   tStringSize.iWidth) {

                                /* draw string */
                                arm_2d_align_mid_left(__horizontal_region, tStringSize) {
                                    //

                                    arm_lcd_text_set_draw_region(&__mid_left_region);
                                    arm_lcd_text_set_colour(GLCD_COLOR_BLACK, GLCD_COLOR_WHITE);
                                    arm_lcd_text_set_font((arm_2d_font_t *)&ARM_2D_FONT_Lato64_A8);
                                    arm_lcd_printf_label(ARM_2D_ALIGN_MIDDLE_LEFT, "The Future is Built");

                                    arm_lcd_text_set_scale(1.0f);
                                }

                                /* draw OnArm logo */
                                arm_2d_align_mid_right(__horizontal_region, c_tileOnArmLogoCCCA8888.tRegion.tSize) {

                                    arm_2d_tile_copy_to_cccn888(&c_tileOnArmLogoCCCA8888,
                                                                ptTile,
                                                                &__mid_right_region);

                                }
                            }

                        }

                        /* draw explanations */
                        __item_line_dock_vertical(250) {
                            //arm_2d_draw_box(ptTile, &__item_region, 4, GLCD_COLOR_BLACK, 32);

                            arm_lcd_text_set_draw_region(&__item_region);
                            arm_lcd_text_set_font((arm_2d_font_t *)&ARM_2D_FONT_Calibri46_A8);
                            arm_lcd_text_set_scale(1.30f);
                            arm_lcd_printf("The above story was generated based on the image on the reverse of this card. AI models running solely on a ");
                            arm_lcd_text_set_colour(GLCD_COLOR_RED, GLCD_COLOR_WHITE);
                            arm_lcd_printf("Raspberry Pi ");
                            arm_lcd_text_set_colour(__RGB(32, 200, 32), GLCD_COLOR_WHITE);
                            arm_lcd_printf("5 ");
                            arm_lcd_text_set_colour(GLCD_COLOR_BLACK, GLCD_COLOR_WHITE);
                            arm_lcd_printf("were used to detect objects in the camera view and develop a narrative, utilizing ");
                            arm_lcd_text_set_colour(__RGB(00, 0x8f, 0xbe), GLCD_COLOR_WHITE);
                            arm_lcd_printf("Arm Technologies.");
                            arm_lcd_text_set_scale(1.00f);
                        }

                        /* draw story */
                        __item_line_dock_vertical(0, 0, 50, 50) {
                            
                            arm_2d_fill_colour_with_vertical_alpha_gradient_and_opacity(ptTile, 
                                                                                        &__item_region, 
                                                                                        (__arm_2d_color_t){__RGB(00, 0x8f, 0xbe)}, 
                                                                                        128, 
                                                                                        (arm_2d_alpha_samples_2pts_t){
                                                                                            {128, 0}
                                                                                        });

                            arm_2d_dock(__item_region, 50) {
                                arm_lcd_text_set_draw_region(&__dock_region);
                                arm_lcd_text_set_font((arm_2d_font_t *)&ARM_2D_FONT_BradleyHand64_A8);
                                arm_lcd_text_set_scale(0.80f);
                                arm_lcd_text_set_colour(GLCD_COLOR_BLACK, GLCD_COLOR_WHITE);
                                arm_lcd_printf("%s", SYSTEM_CFG.Story.pchStory);
                                arm_lcd_text_set_scale(1.00f);
                            }
                        }

                    }

                }
            }

        }
    }

    ARM_2D_OP_WAIT_ASYNC();

    return arm_fsm_rt_cpl;
}

ARM_NONNULL(1)
user_scene_back_t *__arm_2d_scene_back_init(   arm_2d_scene_player_t *ptDispAdapter, 
                                        user_scene_back_t *ptThis)
{
    bool bUserAllocated = false;
    assert(NULL != ptDispAdapter);

    if (NULL == ptThis) {
        ptThis = (user_scene_back_t *)
                    __arm_2d_allocate_scratch_memory(   sizeof(user_scene_back_t),
                                                        __alignof__(user_scene_back_t),
                                                        ARM_2D_MEM_TYPE_UNSPECIFIED);
        assert(NULL != ptThis);
        if (NULL == ptThis) {
            return NULL;
        }
    } else {
        bUserAllocated = true;
    }

    memset(ptThis, 0, sizeof(user_scene_back_t));

    *ptThis = (user_scene_back_t){
        .use_as__arm_2d_scene_t = {

            /* the canvas colour */
            .tCanvas = {GLCD_COLOR_WHITE}, 

            /* Please uncommon the callbacks if you need them
             */
            .fnOnLoad       = &__on_scene_back_load,
            .fnScene        = &__pfb_draw_scene_back_handler,
            //.fnAfterSwitch  = &__after_scene_back_switching,

            /* if you want to use predefined dirty region list, please uncomment the following code */
            //.ptDirtyRegion  = (arm_2d_region_list_item_t *)s_tDirtyRegions,
            

            //.fnOnBGStart    = &__on_scene_back_background_start,
            //.fnOnBGComplete = &__on_scene_back_background_complete,
            .fnOnFrameStart = &__on_scene_back_frame_start,
            //.fnBeforeSwitchOut = &__before_scene_back_switching_out,
            .fnOnFrameCPL   = &__on_scene_back_frame_complete,
            .fnDepose       = &__on_scene_back_depose,

            .bUseDirtyRegionHelper = false,
        },
        .bUserAllocated = bUserAllocated,
    };

    /* ------------   initialize members of user_scene_back_t begin ---------------*/


    /* ------------   initialize members of user_scene_back_t end   ---------------*/

    arm_2d_scene_player_append_scenes(  ptDispAdapter, 
                                        &this.use_as__arm_2d_scene_t, 
                                        1);

    return ptThis;
}


#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

#endif


