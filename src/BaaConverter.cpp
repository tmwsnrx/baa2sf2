#include "BaaConverter.hpp"

#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace sf2cute;

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

      sf2cute::SFInstrumentZone instrument_zone{
        *sample,
        std::vector {
          sf2cute::SFGeneratorItem(sf2cute::SFGenerator::kKeyRange, sf2cute::RangesType(key_zone.lower_key_limit, key_zone.upper_key_limit))
        },
        std::vector<sf2cute::SFModulatorItem>{}
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