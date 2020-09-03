#include "stdafx_ao.h"
#include "Function.hpp"
#include "Slog.hpp"
#include "Abe.hpp"
#include "MusicController.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Midi.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "Collisions.hpp"
#include "Blood.hpp"
#include "Sfx.hpp"
#include "PsxDisplay.hpp"
#include "Blood.hpp"
#include "Gibs.hpp"
#include "Bullet.hpp"
#include "SwitchStates.hpp"
#include "PlatformBase.hpp"
#include "CameraSwapper.hpp"
#include "SnoozeParticle.hpp"

void Slog_ForceLink() {}

START_NS_AO

struct Path_EnemyStopper : public Path_TLV
{
    __int16 field_18_direction;
    __int16 field_1A_id;
};

using TSlogStateFunction = decltype(&Slog::State_0_Idle_4742E0);

const TSlogStateFunction sSlogMotionTable_4CFD30[] =
{
    &Slog::State_0_Idle_4742E0,
    &Slog::State_1_Walk_4743F0,
    &Slog::State_2_Run_4749A0,
    &Slog::State_3_TurnAround_474C70,
    &Slog::State_4_Fall_4750C0,
    &Slog::State_5_Unknown_474070,
    &Slog::State_6_MoveHeadUpwards_474220,
    &Slog::State_7_SlideTurn_474DB0,
    &Slog::State_8_StopRunning_474EC0,
    &Slog::State_9_StartWalking_474690,
    &Slog::State_10_EndWalking_4747D0,
    &Slog::State_11_Land_475AB0,
    &Slog::State_12_Unknown_475B50,
    &Slog::State_13_StartFastBarking_475B70,
    &Slog::State_14_EndFastBarking_475BB0,
    &Slog::State_15_Empty_475290,
    &Slog::State_16_Sleeping_4752E0,
    &Slog::State_17_FallAsleep_475510,
    &Slog::State_18_WakeUp_475460,
    &Slog::State_19_JumpForwards_475610,
    &Slog::State_20_JumpUpwards_475890,
    &Slog::State_21_Eating_475900,
    &Slog::State_22_Empty_475A90,
    &Slog::State_23_Scratch_475550,
    &Slog::State_24_Growl_475590,
};

const int dword_4CFDD8[10] = { 14784, 14832, 14808, 37952, 38008, 38112, 12220, 12412, 12500, 0 };


ALIVE_VAR(1, 0x9F11C8, short, gNumSlogs_9F11C8, 0);
ALIVE_VAR(1, 0x9F11C4, BYTE, sSlogRndSeed_9F11C4, 0);

static BYTE Slog_NextRandom()
{
    return sRandomBytes_4BBE30[sSlogRndSeed_9F11C4++];
}

Slog* Slog::ctor_472EE0(Path_Slog* pTlv, int tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BCBC8);

    field_148 = -1;

    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);

    if (pTlv->field_18_scale == 0)
    {
        field_BC_sprite_scale = FP_FromInteger(1);
    }
    else
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
    }

    Init_473130();

    field_138_tlvInfo = tlvInfo;
    field_114_brain_idx = 1;
    field_176 = 1;

    field_158_bark_anger = pTlv->field_1E_bark_anger;

    field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, pTlv->field_1A_direction == 0);

    field_15A = pTlv->field_1E_bark_anger + pTlv->field_20_sleeps + pTlv->field_22_chase_anger;
    field_15C = pTlv->field_1E_bark_anger + pTlv->field_20_sleeps + pTlv->field_22_chase_anger;
    field_10C = 0;
    field_17E = pTlv->field_1C_wakeup_anger;
    field_170 = pTlv->field_24_jump_attack_delay;
    field_168 = pTlv->field_28_anger_trigger_id;

    if (pTlv->field_1C_wakeup_anger)
    {
        field_FC_current_motion = eSlogStates::State_16_Sleeping_4752E0;
        field_13C_res_idx = 1;
        field_10_anim.Set_Animation_Data_402A40(dword_4CFDD8[0], field_180_resources[2]);
    }
    else
    {
        field_FC_current_motion = eSlogStates::State_0_Idle_4742E0;
    }
    return this;
}

