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

#define __USER_SCENE_ONESIDE_IMPLEMENT__
#include "arm_2d_scene_oneside.h"

#include "Virtual_TFT_Port.h"

#if defined(RTE_Acceleration_Arm_2D_Helper_PFB)

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "postcard.h"

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

/*============================ GLOBAL VARIABLES ==============================*/

extern system_cfg_t SYSTEM_CFG;

extern 
const arm_2d_tile_t c_tileOnArmLogoCCCA8888;

extern
const arm_2d_tile_t c_tileEventLogoA4Mask;

extern const arm_2d_tile_t c_tileArmLogoMask;

extern 
const arm_2d_tile_t c_tileOnArmLogoMask;

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
} ARM_2D_FONT_Calibri_A8;

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
} ARM_2D_FONT_BradleyHand_A8;

extern
const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_Chalkboard_A8;

extern
const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_Chalkboard32_A8;

/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/

static void __on_scene_oneside_load(arm_2d_scene_t *ptScene)
{
    user_scene_oneside_t *ptThis = (user_scene_oneside_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

    text_box_on_load(&this.tStoryBoard);
}

static void __after_scene_oneside_switching(arm_2d_scene_t *ptScene)
{
    user_scene_oneside_t *ptThis = (user_scene_oneside_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}

static void __on_scene_oneside_depose(arm_2d_scene_t *ptScene)
{
    user_scene_oneside_t *ptThis = (user_scene_oneside_t *)ptScene;
    ARM_2D_UNUSED(ptThis);
    
    text_box_depose(&this.tStoryBoard);

    ARM_2D_OP_DEPOSE(this.tTransOP[0]);
    ARM_2D_OP_DEPOSE(this.tTransOP[1]);
    
    arm_foreach(int64_t,this.lTimestamp, ptItem) {
        *ptItem = 0;
    }
    ptScene->ptPlayer = NULL;
    if (!this.bUserAllocated) {
        __arm_2d_free_scratch_memory(ARM_2D_MEM_TYPE_UNSPECIFIED, ptScene);
    }
}

/*----------------------------------------------------------------------------*
 * Scene back                                                                    *
 *----------------------------------------------------------------------------*/

static void __on_scene_oneside_background_start(arm_2d_scene_t *ptScene)
{
    user_scene_oneside_t *ptThis = (user_scene_oneside_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}

static void __on_scene_oneside_background_complete(arm_2d_scene_t *ptScene)
{
    user_scene_oneside_t *ptThis = (user_scene_oneside_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}


static void __on_scene_oneside_frame_start(arm_2d_scene_t *ptScene)
{
    user_scene_oneside_t *ptThis = (user_scene_oneside_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

    text_box_on_frame_start(&this.tStoryBoard);

}

static void __on_scene_oneside_frame_complete(arm_2d_scene_t *ptScene)
{
    user_scene_oneside_t *ptThis = (user_scene_oneside_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

    text_box_on_frame_complete(&this.tStoryBoard);

    if (!this.bFinishedDrawing) {
        if (SYSTEM_CFG.Input.bAutoScaling) {
            int64_t iStoryHeight = text_box_get_line_count(&this.tStoryBoard, 0) * text_box_get_line_height(&this.tStoryBoard);

            float fScale = text_box_get_scale(&this.tStoryBoard);
            if (iStoryHeight > (int64_t) this.iStoryBoxHeight) {
                text_box_set_scale(&this.tStoryBoard, fScale - 0.01f);
            } else {
                int16_t iDelta = this.iStoryBoxHeight - iStoryHeight;
                if (iDelta < 100) {
                    text_box_set_scale(&this.tStoryBoard, fScale - 0.01f);
                } else if (iDelta < 200) {
                    this.bFinishedDrawing = true;
                } else {
                    text_box_set_scale(&this.tStoryBoard, fScale + 0.01f);
                }
            }

        } else {
            this.bFinishedDrawing = true;
        }
    } else {
        VT_save_screenshot(SYSTEM_CFG.Output.chBackFileName);
        VT_request_quit();
    }


}

static void __before_scene_oneside_switching_out(arm_2d_scene_t *ptScene)
{
    user_scene_oneside_t *ptThis = (user_scene_oneside_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}

static void __draw_footnote(user_scene_oneside_t *ptThis, 
                            const arm_2d_tile_t *ptTile, 
                            arm_2d_region_t *ptRegion, 
                            bool bIsNewFrame)
{
    assert(NULL != ptThis);
    assert(NULL != ptTile);
    assert(NULL != ptRegion);

    __arm_2d_layout_align_tab_t tAlignPolict = ARM_2D_LAYOUT_ALIGN_RIGHT_TO_LEFT_TOP_DOWN;
    switch (SYSTEM_CFG.Input.u2FootnoteLogoAlign) {
        default:
        case FOOTNOTE_ALIGN_DEFAULT:
        case FOOTNOTE_ALIGN_RIGHT:
            break;
        case FOOTNOTE_ALIGN_LEFT:
            tAlignPolict = ARM_2D_LAYOUT_ALIGN_LEFT_TO_RIGHT_TOP_DOWN;
            break;
        case FOOTNOTE_ALIGN_MIDDLE:
            break;
    }
    
    arm_2d_region_t tFootNoteRegion = *ptRegion;
    if (SYSTEM_CFG.Input.u2FootnoteLogoAlign == FOOTNOTE_ALIGN_MIDDLE) {

        int16_t iContentWidth = 0;
        if (NULL != SYSTEM_CFG.Input.pchLogoPath) {
            
            int16_t iEventLogoWidthAllowance = ptRegion->tSize.iWidth;
            if (!SYSTEM_CFG.Input.bHideDefaultLogo) {
                iEventLogoWidthAllowance -= c_tileArmLogoMask.tRegion.tSize.iWidth;
                iContentWidth += c_tileArmLogoMask.tRegion.tSize.iWidth + 50;
            }

            float fScale = (float)ptRegion->tSize.iHeight 
                            / (float)SYSTEM_CFG.Logo.tTile.tRegion.tSize.iHeight;

            arm_2d_size_t tLogoSize = {
                .iWidth = (float)SYSTEM_CFG.Logo.tTile.tRegion.tSize.iWidth * fScale + 2,
                .iHeight = ptRegion->tSize.iHeight,
            };

            if (tLogoSize.iWidth > iEventLogoWidthAllowance) {
                /* recalculate*/

                fScale = (float)iEventLogoWidthAllowance 
                        / (float)SYSTEM_CFG.Logo.tTile.tRegion.tSize.iWidth;

                tLogoSize.iWidth = iEventLogoWidthAllowance;
                tLogoSize.iHeight = (float)SYSTEM_CFG.Logo.tTile.tRegion.tSize.iHeight * fScale;

            }

            iContentWidth += tLogoSize.iWidth;
        } else if (!SYSTEM_CFG.Input.bHideDefaultLogo) {
            iContentWidth = c_tileArmLogoMask.tRegion.tSize.iWidth;
        }

        arm_2d_dock_horizontal_open(*ptRegion, iContentWidth) {
            tFootNoteRegion = __horizontal_region;
        }
    }


    arm_2d_layout_with_align_mode(tFootNoteRegion, tAlignPolict) {

        if (!SYSTEM_CFG.Input.bHideDefaultLogo) {
            __item_line_dock_horizontal(c_tileArmLogoMask.tRegion.tSize.iWidth) {

                arm_2d_align_centre(__item_region, c_tileArmLogoMask.tRegion.tSize) {
                    arm_2d_fill_colour_with_mask_and_opacity(
                            ptTile,
                            &__centre_region,
                            &c_tileArmLogoMask,
                            (__arm_2d_color_t) {GLCD_COLOR_BLACK},
                            255 - 32);
                }
            }
            /* padding */
            __item_line_dock_horizontal(50) {
            }
        }

        if (NULL != SYSTEM_CFG.Input.pchLogoPath) {
            __item_line_dock_horizontal() {

                float fScale = (float)__item_region.tSize.iHeight 
                            / (float)SYSTEM_CFG.Logo.tTile.tRegion.tSize.iHeight;

                arm_2d_size_t tLogoSize = {
                    .iWidth = (float)SYSTEM_CFG.Logo.tTile.tRegion.tSize.iWidth * fScale + 2,
                    .iHeight = __item_region.tSize.iHeight,
                };

                if (tLogoSize.iWidth > __item_region.tSize.iWidth) {
                    /* recalculate*/

                    fScale = (float)__item_region.tSize.iWidth 
                            / (float)SYSTEM_CFG.Logo.tTile.tRegion.tSize.iWidth;

                    tLogoSize.iWidth = __item_region.tSize.iWidth;
                    tLogoSize.iHeight = (float)SYSTEM_CFG.Logo.tTile.tRegion.tSize.iHeight * fScale;

                }

                arm_2d_layout_with_align_mode(__item_region, tAlignPolict) {

                    __item_line_dock_horizontal(tLogoSize.iWidth) {
                        arm_2d_location_t tImageCentre = {
                            .iX = SYSTEM_CFG.Logo.tTile.tRegion.tSize.iWidth / 2,
                            .iY = SYSTEM_CFG.Logo.tTile.tRegion.tSize.iHeight / 2,
                        };

                        arm_2dp_tile_transform_with_src_mask_and_opacity(
                            &this.tTransOP[1],
                            &SYSTEM_CFG.Logo.tTile,
                            &SYSTEM_CFG.Logo.tMaskTile,
                            ptTile,
                            &__item_region,
                            tImageCentre,
                            0.0f,
                            fScale,
                            255);

                        ARM_2D_OP_WAIT_ASYNC(&this.tTransOP[1]);
                    }
                }
            }
        }
    }
         
}

static
IMPL_PFB_ON_DRAW(__pfb_draw_scene_oneside_handler)
{
    ARM_2D_PARAM(pTarget);
    ARM_2D_PARAM(ptTile);
    ARM_2D_PARAM(bIsNewFrame);

    user_scene_oneside_t *ptThis = (user_scene_oneside_t *)pTarget;
    arm_2d_size_t tScreenSize = ptTile->tRegion.tSize;

    ARM_2D_UNUSED(tScreenSize);

    arm_2d_canvas(ptTile, __top_canvas) {


        arm_2d_layout(__top_canvas, BOTTOM_UP) {
            if (SYSTEM_CFG.Input.iFootnoteBoxHeight > 0) {
                __item_line_dock_vertical(SYSTEM_CFG.Input.iFootnoteBoxHeight) {

                    arm_2d_layout(__item_region) {
                        __item_line_dock_vertical(30) {
                            arm_2d_fill_colour_with_vertical_alpha_gradient(
                                ptTile,
                                &__item_region, 
                                (__arm_2d_color_t){GLCD_COLOR_BLACK}, 
                                (arm_2d_alpha_samples_2pts_t){0, 64});
                        }

                        __item_line_dock_vertical(120, 120, 0, 0) {
                            arm_2d_dock_vertical(__item_region, SYSTEM_CFG.Input.iFootnoteHeight) {
                                __draw_footnote(ptThis, ptTile, &__vertical_region, bIsNewFrame);
                            }
                        }
                    }
                }
            }

            __item_line_dock_vertical(120, 120, 100, 100) {

                arm_2d_layout(__item_region) {
                    
                    __item_line_dock_vertical(1024) {


                        arm_2d_location_t tImageCentre = {
                            .iX = SYSTEM_CFG.Picture.tTile.tRegion.tSize.iWidth / 2,
                            .iY = SYSTEM_CFG.Picture.tTile.tRegion.tSize.iHeight / 2,
                        };

                        float fScaleW = (float)__item_region.tSize.iWidth / (float)SYSTEM_CFG.Picture.tTile.tRegion.tSize.iWidth;
                        float fScaleH = (float)__item_region.tSize.iHeight / (float)SYSTEM_CFG.Picture.tTile.tRegion.tSize.iHeight;
                        float fScale = MIN(fScaleW, fScaleH);

                        arm_2dp_tile_transform_with_src_mask_and_opacity(
                            &this.tTransOP[0],
                            &SYSTEM_CFG.Picture.tTile,
                            &SYSTEM_CFG.Picture.tMaskTile,
                            ptTile,
                            &__item_region,
                            tImageCentre,
                            0.0f,
                            fScale,
                            255);
                        ARM_2D_OP_WAIT_ASYNC(&this.tTransOP[0]);
                    }

                    if (SYSTEM_CFG.Input.pchEventName != NULL) {
                        __item_line_dock_vertical(46, 0, 0, 50, 0) {
                            arm_lcd_text_set_target_framebuffer(ptTile);
                            
                            arm_lcd_text_set_draw_region(&__item_region);
                            arm_lcd_text_set_colour(GLCD_COLOR_BLACK, GLCD_COLOR_WHITE);
                            arm_lcd_text_set_font((arm_2d_font_t *)&ARM_2D_FONT_CalibriBold46_A8);
                            //arm_lcd_text_set_scale(1.1f);

                            time_t now;
                            time(&now);
                            struct tm *ptTimeinfo = localtime(&now);
                            static const char *c_chMonths[] = {
                                "January",
                                "February",
                                "March",
                                "April",
                                "May",
                                "June",
                                "July",
                                "August",
                                "September",
                                "October",
                                "November",
                                "December"
                            };
                            
                            arm_lcd_printf_label(ARM_2D_ALIGN_MIDDLE_LEFT, 
                                                "%d %s %04d, %s",
                                                ptTimeinfo->tm_mday,
                                                c_chMonths[ptTimeinfo->tm_mon],
                                                ptTimeinfo->tm_year + 1900,
                                                SYSTEM_CFG.Input.pchEventName
                                                );
                            //arm_lcd_text_set_scale(1.0f);
                        }
                    } else {
                        __item_line_dock_vertical(70) {}
                    }

                    __item_line_dock_vertical() {

                        arm_2d_layout(__item_region, BOTTOM_UP) {

                            if (SYSTEM_CFG.Input.iFootnoteBoxHeight <=0) {
                                /* draw footnote */

                                __item_line_dock_vertical(SYSTEM_CFG.Input.iFootnoteHeight) {
                                    __draw_footnote(ptThis, ptTile, &__item_region, bIsNewFrame);
                                }
                            }

                            /* draw story */
                            __item_line_dock_vertical(0, 0, 10, 20) {
                            
                                if (!SYSTEM_CFG.Input.bNoBackgroundColour) {
                                    arm_2d_fill_colour_with_vertical_alpha_gradient_and_opacity(
                                        ptTile, 
                                        &__item_region, 
                                        (__arm_2d_color_t){__RGB(00, 0x8f, 0xbe)}, 
                                        128, 
                                        (arm_2d_alpha_samples_2pts_t){
                                            {128, 0}
                                        });
                                }

                                if (SYSTEM_CFG.Input.iStoryBoardMargin > 0) {
                                    arm_2d_helper_draw_box(ptTile, &__item_region, 4, GLCD_COLOR_BLACK, 64);

                                    arm_2d_dock(__item_region, SYSTEM_CFG.Input.iStoryBoardMargin) {

                                        if (bIsNewFrame) {
                                            this.iStoryBoxHeight = __dock_region.tSize.iHeight;
                                        }

                                        text_box_show(  &this.tStoryBoard, 
                                                        ptTile, 
                                                        &__dock_region,
                                                        (__arm_2d_color_t) {GLCD_COLOR_BLACK},
                                                        255,
                                                        bIsNewFrame);
                                    }
                                } else {
                                    arm_2d_dock(__item_region, 0, 0, 10, 0) {

                                        if (bIsNewFrame) {
                                            this.iStoryBoxHeight = __dock_region.tSize.iHeight;
                                        }

                                        text_box_show(  &this.tStoryBoard, 
                                                        ptTile, 
                                                        &__dock_region,
                                                        (__arm_2d_color_t) {GLCD_COLOR_BLACK},
                                                        255,
                                                        bIsNewFrame);
                                    }
                                }

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
user_scene_oneside_t *__arm_2d_scene_oneside_init(   arm_2d_scene_player_t *ptDispAdapter, 
                                        user_scene_oneside_t *ptThis)
{
    bool bUserAllocated = false;
    assert(NULL != ptDispAdapter);

    if (NULL == ptThis) {
        ptThis = (user_scene_oneside_t *)
                    __arm_2d_allocate_scratch_memory(   sizeof(user_scene_oneside_t),
                                                        __alignof__(user_scene_oneside_t),
                                                        ARM_2D_MEM_TYPE_UNSPECIFIED);
        assert(NULL != ptThis);
        if (NULL == ptThis) {
            return NULL;
        }
    } else {
        bUserAllocated = true;
    }

    memset(ptThis, 0, sizeof(user_scene_oneside_t));

    *ptThis = (user_scene_oneside_t){
        .use_as__arm_2d_scene_t = {

            /* the canvas colour */
            .tCanvas = {GLCD_COLOR_WHITE}, 

            /* Please uncommon the callbacks if you need them
             */
            .fnOnLoad       = &__on_scene_oneside_load,
            .fnScene        = &__pfb_draw_scene_oneside_handler,
            //.fnAfterSwitch  = &__after_scene_oneside_switching,

            /* if you want to use predefined dirty region list, please uncomment the following code */
            //.ptDirtyRegion  = (arm_2d_region_list_item_t *)s_tDirtyRegions,
            

            //.fnOnBGStart    = &__on_scene_oneside_background_start,
            //.fnOnBGComplete = &__on_scene_oneside_background_complete,
            .fnOnFrameStart = &__on_scene_oneside_frame_start,
            //.fnBeforeSwitchOut = &__before_scene_oneside_switching_out,
            .fnOnFrameCPL   = &__on_scene_oneside_frame_complete,
            .fnDepose       = &__on_scene_oneside_depose,

            .bUseDirtyRegionHelper = false,
        },
        .bUserAllocated = bUserAllocated,
    };

    /* ------------   initialize members of user_scene_oneside_t begin ---------------*/
    /* initialize textbox */
    do {
        text_box_c_str_reader_init( &this.tCStringReader,
                                    SYSTEM_CFG.Story.pchStory,
                                    SYSTEM_CFG.Story.tSize);

        text_box_cfg_t tCFG = {
            .ptFont = (arm_2d_font_t *)&ARM_2D_FONT_Chalkboard32_A8,
            .tStreamIO = {
                .ptIO       = &TEXT_BOX_IO_C_STRING_READER,
                .pTarget    = (uintptr_t)&this.tCStringReader,
            },
            .u2LineAlign = TEXT_BOX_LINE_ALIGN_JUSTIFIED,
            .fScale = 0.85f * SYSTEM_CFG.Input.fScale,
            .chSpaceBetweenParagraph = 10,
            .tMargin = {5, 0, 0, 0},

            .ptScene = (arm_2d_scene_t *)ptThis,
        };

        text_box_init(&this.tStoryBoard, &tCFG);
    } while(0);

    ARM_2D_OP_INIT(this.tTransOP[0]);
    ARM_2D_OP_INIT(this.tTransOP[1]);
    /* ------------   initialize members of user_scene_oneside_t end   ---------------*/

    arm_2d_scene_player_append_scenes(  ptDispAdapter, 
                                        &this.use_as__arm_2d_scene_t, 
                                        1);

    return ptThis;
}


#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

#endif


