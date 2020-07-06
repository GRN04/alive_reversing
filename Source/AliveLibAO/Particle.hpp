#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"

START_NS_AO

class Particle : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Particle* ctor_478880(FP xpos, FP ypos, int animFrameTableOffset, int maxW, int maxH, BYTE** ppAnimData);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;
    int field_E4_scale_amount;
};
ALIVE_ASSERT_SIZEOF(Particle, 0xE8);

EXPORT Particle* CC New_Particle_419D00(FP xpos, FP ypos, FP scale);


END_NS_AO