Slog* Slog::ctor_473050(FP xpos, FP ypos, FP scale)
{
    ctor_401090();
    SetVTable(this, 0x4BCBC8);

    field_148 = -1;

    field_A8_xpos = xpos;
    field_AC_ypos = ypos;
    field_BC_sprite_scale = scale;

    Init_473130();

    field_116_brain_state = 0;

    field_10C = sControlledCharacter_50767C;
    field_176 = 0;
    sControlledCharacter_50767C->field_C_refCount++;
    field_17E = 0;
    field_158_bark_anger = 0;

    field_170 = 0;
    field_168 = 0;
    field_FC_current_motion = 0;
    field_138_tlvInfo = 0xFFFF;
    field_114_brain_idx = 2;
    field_15A = 10;
    field_15C = 20;

    return this;
}

BaseGameObject* Slog::VDestructor(signed int flags)
{
    return Vdtor_473CB0(flags);
}

Slog* Slog::Vdtor_473CB0(signed int flags)
{
    dtor_473370();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* Slog::dtor_473370()
{
    SetVTable(this, 0x4BCBC8);

    if (field_10C)
    {
        field_10C->field_C_refCount--;
        field_10C = nullptr;
    }

    if (field_14C)
    {
        field_14C->field_C_refCount--;
        field_14C = nullptr;
    }

    if (field_16C)
    {
        field_16C->field_C_refCount--;
        field_16C = nullptr;
    }

    if (field_138_tlvInfo != 0xFFFF)
    {
        if (field_100_health <= FP_FromInteger(0))
        {
            gMap_507BA8.TLV_Reset_446870(field_138_tlvInfo, -1, 0, 1);
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(field_138_tlvInfo, -1, 0, 0);
        }
    }

    for (int i = 1; i < ALIVE_COUNTOF(field_180_resources); i++)
    {
        if (field_10_anim.field_20_ppBlock != field_180_resources[i])
        {
            if (field_180_resources[i])
            {
                ResourceManager::FreeResource_455550(field_180_resources[i]);
                field_180_resources[i] = nullptr;
            }
        }
    }

    MusicController::sub_443810(MusicController::MusicTypes::eType0, this, 0, 0);

    if (!field_178)
    {
        gNumSlogs_9F11C8--;
    }

    return dtor_401000();
}

__int16 Slog::VTakeDamage(BaseGameObject* pFrom)
{
    return VTakeDamage_473610(pFrom);
}

__int16 Slog::VTakeDamage_473610(BaseGameObject* pFrom)
{
    if (field_100_health <= FP_FromInteger(0))
    {
        return 1;
    }

    switch (pFrom->field_4_typeId)
    {
    case Types::eBullet_10:
    {
        auto pBullet = static_cast<Bullet*>(pFrom);

        if (pBullet->field_20 <= FP_FromInteger(0))
        {
            auto pBlood = ao_new<Blood>();
            if (pBlood)
            {
                pBlood->ctor_4072B0(
                    field_A8_xpos,
                    pBullet->field_1C_ypos,
                    -FP_FromInteger(24),
                    FP_FromInteger(0),
                    field_BC_sprite_scale,
                    50);
            }
        }
        else
        {
            auto pBlood = ao_new<Blood>();
            if (pBlood)
            {
                pBlood->ctor_4072B0(
                    field_A8_xpos,
                    pBullet->field_1C_ypos,
                    FP_FromInteger(24),
                    FP_FromInteger(0),
                    field_BC_sprite_scale,
                    50);
            }
        }

        Sfx_475BD0(9);

        field_100_health = FP_FromInteger(0);
        field_114_brain_idx = 3;
        field_134 = 2;
        field_11C = gnFrameCount_507670 + 90;
        field_FC_current_motion = eSlogStates::State_22_Empty_475A90;
        field_13C_res_idx = 3;
        field_10_anim.Set_Animation_Data_402A40(dword_4CFDD8[6], field_180_resources[4]);
        field_10_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
        gNumSlogs_9F11C8--;
        field_178 = 1;
        return 1;
    }

    case Types::eBaseBomb_30:
    case Types::eMeatSaw_56:
    case Types::eExplosion_74:
    {
        Sfx_475BD0(9);
        field_100_health = FP_FromInteger(0);
        auto pGibs = ao_new<Gibs>();
        if (pGibs)
        {
            pGibs->ctor_407B20(
                2,
                field_A8_xpos,
                field_AC_ypos,
                field_B4_velx,
                field_B8_vely,
                field_BC_sprite_scale);
        }

        PSX_RECT bRect = {};
        VGetBoundingRect(&bRect, 1);
        auto pBlood = ao_new<Blood>();
        if (pBlood)
        {
            pBlood->ctor_4072B0(
                FP_FromInteger((bRect.w + bRect.x) / 2),
                FP_FromInteger((bRect.h + bRect.y) / 2),
                FP_FromInteger(0),
                FP_FromInteger(0),
                field_BC_sprite_scale,
                50);
        }
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return 1;
    }

    case Types::eAbilityRing_69:
    case Types::eSlig_88:
        if (field_17C)
        {
            return 1;
        }
        field_17C = 1;
        Sfx_475BD0(9);
        break;

    case Types::eZBall_92:
        return 1;

    case Types::eRockSpawner_32:
    case Types::eRollingBall_72:
        Slog::Sfx_475BD0(9);
        // Fall through

    case Types::eElectrocute_103:
        field_100_health = FP_FromInteger(0);
        field_114_brain_idx = 3;
        field_134 = 2;
        field_11C = gnFrameCount_507670 + 90;
        field_FC_current_motion = eSlogStates::State_22_Empty_475A90;
        field_13C_res_idx = 3;
        field_10_anim.Set_Animation_Data_402A40(dword_4CFDD8[6], field_180_resources[4]);
        field_10_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
        break;

    default:
        Sfx_475BD0(9);
        break;
    }
    return 1;
}

void Slog::VOn_TLV_Collision(Path_TLV* pTlv)
{
    VOn_Tlv_Collision_473970(pTlv);
}

void Slog::VOn_Tlv_Collision_473970(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::DeathDrop_5)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            field_100_health = FP_FromInteger(0);
        }
        pTlv = gMap_507BA8.TLV_Get_At_446060(pTlv, field_A8_xpos, field_AC_ypos, field_A8_xpos, field_AC_ypos);
    }
}

