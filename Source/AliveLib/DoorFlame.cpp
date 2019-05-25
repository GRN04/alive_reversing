#include "stdafx.h"
#include "DoorFlame.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "ObjectIds.hpp"
#include "Midi.hpp"
#include "Game.hpp"
#include "ScreenManager.hpp"

ALIVE_VAR(1, 0x5C2C6C, DoorFlame*, pFlameControllingTheSound_5C2C6C, nullptr);

struct FlameSpark
{
    FP field_0_x;
    FP field_4_y;
    FP field_8_off_x;
    FP field_C_off_y;
    __int16 field_10_random64;
    __int16 field_12_bVisible;
    AnimationUnknown field_14;
};
ALIVE_ASSERT_SIZEOF(FlameSpark, 0x84);

// These flame sparks are extremely subtle and are easily missed!
class FlameSparks : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT FlameSparks* ctor_45DE00(FP xpos, FP ypos)
    {
        BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
        for (auto& anim : field_F8_sparks)
        {
            SetVTable(&anim.field_14, 0x5447CC);
        }

        SetVTable(this, 0x545974);
        field_4_typeId = Types::eNone_0;

        BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 312);
        Animation_Init_424E10(1672, 39, 21, ppRes, 1, 1);
        field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

        field_DC_bApplyShadows |= 1u;
        field_20_animation.field_C_render_layer = 17;

        field_B8_xpos = xpos;
        field_BC_ypos = ypos;
        field_410_xpos = xpos;
        field_414_ypos = ypos;

        field_CC_sprite_scale = FP_FromDouble(0.3);

        for (auto& anim : field_F8_sparks)
        {
            anim.field_14.field_68_anim_ptr = &field_20_animation;

            anim.field_14.field_4_flags.Set(AnimFlags::eBit3_Render);
            anim.field_14.field_4_flags.Set(AnimFlags::eBit16_bBlending);

            anim.field_14.field_C_render_layer = field_20_animation.field_C_render_layer + Math_RandomRange_496AB0(-1, 1);
            anim.field_14.field_6C_scale = field_CC_sprite_scale;

            anim.field_0_x = field_B8_xpos;
            anim.field_4_y = field_BC_ypos;

            anim.field_8_off_x = FP_FromInteger(0);
            anim.field_C_off_y = FP_FromInteger(0);

            anim.field_10_random64 = Math_RandomRange_496AB0(0, 64);
            anim.field_12_bVisible = 0;
        }

        field_F4_bRender = 0;
        return this;
    }

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return vdtor_45DF90(flags);
    }

    virtual void VUpdate() override
    {
        vUpdate_45DFE0();
    }

    virtual void VRender(int** pOrderingTable) override
    {
        vRender_45E260(pOrderingTable);
    }

private:
    EXPORT void vUpdate_45DFE0()
    {
        // HACK/WTF this seems to move the base animation off screen so it can never been seen??
        PSX_RECT rect = {};
        gMap_5C3030.Get_Camera_World_Rect_481410(CameraPos::eCamCurrent_0, &rect);
        field_B8_xpos = FP_FromInteger(rect.w + 16);
        field_BC_ypos = FP_FromInteger(rect.y - 16);

        if (field_F4_bRender)
        {
            for (auto& anim : field_F8_sparks)
            {
                anim.field_10_random64--;
                if (anim.field_12_bVisible == 0)
                {
                    if (anim.field_10_random64 <= 0)
                    {
                        anim.field_12_bVisible = 1;
                        anim.field_10_random64 = Math_RandomRange_496AB0(7, 9);

                        anim.field_0_x = field_410_xpos;
                        anim.field_4_y = field_414_ypos;

                        anim.field_8_off_x = (FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(96));
                        anim.field_C_off_y = (FP_FromInteger(-Math_NextRandom()) / FP_FromInteger(96)); // TODO: Check this is right ??
                    }
                }
                else if (anim.field_10_random64 > 0)
                {
                    anim.field_0_x += anim.field_8_off_x;
                    anim.field_4_y += anim.field_C_off_y;

                    if (!(anim.field_10_random64 % 3))
                    {
                        anim.field_8_off_x += (FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64));
                        anim.field_C_off_y += (FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64));
                    }
                }
                else
                {
                    anim.field_12_bVisible = 0;
                    anim.field_10_random64 = Math_RandomRange_496AB0(90, 240);
                }
            }
        }
    }

    EXPORT void vRender_45E260(int **pOt)
    {
        if (sNum_CamSwappers_5C1B66 == 0)
        {
            if (field_F4_bRender)
            {
                field_20_animation.field_8_r = 240;
                field_20_animation.field_9_g = 32;
                field_20_animation.field_A_b = 32;

                field_20_animation.vRender_40B820(
                        FP_GetExponent(field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x),
                        FP_GetExponent(field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y),
                        pOt,
                        0,
                        0);

                PSX_RECT frameRect = {};
                field_20_animation.Get_Frame_Rect_409E10(&frameRect);
                pScreenManager_5BB5F4->InvalidateRect_40EC90(
                    frameRect.x,
                    frameRect.y,
                    frameRect.w,
                    frameRect.h,
                    pScreenManager_5BB5F4->field_3A_idx);

                for (auto& anim : field_F8_sparks)
                {
                    // Visible?
                    if (anim.field_12_bVisible)
                    {
                        // And in screen bounds?
                        if (anim.field_0_x >= pScreenManager_5BB5F4->field_20_pCamPos->field_0_x &&
                            anim.field_0_x <= pScreenManager_5BB5F4->field_20_pCamPos->field_0_x + FP_FromInteger(368))
                        {
                            if (anim.field_4_y >= pScreenManager_5BB5F4->field_20_pCamPos->field_4_y && 
                                anim.field_4_y <= pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + FP_FromInteger(240))
                            {
                                anim.field_14.vRender_40B820(
                                    FP_GetExponent(anim.field_0_x - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x),
                                    FP_GetExponent(anim.field_4_y - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y),
                                    pOt,
                                    0,
                                    0);

                                anim.field_14.GetRenderedSize_40C980(&frameRect);
                                pScreenManager_5BB5F4->InvalidateRect_40EC90(
                                    frameRect.x,
                                    frameRect.y,
                                    frameRect.w,
                                    frameRect.h,
                                    pScreenManager_5BB5F4->field_3A_idx);
                            }
                        }
                    }
                }
            }
        }
    }

    EXPORT void SetRenderEnabled_45E240(__int16 bEnable)
    {
        field_F4_bRender = bEnable;
    }

    EXPORT void dtor_45DFC0()
    {
        SetVTable(this, 0x545974);
        BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
    }

    EXPORT FlameSparks* vdtor_45DF90(signed int flags)
    {
        dtor_45DFC0();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
        return this;
    }


