#pragma once

#include "Oscillator.hpp"

struct Adsr
{
    int16_t attack_time;
    int16_t decay_time;
    int16_t sustain_attenuation;
    int16_t release_time;

    static Adsr
    from_oscillator(const z2sound::Oscillator &oscillator);

    static Adsr
    from_percussion_release(uint16_t release);
};