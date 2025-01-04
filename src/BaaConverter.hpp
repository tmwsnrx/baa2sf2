#pragma once

#include <map>
#include <memory>
#include <optional>

#include <sf2cute.hpp>

#include "AudioArchive.hpp"

class BaaConverter
{
public:
    explicit BaaConverter(z2sound::AudioArchive &audio_archive);

    std::optional<sf2cute::SoundFont>
    to_sf2(uint8_t wave_bank_no);

private:
    std::optional<std::shared_ptr<sf2cute::SFSample>>
    fetch_sample(const z2sound::WaveBank &wave_bank, uint16_t wave_id);

    std::reference_wrapper<const z2sound::AudioArchive> audio_archive_;

    sf2cute::SoundFont sf2_;
    std::map<uint16_t, std::shared_ptr<sf2cute::SFSample>> samples_;
};
