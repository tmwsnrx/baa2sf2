#include <fstream>
#include <iostream>
#include <vector>

#include "Poco/ConsoleChannel.h"
#include "Poco/Logger.h"

#include "AudioArchiveLoader.hpp"
#include "BaaConverter.hpp"
#include "WavePool.hpp"

Poco::Logger& create_logger();

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        std::cout << "Usage: " << argv[0] << " <*.baa> <wave folder> <output folder>" << std::endl;
        return -1;
    }

    std::filesystem::path baaPath = argv[1];
    std::string waveFolder = argv[2];
    std::filesystem::path outputFolder{argv[3]};

  Poco::Logger& logger = create_logger();

  std::ifstream sound_file{baaPath, std::ios::in | std::ios::binary};

  if (!sound_file)
  {
    std::cout << "Could not open " << baaPath << std::endl;
  }

  z2sound::AudioArchiveLoader loader{sound_file, logger};
  auto audio_archive = loader.load();

  if (!audio_archive)
  {
    std::cout << "Failed to load audio archive" << std::endl;
  }

  const uint8_t wave_bank_no = 1;

  auto bgm_bank_iterator = audio_archive->wave_banks_.find(wave_bank_no);
  z2sound::WavePool wave_pool{bgm_bank_iterator->second};
  wave_pool.set_base_directory(waveFolder);

  for (auto& group : bgm_bank_iterator->second.get_wave_groups())
  {
    logger.information("Loading %s", group.get_filename());
    wave_pool.load_group(group);
  }

  logger.information("Finished loading wave groups");

  BaaConverter baa_converter{*audio_archive};
  auto sf2 = baa_converter.to_sf2(wave_bank_no);

  if (!sf2.has_value())
  {
    logger.error("Failed converting wave bank %u to SoundFont", static_cast<unsigned>(wave_bank_no));
    return -1;
  }

  std::ostringstream filename_stream;
    filename_stream << baaPath.stem().string() << "_" << std::to_string(wave_bank_no) << ".sf2";
  std::filesystem::path outputFile = outputFolder / filename_stream.str();

  std::ofstream sf2_file{outputFile, std::ios::binary};

  if (!sf2_file)
  {
    logger.error("Could not open output file %s for writing", outputFile.string());
    return -1;
  }

  sf2->Write(sf2_file);
  logger.information("Finished writing " + outputFile.string());

  // for (const auto& bank : audio_archive->instrument_banks_)
  // {
  //   if (bank.second.get_wave_bank_id() != wave_bank_no)
  //   {
  //     continue;
  //   }

  //   BaaConverter baa_converter{*audio_archive};
  //   uint8_t bank_id = bank.second.get_id();
  //   auto sf2 = baa_converter.to_sf2(bank_id);

  //   if (!sf2.has_value())
  //   {
  //     logger.error("Failed converting bank %u to SoundFont", static_cast<unsigned>(bank_id));
  //     return -1;
  //   }

  //   auto outputFile = "TP_Bank_" + std::to_string(bank_id) + ".sf2";

  //   std::ofstream sf2_file{outputFile, std::ios::binary};
  //   sf2->Write(sf2_file);
  //   logger.information("Finished writing " + outputFile);
  // }
}

Poco::Logger& create_logger()
{
  Poco::Logger& logger = Poco::Logger::root();
  Poco::AutoPtr<Poco::ColorConsoleChannel> console_channel{new Poco::ColorConsoleChannel};

  logger.setChannel(console_channel);
  logger.setLevel(Poco::Message::PRIO_TRACE);

  return logger;
}