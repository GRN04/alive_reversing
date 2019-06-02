#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

struct Path_Paramite : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_entrace_type;
    __int16 field_14_attack_delay;
    __int16 field_16_drop_delay;
    __int16 field_18_meat_eating_time;
    unsigned __int16 field_1A_attack_duration;
    __int16 field_1C_disabled_resources;
    __int16 field_1E_id;
    __int16 field_20_hiss_before_attack;
    __int16 field_22_delete_when_far_away;
    __int16 field_24_deadly_scratch;
    __int16 field_26;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Paramite, 0x28);

class Paramite;
using TParamiteAIFn = __int16 (Paramite::*)();
using TParamiteMotionFn = void (Paramite::*)();

enum class LevelIds : __int16;

class Paramite : public BaseAliveGameObject
{
public:
    EXPORT Paramite* ctor_4879B0(Path_Paramite* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

public:
    EXPORT __int16 AI_0_4835B0();
    EXPORT __int16 AI_1_484CD0();
    EXPORT __int16 AI_2_4859D0();
    EXPORT __int16 AI_3_4851B0();
    EXPORT __int16 AI_4_48F8F0();
    EXPORT __int16 AI_5_486880();
    EXPORT __int16 AI_6_484BC0();
    EXPORT __int16 AI_7_484FF0();
    EXPORT __int16 AI_8_48DFC0();
    EXPORT __int16 AI_9_48ED80();

public:
    EXPORT void M_0_489FB0();
    EXPORT void M_1_48A7B0();
    EXPORT void M_2_48A2D0();
    EXPORT void M_3_48AA00();
    EXPORT void M_4_48B180();
    EXPORT void M_5_48B5B0();
    EXPORT void M_6_48A930();
    EXPORT void M_7_48B0C0();
    EXPORT void M_8_48A870();
    EXPORT void M_9_48AF10();
    EXPORT void M_10_48B000();
    EXPORT void M_11_48B200();
    EXPORT void M_12_48BE40();
    EXPORT void M_13_48BAF0();
    EXPORT void M_14_48BF00();
    EXPORT void M_15_48D930();
    EXPORT void M_16_48DDA0();
    EXPORT void M_17_48B590();
    EXPORT void M_18_48DF60();
    EXPORT void M_19_48BF50();
    EXPORT void M_20_48C010();
    EXPORT void M_21_48C180();
    EXPORT void M_22_48C3E0();
    EXPORT void M_23_48C600();
    EXPORT void M_24_48C680();
    EXPORT void M_25_48C6A0();
    EXPORT void M_26_48C6F0();
    EXPORT void M_27_48C780();
    EXPORT void M_28_48C8B0();
    EXPORT void M_29_48D9D0();
    EXPORT void M_30_48DB50();
    EXPORT void M_31_48C9E0();
    EXPORT void M_32_48D740();
    EXPORT void M_33_48D760();
    EXPORT void M_34_48D870();
    EXPORT void M_35_48D400();
    EXPORT void M_36_48D000();
    EXPORT void M_37_48CC60();
    EXPORT void M_38_48D6C0();
    EXPORT void M_39_48D8C0();
    EXPORT void M_40_48A0F0();
    EXPORT void M_41_48D8E0();
    EXPORT void M_42_48D900();
    EXPORT void M_43_48DB70();

private:
    EXPORT void sub_489EA0();

    void SetBrain(TParamiteAIFn fn);
    bool BrainIs(TParamiteAIFn fn);

private:
    EXPORT void dtor_487FC0();
    EXPORT Paramite* vdtor_487F90(signed int flags);

    EXPORT void vUpdate_4871B0();

    EXPORT void sub_489BB0();

    EXPORT __int16 Find_Paramite_488810();

    EXPORT void vUpdateAnim_487170();

private:

    __int16 field_116;
    int field_118;
    int field_11C_obj_id;
    int field_120_obj_id;
    int field_124;
    TParamiteAIFn field_128_fn_brainState;
    __int16 field_12C_brain_ret;
    __int16 field_12E_drop_delay;
    int field_130_timer;
    __int16 field_134_meat_eating_time;
    __int16 field_136_attack_delay;
    int field_138;
    int field_13C;
    int field_140_tlvInfo;
    int field_144_attack_duration;
    int field_148_timer;
    __int16 field_14C_id;
    LevelIds field_14E_return_level;
    __int16 field_150_return_path;
    __int16 field_152_return_camera;
    int field_154;
    __int16 field_158;
    __int16 field_15A;
    __int16 field_15C;
    __int16 field_15E;
    int field_160;
    __int16 field_164;
    __int16 field_166;
    __int16 field_168;
    __int16 field_16A;
    __int16 field_16C;
    __int16 field_16E;
    __int16 field_170;
    __int16 field_172;
    __int16 field_174;
    __int16 field_176;
    enum Flags_178 : __int16
    {
        eBit1 = 0x1,
        eBit2 = 0x2,
        eBit3 = 0x4,
        eBit4 = 0x8,
        eBit5 = 0x10,
        eBit6 = 0x20,
        eBit7 = 0x40,
        eBit8 = 0x80,
        eBit9 = 0x100,
        eBit10 = 0x200,
        eBit11 = 0x400,
        eBit12 = 0x800,
        eBit13 = 0x1000,
        eBit14 = 0x2000,
        eBit15 = 0x4000,
    };
    BitField16<Flags_178> field_178_flags;
    __int16 field_17A;
};
ALIVE_ASSERT_SIZEOF(Paramite, 0x17C);
