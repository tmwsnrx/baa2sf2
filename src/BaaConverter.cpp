#include "BaaConverter.hpp"

#include <cmath>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace sf2cute;

struct Tuning
{
  int16_t coarse;
  int16_t fine;
};

static constexpr int16_t volume_to_attenuation(float volume_multiplier);
static constexpr Tuning pitch_to_tuning(float pitch_multiplier);

BaaConverter::BaaConverter(z2sound::AudioArchive& audio_archive)
: audio_archive_{audio_archive} {}

std::optional<SoundFont> BaaConverter::to_sf2(uint8_t instrument_bank_no)
{
  sf2_ = SoundFont{};
  samples_.clear();

  const auto& baa = audio_archive_.get();

  auto instrument_bank_iter = baa.instrument_banks_.find(instrument_bank_no);
  if (instrument_bank_iter == baa.instrument_banks_.end())
  {
    return std::nullopt;
  }

  const auto& instrument_bank = instrument_bank_iter->second;

  auto wave_bank_iter = baa.wave_banks_.find(instrument_bank.get_wave_bank_id());
  if (wave_bank_iter == baa.wave_banks_.end())
  {
    return std::nullopt;
  }

  const auto& wave_bank = wave_bank_iter->second;
  const auto& instruments = instrument_bank.get_instruments();
  std::vector<uint16_t> required_samples;

  for (size_t instrument_no = 0; const auto& instrument : instruments)
  {
    auto name = std::to_string(instrument_no);

    auto sf2instrument = sf2_.NewInstrument("Instrument_" + name);

    for (const auto& key_zone : instrument->get_key_zones())
    {
      auto sample = fetch_sample(wave_bank, key_zone.wave_id);
      if (!sample.has_value())
      {
        continue;
      }

      auto attenuation = volume_to_attenuation(key_zone.volume_multiplier);
      auto tuning = pitch_to_tuning(key_zone.pitch_multiplier);

      SFInstrumentZone instrument_zone{
        *sample,
        std::vector {
          SFGeneratorItem{SFGenerator::kKeyRange, RangesType{key_zone.lower_key_limit, key_zone.upper_key_limit}},
          SFGeneratorItem{SFGenerator::kInitialAttenuation, GenAmountType{attenuation}},
          SFGeneratorItem{SFGenerator::kCoarseTune, GenAmountType{tuning.coarse}},
          SFGeneratorItem{SFGenerator::kFineTune, GenAmountType{tuning.fine}}
        },
        std::vector<SFModulatorItem>{}
      };

      sf2instrument->AddZone(std::move(instrument_zone));
    }

    sf2_.NewPreset("Preset_" + name, instrument_no, 0,
      std::vector<sf2cute::SFPresetZone>{
        sf2cute::SFPresetZone{sf2instrument}
      }
    );

    instrument_no++;
  }

  return sf2_;
}

std::optional<std::shared_ptr<SFSample>> BaaConverter::fetch_sample(const z2sound::WaveBank& wave_bank, uint16_t wave_id)
{
  const auto sample_iter = samples_.find(wave_id);

  if (sample_iter == samples_.end())
  {
    const auto wave_handle = wave_bank.get_wave_table().at(wave_id);
    if (!wave_handle.has_value())
    {
      return std::nullopt;
    }

    const z2sound::WaveInfo& wave_info = wave_handle->get_wave_info();
    auto wave_buffer = wave_handle->get_buffer();

    if (!wave_buffer.has_value())
    {
      return std::nullopt;
    }

    std::vector<int16_t> sample_buffer;
    sample_buffer.assign(wave_buffer->begin(), wave_buffer->end());

    std::stringstream sample_name;
    sample_name << "Sample_" << std::setfill('0') << std::setw(4) << wave_id;
    
    auto sample = sf2_.NewSample(
      sample_name.str(),
      sample_buffer,
      wave_info.is_looping ? wave_info.loop_start : 0,
      wave_info.is_looping ? wave_info.loop_end : 0,
      static_cast<uint32_t>(wave_info.sample_rate),
      wave_info.root_key,
      0
    );

    samples_.emplace(wave_id, sample);
    return sample;
  }

  return sample_iter->second;
}

static constexpr int16_t volume_to_attenuation(float volume_multiplier)
{
  if (volume_multiplier <= 0.0f)
  {
    return 1440;
  }
  else if (volume_multiplier >= 1.0f)
  {
    return 0;
  }
  
  return static_cast<int16_t>(std::round(1000.0f * std::log10(volume_multiplier)));
}

static constexpr Tuning pitch_to_tuning(float pitch_multiplier)
{
  if (pitch_multiplier == 1.0f)
  {
    return Tuning{
      .coarse = 0,
      .fine = 0
    };
  }

  auto cents = 1200.0f * std::log2(pitch_multiplier);
  int16_t semitones = static_cast<int16_t>(cents / 100);

  cents -= semitones * 100;

  return Tuning{
    .coarse = semitones,
    .fine = static_cast<int16_t>(std::round(cents))
  };
}