void Slog::VUpdate_Real_4739C0()
{
    NOT_IMPLEMENTED();
}

void Slog::VUpdate_4739C0()
{
    const __int16 oldMotion = field_FC_current_motion;

    VUpdate_Real_4739C0();

    if (oldMotion != field_FC_current_motion)
    {
        LOG_INFO("oldMotion = " << oldMotion << " newMotion = " << field_FC_current_motion);
    }
}

void Slog::Init_473130()
{
    NOT_IMPLEMENTED();
}

__int16 Slog::ToNextMotion_473CE0()
{
    switch (field_FE_next_state)
    {
    case eSlogStates::State_3_TurnAround_474C70:
        field_FC_current_motion = eSlogStates::State_3_TurnAround_474C70;
        field_FE_next_state = -1;
        return 1;

    case eSlogStates::State_1_Walk_4743F0:
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
        }
        else
        {
            field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
        }

        if (!WallHit_401930(field_BC_sprite_scale * FP_FromInteger(20), field_B4_velx * FP_FromInteger(9)))
        {
            field_FC_current_motion = eSlogStates::State_9_StartWalking_474690;
            field_FE_next_state = -1;
            return 1;
        }

        field_10_anim.field_4_flags.Clear(AnimFlags::eBit6_FlipY);
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit7_SwapXY);

        ToIdle();

        field_128 = 60 * Slog_NextRandom() / 256 + gnFrameCount_507670 + 120;
        return 0;

    case eSlogStates::State_2_Run_4749A0:
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(3));
        }
        else
        {
            field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(3));
        }

        if (!WallHit_401930(field_BC_sprite_scale * FP_FromInteger(20), field_B4_velx * FP_FromInteger(4)))
        {
            field_FC_current_motion = eSlogStates::State_2_Run_4749A0;
            field_FE_next_state = -1;
            return 1;
        }

        field_10_anim.field_4_flags.Clear(AnimFlags::eBit6_FlipY);
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit7_SwapXY);

        ToIdle();

        field_128 = 60 * Slog_NextRandom() / 256 + gnFrameCount_507670 + 120;
        return 0;
    }
    return 0;
}

