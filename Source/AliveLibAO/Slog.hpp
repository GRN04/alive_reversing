#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"

void Slog_ForceLink();

namespace AO {

struct Path_Slog : public Path_TLV
{
    __int16 field_18_scale;
    __int16 field_1A_direction;
    __int16 field_1C_wakeup_anger;
    __int16 field_1E_bark_anger;
    __int16 field_20_sleeps;
    __int16 field_22_chase_anger;
    __int16 field_24_jump_attack_delay;
    __int16 field_26_disabled_resources;
    __int16 field_28_anger_trigger_id;
    __int16 field_2A_pad;
};
ALIVE_ASSERT_SIZEOF(Path_Slog, 0x2C);

#define SLOG_STATES_ENUM(ENTRY) \
    ENTRY(State_0_Idle_4742E0) \
    ENTRY(State_1_Walk_4743F0) \
    ENTRY(State_2_Run_4749A0) \
    ENTRY(State_3_TurnAround_474C70) \
    ENTRY(State_4_Fall_4750C0) \
    ENTRY(State_5_Unknown_474070) \
    ENTRY(State_6_MoveHeadUpwards_474220) \
    ENTRY(State_7_SlideTurn_474DB0) \
    ENTRY(State_8_StopRunning_474EC0) \
    ENTRY(State_9_StartWalking_474690) \
    ENTRY(State_10_EndWalking_4747D0) \
    ENTRY(State_11_Land_475AB0) \
    ENTRY(State_12_Unknown_475B50) \
    ENTRY(State_13_StartFastBarking_475B70) \
    ENTRY(State_14_EndFastBarking_475BB0) \
    ENTRY(State_15_Empty_475290) \
    ENTRY(State_16_Sleeping_4752E0) \
    ENTRY(State_17_FallAsleep_475510) \
    ENTRY(State_18_WakeUp_475460) \
    ENTRY(State_19_JumpForwards_475610) \
    ENTRY(State_20_JumpUpwards_475890) \
    ENTRY(State_21_Eating_475900) \
    ENTRY(State_22_Empty_475A90) \
    ENTRY(State_23_Scratch_475550) \
    ENTRY(State_24_Growl_475590)

#define MAKE_ENUM(VAR) VAR,
enum eSlogStates : int
{
    SLOG_STATES_ENUM(MAKE_ENUM)
};

class Slog : public BaseAliveGameObject
{
public:
    EXPORT Slog* ctor_472EE0(Path_Slog* pTlv, int tlvInfo);
    EXPORT Slog* ctor_473050(FP xpos, FP ypos, FP scale);
    EXPORT BaseGameObject* dtor_473370();

    EXPORT __int16 HandleEnemyStopper_473BD0();
    EXPORT void ToJump_473FB0();
    EXPORT void Sfx_475BD0(int idx);
    EXPORT __int16 IsPlayerNear_471930();
    EXPORT BaseAliveGameObject* FindAbeMudOrSlig_4722F0();
    EXPORT void Init_473130();
    EXPORT __int16 ToNextMotion_473CE0();
    void ToIdle();

    virtual __int16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOn_TLV_Collision(Path_TLV* pTlv) override;
    virtual void VScreenChanged() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VUpdate() override;
    virtual BaseGameObject* VDestructor(signed int flags) override;

private:
    void VOnTrapDoorOpen_4740C0();
    EXPORT void VOnThrowableHit_4735F0(BaseGameObject* pFrom);
    EXPORT void VRender_4735D0(int** ppOt);
    EXPORT void VScreenChanged_473480();
    EXPORT void VUpdate_4739C0();
    EXPORT void VOn_Tlv_Collision_473970(Path_TLV* pTlv);
    EXPORT __int16 VTakeDamage_473610(BaseGameObject* pFrom);
    EXPORT Slog* Vdtor_473CB0(signed int flags);

public:
    void SetAnimFrame();

    BYTE** ResBlockForMotion(__int16 motion);

    EXPORT void MoveOnLine_4740F0();

    // States
    EXPORT void State_0_Idle_4742E0();
    EXPORT void State_1_Walk_4743F0();
    EXPORT void State_2_Run_4749A0();
    EXPORT void State_3_TurnAround_474C70();
    EXPORT void State_4_Fall_4750C0();
    EXPORT void State_5_Unknown_474070();
    EXPORT void State_6_MoveHeadUpwards_474220();
    EXPORT void State_7_SlideTurn_474DB0();
    EXPORT void State_8_StopRunning_474EC0();
    EXPORT void State_9_StartWalking_474690();
    EXPORT void State_10_EndWalking_4747D0();
    EXPORT void State_11_Land_475AB0();
    EXPORT void State_12_Unknown_475B50();
    EXPORT void State_13_StartFastBarking_475B70();
    EXPORT void State_14_EndFastBarking_475BB0();
    EXPORT void State_15_Empty_475290();
    EXPORT void State_16_Sleeping_4752E0();
    EXPORT void State_17_FallAsleep_475510();
    EXPORT void State_18_WakeUp_475460();
    EXPORT void State_19_JumpForwards_475610();
    EXPORT void State_20_JumpUpwards_475890();
    EXPORT void State_21_Eating_475900();
    EXPORT void State_22_Empty_475A90();
    EXPORT void State_23_Scratch_475550();
    EXPORT void State_24_Growl_475590();

    // Brains
    EXPORT __int16 Brain_0_ListeningToSlig_472450();
    EXPORT __int16 Brain_1_Idle_4719C0();
    EXPORT __int16 Brain_2_ChasingAbe_470F50();
    EXPORT __int16 Brain_3_Dead_4721B0();

    // Inlined
    __int16 Facing(FP xpos);

    // Inlined
    void DelayedResponse(__int16 responseIdx);

    BaseAliveGameObject* field_10C_pTarget;
    __int16 field_110;
    __int16 field_112;
    __int16 field_114_brain_idx;
    __int16 field_116_brain_state;
    __int16 field_118;
    __int16 field_11A;
    int field_11C_timer;
    int field_120;
    __int16 field_124;
    __int16 field_126_movedOffScreen;
    int field_128;
    FP field_12C;
    int field_130;
    __int16 field_134;
    __int16 field_136;
    int field_138_tlvInfo;
    __int16 field_13C_res_idx;
    __int16 field_13E;
    int field_140;
    int field_144;
    int field_148;
    BaseAliveGameObject* field_14C_pSlig;
    __int16 field_150_waiting_counter;
    __int16 field_152_response_index;
    __int16 field_154_response_part;
    __int16 field_156;
    __int16 field_158_bark_anger;
    __int16 field_15A;
    __int16 field_15C;
    __int16 field_15E;
    int field_160;
    int field_164_timer;
    __int16 field_168_anger_switch_id;
    __int16 field_16A;
    BaseAliveGameObject* field_16C;
    __int16 field_170;
    __int16 field_172;
    __int16 field_174;
    __int16 field_176;
    __int16 field_178;
    __int16 field_17A;
    __int16 field_17C_res;
    __int16 field_17E;
    __int16 field_180;
    __int16 field_182_pad;
    BYTE** field_184_resources[5];
};
ALIVE_ASSERT_SIZEOF(Slog, 0x198);

ALIVE_VAR_EXTERN(short, gNumSlogs_9F11C8);

}