private:
    int field_E4_not_used[4];
    __int16 field_F4_bRender;
    //__int16 field_F6_pad;
    FlameSpark field_F8_sparks[6];
    FP field_410_xpos;
    FP field_414_ypos;
};
ALIVE_ASSERT_SIZEOF(FlameSparks, 0x418);

DoorFlame* DoorFlame::ctor_45E460(Path_DoorFlame* pTlv, int tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x5459BC);

    field_4_typeId = Types::eNone_0;
    field_F4_tlvInfo = tlvInfo;
    
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 304);
    Animation_Init_424E10(5156, 22, 21u, ppRes, 1, 1u);

    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_DC_bApplyShadows |= 1u;
    field_20_animation.field_C_render_layer = 17;
    field_FA_frame_count = field_20_animation.Get_Frame_Count_40AC70();
    field_20_animation.SetFrame_409D50(Math_RandomRange_496AB0(0, field_FA_frame_count - 1));
    
    field_F8_switch_id = pTlv->field_10_id;

    if (pTlv->field_12_scale)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
    }

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x)  + (FP_FromInteger(12) * field_CC_sprite_scale);
    field_108 = -1;
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y) + (FP_FromInteger(15) * field_CC_sprite_scale);

    if (SwitchStates_Get_466020(field_F8_switch_id))
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
        field_FC_bOn = 1;
    }
    else
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_FC_bOn = 0;
    }
    
    field_FE_2_random = Math_NextRandom() % 2;
    
    auto pFlameSparks = alive_new<FlameSparks>();
    if (pFlameSparks)
    {
        pFlameSparks->ctor_45DE00(field_B8_xpos, field_BC_ypos);
        field_10C_flame_sparks_id = pFlameSparks->field_8_object_id;
    }
    
    return this;
}

DoorFlame* DoorFlame::vdtor_45E690(signed int flags)
{
    dtor_45E6C0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void DoorFlame::dtor_45E6C0()
{
    SetVTable(this, 0x5459BC);

    BaseGameObject* v2 = sObjectIds_5C1B70.Find_449CF0(field_108);
    BaseGameObject* pFlameSparks = sObjectIds_5C1B70.Find_449CF0(field_10C_flame_sparks_id);
    
    if (v2)
    {
        v2->field_6_flags.Set(BaseGameObject::eDead);
        field_108 = -1;
    }

    if (pFlameSparks)
    {
        pFlameSparks->field_6_flags.Set(BaseGameObject::eDead);
        field_10C_flame_sparks_id = -1;
    }

    if (pFlameControllingTheSound_5C2C6C == this)
    {
        pFlameControllingTheSound_5C2C6C = 0;
        SND_Stop_Channels_Mask_4CA810(field_100_sounds_mask);
    }

    Path::TLV_Reset_4DB8E0(field_F4_tlvInfo, -1, 0, 0);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void DoorFlame::vStopAudio_45E7E0()
{
    if (pFlameControllingTheSound_5C2C6C == this)
    {
        pFlameControllingTheSound_5C2C6C = nullptr;
        SND_Stop_Channels_Mask_4CA810(field_100_sounds_mask);
        field_100_sounds_mask = 0;
    }
}

void DoorFlame::vScreenChanged_45EA90()
{
    BaseGameObject* v2 = sObjectIds_5C1B70.Find_449CF0(field_108);
    BaseGameObject* pFlameSparks = sObjectIds_5C1B70.Find_449CF0(field_10C_flame_sparks_id);

    field_6_flags.Set(BaseGameObject::eDead);
    
    if (v2)
    {
        v2->field_6_flags.Set(BaseGameObject::eDead);
        field_108 = -1;
    }

    if (pFlameSparks)
    {
        pFlameSparks->field_6_flags.Set(BaseGameObject::eDead);
        field_10C_flame_sparks_id = -1;
    }
}