void Slog::ToIdle()
{
    MapFollowMe_401D30(0);
    field_12C = 0;
    field_130 = 0;
    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    field_FC_current_motion = eSlogStates::State_0_Idle_4742E0;
    field_FE_next_state = -1;
}

void Slog::ToJump_473FB0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = (FP_FromDouble(-10.18) * field_BC_sprite_scale);
    }
    else
    {
        field_B4_velx = (FP_FromDouble(10.18) * field_BC_sprite_scale);
    }

    field_B8_vely = (FP_FromInteger(-8) * field_BC_sprite_scale);
    field_E8_LastLineYPos = field_AC_ypos;

    field_FC_current_motion = eSlogStates::State_19_JumpForwards_475610;
    
    VOnTrapDoorOpen();

    field_F4_pLine = nullptr;

    Sfx_475BD0(8);

    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_443810(MusicController::MusicTypes::eType8, this, 0, 0);
    }
}

void Slog::Sfx_475BD0(int /*idx*/)
{
    NOT_IMPLEMENTED();
}

__int16 Slog::IsPlayerNear_471930()
{
    if (FP_Abs(sActiveHero_507678->field_A8_xpos - field_A8_xpos) >= (FP_FromInteger(100) * field_BC_sprite_scale) ||
        FP_Abs(sActiveHero_507678->field_AC_ypos - field_AC_ypos) >= (FP_FromInteger(25) * field_BC_sprite_scale) ||
        sActiveHero_507678->field_BC_sprite_scale != field_BC_sprite_scale)
    {
        return 0;
    }
    return 1;
}

BaseAliveGameObject* Slog::FindAbeMudOrSlig_4722F0()
{
    BaseAliveGameObject* pResult = nullptr;
    FP minDist = FP_FromInteger(gPsxDisplay_504C78.field_0_width);

    PSX_RECT bRect = {};
    VGetBoundingRect(&bRect, 1);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        bRect.x -= gPsxDisplay_504C78.field_0_width;
    }
    else
    {
        bRect.w += gPsxDisplay_504C78.field_0_width;
    }

    for (int i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj != field_14C && pObj != this)
        {
            if (pObj->field_4_typeId == Types::eAbe_43 || pObj->field_4_typeId == Types::eMudokon_75 || pObj->field_4_typeId == Types::eSlig_88)
            {
                PSX_RECT objRect = {};
                pObj->VGetBoundingRect(&objRect, 1);

                if (objRect.x <= bRect.w
                    && objRect.w >= bRect.x
                    && objRect.h >= bRect.y
                    && objRect.y <= bRect.h)
                {
                    const FP xd = FP_Abs(pObj->field_A8_xpos - field_A8_xpos);
                    if (xd < minDist)
                    {
                        pResult = pObj;
                        minDist = xd;
                    }
                }
            }
        }
    }
    return pResult;
}

void Slog::VScreenChanged()
{
    VScreenChanged_473480();
}

void Slog::VScreenChanged_473480()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level
        || gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path
        || gMap_507BA8.field_28_cd_or_overlay_num != gMap_507BA8.GetOverlayId_4440B0())
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);

        if (field_10C)
        {
            field_10C->field_C_refCount--;
            field_10C = nullptr;
        }

        if (field_14C)
        {
            field_14C->field_C_refCount--;
            field_14C = nullptr;
        }

        if (field_16C)
        {
            field_16C->field_C_refCount--;
            field_16C = nullptr;
        }
    }
}

void Slog::VRender(int** pOrderingTable)
{
    VRender_4735D0(pOrderingTable);
}

void Slog::VRender_4735D0(int** ppOt)
{
    if (field_8_update_delay == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender_417DA0(ppOt);
    }
}

void Slog::VOnThrowableHit(BaseGameObject* pFrom)
{
    VOnThrowableHit_4735F0(pFrom);
}

void Slog::VOnThrowableHit_4735F0(BaseGameObject* /*pFrom*/)
{
    field_156 += field_15C;
}

