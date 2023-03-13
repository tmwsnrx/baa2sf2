#include "BaaConverter.hpp"

#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Adsr.hpp"
#include "UnitConversions.hpp"

using namespace sf2cute;

BaaConverter::BaaConverter(z2sound::AudioArchive &audio_archive)
    : audio_archive_{audio_archive} {}

std::optional<SoundFont> BaaConverter::to_sf2(uint8_t wave_bank_no)
{
  sf2_ = SoundFont{};
  samples_.clear();

  sf2_.set_comment("WIP version");
  sf2_.set_bank_name("Wave Bank " + std::to_string(wave_bank_no));

  const auto &baa = audio_archive_.get();

  auto wave_bank_iter = baa.wave_banks_.find(wave_bank_no);
  if (wave_bank_iter == baa.wave_banks_.end())
  {
    return std::nullopt;
  }

  const auto &wave_bank = wave_bank_iter->second;

  // for (const auto& wave : wave_bank.get_wave_table())
  // {
  //   wave->wave_info_.
  // }

  for (const auto &[instrument_bank_id, instrument_bank] : baa.instrument_banks_)
  {
    if (instrument_bank.get_wave_bank_id() != wave_bank_no)
    {
      continue;
    }

    auto instrument_bank_string = std::to_string(instrument_bank_id);
    const auto &instruments = instrument_bank.get_instruments();

    for (size_t instrument_no = 0; const auto &instrument : instruments)
    {
      if (!instrument)
      {
        instrument_no++;
        continue;
      }

      auto name = std::to_string(instrument_no);

      auto sf2instrument = sf2_.NewInstrument("Instrument_" + instrument_bank_string + "_" + name);

      for (const auto &key_zone : instrument->get_key_zones())
      {
        auto sample = fetch_sample(wave_bank, key_zone.wave_id);
        if (!sample.has_value())
        {
          continue;
        }

        auto attenuation = volume_to_attenuation(key_zone.volume_multiplier);
        auto tuning = pitch_to_tuning(key_zone.pitch_multiplier);
        auto pan = pan_float_to_promille(key_zone.pan);

        Adsr adsr;

        if (key_zone.oscillator.has_value())
        {
          adsr = Adsr::from_oscillator(*key_zone.oscillator);
        }
        else if (key_zone.release != 0)
        {
          adsr = Adsr::from_percussion_release(key_zone.release);
        }

        SFInstrumentZone instrument_zone{
            *sample,
            std::vector{
                SFGeneratorItem{SFGenerator::kKeyRange, RangesType{key_zone.lower_key_limit, key_zone.upper_key_limit}},
                SFGeneratorItem{SFGenerator::kInitialAttenuation, GenAmountType{attenuation}},
                SFGeneratorItem{SFGenerator::kCoarseTune, GenAmountType{tuning.coarse}},
                SFGeneratorItem{SFGenerator::kFineTune, GenAmountType{tuning.fine}},
                SFGeneratorItem{SFGenerator::kPan, GenAmountType{pan}},
                SFGeneratorItem{SFGenerator::kAttackVolEnv, GenAmountType{adsr.attack_time}},
                SFGeneratorItem{SFGenerator::kDecayVolEnv, GenAmountType{adsr.decay_time}},
                SFGeneratorItem{SFGenerator::kSustainVolEnv, GenAmountType{adsr.sustain_attenuation}},
                SFGeneratorItem{SFGenerator::kReleaseVolEnv, GenAmountType{adsr.release_time}}},
            std::vector<SFModulatorItem>{}};

        if (instrument->get_type() == z2sound::Instrument::Type::Percussion)
        {
          instrument_zone.SetGenerator(SFGeneratorItem{SFGenerator::kOverridingRootKey, GenAmountType{key_zone.lower_key_limit}});
        }

        if (!(sample->get()->start_loop() == 0 && sample->get()->end_loop() == 0))
        {
          instrument_zone.SetGenerator(
              SFGeneratorItem{SFGenerator::kSampleModes, static_cast<uint16_t>(SampleMode::kLoopContinuously)});
        }

        sf2instrument->AddZone(std::move(instrument_zone));
      }

      uint16_t preset_no = instrument_no & 0x7f;
      uint16_t bank_no = instrument_bank_id * 2 + (instrument_no >> 7);

      sf2_.NewPreset("Preset_" + name, preset_no, bank_no,
                     std::vector<sf2cute::SFPresetZone>{
                         sf2cute::SFPresetZone{sf2instrument}});

      instrument_no++;
    }
  }

  return sf2_;
}

std::optional<std::shared_ptr<SFSample>> BaaConverter::fetch_sample(const z2sound::WaveBank &wave_bank, uint16_t wave_id)
{
  const auto sample_iter = samples_.find(wave_id);

  if (sample_iter == samples_.end())
  {
    const auto wave_handle = wave_bank.get_wave_table().at(wave_id);
    if (!wave_handle.has_value())
    {
      return std::nullopt;
    }

    const z2sound::WaveInfo &wave_info = wave_handle->get_wave_info();
    auto wave_buffer = wave_handle->get_buffer();

    if (!wave_buffer.has_value())
    {
      return std::nullopt;
    }

    std::vector<int16_t> sample_buffer;
    sample_buffer.assign(wave_buffer->begin(), wave_buffer->end());

    std::stringstream sample_name;
    sample_name << "Sample_" << std::setfill('0') << std::setw(4) << std::hex << wave_id;

    auto sample = sf2_.NewSample(
        sample_name.str(),
        sample_buffer,
        wave_info.is_looping ? wave_info.loop_start : 0,
        wave_info.is_looping ? wave_info.loop_end : 0,
        static_cast<uint32_t>(wave_info.sample_rate),
        wave_info.root_key,
        0);

    samples_.emplace(wave_id, sample);
    return sample;
  }

  return sample_iter->second;
}
