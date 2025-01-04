#include "Adsr.hpp"

#include <limits>

#include "UnitConversions.hpp"

enum class EnvelopePhase
{
    Attack,
    Decay,
    Sustain,
    Release,
    Stopped
};

Adsr
Adsr::from_oscillator(const z2sound::Oscillator &oscillator)
{
    Adsr adsr{
        .attack_time = std::numeric_limits<int16_t>::min(),
        .decay_time = std::numeric_limits<int16_t>::min(),
        .sustain_attenuation = 0,
        .release_time = std::numeric_limits<int16_t>::min()};

    // if (oscillator.pre_sustain.has_value())
    // {
    //   EnvelopePhase phase = EnvelopePhase::Attack;

    //   for (auto envelope_iter = *oscillator.pre_sustain; phase != EnvelopePhase::Sustain; envelope_iter++)
    //   {
    //     auto curve_type = envelope_iter->get_curve_type();

    //     switch (curve_type)
    //     {
    //     case z2sound::Envelope::CurveType::Jump:
    //     case z2sound::Envelope::CurveType::Sustain:
    //     case z2sound::Envelope::CurveType::Stop:
    //       phase = EnvelopePhase::Sustain;
    //       break;

    //     default:
    //       if (phase == EnvelopePhase::Attack)
    //       {
    //         adsr.attack_time = milliseconds_to_timescents(envelope_iter->time);
    //         adsr.sustain_attenuation = volume_to_attenuation(envelope_iter->volume);
    //         phase = EnvelopePhase::Decay;
    //       }
    //       else if (phase == EnvelopePhase::Decay)
    //       {
    //         adsr.decay_time = milliseconds_to_timescents(envelope_iter->time * 3);
    //         adsr.sustain_attenuation = volume_to_attenuation(envelope_iter->volume);
    //       }
    //       break;
    //     }
    //   }
    // }

    if (oscillator.post_sustain.has_value())
    {
        EnvelopePhase phase = EnvelopePhase::Release;

        for (auto envelope_iter = *oscillator.post_sustain; phase != EnvelopePhase::Stopped; envelope_iter++)
        {
            auto curve_type = envelope_iter->get_curve_type();

            switch (curve_type)
            {
                case z2sound::Envelope::CurveType::Jump:
                case z2sound::Envelope::CurveType::Sustain:
                case z2sound::Envelope::CurveType::Stop:phase = EnvelopePhase::Stopped;
                    break;

                default:
                    if (phase == EnvelopePhase::Release)
                    {
                        adsr.release_time = adsr_ticks_to_timescents(envelope_iter->time);
                    }
                    break;
            }
        }
    }

    return adsr;
}

Adsr
Adsr::from_percussion_release(uint16_t release)
{
    int16_t release_time = adsr_ticks_to_timescents(release & 0x3fff);

    return Adsr{
        .attack_time = std::numeric_limits<int16_t>::min(),
        .decay_time = std::numeric_limits<int16_t>::min(),
        .sustain_attenuation = 0,
        .release_time = release_time};
}