__int16 Slog::HandleEnemyStopper_473BD0()
{
    FP xpos = {};
    if (field_B4_velx >= FP_FromInteger(0))
    {
        xpos = field_A8_xpos + (ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(2));
    }
    else
    {
        xpos = field_A8_xpos - (ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(2));
    }

    auto pStopper = static_cast<Path_EnemyStopper*>(gMap_507BA8.TLV_Get_At_446260(
        FP_GetExponent(xpos),
        FP_GetExponent(field_AC_ypos),
        FP_GetExponent(xpos),
        FP_GetExponent(field_AC_ypos),
        TlvTypes::EnemyStopper_79));

    if (!pStopper)
    {
        return 0;
    }

    if (pStopper->field_18_direction != (field_B4_velx > FP_FromInteger(0) ? 1 : 0))
    {
        return 0;
    }

    if (!SwitchStates_Get(pStopper->field_1A_id))
    {
        return 1;
    }

    return 0;
}

void Slog::VOnTrapDoorOpen()
{
    VOnTrapDoorOpen_4740C0();
}

void Slog::VOnTrapDoorOpen_4740C0()
{
    if (field_F8_pLiftPoint)
    {
        field_F8_pLiftPoint->VRemove_451680(this);
        field_F8_pLiftPoint->field_C_refCount--;
        field_F8_pLiftPoint = nullptr;
    }
}

void Slog::State_0_Idle_4742E0()
{
    if (!ToNextMotion_473CE0())
    {
        if (field_FE_next_state == eSlogStates::State_19_JumpForwards_475610)
        {
            ToJump_473FB0();
        }
        else if (field_FE_next_state == -1)
        {
            if (field_FC_current_motion != eSlogStates::State_0_Idle_4742E0)
            {
                if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos,
                    0))
                {
                    SND_SEQ_PlaySeq_4775A0(17, 1, 0);
                }

                if (gMap_507BA8.GetDirection(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos) >= CameraPos::eCamCurrent_0
                    && gMap_507BA8.GetDirection(
                        field_B2_lvl_number,
                        field_B0_path_number,
                        field_A8_xpos,
                        field_AC_ypos) >= CameraPos::eCamCurrent_0)
                {
                    if (MusicController::sub_443840(0, 0, 0) == 9)
                    {
                        MusicController::sub_443810(MusicController::MusicTypes::eType9, this, 0, 0);
                    }
                    else
                    {
                        MusicController::sub_443810(MusicController::MusicTypes::eType7, this, 0, 0);
                    }
                }
            }
        }
        else
        {
            field_FC_current_motion = field_FE_next_state;
            field_FE_next_state = -1;
        }
    }
}

void Slog::State_1_Walk_4743F0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_2_Run_4749A0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_3_TurnAround_474C70()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        ToIdle();
        field_128 = 60 * Math_NextRandom() / 256 + gnFrameCount_507670 + 120;
    }
}

void Slog::State_4_Fall_4750C0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_5_Unknown_474070()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        field_FC_current_motion = field_E4;
        if (field_F8_pLiftPoint)
        {
            field_A8_xpos = FP_FromInteger((field_F4_pLine->field_0_rect.x + field_F4_pLine->field_0_rect.w) / 2);
            field_AC_ypos = FP_FromInteger(field_F4_pLine->field_0_rect.y);
        }
    }
}

void Slog::State_6_MoveHeadUpwards_474220()
{
    if (field_10_anim.field_92_current_frame == 0)
    {
        Sfx_475BD0(2);
        field_144 = 1;
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (Math_RandomRange_450F20(0, 100) < 30)
        {
            Sfx_475BD0(4);
        }
        
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit6_FlipY);
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit7_SwapXY);

        ToIdle();

        field_128 = 60 * Slog_NextRandom() / 256 + gnFrameCount_507670 + 120;
    }
}

void Slog::State_7_SlideTurn_474DB0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_8_StopRunning_474EC0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_9_StartWalking_474690()
{
    NOT_IMPLEMENTED();
}

void Slog::State_10_EndWalking_4747D0()
{
    NOT_IMPLEMENTED();
}

void Slog::State_11_Land_475AB0()
{
    if (!field_10_anim.field_92_current_frame)
    {
        Sfx_475BD0(15);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit6_FlipY);
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit7_SwapXY);

        ToIdle();

        field_128 = 60 * Slog_NextRandom() / 256 + gnFrameCount_507670 + 120;
    }
}

void Slog::State_12_Unknown_475B50()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        field_FC_current_motion = eSlogStates::State_13_StartFastBarking_475B70;
    }
}

