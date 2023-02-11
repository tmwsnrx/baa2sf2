#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include "Poco/ConsoleChannel.h"
#include "Poco/Logger.h"

#include "AudioArchiveLoader.hpp"
#include "BaaConverter.hpp"
#include "WavePool.hpp"

Poco::Logger& create_logger();

int main(int, char**)
{
  Poco::Logger& logger = create_logger();

  std::ifstream sound_file{"Z2Sound.baa", std::ios::in | std::ios::binary};

  if (!sound_file)
  {
    std::cout << "Could not open Z2Sound.baa" << std::endl;
  }

  z2sound::AudioArchiveLoader loader{sound_file, logger};
  auto audio_archive = loader.load();

  if (!audio_archive)
  {
    std::cout << "Failed to load audio archive" << std::endl;
  }

  auto bgm_bank_iterator = audio_archive->wave_banks_.find(1);
  z2sound::WavePool wave_pool{bgm_bank_iterator->second};
  wave_pool.set_base_directory("/home/tim/roms/TP/files/Audiores/Waves");

  for (auto& group : bgm_bank_iterator->second.get_wave_groups())
  {
    logger.information("Loading %s", group.get_filename());
    wave_pool.load_group(group);
  }

  logger.information("Finished loading wave groups");

  const uint8_t bank_no = 11;

  BaaConverter baa_converter{*audio_archive};
  auto sf2 = baa_converter.to_sf2(bank_no);

  if (!sf2.has_value())
  {
    logger.error("Failed converting bank %u to SoundFont", static_cast<unsigned>(bank_no));
    return -1;
  }

  auto filename = "TP_Bank_" + std::to_string(bank_no) + ".sf2";

  std::ofstream sf2_file{filename, std::ios::binary};
  sf2->Write(sf2_file);
  logger.information("Finished writing " + filename);
}

Poco::Logger& create_logger()
{
  Poco::Logger& logger = Poco::Logger::root();
  Poco::AutoPtr<Poco::ColorConsoleChannel> console_channel{new Poco::ColorConsoleChannel};

  logger.setChannel(console_channel);
  logger.setLevel(Poco::Message::PRIO_TRACE);

  return logger;
}