void Slog::State_13_StartFastBarking_475B70()
{
    if (field_10_anim.field_92_current_frame == 0)
    {
        Sfx_475BD0(2);
        field_144 = 1;
    }

    if (field_FE_next_state != -1)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = eSlogStates::State_14_EndFastBarking_475BB0;
        }
    }
}

void Slog::State_14_EndFastBarking_475BB0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        field_FC_current_motion = eSlogStates::State_0_Idle_4742E0;
    }
}

void Slog::State_15_Empty_475290()
{
    if (field_10_anim.field_92_current_frame == 0 || field_10_anim.field_92_current_frame == 6)
    {
        Sfx_475BD0(5);
    }

    if (field_FE_next_state != -1)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = field_FE_next_state;
            field_FE_next_state = -1;
        }
    }
}

void Slog::State_16_Sleeping_4752E0()
{
    if (field_FE_next_state != -1 && field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = field_FE_next_state;
        field_FE_next_state = -1;
        return;
    }

    bool bSpawnParticle = false;
    if (gnFrameCount_507670 % 60)
    {
        if (!((gnFrameCount_507670 - 20) % 60))
        {
            Sfx_475BD0(11);
            if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos,
                0))
            {
                bSpawnParticle = true;
            }
        }
    }
    else
    {
        Sfx_475BD0(10);
        if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0))
        {
            bSpawnParticle = true;
        }
    }

    auto pSnoozeParticle = ao_new<SnoozeParticle>();
    if (pSnoozeParticle && bSpawnParticle)
    {
        pSnoozeParticle->ctor_464320(
            field_A8_xpos
            + ((field_10_anim.field_4_flags.Get(AnimFlags::eBit10_alternating_flag)) != 0 ? FP_FromInteger(-18) : FP_FromInteger(18)),
            field_AC_ypos - FP_FromInteger(13),
            field_10_anim.field_C_layer,
            field_10_anim.field_14_scale);
    }
}

void Slog::State_17_FallAsleep_475510()
{
    if (field_FE_next_state != -1)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = field_FE_next_state;
            field_FE_next_state = -1;
        }
    }

    MusicController::sub_443810(MusicController::MusicTypes::eType0, this, 0, 0);
}

void Slog::State_18_WakeUp_475460()
{
    for (int i=0; i<gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eSnoozParticle_87)
        {
            static_cast<SnoozeParticle*>(pObj)->field_1D4_state = 2;
        }
    }

    if (field_FE_next_state != -1)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = field_FE_next_state;
            field_FE_next_state = -1;
            Sfx_475BD0(4);
        }
    }

    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_443810(MusicController::MusicTypes::eType7, this, 0, 0);
    }
}

void Slog::State_19_JumpForwards_475610()
{
    field_B8_vely += (field_BC_sprite_scale * FP_FromDouble(1.8));

    if (field_B8_vely > (field_BC_sprite_scale * FP_FromInteger(20)))
    {
        field_B8_vely = (field_BC_sprite_scale * FP_FromInteger(20));
    }

    const FP oldXPos = field_A8_xpos;
    const FP ypos1 = field_AC_ypos - (field_BC_sprite_scale * FP_FromInteger(20));
    
    field_A8_xpos += field_B4_velx;
    field_AC_ypos += field_B8_vely;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_504C6C->RayCast_40C410(
        oldXPos,
        ypos1,
        field_A8_xpos,
        field_AC_ypos,
        &pLine,
        &hitX,
        &hitY,
        field_BC_sprite_scale != FP_FromDouble(0.5) ? 6 : 0x60) == 1)
    {
        switch (pLine->field_8_type)
        {
        case 1:
        case 5:
            if (field_B4_velx < FP_FromInteger(0))
            {
                field_B4_velx = (-field_B4_velx /  FP_FromInteger(2));
                field_A8_xpos = oldXPos;
            }
            break;

        case 2:
        case 6:
            if (field_B4_velx > FP_FromInteger(0))
            {
                field_B4_velx = (-field_B4_velx / FP_FromInteger(2));
                field_A8_xpos = oldXPos;
            }
            break;
        default:
            break;
        }
    }

    if (sCollisions_DArray_504C6C->RayCast_40C410(
        oldXPos,
        ypos1,
        field_A8_xpos,
        field_AC_ypos,
        &pLine,
        &hitX,
        &hitY,
        field_BC_sprite_scale != FP_FromDouble(0.5) ? 1 : 0x10))
    {
        switch (pLine->field_8_type)
        {
        case 0:
        case 4:
        case 32:
        case 36:
            if (field_B8_vely > FP_FromInteger(0))
            {
                field_F4_pLine = pLine;
                field_FE_next_state = -1;
                field_FC_current_motion = eSlogStates::State_2_Run_4749A0;
                field_AC_ypos = hitY;
                field_B8_vely = FP_FromInteger(0);
            }
            break;
        default:
            break;
        }
    }

    if (field_AC_ypos - field_E8_LastLineYPos > FP_FromInteger(2))
    {
        field_12C = 0;
        field_E8_LastLineYPos = field_AC_ypos;
        field_FC_current_motion = eSlogStates::State_4_Fall_4750C0;
    }
}

void Slog::State_20_JumpUpwards_475890()
{
    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0)
        MusicController::sub_443810(MusicController::MusicTypes::eType9, this, 0, 0);

    if (field_10_anim.field_92_current_frame == 5)
    {
        Sfx_475BD0(6);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eSlogStates::State_0_Idle_4742E0;
        field_FE_next_state = -1;
    }
}

void Slog::State_21_Eating_475900()
{
    SND_Seq_Stop_477A60(17u);

    if (field_10_anim.field_92_current_frame != 0
        || (field_10_anim.field_4_flags.Clear(AnimFlags::eBit19_LoopBackwards),
            field_FE_next_state == -1)
        || field_FE_next_state == eSlogStates::State_21_Eating_475900)
    {
        if (field_10_anim.field_92_current_frame == 3 && !field_10_anim.field_4_flags.Get(AnimFlags::eBit19_LoopBackwards))
        {
            SFX_Play_43AD70(static_cast<char>(Math_RandomRange_450F20(79, 80)), 100, 0);
            auto pBlood = ao_new<Blood>();
            if (pBlood)
            {
                const FP bloodYPos = field_AC_ypos - (FP_FromInteger(4) * field_BC_sprite_scale);
                const FP bloodXPos = ((field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX)) != 0 ? -FP_FromInteger(25) : FP_FromInteger(25) * field_BC_sprite_scale);
                pBlood->ctor_4072B0(
                    bloodXPos + field_A8_xpos,
                    bloodYPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    field_BC_sprite_scale,
                    12);
            }
        }

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (Math_RandomRange_450F20(0, 100) < 85)
            {
                if (static_cast<int>(gnFrameCount_507670) > field_164_timer && Math_RandomRange_450F20(0, 100) < 60)
                {
                    field_164_timer = gnFrameCount_507670 + 16;
                    Sfx_475BD0(3);
                }
                field_10_anim.field_4_flags.Set(AnimFlags::eBit19_LoopBackwards);
            }
        }

        if (field_10_anim.field_92_current_frame == 0)
        {
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit19_LoopBackwards);
        }
    }
    else
    {
        field_FC_current_motion = eSlogStates::State_0_Idle_4742E0;
    }
}

void Slog::State_22_Empty_475A90()
{
    if (!field_F4_pLine)
    {
        State_4_Fall_4750C0();
        field_FC_current_motion = eSlogStates::State_22_Empty_475A90;
    }
}

void Slog::State_23_Scratch_475550()
{
    if (field_10_anim.field_92_current_frame == 4)
    {
        SND_SEQ_PlaySeq_4775A0(16u, 1, 1);
    }

    if (field_FE_next_state != -1)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = field_FE_next_state;
            field_FE_next_state = -1;
        }
    }
}

void Slog::State_24_Growl_475590()
{
    if (field_10_anim.field_92_current_frame == 3)
    {
        if (field_144)
        {
            Sfx_475BD0(3);
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
            field_144 = 0;
            field_164_timer = gnFrameCount_507670 + 12;
        }
    }

    if (static_cast<int>(gnFrameCount_507670) > field_164_timer)
    {
        field_10_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
    }

    if (field_FE_next_state != -1)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = field_FE_next_state;
            field_FE_next_state = -1;
        }
    }
}

END_NS_AO

